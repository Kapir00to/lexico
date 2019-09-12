# Sobre
Analisador Léxico de um código em C.

# Autor
Lucas Rezende Bruno

# Data
11 de setembro de 2019 (ultimo commit)

# Propósito
O código foi feito como trabalho prático da disciplina de Compiladores, ministrada pelo professor Flavio Schiavoni do curso de Ciência da Computação da Universidade Federal de São João del Rei (UFSJ)

# Instruções para compilar
Para a compilação do código no Linux, é necessário executar a seguinte instrução:
make

# Instrução para executar
Para a execução do código no Linux, deve ser executado a seguinte instrução (subsituir nome_do_codigo_de_teste pelo nome do codigo que deseja ser analisado junto com o formato do mesmo):
./lexico 1 nome_do_codigo_de_teste

# Exemplos
No teste executado, foi analisado o seguinte código:

#do<stdio.h>
#define PI 3.14
int main(){
//ignore ignore ignore ignore

/* ignore ignore ignore
ignore ignore*/
    int 2a = 1 + 2;
    float b_a = 3 + 1;
    long c.c = a_a.c + b;
    c++;
    printf("%d + %d = %d", a, b, c);
    if (c == 3){
    	funcao(a,b);
    }
    return 0;
}

Como saída, o código gera os arquivos identificadores.txt(contendo todos os identificadores encontrados no codigo, um pra cada linha), literais.txt(contendo um literal por linha), numeros.txt(um numero por linha), operadores.txt(contendo um operador por linha), reservadas.txt (contendo a lista das palavras reservadas, uma pra cada linha), separadores.txt (contém todos os tokens considerados separadores) e tokens.txt (contem todos os tokens aceitos sem erro pelo codigo, um por cada linha, tendo ele o seu valor, tipo, linha e coluna onde se encontram).

O arquivo de tokens.txt após a execução do código fica com o seguinte conteúdo:

separador # 0 0
palavra reservada do 0 1
operador < 0 4
identificador stdio.h 0 6
operador > 0 14
separador \n 1 1
separador # 1 1
palavra reservada define 1 2
separador espaco 1 9
identificador PI 1 9
separador espaco 1 13
numero 3.14 1 14
separador \n 1 19
palavra reservada int 1 1
separador espaco 1 5
identificador main 1 5
separador ( 1 11
separador ) 1 12
separador { 1 13
separador \n 1 14
separador \n 2 1
separador \n 3 16
separador espaco 3 1
separador espaco 3 2
separador espaco 3 3
separador espaco 3 4
palavra reservada int 3 5
separador espaco 3 9
separador espaco 3 12
operador = 3 13
separador espaco 3 16
numero 1 3 17
separador espaco 3 19
operador + 3 20
separador espaco 3 23
numero 2 3 24
separador ; 3 26
separador \n 3 27
separador espaco 3 1
separador espaco 3 2
separador espaco 3 3
separador espaco 3 4
palavra reservada float 3 5
separador espaco 3 11
identificador b_a 3 11
separador espaco 3 16
operador = 3 17
separador espaco 3 20
numero 3 3 21
separador espaco 3 23
operador + 3 24
separador espaco 3 27
numero 1 3 28
separador ; 3 30
separador \n 3 31
separador espaco 3 1
separador espaco 3 2
separador espaco 3 3
separador espaco 3 4
palavra reservada long 3 5
separador espaco 3 10
identificador c.c 3 10
separador espaco 3 15
operador = 3 16
separador espaco 3 19
identificador a_a.c 3 19
separador espaco 3 26
operador + 3 27
separador espaco 3 30
identificador b 3 30
separador ; 3 33
separador \n 3 34
separador espaco 3 1
separador espaco 3 2
separador espaco 3 3
separador espaco 3 4
identificador c 3 4
operador ++ 3 7
separador ; 3 10
separador \n 3 11
separador espaco 3 1
separador espaco 3 2
separador espaco 3 3
separador espaco 3 4
identificador printf 3 4
separador ( 3 12
literal "%d + %d = %d" 3 13
separador , 3 27
separador espaco 3 28
identificador a 3 28
separador , 3 31
separador espaco 3 32
identificador b 3 32
separador , 3 35
separador espaco 3 36
identificador c 3 36
separador ) 3 39
separador ; 3 40
separador \n 3 41
separador espaco 3 1
separador espaco 3 2
separador espaco 3 3
separador espaco 3 4
palavra reservada if 3 5
separador espaco 3 8
separador ( 3 9
identificador c 3 9
separador espaco 3 12
operador == 3 13
separador espaco 3 16
numero 3 3 17
separador ) 3 19
separador { 3 20
separador \n 3 21
separador espaco 3 1
separador espaco 3 2
separador espaco 3 3
separador espaco 3 4
separador 	 3 5
identificador funcao 3 5
separador ( 3 13
identificador a 3 13
separador , 3 16
identificador b 3 16
separador ) 3 19
separador ; 3 20
separador \n 3 21
separador espaco 3 1
separador espaco 3 2
separador espaco 3 3
separador espaco 3 4
separador } 3 5
separador \n 3 6
separador espaco 3 1
separador espaco 3 2
separador espaco 3 3
separador espaco 3 4
palavra reservada return 3 5
separador espaco 3 12
numero 0 3 13
separador ; 3 15
separador \n 3 16
separador } 3 1
separador \n 3 2

# Limitações
O código possui a limitação de não estar calculando o valor da linha e coluna corretamente devido à maneira complexa com que foi programada.

# Exemplos de arquivo de entrada
O arquivo de entrada pode possuir qualquer conteúdo, desde que seja salvo em um formato onde se possa ler o conteúdo textual do mesmo.



