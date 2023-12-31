#include "9cc.h"

char *user_input;
Token *token;

// エラーを報告するための関数
// printfと同じ引数を取る
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void verror_at(char *loc, char *fmt, va_list ap){
  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, " "); // pos個の空白を出力
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// エラー箇所を報告する
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  verror_at(loc, fmt, ap);
}

void error_tok(Token *tok, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  verror_at(tok->str, fmt, ap);
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
Token *consume(char *op) {
  if (token->kind != TK_RESERVED || strlen(op) != token->len ||
       strncmp(token->str, op, token->len))
    return NULL;
  Token *t = token;
  token = token->next;
  return t;
}

// 現在のトークンと与えられた文字列がマッチしていれば true を返す
Token *peek(char *s) {
  if (token->kind != TK_RESERVED || strlen(s) != token->len || \
    strncmp(token->str, s, token->len)){
      return NULL;
  }
  return token;
}

Token *consume_ident(){
  if(token->kind != TK_IDENT) return NULL;
  Token *t = token;
  token = token->next;
  return t;
}

// 次のトークンが与えられた文字列のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char *s) {
  if (!peek(s)) {
    error_tok(token, "\"%s\" ではありません", s);
  }
  token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number() {
  if (token->kind != TK_NUM)
    error_tok(token, "数ではありません");
  int val = token->val;
  token = token->next;
  return val;
}

// 次のトークンが変数の場合、トークンを1つ読み進めてその変数名を返す。
// それ以外の場合にはエラーを報告する。
char *expect_ident() {
  if (token->kind != TK_IDENT) {
    error_tok(token, "expected an identifier");
  }
  char *s = strndup(token->str, token->len);
  token = token->next;
  return s;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  tok->len = len;
  cur->next = tok;
  return tok;
}

  // p が q で始まる
bool startswith(char *p, char *q){
  return strncmp(p, q, strlen(q)) == 0;
}

bool is_alpha(char c){
  return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c=='_');
}

bool is_alnum(char c){
  return is_alpha(c) || ('0' <= c && c <= '9');
}

char *starts_with_reserved(char *p){
  static char *kw[] = {"return", "if", "else", "while", "for", "int", "char", "sizeof"};

  for(int i=0; i<sizeof(kw)/sizeof(*kw); i++) {
    int len = strlen(kw[i]);
    if (startswith(p, kw[i]) && !is_alnum(p[len])) {
      return kw[i];
    }
  }

  static char *ops[] = {"==", "!=", "<=", ">="};

  for(int i=0; i<sizeof(ops)/sizeof(*ops); i++) {
    if(startswith(p, ops[i])) {
      return ops[i];
    }
  }

  return NULL;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize() {
  char *p = user_input;
  Token head = {};
  Token *cur = &head;

  while (*p) {
    // 空白文字をスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    // 文字列リテラル
    if (*p == '"') {
      char *q = p++;
      while (*p && *p != '"') p++;
      if (!*p) error_at(q, "文字列リテラルが閉じられていません");
      p++;

      cur = new_token(TK_STR, cur, q, p-q);
      cur->contents = strndup(q+1, p-q-2);
      cur->cont_len = p-q-1;
      continue;
    }

    // 予約語と演算子を同時に処理
    char *kw = starts_with_reserved(p);
    if (kw) {
      int len = strlen(kw);
      cur = new_token(TK_RESERVED, cur, p, len);
      p += len;
      continue;
    }

    // 変数
    if (is_alpha(*p)) {
      char *q = p++;
      while (is_alnum(*p)) p++;
      cur = new_token(TK_IDENT, cur, q, p - q);
      continue;
    }

    // Single-letter punctuators
    if (ispunct(*p)) {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p, 0);
      char *q = p;
      cur->val = strtol(p, &p, 10);
      cur->len = p - q;
      continue;
    }

    error("トークナイズできません");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}