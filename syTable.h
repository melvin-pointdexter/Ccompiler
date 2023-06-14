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
	int scopesNum;
	char* idenStrKey;
	char** idenTypeStrArr;
	struct varScopeNode** keyScopes;
	struct scopeTaNode* nextEntry;
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

// variables - scopes table:
struct scopeTaNode* createTableEntry(char* idenKey);
void addTableEntry(struct scopeTaNode** tableAddr, char* idenKey);

struct scopeTaNode* findEntry(struct scopeTaNode* tableList, char* idenKey);

int hasScopeInEntry(struct scopeTaNode* idenEntry, struct varScopeNode* idenScopeToCheck);

void addScope(struct scopeTaNode* tableList, struct varScopeNode* scope, char* idenType, char* idenKey);
struct varScopeNode* findIdenScope(struct scopeTaNode* idenEntry, char* idenType, struct varScopeNode*
 idenScopeStartPoint);

void delEntryVarsMem(struct scopeTaNode* entry);

void delAllEntries(struct scopeTaNode** tableAddr);

// variables

struct varDataNode* createVarDataNode (char* idenStr, char* idenType);
void addVarData (struct varDataNode** varDataListAddr, char* idenStr, char* idenType);

struct varDataNode* createVarDataList(struct node* varDecTree);

void delDataNodeVarsMem(struct varDataNode* dataNode);
void delVarDataList(struct varDataNode** varDataListAddr);

// function scopes
void setFaScopeInScopesArr(struct varScopeNode* faScope, int scopesNum, struct varScopeNode** scopesArr);
struct varScopeNode* createScopeForFunc(char* retType, char* newFuncId, struct varDataNode* newArgList, struct varDataNode* newVarList
, int funcScopesNum, int stateScopesNum, struct varScopeNode** funcScopesArr, struct varScopeNode** stateScopesArr);

struct varScopeNode** createFuncScopesArr(int*  funcScopesNumAddr, struct node* currFunc);
 
void delScope(struct varScopeNode * scopeTreeRoot);
void delScopesTree(int scopesNum, struct varScopeNode** funcScopesArr);

struct scopeTaNode* createTableEntry(char* idenKey)
{
	struct scopeTaNode* newEntry = (scopeTaNode*)malloc(sizeof(scopeTaNode));
	
	newEntry->scopesNum = 0;
	
	newEntry->idenStrKey = (char*)malloc(sizeof(idenKey) + 1);
	strcpy(newEntry->idenStrKey, idenKey);
	
	newEntry->idenTypeStrArr = 0;
		
	newEntry->keyScopes = 0;
	
	newEntry->nextEntry = 0;
	
	return newEntry;
}

void addTableEntry(struct scopeTaNode** tableAddr, char* idenKey)
{
	struct scopeTaNode* entryAdd = createTableEntry(idenKey);
	
	entryAdd->nextEntry = *(tableAddr);
	
	*(tableAddr) = entryAdd;	
}

struct scopeTaNode* findEntry(struct scopeTaNode* tableList, char* idenKey)
{
	struct scopeTaNode* currEntry = tableList;
	if (0 == tableList)
	{
		return 0;
	}
	else
	{
		while (0 != strcmp(idenKey, currEntry->idenStrKey))
		{
			if (0 == currEntry->nextEntry)
			{
				return 0;
			}
			else
			{
				currEntry = currEntry->nextEntry;	
			}
		}
		
		return currEntry;
	}
}

void addScope(struct scopeTaNode* tableList, struct varScopeNode* scope, char* idenType, char* idenKey)
{
	struct scopeTaNode* updateEntry = findEntry(tableList, idenKey);
	
	if (0 != updateEntry)
	{
		realloc(updateEntry->keyScopes, sizeof(varScopeNode**) * (1 + updateEntry->scopesNum));
		updateEntry->keyScopes[updateEntry->scopesNum] = scope;
		
		realloc(updateEntry->idenTypeStrArr, sizeof(char**) * (1 + updateEntry->scopesNum));
		updateEntry->idenTypeStrArr[updateEntry->scopesNum] = (char*)malloc(1 + sizeof(idenType));
		strcpy(updateEntry->idenTypeStrArr[updateEntry->scopesNum], idenType);
		
		updateEntry->scopesNum++;
	}
}

