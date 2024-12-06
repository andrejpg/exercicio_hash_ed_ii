#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 101


typedef struct Contato {
    char nome[50];
    char telefone[20];
    struct Contato* next;
} Contato;


Contato* hashTable[TABLE_SIZE];


unsigned int hashFunction(const char* nome) {
    unsigned int hash = 0;
    while (*nome) {
        hash += *nome++;
    }
    return hash % TABLE_SIZE;
}


void adicionaContato(const char* nome, const char* telefone) {
    unsigned int index = hashFunction(nome);
    Contato* newContact = (Contato*)malloc(sizeof(Contato));
    if (!newContact) {
        printf("Erro ao alocar memoria.\n");
        return;
    }
    strcpy(newContact->nome, nome);
    strcpy(newContact->telefone, telefone);
    newContact->next = hashTable[index];
    hashTable[index] = newContact;

    printf("Contato adicionado!\n");
}


void pesquisaContato(const char* nome) {
    unsigned int index = hashFunction(nome);
    Contato* current = hashTable[index];

    while (current) {
        if (strcmp(current->nome, nome) == 0) {
            printf("Telefone de %s: %s\n", nome, current->telefone);
            return;
        }
        current = current->next;
    }
    printf("Contato nao encontrado.\n");
}


void removeContato(const char* nome) {
    unsigned int index = hashFunction(nome);
    Contato* current = hashTable[index];
    Contato* previous = NULL;

    while (current) {
        if (strcmp(current->nome, nome) == 0) {
            if (previous == NULL) {
                hashTable[index] = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Contato removido!\n");
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Contato nao encontrado.\n");
}


void mostrarContatos() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contato* current = hashTable[i];
        if (current) {
            printf("Indice %d:\n", i);
            while (current) {
                printf("  Nome: %s, Telefone: %s\n", current->nome, current->telefone);
                current = current->next;
            }
        }
    }
}


void liberaMemoria() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contato* current = hashTable[i];
        while (current) {
            Contato* temp = current;
            current = current->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
}


double measureTime(void (*func)(const char*, const char*), const char* nome, const char* telefone) {
    clock_t start, end;
    start = clock();
    func(nome, telefone);
    end = clock();
    return ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
}


int main() {
    int option;
    char nome[50], telefone[20];


    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    do {
        printf("\nEscolha uma opcao:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1:
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                printf("Telefone: ");
                fgets(telefone, sizeof(telefone), stdin);
                telefone[strcspn(telefone, "\n")] = '\0';

                printf("Tempo de insercao: %.2f ms\n",
                       measureTime((void (*)(const char*, const char*))adicionaContato, nome, telefone));
                break;
            case 2:
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Tempo de busca: %.2f ms\n",
                       measureTime((void (*)(const char*, const char*))pesquisaContato, nome, ""));
                break;
            case 3:
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                removeContato(nome);
                break;
            case 4:
                mostrarContatos();
                break;
            case 0:
                printf("Saindo.\n");
                break;
            default:
                printf("Opcao invalida, tente novamente\n");
        }
    } while (option != 0);

    liberaMemoria();

    return 0;
}
