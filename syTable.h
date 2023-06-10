#define _CRT_SECURE_NO_WARNINGS
#ifndef SYMBOL_TAB_TOOLS_FILE
#define SYMBOL_TAB_TOOLS_FILE

#include "ast.h"
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

void setFaScopeInScopesArr(struct varScopeNode* faScope, int scopesNum, struct varScopeNode** scopesArr);
struct varScopeNode* createScopeForFunc(char* retType, char* newFuncId, struct varDataNode* newArgList, struct varDataNode* newVarList
, int funcScopesNum, int stateScopesNum, struct varScopeNode** funcScopesArr, struct varScopeNode** stateScopesArr);

struct varScopeNode** createFuncScopesArr(int*  funcScopesNumAddr, struct node* currFunc);
 
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
		*(varDataListAddr) = createVarDataNode(idenStr, idenType)
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
	struct varDataNode* newDataList = 0;
	struct node* currVarDecRoot = varDecTree;
	struct node* currVarIdensNode = 0;
	
	while (1 != currVarDecRoot->sons)
	{
		currVarIdensNode = varDecTree->nodes[0];
		
		// if there's second identifer to look at:
		while (0 != (currVarIdensNode->sons % 2)) 
		{
			addVarData(&newDataList, currVarIdensNode->token, currVarDecRoot->token);
			currVarIdensNode = currVarIdensNode->nodes[currVarIdensNode->sons - 1];
		}
		
		addVarData(&newDataList, currVarIdensNode->token, currVarDecRoot->token);
		currVarDecRoot = currVarDecRoot->nodes[1];
	} 
	
	// if there's second identifier to look at:
	while (0 != (currVarIdensNode->sons % 2))
	{
		addVarData(&newDataList, currVarIdensNode->token, currVarDecRoot->token);
		currVarIdensNode = currVarIdensNode->nodes[currVarIdensNode->sons - 1];
	}
	addVarData(&newDataList, currVarIdensNode->token, currVarDecRoot->token);
	//TODO-add for string variables list
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

struct varScopeNode** createFuncScopesArr(int* funcScopesNumAddr, struct node* currFunc)
{
	int funcScopesNum = 0;
	int funcRetScopesNum = 0;
	struct varScopeNode **  currFuncScopesArr = 0;
	struct varScopeNode **  retScopesArr = 0;
	struct varDataNode * currFuncVarDecList = 0;
	 
	while ((size_t)0 == strlen(currFunc->token))
	{																
		if ((1 < currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->sons) &&
			(
				(NODE_FUNC_PROD_LIST == currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->nodes[0]->NODETYPE)
				|| (NODE_PROC == currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->nodes[0]->NODETYPE)
				|| (NODE_FUNC == currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->nodes[0]->NODETYPE)
			)
		{
			currFuncScopesArr = createFuncScopesArr(&funcScopesNum, currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->nodes[0]);	
		}
		else if (1 == currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->sons)
		{
			if ((NODE_FUNC_PROD_LIST == currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->NODETYPE) ||
				(NODE_PROC == currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->NODETYPE) ||
				|| (NODE_FUNC == currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->NODETYPE)
			)
			{
				currFuncScopesArr = createFuncScopeArr(&funcScopesNum, currFunc->nodes[0]->nodes[(currFunc->sons - 1)]);
			}
			else if (DONT_ADD_TAB == currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->NODETYPE)
			{
				currFuncVarDecList = createVarDataList(currFunc->nodes[0]->nodes[(currFunc->sons - 1)]);
			}
			else
			{
				// body made of statement scope/s
			}
		}
		else
		{
			// function's body is empty.
		}
		
		// first , add function to array:
		realloc(retScopesArr, (struct varScopeNode**) * (1 + funcRetScopesNum));
		retScopesArr[funcRetScopesNum] = createScopeForFunc( ( 4 == currFunc->nodes[0]->sons ) ? ( currFunc->nodes[0]->nodes[2] ) :
			(  currFunc->nodes[0]->nodes[1] ),
			currFunc->nodes[0]->nodes[0], 0, currFuncVarDecList, funcScopesNum, 0, currFuncScopesArr, 0); 
		++funcRetScopesNum;
		
		currFunc = currFunc->nodes[1];
	}
	
	return retScopesArr;
}

#endif
