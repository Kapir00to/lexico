#ifndef ANALISADORLEXICO_H
#define ANALISADORLEXICO_H

#define MAX_STRINGS 256
#define NUM_LEXEMAS_RESERVADAS 26
#define NUM_LEXEMAS_SEPARADORES 13
#define NUM_LEXEMAS_OP_ARITMETICOS 12
#define NUM_LEXEMAS_OP_LOGICOS 10
#define MAX_TOKENS 12000

typedef struct tok{
    char tipo[MAX_STRINGS];
    char valor[MAX_STRINGS];
    int linha;
    int coluna;
} token_type;

int match(const char *string, const char *pattern);
int e_numero (char* string);
int e_identificador (char* string);
int busca(char* palavra, int tipo );
void lexico(char arqv[]);
int main(int argc, char* argv[ ]);
#endif