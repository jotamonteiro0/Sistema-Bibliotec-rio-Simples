#include <stdio.h>
#include <stdlib.h>

int valor1, valor2, valor3, maiornum;
char nome[20];

int main(void){

    printf("Digite seu nome: ");
    scanf("%s", nome);

    printf("Digite o primeiro valor: ");
    scanf("%i", &valor1);

    printf("Digite o segundo valor: ");
    scanf("%i", &valor2);

    printf("Digite o terceiro valor: ");
    scanf("%i", &valor3);

    if (valor1 == valor2 && valor2 == valor3){
        printf("Todos os valores digitados sao iguais");
    } else {
        maiornum = valor1;

        if (valor2 > maiornum){
            maiornum = valor2;
        }

        if (valor3 > maiornum){
            maiornum = valor3;
        }

        printf("O maior valor digitado foi: %d", maiornum);
    }

    return 0;
}