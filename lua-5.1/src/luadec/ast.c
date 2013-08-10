#include <stdlib.h>

#include "ast.h"

AstBlock* MakeAstBlock(BlockType type) {
	AstBlock* block = (AstBlock*)calloc(1, sizeof(AstBlock));
	block->type = type;
	block->statements = NewList();
	return block;
}

void DeleteAstBlock(AstBlock* block) {
	if (block == NULL) {
		return;
	}
	ClearList(block->statements, (ListItemFn)ClearAstStatement);
	free(block->statements);
	free(block);
}

void PrintAstBlock(AstBlock* block, StringBuffer* buff, int indent) {
	ListItem* walk;
	if (block == NULL) {
		return;
	}
	walk = block->statements->head;
	while (walk) {
		PrintAstStatement(cast(AstStatement*, walk), buff, indent);
		walk = walk->next;
	}
}

AstStatement* MakeSimpleStatement(char* code) {
	AstStatement* stmt = (AstStatement*)calloc(1, sizeof(AstStatement));
	stmt->type = SIMPLE_STMT;
	stmt->code = code;
	return stmt;
}

void ClearSimpleStatement(AstStatement* stmt) {
	if (stmt == NULL) {
		return;
	}
	free(stmt->code);
	stmt->code = NULL;
}

LoopStatement* MakeBlockStatement() {
	LoopStatement* stmt = (LoopStatement*)calloc(1, sizeof(LoopStatement));
	stmt->super.type = BLOCK_STMT;
	stmt->super.code = NULL;
	stmt->body = MakeAstBlock(BLOCK_BODY);
	stmt->body->parent = (AstStatement*)stmt;
	return stmt;
}

LoopStatement* MakeLoopStatement(StatementType type, char* test) {
	LoopStatement* stmt;
	BlockType blockType;
	switch(type) {
	case WHILE_STMT:
		blockType = WHILE_BODY;
		break;
	case REPEAT_STMT:
		blockType = REPEAT_BODY;
		break;
	case FORLOOP_STMT:
		blockType = FORLOOP_BODY;
		break;
	case TFORLOOP_STMT:
		blockType = TFORLOOP_BODY;
		break;
	default:
		return NULL;
	}
	stmt = (LoopStatement*)calloc(1, sizeof(LoopStatement));
	stmt->super.type = type;
	stmt->super.code = test;
	stmt->body = MakeAstBlock(blockType);
	stmt->body->parent = (AstStatement*)stmt;
	return stmt;
}

void ClearLoopStatement(LoopStatement* stmt) {
	if (stmt == NULL) {
		return;
	}
	ClearSimpleStatement(&(stmt->super));
	DeleteAstBlock(stmt->body);
	stmt->body = NULL;
}

IfStatement* MakeIfStatement(char* test) {
	IfStatement* stmt = (IfStatement*)calloc(1, sizeof(IfStatement));
	stmt->super.type = IF_STMT;
	stmt->super.code = test;
	stmt->thenBlock = MakeAstBlock(IF_THEN_BLOCK);
	stmt->thenBlock->parent = (AstStatement*)stmt;
	stmt->elseBlock = MakeAstBlock(IF_ELSE_BLOCK);
	stmt->elseBlock->parent = (AstStatement*)stmt;
	return stmt;
}

void ClearIfStatement(IfStatement* stmt) {
	if (stmt == NULL) {
		return;
	}
	ClearSimpleStatement(&(stmt->super));
	DeleteAstBlock(stmt->thenBlock);
	stmt->thenBlock = NULL;
	DeleteAstBlock(stmt->elseBlock);
	stmt->elseBlock = NULL;
}

void ClearAstStatement(AstStatement* stmt, void* dummy) {
	if (stmt == NULL) {
		return;
	}
	switch (stmt->type) {
	case SIMPLE_STMT:
		ClearSimpleStatement(stmt);
		break;
	case BLOCK_STMT:
	case WHILE_STMT:
	case REPEAT_STMT:
	case FORLOOP_STMT:
	case TFORLOOP_STMT:
		ClearLoopStatement((LoopStatement*)stmt);
		break;
	case IF_STMT:
		ClearIfStatement((IfStatement*)stmt);
		break;
	}
}