struct varScopeNode* findIdenScope(struct scopeTaNode* idenEntry, char* idenType, struct varScopeNode*
 idenScopeStartPoint)
 {
 	int i = 0;
 	
 	if (0 == idenEntry)
 	{
 		return 0;
 	}
 	else
 	{
 		if (0 != idenScopeStartPoint)
 		{
 			for (i = 0; i < (idenEntry->scopesNum - 1); i++)
			{
				if (idenScopeStartPoint == (idenEntry->keyScopes[i]))
				{
					break;
				}
			}
			
			if (i != (idenEntry->scopesNum - 1))
			{
				i++;
			}
			else
			{
				return 0; // didn't found scope given as starting point.
			}
		}
		
		for (/*no need to init i*/ ; i < (idenEntry->scopesNum); i++)
		{
			if (0 == strcmp(idenType, (idenEntry->idenTypeStrArr)[i]))
			{
				return ((idenEntry->keyScopes)[i]);
			}
		}
		
		return 0; // didn't found scope of given identifier's type (such as: function).
	}		
 }
 
/* void delEntryVarsMem(struct scopeTaNode* entry){} */

void delAllEntries(struct scopeTaNode** tableAddr)
{
	
}

// func(variables)
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
			currVarIdensNode = currVarIdensNode->nodes[(currVarIdensNode->sons) - 1];
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

void delDataNodeVarsMem(struct varDataNode* dataNode)
{
	if (0 != dataNode)
	{
		free(dataNode->idenStr);
		dataNode->idenStr = 0;
		free(dataNode->idenType);
		dataNode->idenType = 0;
	}
}

void delVarDataList(struct varDataNode** varDataListAddr)
{
	struct varDataNode* currVarData = *(varDataListAddr);
	struct varDataNode* nextVarData = 0;
	
	if (0 != currVarData)
	{
		while (0 != (currVarData->nextVar))
		{
			nextVarData = currVarData->nextVar;
			
			delDataNodeVarsMem(currVarData);
			free(currVarData);
			
			currVarData = nextVarData;
		}
		
		delDataNodeVarsMem(currVarData);
		free(currVarData);
		
		*(varDataListAddr) = 0;
	}
}

// function(func)
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

	//setFaScopeInScopesArr(newScope, funcScopesNum, funcScopesArr);
	
	newScope->stateScopesNum = stateScopesNum;
	
	newScope->stateScopes = stateScopesArr;
	
	//setFaScopeInScopesArr(newScope, stateScopesNum, stateScopesArr);

			printf("in create\n\n\n");
	return newScope;
}

struct varScopeNode** createFuncScopesArr(int* funcScopesNumAddr, struct node* currFunc)
{
	struct node* currBody = 0;
	char* currRetType = 0;
	int funcScopesNum = 0;
	struct varScopeNode **  currFuncScopesArr = 0;
	struct varScopeNode **  retScopesArr = 0;
	struct varDataNode * currFuncVarDecList = 0;
	
