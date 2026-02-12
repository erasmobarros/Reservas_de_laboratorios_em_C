#include "reservas.h"
#include <ctype.h>

// ---------- AUXILIARES ----------
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int gerarIdUnico(Reserva *lista, int qtd) {
    int maxId = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].id > maxId) maxId = lista[i].id;
    }
    return maxId + 1;
}
int validarid(char *laboratorio) {
    for (int i = 0; laboratorio[i] != '\0'; i++) {
        if (!isdigit(laboratorio[i]) ) {
            return 0; // String inválida
        }
    }
    return 1; // String válida
}

int buscarPorId(Reserva *lista, int qtd, int id) {
    for (int i = 0; i < qtd; i++) {
        if (lista[i].id == id) return i;
    }
    return -1;
}
int validarnome(char *solicitante) {
   for (int i =0; solicitante[i] != '\0'; i++) {
        if (!isalpha(solicitante[i]) && !isspace(solicitante[i])) {
            return 0; // Nome inválido
        }
    }
    return 1;
}

int validarData(char *data) {
    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;
    return 1;
}

int validarHorario(char *horario) {
    if (strlen(horario) != 5) return 0;
    if (horario[2] != ':') return 0;
    return 1;
}

// ---------- SISTEMA ----------
Reserva* inicializarSistema(int *capacidade, int *qtd) {
    *capacidade = 10;
    *qtd = 0;
    Reserva *lista = malloc((*capacidade) * sizeof(Reserva));
    if (!lista) {
        printf("Erro de memoria\n");
        exit(1);
    }
    return lista;
}

Reserva* expandirCapacidade(Reserva *lista, int *capacidade) {
    *capacidade += 10;
    Reserva *temp = realloc(lista, (*capacidade) * sizeof(Reserva));
    if (!temp) {
        printf("Erro ao expandir memoria\n");
        return lista;
    }
    return temp;
}

// ---------- ARQUIVO ----------
void salvarDados(Reserva *lista, int qtd) {
    FILE *arquivo = fopen("dados_reservas.txt", "w");
    if (!arquivo) {
        printf("Erro ao salvar.\n");
        return;
    }

    for (int i = 0; i < qtd; i++) {
        fprintf(arquivo, "%d;%s;%s;%s;%s;%s\n",
            lista[i].id,
            lista[i].laboratorio,
            lista[i].solicitante,
            lista[i].data,
            lista[i].horario,
            lista[i].horario_fim);
    }

    fclose(arquivo);
    printf("Dados salvos com sucesso!\n");
}

void carregarDados(Reserva **lista, int *capacidade, int *qtd) {
    FILE *arquivo = fopen("dados_reservas.txt", "r");
    if (!arquivo) return;

    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (*qtd >= *capacidade) {
            *lista = expandirCapacidade(*lista, capacidade);
        }

        Reserva *r = &(*lista)[*qtd];

        sscanf(linha, "%d;%[^;];%[^;];%[^;];%[^;];%[^\n]",
            &r->id,
            r->laboratorio,
            r->solicitante,
            r->data,
            r->horario,
            r->horario_fim);

        (*qtd)++;
    }

    fclose(arquivo);
}

void liberarMemoria(Reserva *lista) {
    free(lista);
}

// ---------- INSERIR ----------
void inserirReserva(Reserva **lista, int *capacidade, int *qtd) {
    if (*qtd >= *capacidade)
        *lista = expandirCapacidade(*lista, capacidade);

    Reserva nova;
    nova.id = gerarIdUnico(*lista, *qtd);

    printf("\nLaboratorio: ");
    scanf(" %[^\n]", nova.laboratorio);
<<<<<<< HEAD
    if (!validarid (nova.laboratorio)){
        printf ("ID invalido. Tente novamente!!\n");
        return;
    }
    printf("Nome do Solicitante: ");
    scanf(" %[^\n]", nova.solicitante);
    if (!validarnome(nova.solicitante)) {
        printf("Nome do solicitante invalido. Tente novamente!!\n");
        return;
    }
    
=======

    printf("Solicitante: ");
    scanf(" %[^\n]", nova.solicitante);

>>>>>>> 0e603b8603cccc6d891d090281060bfb09ce7e85
    printf("Data (DD/MM/AAAA): ");
    scanf("%s", nova.data);
    if (!validarData(nova.data)) {
        printf("Data invalida\n");
        return;
    }

    printf("Horario inicio: ");
    scanf("%s", nova.horario);
    if (!validarHorario(nova.horario)) {
        printf("Horario invalido\n");
        return;
    }

    printf("Horario fim: ");
    scanf("%s", nova.horario_fim);
    if (!validarHorario(nova.horario_fim)) {
        printf("Horario invalido\n");
        return;
    }

    (*lista)[*qtd] = nova;
    (*qtd)++;

    printf("Reserva cadastrada! ID %d\n", nova.id);
}

// ---------- LISTAR ----------
void listarReservas(Reserva *lista, int qtd) {
    if (qtd == 0) {
        printf("\nNenhuma reserva.\n");
        return;
    }

    printf("\nID | Lab | Nome | Data | Inicio | Fim\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d | %s | %s | %s | %s | %s\n",
            lista[i].id,
            lista[i].laboratorio,
            lista[i].solicitante,
            lista[i].data,
            lista[i].horario,
            lista[i].horario_fim);
    }
}

