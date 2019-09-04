#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STRINGS 10
#define NUM_LEXEMAS_RESERVADAS 26
#define NUM_LEXEMAS_SEPARADORES 13
#define NUM_LEXEMAS_OP_ARITMETICOS 12
#define NUM_LEXEMAS_OP_LOGICOS 9
#define MAX_TOKENS 12000


/*ANALISADOR LÉXICO
  Feito por Lucas Rezende Bruno
  Professor: Flavio Schiavoni
  Disciplina: Compiladores
  
  O seguinte código é livre para uso, citação e compartilhamento, 
  desde que mantida sua fonte e seu autor. */

typedef struct tok{
    char tipo[MAX_STRINGS];
    char valor[MAX_STRINGS];
    int linha;
    int coluna;
} token_type;

// regex para identificador: /^[a-zA-Z_][a-zA-Z0-9_]*$/
char **tabela_simbolos;
char palavras_reservadas[NUM_LEXEMAS_RESERVADAS][MAX_STRINGS] = {"int", "char", "float", "long", "include", "define", "struct", "return", "if", "break", "continue", "for", "while", "do", "switch", "case", "else", "typedef", "define", "NULL", "bool", "EOF", "void", "printf", "fopen", "FILE", "const"};
char separadores[NUM_LEXEMAS_SEPARADORES][2] = {";","[","]",",","(",")", "{", "}","\n", " ", "\t","#", "&"};
char operadores_aritmeticos[NUM_LEXEMAS_OP_ARITMETICOS][3] = {"+", "-", "*", "/", "++", "--", "%", "+=", "-=", "*=", "/=", "%="};
char operadores_logicos[NUM_LEXEMAS_OP_LOGICOS][3] = {"||", "&&", "<", ">", "<=", ">=", "=", "==", "!="};

