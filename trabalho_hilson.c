#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int operacao, IDUsuario, IDExemplar, QuantExemplar, IDBuscaExemplar;
int id, quantidade, DiasAtraso, encontrado;
char NomeUsuario[100], DataNasc[15], nome[100], autor[100];
float multa;

FILE *f, *temp;

void MenuPrincipal(){
    system("clear"); //

    const char *lista[] = {
        "Cadastrar Usuario",
        "Listar Usuarios",
        "Cadastrar Exemplar",
        "Listar Exemplares",
        "Emprestimo",
        "Devolucao",
        "Sair"
    };

    int tamanho_lista = sizeof(lista) / sizeof(lista[0]);

    printf("\n+------------------------------+\n");
    printf("|     BIBLIOTECA DARCY         |\n");
    printf("+------------------------------+\n");

    for (int i = 0; i < tamanho_lista; i++) {
        printf("| %-2d - %-22s |\n", i + 1, lista[i]);
    }
    printf("+------------------------------+\n");
    printf("Digite a operacao: ");
    scanf("%d", &operacao);
}

void CadastrarUsuario() {
    f = fopen("Usuarios.txt", "a");
    if (!f) {
        printf("Erro ao abrir arquivo de usuarios.\n");
        return;
    }
    printf("ID: ");
    scanf("%d", &IDUsuario);
    printf("Nome: ");
    scanf(" %[^\n]", NomeUsuario);
    printf("Nascimento: ");
    scanf("%s", DataNasc);

    fprintf(f, "%d;%s;%s\n", IDUsuario, NomeUsuario, DataNasc);
    fclose(f);

    printf("Usuario cadastrado com sucesso.\n");
    system("read -p 'Pressione ENTER para continuar...' temp");
}

void ListarUsuarios(){
    f = fopen("Usuarios.txt", "r");
    if (!f) {
        printf("Erro ao abrir arquivo de usuarios.\n");
        return;
    }

    while (fscanf(f, "%d;%99[^;];%14[^\n]",
                  &IDUsuario, NomeUsuario, DataNasc) == 3) {
        printf("\nID: %d\nNome: %s\nNascimento: %s\n",
               IDUsuario, NomeUsuario, DataNasc);
    }

    fclose(f);
    system("read -p 'Pressione ENTER para continuar...' temp");
}

void CadastrarExemplar(){
    f = fopen("Exemplares.txt", "a");
    if (!f) {
        printf("Erro ao abrir arquivo de exemplares.\n");
        return;
    }

    printf("ID: ");
    scanf("%d", &IDExemplar);
    printf("Nome: ");
    scanf(" %[^\n]", nome);
    printf("Autor: ");
    scanf(" %[^\n]", autor);
    printf("Quantidade: ");
    scanf("%d", &QuantExemplar);

    fprintf(f, "%d;%s;%s;%d\n", IDExemplar, nome, autor, QuantExemplar);
    fclose(f);

    printf("Exemplar cadastrado com sucesso.\n");
    system("read -p 'Pressione ENTER para continuar...' temp");
}

void ListarExemplar(){
    f = fopen("Exemplares.txt", "r");
    if (!f) {
        printf("Erro ao abrir arquivo de exemplares.\n");
        return;
    }

    printf("\n===== EXEMPLARES =====\n");

    while (fscanf(f, "%d;%99[^;];%99[^;];%d",
                  &id, nome, autor, &quantidade) == 4) {
        printf("\nID: %d\nNome: %s\nAutor: %s\nQuantidade: %d\n",
               id, nome, autor, quantidade);
    }

    fclose(f);
    system("read -p 'Pressione ENTER para continuar...' temp");
}

void Emprestimo(){
    f = fopen("Exemplares.txt", "r");
    temp = fopen("temp.txt", "w");

    if (!f || !temp) {
        printf("Erro ao manipular arquivos.\n");
        return;
    }

    printf("ID do exemplar: ");
    scanf("%d", &IDBuscaExemplar);

    encontrado = 0;

    while (fscanf(f, "%d;%99[^;];%99[^;];%d",
                  &id, nome, autor, &quantidade) == 4) {

        if (id == IDBuscaExemplar) {
            encontrado = 1;

            if (quantidade > 0) {
                quantidade--;
                printf("Emprestimo realizado.\n");
            } else {
                printf("Indisponivel.\n");
            }
        }

        fprintf(temp, "%d;%s;%s;%d\n", id, nome, autor, quantidade);
    }

    fclose(f);
    fclose(temp);

    remove("Exemplares.txt");
    rename("temp.txt", "Exemplares.txt");

    if (!encontrado)
        printf("Exemplar nao encontrado.\n");

    system("read -p 'Pressione ENTER para continuar...' temp");
}

void Devolucao(){
    f = fopen("Exemplares.txt", "r");
    temp = fopen("temp.txt", "w");

    if (!f || !temp) {
        printf("Erro ao manipular arquivos.\n");
        return;
    }

    printf("ID do exemplar: ");
    scanf("%d", &IDBuscaExemplar);

    char dataRetirada[15], dataDevolucao[15];
    int diaR, mesR, anoR, diaD, mesD, anoD;

    printf("Data de retirada (DD/MM/AAAA): ");
    scanf("%s", dataRetirada);
    printf("Data de devolucao (DD/MM/AAAA): ");
    scanf("%s", dataDevolucao);

    sscanf(dataRetirada,  "%d/%d/%d", &diaR, &mesR, &anoR);
    sscanf(dataDevolucao, "%d/%d/%d", &diaD, &mesD, &anoD);

    int totalR = anoR * 365 + mesR * 30 + diaR;
    int totalD = anoD * 365 + mesD * 30 + diaD;
    int diasUsados = totalD - totalR;

    DiasAtraso = (diasUsados > 7) ? diasUsados - 7 : 0;
    multa = DiasAtraso * 1.0f;

    encontrado = 0;

    while (fscanf(f, "%d;%99[^;];%99[^;];%d",
                  &id, nome, autor, &quantidade) == 4) {
        if (id == IDBuscaExemplar) {
            encontrado = 1;
            quantidade++;
        }
        fprintf(temp, "%d;%s;%s;%d\n", id, nome, autor, quantidade);
    }

    fclose(f);
    fclose(temp);

    remove("Exemplares.txt");
    rename("temp.txt", "Exemplares.txt");

    printf("\nDias utilizados: %d\n", diasUsados);
    if (DiasAtraso > 0)
        printf("Atraso: %d dia(s) | Multa: R$ %.2f\n", DiasAtraso, multa);
    else
        printf("Devolucao no prazo. Sem multa.\n");

    if (!encontrado)
        printf("Exemplar nao encontrado.\n");

    system("read -p 'Pressione ENTER para continuar...' temp");
}

int main(void){
    do {
        MenuPrincipal();

        switch (operacao) {
            case 1: CadastrarUsuario(); break;
            case 2: ListarUsuarios(); break;
            case 3: CadastrarExemplar(); break;
            case 4: ListarExemplar(); break;
            case 5: Emprestimo(); break;
            case 6: Devolucao(); break;
        }

    } while (operacao != 7);

    return 0;
}