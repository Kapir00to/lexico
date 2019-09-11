#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STRINGS 256
#define NUM_LEXEMAS_RESERVADAS 26
#define NUM_LEXEMAS_SEPARADORES 13
#define NUM_LEXEMAS_OP_ARITMETICOS 12
#define NUM_LEXEMAS_OP_LOGICOS 10
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
char palavras_reservadas[NUM_LEXEMAS_RESERVADAS][MAX_STRINGS] = {"int", "char", "float", "long", "include", "define", "struct", "return", "if", "break", "continue", "for", "while", "do", "switch", "case", "else", "typedef", "define", "NULL", "bool", "EOF", "void", "FILE", "const"};
char separadores[NUM_LEXEMAS_SEPARADORES][2] = {";","[","]",",","(",")", "{", "}","\n", " ", "\t","#", "&"};
char operadores_aritmeticos[NUM_LEXEMAS_OP_ARITMETICOS][3] = {"+", "-", "*", "/", "++", "--", "%", "+=", "-=", "*=", "/=", "%="};
char operadores_logicos[NUM_LEXEMAS_OP_LOGICOS][3] = {"||", "&&", "<", ">", "<=", ">=", "=", "==", "!=", "!"};

int match(const char *string, const char *pattern){  //funcao retirada do site https://www.quora.com/How-do-I-use-regular-expressions-in-the-C-programming-language
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

int e_numero (char* string){ //retorna 1 se a string e um numero valido em C, e 0 se nao e valido.
    return match(string, "[0-9]+|[.][0-9]+|[0-9]+[.][0-9]+");
} 

int e_identificador (char* string){
  return match(string, "^[a-zA-Z_][a-zA-Z0-9_.$]*$");
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
  FILE *codigo_fonte, *sep, *reservadas, *operadores, *identificadores, *literais, *numeros; 
  codigo_fonte = fopen("codigo.c", "r");

  if (codigo_fonte == NULL){
    printf("Arquivo nao encontrado\n");
    return;
  } 
  
  sep = fopen("separadores.txt", "w");
  reservadas = fopen("reservadas.txt", "w");
  operadores = fopen("operadores.txt", "w");
  identificadores = fopen("identificadores.txt", "w");
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
    strcat(palavra, buffer);

    coluna++;
    if(ch == '\n'){
      coluna = 0;
      linha++;
    }
   
    if (busca(palavra, 1) == 1){
    
      token_type token;
      strcpy(token.tipo, "separador");
      strcpy(token.valor, palavra);
      token.linha = linha;
      token.coluna = coluna; 
      int aux_linha = token.linha;
      int aux_coluna = token.coluna;
      if (buffer[0] == '\n') {
        strcpy(token.valor, "\\n");
        aux_coluna = token.coluna;
      }  
      fprintf(sep, "%s\n", token.valor);
      printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, aux_linha, aux_coluna);
      lista_de_tokens[tamanho_lista_de_tokens++] = token;
      strcpy(palavra, "");
      
    }  
    
    else if (busca(buffer, 2) == 1 || busca(buffer, 3) == 1){
      fpos_t pos;
      fgetpos(codigo_fonte, &pos);
      char palavra_auxiliar[MAX_STRINGS];
      strcat(palavra_auxiliar, buffer);      
      strcat(palavra, buffer);
      int coluna_auxiliar = coluna;
      int linha_auxiliar = linha;
      int flag = 0;
      
      if(ch == '/'){          //capturando comentarios
        ch = fgetc(codigo_fonte);
        if (ch == '/'){
          coluna += 2;
          while(ch != '\n'){
            ch = fgetc(codigo_fonte);
            coluna++;          
          }
          coluna = 0;
          linha++;
          flag = 1;
        
        } else if (ch = '*'){
            
          while(1){
            
            ch = fgetc(codigo_fonte);
            coluna++;
            
            if(ch == '\n'){
            
              coluna = 0;
              linha++;
            
            } else if(ch == '*'){

              ch = fgetc(codigo_fonte);
              coluna++;
              
              if(ch == '/'){
                flag = 1;
                break;
              }
            }
          }
        }
      }
      
      if (flag){
        strcpy(palavra, "");
        continue;
      } 


      ch = fgetc(codigo_fonte);
     
      if (ch == EOF){     
        token_type token;
        strcpy(token.tipo, "operador");
        strcpy(token.valor, palavra);
        token.linha = linha;
        token.coluna = coluna;      
        fprintf(operadores, "%s\n", token.valor);
        printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
        lista_de_tokens[tamanho_lista_de_tokens++] = token;
        strcpy(palavra, "");
        strcpy(palavra_auxiliar,"");
        break;
      }

      buffer[0] = ch;
      strcat(palavra_auxiliar, buffer);
      coluna_auxiliar++;
      
      if(ch == '\n'){
        coluna_auxiliar = 0;
        linha_auxiliar++;
      }

      if (busca(palavra_auxiliar, 2) == 1 || busca(palavra_auxiliar, 3) == 1){
       
        token_type token;
        strcpy(token.tipo, "operador");
        strcpy(token.valor, palavra_auxiliar);
        token.linha = linha_auxiliar;
        token.coluna = coluna_auxiliar;      
        fprintf(operadores, "%s\n", token.valor);
        printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
        lista_de_tokens[tamanho_lista_de_tokens++] = token;
        strcpy(palavra_auxiliar,"");
        strcpy(palavra,"");
        linha = linha_auxiliar;
        coluna = coluna_auxiliar;
      } else{
        
        token_type token;
        strcpy(token.tipo, "operador");
        palavra[strlen(palavra) - 1] = '\0';
        strcpy(token.valor, palavra);
        token.linha = linha_auxiliar;
        token.coluna = coluna_auxiliar;      
        fprintf(operadores, "%s\n", token.valor);
        printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, coluna);
        lista_de_tokens[tamanho_lista_de_tokens++] = token;
        strcpy(palavra_auxiliar, ""); 
        strcpy(palavra, "");
        fsetpos(codigo_fonte, &pos);
      }



    }
    
    else if(ch == '\"'){
      int linha_token_inicial = linha;
      int coluna_token_inicial = coluna;
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
        printf("ERRO LEXICO: Sequencia invalida em (%d, %d): %s - FLAG 1\n",linha, coluna - strlen(palavra), palavra);
        break;
      } 
      token_type token;
      strcpy(token.tipo, "literal");
      strcpy(token.valor, palavra);
      token.linha = linha_token_inicial;
      token.coluna = coluna_token_inicial; 
      fprintf(literais, "%s\n", token.valor);
      printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
      lista_de_tokens[tamanho_lista_de_tokens++] = token;
      strcpy(palavra, "");
    }

    else if(ch == '\''){
      int linha_token_inicial = linha;
      int coluna_token_inicial = coluna;
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
        printf("ERRO LEXICO: Sequencia invalida em (%d, %d): %s - FLAG 2\n",linha, coluna - strlen(palavra), palavra);
        break;
      }
      token_type token;
      strcpy(token.tipo, "literal");
      strcpy(token.valor, palavra);
      token.linha = linha_token_inicial;
      token.coluna = coluna_token_inicial; 
      fprintf(literais, "%s\n", token.valor);
      printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
      lista_de_tokens[tamanho_lista_de_tokens++] = token;
      strcpy(palavra, "");    
    } else if ((ch >= '0' && ch <= '9')){
      int linha_token_inicial = linha;
      int coluna_token_inicial = coluna;
        while(1){
         
          if((ch = fgetc(codigo_fonte)) == EOF) 
            break;

          buffer[0] = ch;
          coluna++;
          
          if (ch == '\n'){
            linha++;
            coluna = 0;
          }
          
        if((ch >= 'a' && ch <= 'z')){
          buffer[0] = ch;
          strcat(palavra, buffer);
          printf("ERRO LEXICO: Sequencia invalida em (%d %d): %s - FLAG 3\n",linha, coluna - strlen(palavra), palavra);
          strcpy(palavra, "");
          break;
        }  

          if(!(ch >= '0' && ch <= '9') && ch != '.')
            break;

          strcat(palavra, buffer);          
        }

        if(e_numero(palavra)){
          token_type token;
          strcpy(token.tipo, "numero");
          strcpy(token.valor, palavra);
          token.linha = linha_token_inicial;
          token.coluna = coluna_token_inicial; 
          fprintf(numeros, "%s\n", token.valor);
          printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
          lista_de_tokens[tamanho_lista_de_tokens++] = token;
          strcpy(palavra, "");
          fseek(codigo_fonte, -1, SEEK_CUR);
        }


    } else{
      
      int linha_token_inicial = linha;
      int coluna_token_inicial = coluna;
     
      while(1){
         
        if((ch = fgetc(codigo_fonte)) == EOF) 
          break;

        buffer[0] = ch;
        coluna++;
        
        if (ch == '\n'){
          linha++;
          coluna = 0;
        }
        
        if((busca(buffer, 0) == 1) || (busca(buffer, 1) == 1) || (busca(buffer, 2) == 1)  || (busca(buffer, 3) == 1))
          break;

        strcat(palavra, buffer); 
              
      }

      if (busca(palavra, 0) == 1){
        
          token_type token;
          strcpy(token.tipo, "palavra reservada");
          strcpy(token.valor, palavra);
          token.linha = linha_token_inicial;
          token.coluna = coluna_token_inicial; 
          fprintf(reservadas, "%s\n", token.valor);
          printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
          lista_de_tokens[tamanho_lista_de_tokens++] = token;
          strcpy(palavra, "");
          fseek(codigo_fonte, -1, SEEK_CUR);
     
      } 
        else if (e_identificador(palavra)){
         
          token_type token;
          strcpy(token.tipo, "identificador");
          strcpy(token.valor, palavra);
          token.linha = linha_token_inicial;
          token.coluna = coluna_token_inicial; 
          fprintf(identificadores, "%s\n", token.valor);
          printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
          lista_de_tokens[tamanho_lista_de_tokens++] = token;
          strcpy(palavra, "");
          fseek(codigo_fonte, -1, SEEK_CUR);

        } else{

          printf("ERRO LEXICO: Sequencia invalida em (%d %d): %s - FLAG 3\n",linha_token_inicial, coluna_token_inicial, palavra);
          strcpy(palavra, "");
          fseek(codigo_fonte, -1, SEEK_CUR);

        }

    } 
    
         

       
      

  }

}


int main(){

    lexico();

    return 0;
}