int match(const char *string, const char *pattern){  //funcao retirada do site https://www.quora.com/How-do-I-use-regular-expressions-in-the-C-programming-language
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

int e_numero (char* string){ //retorna 1 se a string e um numero valido em C, e 0 se nao e valido.
    return match(string, "[0-9]*([0-9]([uU](ll?+LL?)+(ll?+LL?)?[uU]?)+(\.[0-9]*)?([eE][+-]?[0-9]+)[fFlL])");
} 

int e_identificador (char* string){
  return match(string, "/^[a-zA-Z_][a-zA-Z0-9_]*$/");
}
int busca(char* palavra, int tipo ){  //verifica se a palavra e do determinado tipo. 0 para reservada, 1 para separadores, 2 para operadores aritmeticos, 3 para operadores logicos
    int i;
    
    if (tipo == 0)
      for(i = 0; i < NUM_LEXEMAS_RESERVADAS; i++){
        if (strcmp(palavras_reservadas[i], palavra) == 0)
          return 1;
      }

    else if (tipo == 1)
      for(i = 0; i < NUM_LEXEMAS_SEPARADORES; i++){
        if (strcmp(separadores[i], palavra) == 0)
          return 1;
      }

    else if (tipo == 2)
      for(i = 0; i < NUM_LEXEMAS_OP_ARITMETICOS; i++){
        if (strcmp(operadores_aritmeticos[i], palavra) == 0)
          return 1;
      }

    else if (tipo == 3)
      for(i = 0; i < NUM_LEXEMAS_OP_LOGICOS; i++){
        if (strcmp(operadores_logicos[i], palavra) == 0)
          return 1;
      }      
}


void lexico(){
  FILE *codigo_fonte, *sep, *reservadas, *operadores, *simbolos, *literais, *numeros; 
  codigo_fonte = fopen("codigo.txt", "r");

  if (codigo_fonte == NULL) printf("Arquivo nao encontrado\n");
  
  sep = fopen("separadores.txt", "w");
  reservadas = fopen("reservadas.txt", "w");
  operadores = fopen("operadores.txt", "w");
  simbolos = fopen("simbolos.txt", "w");
  literais = fopen("literais.txt", "w");
  numeros = fopen("numeros.txt", "w");
  char palavra[MAX_STRINGS] = "";
  int linha = 0, coluna = 0;
  char ch;
  token_type lista_de_tokens[MAX_TOKENS];
  int tamanho_lista_de_tokens = 0;
  int cabou_arquivo = 0;

  while((ch = fgetc(codigo_fonte)) != EOF){
    char buffer[2];
    buffer[0] = ch;
    buffer[1] = '\0';

    coluna++;
    if(ch == '\n'){
      coluna = 0;
      linha++;
    }
   
    if (busca(buffer, 1) == 1){
      token_type token;
      strcpy(token.tipo, "separador");
      strcpy(token.valor, buffer);
      token.linha = linha;
      token.coluna = coluna; 
      int aux_linha = token.linha;
      int aux_coluna = token.coluna - strlen(token.valor);
      if (buffer[0] == '\n') {
        strcpy(token.valor, "\\n");
        aux_coluna = token.coluna;
      }  
      fprintf(sep, "%s\n", token.valor);
      printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, aux_linha, aux_coluna);
      lista_de_tokens[tamanho_lista_de_tokens++] = token;
    }
    
    else if(ch == '\"'){
      buffer[0] = ch;
      strcat(palavra, buffer);
      
      do{
        if((ch = fgetc(codigo_fonte)) == EOF) {
          cabou_arquivo = 1;
          break;
        }
        coluna++;
        if (ch == '\n'){
          linha++;
          coluna = 0;
        }
        buffer[0] = ch;
        strcat(palavra, buffer);
      } while(ch != '\"');
      
      if(cabou_arquivo == 1){
        printf("ERRO LEXICO: Sequencia invalida em (%d, %d): %s",linha, coluna - strlen(palavra), palavra);
        break;
      } 
      token_type token;
      strcpy(token.tipo, "literal");
      strcpy(token.valor, palavra);
      token.linha = linha;
      token.coluna = coluna; 
      fprintf(literais, "%s\n", buffer);
      printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna - strlen(token.valor));
      lista_de_tokens[tamanho_lista_de_tokens++] = token;
      strcpy(palavra, "");
    }

    else if(ch == '\''){
      buffer[0] = ch;
      strcat(palavra, buffer);
      
      do{
        
        if((ch = fgetc(codigo_fonte)) == EOF) {
          cabou_arquivo = 1;
          break;
        }
        
        coluna++;
        
        if (ch == '\n'){
          linha++;
          coluna = 0;
        }
        buffer[0] = ch;
        strcat(palavra, buffer);
      
      } while(ch != '\'');
      
      if(cabou_arquivo == 1){
        printf("ERRO LEXICO: Sequencia invalida em (%d, %d): %s",linha, coluna - strlen(palavra), palavra);
        break;
      }
      token_type token;
      strcpy(token.tipo, "literal");
      strcpy(token.valor, palavra);
      token.linha = linha;
      token.coluna = coluna; 
      fprintf(literais, "%s\n", buffer);
      printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna - strlen(token.valor));
      lista_de_tokens[tamanho_lista_de_tokens++] = token;
      strcpy(palavra, "");
    
    } else if ((ch >= '0' && ch <= '9')){
        
        buffer[0] = ch;
        strcat(palavra, ch);
        
        while(true){
         
          if((ch = fgetc(codigo_fonte)) == EOF) 
            break;

          buffer[0] = ch;
          coluna++;
          
          if (ch == '\n'){
            linha++;
            coluna = 0;
          }
          
          if(busca(buffer, 1) == 1)
            break;

          strcat(palavra, ch);          
        }

        if(e_numero(palavra)){
          token_type token;
          strcpy(token.tipo, "numero");
          strcpy(token.valor, palavra);
          token.linha = linha;
          token.coluna = coluna; 
          fprintf(numeros, "%s\n", buffer);
          printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna - strlen(token.valor));
          lista_de_tokens[tamanho_lista_de_tokens++] = token;
          strcpy(palavra, "");
        }


    } else{
      buffer[0] = ch;
      strcat(palavra, ch);
      while(true){
         
          if((ch = fgetc(codigo_fonte)) == EOF) 
            break;

          buffer[0] = ch;
          coluna++;
          
          if (ch == '\n'){
            linha++;
            coluna = 0;
          }
          
          if(busca(buffer, 1) == 1)
            break;

          strcat(palavra, ch);          
        }

      if (busca(palavra, 0){
          token_type token;
          strcpy(token.tipo, "palavra reservada");
          strcpy(token.valor, palavra);
          token.linha = linha;
          token.coluna = coluna; 
          fprintf(reservadas, "%s\n", buffer);
          printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna - strlen(token.valor));
          lista_de_tokens[tamanho_lista_de_tokens++] = token;
          strcpy(palavra, "");
     
      } else if (busca(palavra, 2) == 1 || busca(palavra, 3) == 1){
          token_type token;
          strcpy(token.tipo, "operador");
          strcpy(token.valor, palavra);
          token.linha = linha;
          token.coluna = coluna; 
          fprintf(operadores, "%s\n", buffer);
          printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna - strlen(token.valor));
          lista_de_tokens[tamanho_lista_de_tokens++] = token;
          strcpy(palavra, "");
        }
        else{
          printf("ERRO LEXICO: Sequencia invalida em (%d %d): %s",linha, coluna - strlen(palavra), palavra);
          strcpy(palavra, "");
        }

      } 
    }
         

       
      

  }

}


int main(){

    lexico();

    return 0;
}