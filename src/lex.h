#ifndef LEX_H
#define LEX_H

static const char *TOKEN_STRING[] = {
    "PlusSign", "NegativeSign", "AstrixSign", "SlashSign", "Identifier",
    "Integer", "Float", "LeftBrace", "RightBrace", "LeftBracket", "RightBracket"
};

enum TokenType {
    ttPlus = 0,
    ttNegative,
    ttAstrix,
    ttSlash,
    ttIdentifier,
    ttInteger,
    ttFloat,
    ttLeftBrace,
    ttRightBrace,
    ttLeftBracket,
    ttRightBracket
};

struct Token {
    enum TokenType type;
    char *value;
    struct Token *next;
};

void printTokenChain(struct Token *chain);
void freeTokenChain(struct Token *chain);
struct Token *lex(char* code);
struct Token *lexFile(char* filename);

struct TokenList;

#endif
