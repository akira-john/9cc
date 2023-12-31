#define _GNU_SOURCE
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Type Type;

// 
// tokenize.c
// 

// トークンの種類
typedef enum {
  TK_RESERVED, // 記号
  TK_IDENT,    // 識別子
  TK_STR,    // 文字列
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

  char *contents;
  char cont_len;
};

extern void error(char *fmt, ...);
extern void error_at(char *loc, char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);
Token *consume(char *op);
Token *peek(char *s);
extern Token *consume_ident();
void expect(char *op);
int expect_number();
char *expect_ident();
bool at_eof();
Token *tokenize();

extern char *user_input;
extern Token *token;

//
// parse.c
//

// 変数
typedef struct Var Var;
struct Var {
  char *name;
  Type *ty;
  bool is_local;

  // Local variable
  int offset; // Offset from RBP

  // グローバル変数
  char *contents;
  int cont_len;
};

typedef struct VarList VarList;
struct VarList {
  VarList *next;
  Var *var;
};

typedef enum {
  ND_ADD, // +
  ND_PTR_ADD, // num + ptr
  ND_SUB, // -
  ND_PTR_SUB, // num - ptr
  ND_PTR_DIFF,  // ptr - ptr
  ND_MUL, // *
  ND_DIV, // /
  ND_EQ,  // ==
  ND_NE,  // !=
  ND_LT,  // <
  ND_LE,  // <=
  ND_ASSIGN,  // =
  ND_ADDR,    // unary &
  ND_DEREF,   // unary *
  ND_RETURN,  // return
  ND_IF,  // "if"
  ND_WHILE,  // "while"
  ND_FOR,  // "for"
  ND_BLOCK,  // "{...}"
  ND_FUNCALL,  // Function call
  ND_EXPR_STMT, // 式文
  ND_VAR, // 変数
  ND_NUM, // 整数
  ND_NULL, // 空
} NodeKind;

typedef struct Node Node;
// AST node type
struct Node {
  NodeKind kind; // Node kind
  Node *next;
  Type *ty;
  Token *tok;

  Node *lhs;     // Left-hand side
  Node *rhs;     // Right-hand side

  // "if | while | for"
  Node *cond;
  Node *then;
  Node *els;
  Node *init;
  Node *inc;

  // Block
  Node *body;

  char *funcname;
  Node *args;

  Var *var;     // Used if kind == ND_VAR
  int val;      // Used if kind == ND_NUM
};

typedef struct Function Function;
struct Function {
  Function *next;
  char *name;
  VarList *params;

  Node *node;
  VarList *locals;
  int stack_size;
};

typedef struct {
  VarList *globals;
  Function *fns;
} Program;

Program *program();

// 
// typing.c
// 

typedef enum {
  TY_CHAR,
  TY_INT,
  TY_PTR,
  TY_ARRAY,
} TypeKind;

struct Type {
  TypeKind kind;
  int size;
  Type *base;
  int array_len;
};

extern Type *int_type;
extern Type *char_type;

bool is_integer(Type *ty);
Type *pointer_to(Type *base);
Type *array_of(Type *base, int size);
void add_type(Node *node);

// 
// codegen.c
// 

void codegen(Program *prog);
