#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 
// tokenize.c
// 

// トークンの種類
typedef enum {
  TK_RESERVED, // 記号
  TK_IDENT,    // 識別子
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;
// トークン型
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
  int len;        // トークンの長さ
};

extern void error(char *fmt, ...);
extern void error_at(char *loc, char *fmt, ...);
extern bool consume(char* op);
void expect(char *op);
int expect_number();
bool at_eof();
Token *tokenize();

extern char *user_input;
extern Token *token;

//
// parse.c
//

typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_ASSIGN, // =
  ND_LVAR,   // ローカル変数
  ND_EQ,  // ==
  ND_NE,  // !=
  ND_LT,  // <
  ND_LE,  // <=
  ND_RETURN,  // return
  ND_EXPR_STMT, // 式文
  ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;
// AST node type
struct Node {
  NodeKind kind; // Node kind
  Node *next;
  Node *lhs;     // Left-hand side
  Node *rhs;     // Right-hand side
  int val;       // Used if kind == ND_NUM
  int offset;
};

Node *program();

// 
// codegen.c
// 

void codegen(Node *node);