// ---------- ATUALIZAR ----------
void atualizarReserva(Reserva *lista, int qtd) {
    if (qtd == 0) {
        printf("Nenhuma reserva cadastrada.\n");
        return;
    }

    int id;
    listarReservas(lista, qtd);
    printf("\nDigite o ID da reserva: ");
    scanf("%d", &id);
    limparBuffer();

    int idx = buscarPorId(lista, qtd, id);
    if (idx == -1) {
        printf("Reserva nao encontrada.\n");
        return;
    }

    int op;
    printf("\n1-Nome\n2-Laboratorio\n3-Data\n4-Hora inicio\n5-Hora fim\nOpcao: ");
    scanf("%d", &op);
    limparBuffer();
<<<<<<< HEAD
    
    switch(opcao) {
=======

    char temp[50];

    switch(op){

>>>>>>> 0e603b8603cccc6d891d090281060bfb09ce7e85
        case 1:
            printf("Novo nome: ");
            scanf(" %[^\n]", lista[idx].solicitante);
            printf("Atualizado!\n");
            break;

        case 2:
<<<<<<< HEAD
            printf("Novo ID do laboratorio: ");
            scanf(" %[^\n]", lista[idx].laboratorio);
=======
            printf("Novo laboratorio: ");
            scanf(" %[^\n]", temp);
>>>>>>> 0e603b8603cccc6d891d090281060bfb09ce7e85

            for(int i=0;i<qtd;i++){
                if(i!=idx &&
                   strcmp(lista[i].laboratorio,temp)==0 &&
                   strcmp(lista[i].data,lista[idx].data)==0 &&
                   strcmp(lista[i].horario,lista[idx].horario)==0 &&
                   strcmp(lista[i].horario_fim,lista[idx].horario_fim)==0){
                        printf("Ja existe reserva nesse laboratorio nesse horario!\n");
                        return;
                }
            }
<<<<<<< HEAD
            else {
                printf("Laboratorio atualizado!\n");
            break;}
=======

            strcpy(lista[idx].laboratorio,temp);
            printf("Atualizado!\n");
            break;
>>>>>>> 0e603b8603cccc6d891d090281060bfb09ce7e85

        case 3:
            printf("Nova data: ");
            scanf("%s", temp);

            if(!validarData(temp)){
                printf("Data invalida!\n");
                return;
            }

            for(int i=0;i<qtd;i++){
                if(i!=idx &&
                   strcmp(lista[i].laboratorio,lista[idx].laboratorio)==0 &&
                   strcmp(lista[i].data,temp)==0 &&
                   strcmp(lista[i].horario,lista[idx].horario)==0 &&
                   strcmp(lista[i].horario_fim,lista[idx].horario_fim)==0){
                        printf("Ja existe reserva nessa data e horario!\n");
                        return;
                }
            }
<<<<<<< HEAD
            else {
                printf("Data atualizada!\n");
            break;}
=======

            strcpy(lista[idx].data,temp);
            printf("Atualizado!\n");
            break;
>>>>>>> 0e603b8603cccc6d891d090281060bfb09ce7e85

        case 4:
            printf("Novo horario inicio: ");
            scanf("%s", temp);

            if(!validarHorario(temp)){
                printf("Horario invalido!\n");
                return;
            }
<<<<<<< HEAD
            else {
                printf("Horario inicial atualizado!\n");
            break;}
=======

            for(int i=0;i<qtd;i++){
                if(i!=idx &&
                   strcmp(lista[i].laboratorio,lista[idx].laboratorio)==0 &&
                   strcmp(lista[i].data,lista[idx].data)==0 &&
                   strcmp(lista[i].horario,temp)==0 &&
                   strcmp(lista[i].horario_fim,lista[idx].horario_fim)==0){
                        printf("Horario ja ocupado!\n");
                        return;
                }
            }

            strcpy(lista[idx].horario,temp);
            printf("Atualizado!\n");
            break;
>>>>>>> 0e603b8603cccc6d891d090281060bfb09ce7e85

        case 5:
            printf("Novo horario fim: ");
            scanf("%s", temp);

            if(!validarHorario(temp)){
                printf("Horario invalido!\n");
                return;
            }
<<<<<<< HEAD
            else {printf("Horario final atualizado!\n");
            break;}
=======
>>>>>>> 0e603b8603cccc6d891d090281060bfb09ce7e85

            for(int i=0;i<qtd;i++){
                if(i!=idx &&
                   strcmp(lista[i].laboratorio,lista[idx].laboratorio)==0 &&
                   strcmp(lista[i].data,lista[idx].data)==0 &&
                   strcmp(lista[i].horario,lista[idx].horario)==0 &&
                   strcmp(lista[i].horario_fim,temp)==0){
                        printf("Horario ja ocupado!\n");
                        return;
                }
            }

            strcpy(lista[idx].horario_fim,temp);
            printf("Atualizado!\n");
            break;

        default:
            printf("Opcao invalida\n");
    }
}



// ---------- REMOVER ----------
void removerReserva(Reserva *lista, int *qtd) {
    if (*qtd == 0) {
        printf("Nenhuma reserva.\n");
        return;
    }

    int id;
    listarReservas(lista, *qtd);
    printf("ID remover: ");
    scanf("%d", &id);

    for (int i = 0; i < *qtd; i++) {
        if (lista[i].id == id) {
            for (int j = i; j < *qtd-1; j++)
                lista[j] = lista[j+1];

            (*qtd)--;
            printf("Removido!\n");
            return;
        }
    }

    printf("ID nao encontrado\n");
}