				printf("\nmes\n");
	while (NODE_FUNC_PROD_LIST == currFunc->NODETYPE)
	{	
				printf("\nif\n");
		
		// func
		if (NODE_FUNC == currFunc->nodes[0]->NODETYPE)
		{
			currBody = currFunc->nodes[0]->nodes[((currFunc->nodes[0]->sons) - 2)];
			
			currRetType = currFunc->nodes[1]->token;
		}
		else // proc
		{
			currBody = currFunc->nodes[0]->nodes[((currFunc->nodes[0]->sons) - 1)];
			
			currRetType =  ( 4 == currFunc->nodes[0]->sons ) ? ( currFunc->nodes[0]->nodes[2]->token ) :
			(  currFunc->nodes[0]->nodes[1]->token );
		}
		
												
		if ((1 < (currBody)->sons) &&
			(
				(NODE_FUNC_PROD_LIST == currBody->nodes[0]->NODETYPE)
				|| (NODE_PROC == currBody->nodes[0]->NODETYPE)
				|| (NODE_FUNC == currBody->nodes[0]->NODETYPE)
			)
		)
		{
			//currFuncScopesArr = createFuncScopesArr(&funcScopesNum, currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->nodes[0]);	
		}
		else if (1 == (currBody->sons))
		{		printf("\nif2\n");
		
		
		
		
			if ((NODE_FUNC_PROD_LIST == currBody->NODETYPE) ||
				(NODE_PROC == currBody->NODETYPE) ||
				(NODE_FUNC == currBody->NODETYPE)
			)
			{
				currFuncScopesArr = createFuncScopesArr(&funcScopesNum, currBody);
			}
			else if (DONT_ADD_TAB == currBody->NODETYPE)
			{
				currFuncVarDecList = createVarDataList(currBody);
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
		
		// first, add function to array:
		if (0 == retScopesArr)
		{
			retScopesArr = (varScopeNode**)malloc(sizeof(varScopeNode*));
		}
		else
		{
			realloc(retScopesArr, sizeof(varScopeNode*) * (1 + (*(funcScopesNumAddr))));
		}
				printf("\nmess2\n");
		// cmp string if Proc, else node is Func
		retScopesArr[(*(funcScopesNumAddr))] = createScopeForFunc( currRetType,
			currFunc->nodes[0]->nodes[0]->token, 0, currFuncVarDecList, funcScopesNum, 0, currFuncScopesArr, 0);
							printf("\nmess4\n");
		(*(funcScopesNumAddr))++;
		
		currFunc = currFunc->nodes[1];
			printf("\nmess4\n");
	}
	
	// func or proc:
	retScopesArr = (varScopeNode**)malloc(sizeof(varScopeNode*));
	
	// func:
	if (NODE_FUNC == currFunc->NODETYPE)
	{	
				printf("\nif\n");	// remember: func body in sons -2: 														
		if ((1 < (currFunc->nodes[((currFunc->sons) - 2)])->sons) &&
			(
				(NODE_FUNC_PROD_LIST == currFunc->nodes[(currFunc->sons - 2)]->nodes[0]->NODETYPE)
				|| (NODE_PROC == currFunc->nodes[(currFunc->sons - 2)]->nodes[0]->NODETYPE)
				|| (NODE_FUNC == currFunc->nodes[(currFunc->sons - 2)]->nodes[0]->NODETYPE)
			)
		)
		{
			//currFuncScopesArr = createFuncScopesArr(&funcScopesNum, currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->nodes[0]);	
		}
		else if (1 == ((currFunc->nodes)[((currFunc->sons) - 2)]->sons))
		{		printf("\nfunc2\n");
		
		
		
		
			if ((NODE_FUNC_PROD_LIST == currFunc->nodes[(currFunc->sons - 2)]->NODETYPE) ||
				(NODE_PROC == currFunc->nodes[(currFunc->sons - 2)]->NODETYPE) ||
				(NODE_FUNC == currFunc->nodes[(currFunc->sons - 2)]->NODETYPE)
			)
			{
				currFuncScopesArr = createFuncScopesArr(&funcScopesNum, currFunc->nodes[(currFunc->sons - 2)]);
			}
			else if (DONT_ADD_TAB == currFunc->nodes[(currFunc->sons - 2)]->NODETYPE)
			{
				currFuncVarDecList = createVarDataList(currFunc->nodes[(currFunc->sons - 2)]);
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
		
		retScopesArr[(*(funcScopesNumAddr))] = createScopeForFunc(currFunc->nodes[1]->token,
			currFunc->nodes[0]->token, 0, currFuncVarDecList, funcScopesNum, 0, currFuncScopesArr, 0);
		(*(funcScopesNumAddr))++;
	}
	else // proc:
	{
					printf("\nif\n");														
		if ((1 < (currFunc->nodes[((currFunc->sons) - 1)])->sons) &&
			(
				(NODE_FUNC_PROD_LIST == currFunc->nodes[(currFunc->sons - 2)]->nodes[0]->NODETYPE)
				|| (NODE_PROC == currFunc->nodes[(currFunc->sons - 2)]->nodes[0]->NODETYPE)
				|| (NODE_FUNC == currFunc->nodes[(currFunc->sons - 2)]->nodes[0]->NODETYPE)
			)
		)
		{
			//currFuncScopesArr = createFuncScopesArr(&funcScopesNum, currFunc->nodes[0]->nodes[(currFunc->sons - 1)]->nodes[0]);	
		}
		else if (1 == ((currFunc->nodes)[((currFunc->sons) - 1)]->sons))
		{		printf("\nfunc2\n");
		
		
		
		
			if ((NODE_FUNC_PROD_LIST == currFunc->nodes[(currFunc->sons - 1)]->NODETYPE) ||
				(NODE_PROC == currFunc->nodes[(currFunc->sons - 1)]->NODETYPE) ||
				(NODE_FUNC == currFunc->nodes[(currFunc->sons - 1)]->NODETYPE)
			)
			{
				currFuncScopesArr = createFuncScopesArr(&funcScopesNum, currFunc->nodes[(currFunc->sons - 1)]);
			}
			else if (DONT_ADD_TAB == currFunc->nodes[(currFunc->sons - 1)]->NODETYPE)
			{
				currFuncVarDecList = createVarDataList(currFunc->nodes[(currFunc->sons - 1)]);
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
		
		retScopesArr[(*(funcScopesNumAddr))] = createScopeForFunc( ( 4 == currFunc->sons ) ? ( currFunc->nodes[2]->token ) :
			(  currFunc->nodes[1]->token ),
			currFunc->nodes[0]->token, 0, currFuncVarDecList, funcScopesNum, 0, currFuncScopesArr, 0);
		(*(funcScopesNumAddr))++;
	}
	return retScopesArr;
}

void delScope(struct varScopeNode * scopeTreeRoot)
{	
	free(scopeTreeRoot->retTypeStr);
	
	scopeTreeRoot->retTypeStr = 0;
	
	free(scopeTreeRoot->funcId);
	
	scopeTreeRoot->funcId = 0;
	
	printf("delete arry\n\n");
	delVarDataList(&(scopeTreeRoot->argListHead));
	
	delVarDataList(&(scopeTreeRoot->varListHead));

}

void delScopesTree(int scopesNum, struct varScopeNode** funcScopesArr)
{
	int i = 0;
	int currDelFuncIn = 0;
		
	for (currDelFuncIn = 0; currDelFuncIn < scopesNum; currDelFuncIn++)
	{
		if (0 < funcScopesArr[currDelFuncIn]->funcScopesNum)
		{
			delScopesTree(funcScopesArr[currDelFuncIn]->funcScopesNum, funcScopesArr[currDelFuncIn]->funcScopes);
		}
		
		if (0 < funcScopesArr[currDelFuncIn]->stateScopesNum)
		{
			delScopesTree(funcScopesArr[currDelFuncIn]->stateScopesNum, funcScopesArr[currDelFuncIn]->stateScopes);
		}
		
		printf("toekn: %s", funcScopesArr[currDelFuncIn]->funcId);
		delScope(funcScopesArr[currDelFuncIn]);
		free(funcScopesArr[currDelFuncIn]);
	}
}

#endif
