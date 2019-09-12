#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "analisadorlexico.h"



/*ANALISADOR LÉXICO
  Feito por Lucas Rezende Bruno
  Professor: Flavio Schiavoni
  Disciplina: Compiladores
  
  O seguinte código é livre para uso, citação e compartilhamento, 
  desde que mantida sua fonte e seu autor. */


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

int e_identificador (char* string){ ////retorna 1 se a string e um identificador valido em C, e 0 se nao e valido.
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




void lexico(char arqv[]){
  FILE *codigo_fonte, *sep, *reservadas, *operadores, *identificadores, *literais, *numeros, *tokens; 
  codigo_fonte = fopen(arqv, "r");

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
  tokens = fopen("tokens.txt", "w");
  char palavra[MAX_STRINGS] = "";
  int linha = 0, coluna = -1;
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

    
   
    if (busca(palavra, 1) == 1){  //verificada se o caractere lido um separador, se sim ira salvar o token
    
      token_type token;
      strcpy(token.tipo, "separador");
      strcpy(token.valor, palavra);
      token.linha = linha;
      token.coluna = coluna; 

      if(ch == '\n'){
      
      coluna = 0;
      strcpy(token.valor, "\\n");
      
      } else if (buffer[0] == ' ') {
       
        strcpy(token.valor, "espaco");
      
      }  
      fprintf(sep, "%s\n", token.valor);
      printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
      lista_de_tokens[tamanho_lista_de_tokens++] = token;
      fprintf(tokens, "%s %s %d %d\n", token.tipo, token.valor, token.linha, token.coluna);
      strcpy(palavra, "");
      
    }  
    
    else if (busca(buffer, 2) == 1 || busca(buffer, 3) == 1){ //verifica se o caractere e um operador. Se sim, e verificado se e um operador de um ou dois caracteres
      fpos_t pos;
      fgetpos(codigo_fonte, &pos);
      char palavra_auxiliar[MAX_STRINGS];
      strcat(palavra_auxiliar, buffer);      
      strcat(palavra, buffer);
      coluna++;
      int flag = 0;
      
      if(ch == '/'){          //capturando comentarios
        ch = fgetc(codigo_fonte);
        coluna++;
        
        if (ch == '/'){
          coluna++;
        
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
        } else{

          fseek(codigo_fonte, -1, SEEK_CUR);

        }
      }
      
      if (flag){
        strcpy(palavra, "");
        continue;
      } 


      ch = fgetc(codigo_fonte);
     
      if (ch == EOF){     //ultimo caractere
        token_type token;
        strcpy(token.tipo, "operador");
        strcpy(token.valor, palavra);
        token.linha = linha;
        token.coluna = coluna;      
        fprintf(operadores, "%s\n", token.valor);
        printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
        lista_de_tokens[tamanho_lista_de_tokens++] = token;
        fprintf(tokens, "%s %s %d %d\n", token.tipo, token.valor, token.linha, token.coluna);
        strcpy(palavra, "");
        strcpy(palavra_auxiliar,"");
        break;
      }

      buffer[0] = ch;
      strcat(palavra_auxiliar, buffer);
      coluna++;
      int coluna_aux = coluna;
      if(ch == '\n'){
        coluna = 0;
        linha++;
      }

      if (busca(palavra_auxiliar, 2) == 1 || busca(palavra_auxiliar, 3) == 1){  //operador com 2 caracteres
       
        token_type token;
        strcpy(token.tipo, "operador");
        strcpy(token.valor, palavra_auxiliar);
        token.linha = linha;
        token.coluna = coluna - 2;      
        fprintf(operadores, "%s\n", token.valor);
        printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
        lista_de_tokens[tamanho_lista_de_tokens++] = token;
        fprintf(tokens, "%s %s %d %d\n", token.tipo, token.valor, token.linha, token.coluna);
        strcpy(palavra_auxiliar,"");
        strcpy(palavra,"");
      } else{         //operador com 1 caractere
        
        token_type token;
        strcpy(token.tipo, "operador");
        palavra[strlen(palavra) - 1] = '\0';
        strcpy(token.valor, palavra);
        
        if (ch != '\n'){
          token.linha = linha;
          token.coluna = coluna - 2;
        } else{
          token.linha = linha - 1;
          token.coluna = coluna_aux - 3;
        }
        
        fprintf(operadores, "%s\n", token.valor);
        printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
        lista_de_tokens[tamanho_lista_de_tokens++] = token;
        fprintf(tokens, "%s %s %d %d\n", token.tipo, token.valor, token.linha, token.coluna);
        strcpy(palavra_auxiliar, ""); 
        strcpy(palavra, "");
        fsetpos(codigo_fonte, &pos);
      }

    }
    
    else if(ch == '\"'){  //verifica se ira comecar um literal a partir do caractere ch
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
      
      if(cabou_arquivo == 1){ //da erro caso o codigo acabe antes de se fechar as aspas do literal
        printf("ERRO LEXICO: Sequencia invalida em (%d, %d): %s - FLAG 1\n",linha, coluna - strlen(palavra), palavra);
        break;
      } 
            //guardando o literal
      token_type token;
      strcpy(token.tipo, "literal");
      strcpy(token.valor, palavra);
      token.linha = linha_token_inicial;
      token.coluna = coluna_token_inicial; 
      fprintf(literais, "%s\n", token.valor);
      printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
      lista_de_tokens[tamanho_lista_de_tokens++] = token;
      fprintf(tokens, "%s %s %d %d\n", token.tipo, token.valor, token.linha, token.coluna);
      strcpy(palavra, "");
    
    }

    else if(ch == '\''){    //verifica se e um literal de aspas simples, ou seja, char(nao verifica se as aspas simples representa mais de um caractere)
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
      
      if(cabou_arquivo == 1){ //verifica se o codigo acaba antes de ser fechada a aspa
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
      fprintf(tokens, "%s %s %d %d\n", token.tipo, token.valor, token.linha, token.coluna);
      strcpy(palavra, "");    

    } else if ((ch >= '0' && ch <= '9')){ //verifica se e um numero
     
      int linha_token_inicial = linha;
      int coluna_token_inicial = coluna;
     
        while(1){
         
          if((ch = fgetc(codigo_fonte)) == EOF) 
            break;

          buffer[0] = ch;
          coluna++;
          
          
        if((ch >= 'a' && ch <= 'z')){ //numero seguido de letra, sequencia invalida
          buffer[0] = ch;
          strcat(palavra, buffer);
          printf("ERRO LEXICO: Sequencia invalida em (%d %d): %s - FLAG 3\n",linha, coluna - strlen(palavra), palavra);
          strcpy(palavra, "");
          break;
        }  

          if(!(ch >= '0' && ch <= '9') && ch != '.')    //caso o numero acabe
            break;

          strcat(palavra, buffer);          
        }

        if(e_numero(palavra)){    //registra o token do numero
          token_type token;
          strcpy(token.tipo, "numero");
          strcpy(token.valor, palavra);
          token.linha = linha_token_inicial;
          token.coluna = coluna_token_inicial; 
          fprintf(numeros, "%s\n", token.valor);
          printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
          lista_de_tokens[tamanho_lista_de_tokens++] = token;
          fprintf(tokens, "%s %s %d %d\n", token.tipo, token.valor, token.linha, token.coluna);
          strcpy(palavra, "");
          fseek(codigo_fonte, -1, SEEK_CUR);
        }


    } else{   //se nao cair em nenhum dos ifs, vai verificar se e uma palavra reservada ou identificador
      
      int linha_token_inicial = linha;
      int coluna_token_inicial = coluna;
     
      while(1){
         
        if((ch = fgetc(codigo_fonte)) == EOF) 
          break;

        buffer[0] = ch;
        coluna++;
        
        if((busca(buffer, 0) == 1) || (busca(buffer, 1) == 1) || (busca(buffer, 2) == 1)  || (busca(buffer, 3) == 1))
          break;

        strcat(palavra, buffer); 
              
      }

      if (busca(palavra, 0) == 1){ //verifica se a palavra esta na lista das palavras reservadas
        
          token_type token;
          strcpy(token.tipo, "palavra reservada");
          strcpy(token.valor, palavra);
          token.linha = linha_token_inicial;
          token.coluna = coluna_token_inicial; 
          fprintf(reservadas, "%s\n", token.valor);
          printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
          lista_de_tokens[tamanho_lista_de_tokens++] = token;
          fprintf(tokens, "%s %s %d %d\n", token.tipo, token.valor, token.linha, token.coluna);
          strcpy(palavra, "");
          fseek(codigo_fonte, -1, SEEK_CUR);
     
      } 
        else if (e_identificador(palavra)){ //verifica se e id, se for, registra o id
         
          token_type token;
          strcpy(token.tipo, "identificador");
          strcpy(token.valor, palavra);
          token.linha = linha_token_inicial;
          token.coluna = coluna_token_inicial - 1; 
          fprintf(identificadores, "%s\n", token.valor);
          printf("Tipo: %s  Valor: %s  Linha: %d  Coluna: %d\n", token.tipo, token.valor, token.linha, token.coluna);
          lista_de_tokens[tamanho_lista_de_tokens++] = token;
          fprintf(tokens, "%s %s %d %d\n", token.tipo, token.valor, token.linha, token.coluna);
          strcpy(palavra, "");
          fseek(codigo_fonte, -1, SEEK_CUR);

        } else{
          //erro caso a palavra nao se encaixe em nenhuma das definicoes
          printf("ERRO LEXICO: Sequencia invalida em (%d %d): %s - FLAG 3\n",linha_token_inicial, coluna_token_inicial, palavra);
          strcpy(palavra, "");
          fseek(codigo_fonte, -1, SEEK_CUR);

        }

    } 
    
         

       
      

  }

}


int main(int argc, char* argv[]){

    lexico(argv[2]);

    return 0;
}