#include "common.h"

#include <stdlib.h>

#include "ast.h"

const char* stmttype[22] = {
	"SIMPLE_STMT",
	"BLOCK_STMT",
	"WHILE_STMT",
	"REPEAT_STMT",
	"FORLOOP_STMT",
	"TFORLOOP_STMT",
	"IF_STMT",
	"IF_THEN_STMT",
	"IF_ELSE_STMT"
};

void PrintIndent(StringBuffer* buff, int indent) {
	int i = 0;
	for (i = 0; i < indent; i++) {
		StringBuffer_add(buff, "  ");
	}
}

void PrintAstSub(List* sub, StringBuffer* buff, int indent) {
	ListItem* walk;
	if (sub == NULL) {
		return;
	}
	walk = sub->head;
	while (walk) {
		PrintAstStatement(cast(AstStatement*, walk), buff, indent);
		walk = walk->next;
	}
}

AstStatement* MakeSimpleStatement(char* code) {
	AstStatement* stmt = (AstStatement*)calloc(1, sizeof(AstStatement));
	stmt->type = SIMPLE_STMT;
	stmt->code = code;
	stmt->sub = NULL;
	return stmt;
}

AstStatement* MakeBlockStatement() {
	AstStatement* stmt = (AstStatement*)calloc(1, sizeof(AstStatement));
	stmt->type = BLOCK_STMT;
	stmt->code = NULL;
	stmt->sub = NewList();
	return stmt;
}

AstStatement* MakeLoopStatement(StatementType type, char* test) {
	AstStatement* stmt = (AstStatement*)calloc(1, sizeof(AstStatement));
	stmt->type = type;
	stmt->code = test;
	stmt->sub = NewList();
	return stmt;
}

AstStatement* MakeIfStatement(char* test) {
	AstStatement* ifstmt = (AstStatement*)calloc(1, sizeof(AstStatement));

	ifstmt->type = IF_STMT;
	ifstmt->code = test;
	ifstmt->sub = NewList();

	AddToStatement(ifstmt, MakeLoopStatement(IF_THEN_STMT, NULL));
	AddToStatement(ifstmt, MakeLoopStatement(IF_ELSE_STMT, NULL));

	return ifstmt;
}

void ClearAstStatement(AstStatement* stmt, void* dummy) {
	if (stmt == NULL) {
		return;
	}
	stmt->type = SIMPLE_STMT;
	stmt->line = 0;
	free(stmt->code);
	stmt->code = NULL;
	if (stmt->sub) {
		ClearList(stmt->sub, (ListItemFn)ClearAstStatement);
		free(stmt->sub);
		stmt->sub = NULL;
	}
}

void DeleteAstStatement(AstStatement* stmt) {
	ClearAstStatement(stmt, NULL);
	free(stmt);
}

void PrintSimpleStatement(AstStatement* stmt, StringBuffer* buff, int indent) {
	PrintIndent(buff, indent);
	StringBuffer_addPrintf(buff, "%s\n", stmt->code);
}

void PrintLoopStatement(AstStatement* stmt, StringBuffer* buff, int indent) {
	StringBuffer* startCode = StringBuffer_new(NULL);
	StringBuffer* endCode = StringBuffer_new(NULL);
	switch (stmt->type) {
	case BLOCK_STMT:
		StringBuffer_printf(startCode, "do");
		StringBuffer_printf(endCode, "end");
		break;
	case WHILE_STMT:
		StringBuffer_printf(startCode, "while %s do", stmt->code);
		StringBuffer_printf(endCode, "end");
		break;
	case REPEAT_STMT:
		StringBuffer_printf(startCode, "repeat");
		StringBuffer_printf(endCode, "until %s", stmt->code);
		break;
	case FORLOOP_STMT:
		StringBuffer_printf(startCode, "for %s do", stmt->code);
		StringBuffer_printf(endCode, "end");
		break;
	case TFORLOOP_STMT:
		StringBuffer_printf(startCode, "for %s do", stmt->code);
		StringBuffer_printf(endCode, "end");
		break;
	}
	PrintIndent(buff, indent);
	StringBuffer_addPrintf(buff, "%s\n", StringBuffer_getRef(startCode));
	PrintAstSub(stmt->sub, buff, indent + 1);
	PrintIndent(buff, indent);
	StringBuffer_addPrintf(buff, "%s\n", StringBuffer_getRef(endCode));
	StringBuffer_delete(startCode);
	StringBuffer_delete(endCode);
}

void PrintIfStatement(AstStatement* stmt, StringBuffer* buff, int indent, int elseif) {
	AstStatement* thenstmt = cast(AstStatement*, stmt->sub->head);
	AstStatement* elsestmt = cast(AstStatement*, stmt->sub->tail);
	List* elsesub = elsestmt->sub;
	int elsesize = elsesub->size;
	AstStatement* elsefirst = cast(AstStatement*, elsesub->head);
	if (elseif) {
		PrintIndent(buff, indent);
		StringBuffer_addPrintf(buff, "elseif %s then\n", stmt->code);
	} else {
		PrintIndent(buff, indent);
		StringBuffer_addPrintf(buff, "if %s then\n", stmt->code);
	}
	PrintAstSub(thenstmt->sub, buff, indent + 1);
	if (elsesize == 0) {
		PrintIndent(buff, indent);
		StringBuffer_add(buff, "end\n");
	} else if (elsesize == 1 && elsefirst->type == IF_STMT) {
		PrintIfStatement(elsefirst, buff, indent, 1);
	} else {
		PrintIndent(buff, indent);
		StringBuffer_add(buff, "else\n");
		PrintAstSub(elsesub, buff, indent + 1);
		PrintIndent(buff, indent);
		StringBuffer_add(buff, "end\n");
	}
}

void PrintAstStatement(AstStatement* stmt, StringBuffer* buff, int indent) {
	if (stmt == NULL) {
		return;
	}
	PrintIndent(buff, indent);
	StringBuffer_addPrintf(buff, "-- stmt->type=%s stmt=%08x stmt->code=%08x stmt->sub=%08x\n", stmttype[stmt->type], stmt, stmt->code, stmt->sub);
	switch (stmt->type) {
	case SIMPLE_STMT:
		PrintSimpleStatement(stmt, buff, indent);
		break;
	case BLOCK_STMT:
	case WHILE_STMT:
	case REPEAT_STMT:
	case FORLOOP_STMT:
	case TFORLOOP_STMT:
		PrintLoopStatement(stmt, buff, indent);
		break;
	case IF_STMT:
		PrintIfStatement(stmt, buff, indent, 0);
		break;
	}
}

void AddToStatement(AstStatement* stmt, AstStatement* sub) {
	if (stmt && sub) {
		sub->parent = stmt;
		AddToList(stmt->sub, (ListItem*)sub);
	}
}