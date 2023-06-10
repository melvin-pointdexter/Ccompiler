#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* INT_TYPE = "INT";
const char* REAL_TYPE = "REAL";
const char* CHAR_TYPE = "CHAR";
const char* BOOL_TYPE = "BOOL";
const char* P_INT_TYPE = "INT_P";
const char* P_REAL_TYPE = "REAL_P";
const char* P_CHAR_TYPE = "CHAR_P";

typedef struct semErrNode
{
	char* msgStr;
	char* errTypeStr;
	struct semErrNode* nextErr;
} semErrNode;

typedef struct varDataNode {
	char * idenStr;
	char * idenType;
	struct varDataNode* nextVar;
} varDataNode;

// scope for each function:
typedef struct varScopeNode {
	int funcScopesNum;
	int stateScopesNum;
	char * retTypeStr;
	char * funcId;
	struct varDataNode* argListHead;
	struct varDataNode* varListHead;
	struct varScopeNode** funcScopes; // scopes for functions and / or procedures.
	struct varScopeNode** stateScopes; // scopes for statements.
	struct varScopeNode* faScope; // father scope of current scope.
} varScopeNode;

typedef struct scopeTaNode {
	char* idenStrKey;
	char** idenTypeStrArr;
	varScopeNode** keyScopes;
	struct scopeTaNode* nextPair;
} scopeTaNode;

////////////////////////////////////
// error at semantic              //
// parsing structs and functions: //
////////////////////////////////////

struct semErrNode* createSemErr(char* errMsg, char* errType);
void addSemErrMsg(struct semErrNode** semErrListAddr, char* errMsg, char* errType);
void delSemErrList(struct semErrNode** semErrListAddr);

struct semErrNode* createSemErr(char* errMsg, char* errType)
{
	struct semErrNode* newErrNode = (semErrNode*)malloc(sizeof(semErrNode));
	newErrNode->msgStr = (char*)malloc(sizeof(errMsg) + 1);
	strcpy(newErrNode->msgStr, errMsg);
	
	newErrNode->errTypeStr = (char*)malloc(sizeof(errType) + 1);
	strcpy(newErrNode->errTypeStr, errType);
	newErrNode->nextErr = 0;
		
	return newErrNode;
}


void addSemErrMsg(struct semErrNode** semErrListAddr, char* errMsg, char* errType)
{
	struct semErrNode* currSemErr = *(semErrListAddr);
	if (0 == *(semErrListAddr))
	{
		*(semErrListAddr) = createSemErr(errMsg, errType);
	}
	else
	{
		while (0 != currSemErr->nextErr)
		{
			currSemErr = currSemErr->nextErr;
		}
		currSemErr->nextErr = createSemErr(errMsg, errType);
	}
}

void delSemErrList(struct semErrNode** semErrListAddr)
{
	struct semErrNode* currSemErr = *(semErrListAddr);
	struct semErrNode* nextErr = 0;
	
	if (0 != currSemErr)
	{
		while (0 != currSemErr->nextErr)
		{
			nextErr = currSemErr->nextErr;
			
			
			free(currSemErr->msgStr);
			free(currSemErr->errTypeStr);
			free(currSemErr);
			
			currSemErr = nextErr;				
		}
		
		free(currSemErr->msgStr);
		free(currSemErr->errTypeStr);
		free(currSemErr);
		
		*(semErrListAddr) = 0;
	}
}

/////////////////////////////////////
// semantic structs and functions: //
/////////////////////////////////////

struct varDataNode* createVarDataNode (char* idenStr, char* idenType);
void addVarData (struct varDataNode** varDataListAddr, char* idenStr, char* idenType);
struct varDataNode* createVarDataList(struct node* varDecTree);

struct varScopeNode* createScopeForFunc(char* retType, char* newFuncId, struct varDataNode* newArgList, struct varDataNode* newVarList
, int funcScopesNum, int stateScopesNum, struct varScopeNode** funcScopesArr, struct varScopeNode** stateScopesArr);

void setFaScopeInScopesArr(struct varScopeNode* faScope, int scopesNum, struct varScopeNode** scopesArr);

void delScope(struct scopeTaNode* scopeTaPairNode, struct varScopeNode * scopeStTopNode);
void delScopeTree(struct varScopeNode** scopeTreeHead);

struct varDataNode* createVarDataNode (char* idenStr, char* idenType)
{
	struct varDataNode* newData = (varDataNode*)malloc(sizeof(varDataNode));
	
	newData->idenStr = (char*)malloc(sizeof(idenStr) + 1);
	strcpy(newData->idenStr, idenStr);
	
	newData->idenType = (char*)malloc(sizeof(idenType) + 1);
	strcpy(newData->idenType, idenType);
	
	newData->nextVar = 0;
	
	return newData;
}

void addVarData (struct varDataNode** varDataListAddr, char* idenStr, char* idenType)
{
	struct varDataNode* currData = *(varDataListAddr);
	
	if (0 == *(varDataListAddr))
	{
		*(varDataListAddr) = createVarDataNode(idenStr, idenType);
	}
	else
	{
		while (0 != currData->nextVar)
		{
			currData = currData->nextVar;
		}
		
		currData->nextVar = createVarDataNode(idenStr, idenType);
	}
}

struct varDataNode* createVarDataList(struct node* varDecTree)
{
	return 0;//NOTE: MODIFY CODE HERE <--
}

void setFaScopeInScopesArr(struct varScopeNode* faScope, int scopesNum, struct varScopeNode** scopesArr)
{
	struct varScopeNode* currSelectScope = 0;
	
	if (0 != scopesArr)
	{	
		while (0 != scopesNum)
		{
			currSelectScope = *(scopesArr) + (scopesNum - 1);
			currSelectScope->faScope = faScope;
			scopesNum--;
		}
	}
}

struct varScopeNode* createScopeForFunc(char* retType, char* newFuncId, struct varDataNode* newArgList, struct varDataNode* newVarList
, int funcScopesNum, int stateScopesNum, struct varScopeNode** funcScopesArr, struct varScopeNode** stateScopesArr)
{
	struct varScopeNode* newScope = (varScopeNode*)malloc(sizeof(varScopeNode));
	
	newScope->retTypeStr = (char*)malloc(sizeof(retType) + 1);
	strcpy(newScope->retTypeStr, retType);
	
	newScope->funcId = (char*)malloc(sizeof(newFuncId) + 1);
	strcpy(newScope->funcId, newFuncId);
	
	newScope->argListHead = newArgList;
	
	newScope->varListHead = newVarList;
	
	newScope->funcScopesNum = funcScopesNum;
	
	newScope->funcScopes = funcScopesArr;
	
	setFaScopeInScopesArr(newScope, funcScopesNum, funcScopesArr);
	
	newScope->stateScopesNum = stateScopesNum;
	
	newScope->stateScopes = stateScopesArr;
	
	setFaScopeInScopesArr(newScope, stateScopesNum, stateScopesArr);
	
	return newScope;
}
