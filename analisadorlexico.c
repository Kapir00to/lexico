#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STRINGS 10
#define NUM_LEXEMAS 50

/*ANALISADOR LÉXICO
  Feito por Lucas Rezende Bruno
  Professor: Flavio Schiavoni
  Disciplina: Compiladores
  
  O seguinte código é livre para uso, citação e compartilhamento, 
  desde que mantida sua fonte e seu autor. */

typedef struct tok{
    char *tipo;
    char *valor;
    int linha;
    int coluna;
} token;

char **tabela_simbolos;
char palavras_reservadas[NUM_LEXEMAS][MAX_STRINGS] = {"int", "char", "float", "long", "include", "define", "struct", "return", "if", "break", "continue", "for", "while", "do", "switch", "case", "else", "typedef", "define", "NULL", "bool", ""};
char separadores[NUM_LEXEMAS][2] = {";","[","]",",","(",")","\'", "\"", "{", "}","\n", " ", "\t", "//","#"};
char operadores_aritmeticos[NUM_LEXEMAS][3] = {"+". "-", "*", "/", "++", "--", "%", "+=", "-=", "*=", "/=", "%="}
char operadores_logicos[NUM_LEXEMAS][3] = {"and", "or", "<", ">", "<=", ">=", "=", "==", "!="}

int match(const char *string, const char *pattern){  //funcao retirada do site https://www.quora.com/How-do-I-use-regular-expressions-in-the-C-programming-language
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

int e_numero (char* string){ //retorna 1 se a string e um numero valido em C, e 0 se nao e valido.
    return match(string, "[0-9]*([0-9]([uU](ll?+LL?)+(ll?+LL?)?[uU]?)+(\.[0-9]*)?([eE][+-]?[0-9]+)[fFlL])")
} 



int main(){



    return 0;
}