#ifndef AST_H
#define AST_H

// Abstract syntax tree
// Maybe we should use C++

#include "StringBuffer.h"
#include "structs.h"

typedef enum StatementType_ StatementType;
enum StatementType_ {
	SIMPLE_STMT,
	BLOCK_STMT,
	WHILE_STMT,
	REPEAT_STMT,
	FORLOOP_STMT,
	TFORLOOP_STMT,
	IF_STMT
};

typedef enum BlockType_ BlockType;
enum BlockType_ {
	BLOCK_BODY,
	FUNCTION_BODY,
	WHILE_BODY,
	REPEAT_BODY,
	FORLOOP_BODY,
	TFORLOOP_BODY,
	IF_THEN_BLOCK,
	IF_ELSE_BLOCK
};

typedef struct AstBlock_ AstBlock;
typedef struct AstStatement_ AstStatement;

struct AstBlock_ {
	BlockType type;
	AstStatement* parent;
	List* statements;
};

struct AstStatement_ {
	ListItem super;
	AstBlock* parent;
	StatementType type;
	int line;
	char* code;
};

typedef struct LoopStatement_ LoopStatement;
struct LoopStatement_ {
	AstStatement super;
	AstBlock* body;
};

typedef struct IfStatement_ IfStatement;
struct IfStatement_ {
	AstStatement super;
	AstBlock* thenBlock;
	AstBlock* elseBlock;
};

AstBlock* MakeAstBlock(BlockType type);
void DeleteAstBlock(AstBlock* block);
void PrintAstBlock(AstBlock* block, StringBuffer* buff, int indent);

AstStatement* MakeSimpleStatement(char* code);
LoopStatement* MakeBlockStatement();
LoopStatement* MakeLoopStatement(StatementType type, char* test);
IfStatement* MakeIfStatement(char* test);

void ClearAstStatement(AstStatement* stmt, void* dummy);
void DeleteAstStatement(AstStatement* stmt);

void PrintAstStatement(AstStatement* stmt, StringBuffer* buff, int indent);

void AddToBlock(AstBlock* block, AstStatement* stmt);

#endif
