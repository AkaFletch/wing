#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"

int isInt(char c) {
    return c < 58 && c >  47;
}

int isLetter(char c) {
    int lower = c < 91 && c > 64;
    c = c ^ ' ';
    int higher = c < 91 && c > 64;
    return lower || higher;
}

int isWhitespace(char c) {
    char whitespace[4] = {
        ' ',
        '\n',
        '\t',
        '\r'
    };
    int i;
    int size = sizeof(whitespace)/sizeof(char);
    for(i = 0; i < size; i++) {
        if(c == whitespace[i]) {
            return 1;
        }
    }
    return 0;
}

void freeTokenChain(struct Token *chain) {
    if(!chain) {
        return;
    }
    freeTokenChain(chain->next);
    free(chain->next);
}

void printTokenChain(struct Token *chain) {
    while(chain) {
        printf("Token<%s>: %s\n", TOKEN_STRING[chain->type], chain->value);
        chain = chain->next;
    }
}

struct Token *handleIdentifier(char *code, int *pos, struct Token *chain) {
    char* buf = malloc(32*sizeof(char));
    int startPos = *pos;
    chain->type = ttIdentifier;
    buf[*pos-startPos] = code[*pos];
    *pos = *pos+1;
    while(isLetter(code[*pos])) {
        if(*pos-startPos > 32) {
            return 0;
        }
        buf[*pos-startPos] = code[*pos];
        *pos = *pos+1;
    }
    chain->value = buf;
    return chain;
}

struct Token *handleInt(char *code, int *pos, struct Token *chain) {
    char* buf = malloc(32*sizeof(char));
    int startPos = *pos;
    int dot = 0;
    chain->type = ttInteger;
    buf[*pos-startPos] = code[*pos];
    *pos = *pos+1;
    while(isInt(code[*pos]) || code[*pos] == '.') {
        if(*pos-startPos > 32) {
            return 0;
        }
        if(code[*pos] == '.') {
            dot++;
            chain->type = ttFloat;
        }
        if(dot > 1 ){
            printf("Lex error: position %d two dots in number", *pos);
            return 0;
        }
        buf[*pos-startPos] = code[*pos];
        *pos = *pos+1;
    }
    chain->value = buf;
    return chain;
}

struct Token *recogniseToken(char* code, int *pos) {
    struct Token *newToken = malloc(sizeof(struct Token));
    newToken->next = 0;
    char *character = malloc(sizeof(char)); 
    *character = code[*pos];
    newToken->value = character;
    if(isInt(*character)) {
        return handleInt(code, pos, newToken);
    }
    if(isLetter(*character)) {
        return handleIdentifier(code, pos, newToken);
    }
    switch(code[*pos]){
        case '+': {
            newToken->type = ttPlus;
            break;
        }
        case '-': {
            newToken->type = ttNegative;
            break;
        }
        case '*': {
            newToken->type = ttAstrix;
            break;
        }
        case '/': {
            newToken->type = ttSlash;
            break;
        }
        case '{': {
            newToken->type = ttLeftBrace;
            break;
        }
        case '}': {
            newToken->type = ttRightBrace;
            break;
        }
        case '(': {
            newToken->type = ttLeftBracket;
            break;
        }
        case ')': {
            newToken->type = ttRightBracket;
            break;
        }
        default: {
            return 0;
        }
    }
    return newToken;
}


struct Token *lex(char* code) {
    char c = code[0];
    int pos = 0;
    while(strcmp(&c, "\0") != 0 && isWhitespace(c)) {
        pos++;
        c = code[pos];
    }
    struct Token *tokenChain = recogniseToken(code, &pos);
    struct Token *tailToken = tokenChain; 
    pos++;
    c = code[pos];
    while(strcmp(&c, "\0") != 0) {
        if(isWhitespace(c) == 1) {
            pos++;
            c = code[pos];
            continue; 
        }
        struct Token *newTailToken = recogniseToken(code, &pos);
        if(newTailToken==0) {
            printf("Failed to lex at position %d %s\n", pos, &c);
            return 0;
        }
        tailToken->next = newTailToken;
        tailToken = newTailToken;
        pos++;
        c = code[pos];
    }
    return tokenChain;
}

struct Token *lexFile(char* filename) {
    FILE *file;
    char buff[255];
    snprintf(buff, strlen(filename)+1, "%s", filename);
    file = fopen(buff, "r");
    if(file == 0) {
        printf("Couldn't open file %s\n", buff);
        return 0;
    }
    // TODO @Matt: This is a tiny buffer
    char c;
    int pos = 0;
    while(1) {
        c = fgetc(file);
        if(feof(file)) { 
            buff[pos] = '\0';
            break;
        }
        buff[pos] = c;
        pos++;
    }
    fclose(file);
    return lex(buff);
}