void DeleteAstStatement(AstStatement* stmt) {
	ClearAstStatement(stmt, NULL);
	free(stmt);
}

void PrintSimpleStatement(AstStatement* stmt, StringBuffer* buff, int indent) {
	int i = 0;
	for (i = 0; i < indent; i++) {
		StringBuffer_add(buff, "  ");
	}
	StringBuffer_addPrintf(buff, "%s\n", stmt->code);
}

void PrintLoopStatement(LoopStatement* stmt, StringBuffer* buff, int indent) {
	StringBuffer* startCode = StringBuffer_new(NULL);
	StringBuffer* endCode = StringBuffer_new(NULL);
	int i = 0;
	for (i = 0; i < indent; i++) {
		StringBuffer_add(buff, "  ");
	}
	switch (stmt->super.type) {
	case BLOCK_STMT:
		StringBuffer_printf(startCode, "do");
		StringBuffer_printf(endCode, "end");
		break;
	case WHILE_STMT:
		StringBuffer_printf(startCode, "while %s do", stmt->super.code);
		StringBuffer_printf(endCode, "end");
		break;
	case REPEAT_STMT:
		StringBuffer_printf(startCode, "repeat");
		StringBuffer_printf(endCode, "until %s", stmt->super.code);
		break;
	case FORLOOP_STMT:
		StringBuffer_printf(startCode,  "for %s do", stmt->super.code);
		StringBuffer_printf(endCode, "end");
		break;
	case TFORLOOP_STMT:
		StringBuffer_printf(startCode,  "for %s do", stmt->super.code);
		StringBuffer_printf(endCode, "end");
		break;
	}
	StringBuffer_addPrintf(buff, "%s\n", StringBuffer_getRef(startCode));
	PrintAstBlock(stmt->body, buff, indent + 1);
	StringBuffer_addPrintf(buff, "%s\n", StringBuffer_getRef(endCode));
	StringBuffer_delete(startCode);
	StringBuffer_delete(endCode);
}

void PrintIfStatement(IfStatement* stmt, StringBuffer* buff, int indent, int elseif) {
	AstBlock* elseBlock = stmt->elseBlock;
	List* elseStatements = elseBlock->statements;
	int elseSize = elseStatements->size;
	AstStatement* elseFirst = cast(AstStatement*, elseStatements->head);
	int i = 0;
	for (i = 0; i < indent; i++) {
		StringBuffer_add(buff, "  ");
	}
	if (elseif) {
		StringBuffer_printf(buff,  "elseif %s then\n", stmt->super.code);
	} else {
		StringBuffer_printf(buff,  "if %s then\n", stmt->super.code);
	}
	PrintAstBlock(stmt->thenBlock, buff, indent + 1);
	if (elseSize == 0) {
		StringBuffer_printf(buff,  "end\n");
	} else if (elseSize == 1 && elseFirst->type == IF_STMT) {
		PrintIfStatement((IfStatement*)elseFirst, buff, indent, 1);
	} else {
		StringBuffer_printf(buff,  "else\n");
		PrintAstBlock(elseBlock, buff, indent + 1);
		StringBuffer_printf(buff,  "end\n");
	}
}

void PrintAstStatement(AstStatement* stmt, StringBuffer* buff, int indent) {
	if (stmt == NULL) {
		return;
	}
	switch (stmt->type) {
	case SIMPLE_STMT:
		PrintSimpleStatement(stmt, buff, indent);
		break;
	case BLOCK_STMT:
	case WHILE_STMT:
	case REPEAT_STMT:
	case FORLOOP_STMT:
	case TFORLOOP_STMT:
		PrintLoopStatement((LoopStatement*)stmt, buff, indent);
		break;
	case IF_STMT:
		PrintIfStatement((IfStatement*)stmt, buff, indent, 0);
		break;
	}
}

void AddToBlock(AstBlock* block, AstStatement* stmt) {
	AddToList(block->statements, (ListItem*)stmt);
}