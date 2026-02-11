#include "reservas.h"

// --- Funções Auxiliares (Internas) ---

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

int buscarPorId(Reserva *lista, int qtd, int id) {
    for (int i = 0; i < qtd; i++) {
        if (lista[i].id == id) return i;
    }
    return -1;
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

int verificarDisponibilidadelab(Reserva *lista, int qtd, char *lab) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].laboratorio, lab) == 0){
            return 0; 
        }
    }
    return 1; // Disponível
}
int verificarDisponibilidadedata(Reserva *lista, int qtd, char *data) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].data, data) == 0){
            return 0; 
        }
    }
    return 1; // Disponível
}
int verificarDisponibilidadehora(Reserva *lista, int qtd, char *horario_inicio, char *horario_fim) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].horario, horario_inicio) == 0 && strcmp(lista[i].horario_fim, horario_fim) == 0){
            return 0; 
        }
    }
    return 1; // Disponível
}

// --- Implementação das Funções do .h ---

Reserva* inicializarSistema(int *capacidade, int *qtd) {
    *capacidade = 10;
    *qtd = 0;
    Reserva *lista = (Reserva*) malloc((*capacidade) * sizeof(Reserva));
    if (lista == NULL) {
        printf("Erro fatal: Falha na alocacao de memoria.\n");
        exit(1);
    }
    return lista;
}

Reserva* expandirCapacidade(Reserva *lista, int *capacidade) {
    int novaCapacidade = *capacidade + 10;
    Reserva *temp = (Reserva*) realloc(lista, novaCapacidade * sizeof(Reserva));
    
    if (temp == NULL) {
        printf("Erro: Nao foi possivel expandir a memoria.\n");
        return lista; 
    }
    
    *capacidade = novaCapacidade;
    // printf("DEBUG: Memoria expandida para %d registros.\n", *capacidade);
    return temp;
}

void salvarDados(Reserva *lista, int qtd) {
    FILE *arquivo = fopen("dados_reservas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        fprintf(arquivo, "%d;%s;%s;%s;%s\n", 
            lista[i].id, lista[i].laboratorio, lista[i].solicitante, 
            lista[i].data, lista[i].horario);
    }
    fclose(arquivo);
    printf("Dados salvos com sucesso!\n");
}

void carregarDados(Reserva **lista, int *capacidade, int *qtd) {
    FILE *arquivo = fopen("dados_reservas.txt", "r");
    if (arquivo == NULL) return;

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (*qtd >= *capacidade) {
            *lista = expandirCapacidade(*lista, capacidade);
        }
        
        Reserva *r = &(*lista)[*qtd];
        sscanf(linha, "%d;%[^;];%[^;];%[^;];%[^\n]", 
            &r->id, r->laboratorio, r->solicitante, r->data, r->horario);
        
        (*qtd)++;
    }
    fclose(arquivo);
}

void liberarMemoria(Reserva *lista) {
    if (lista != NULL) free(lista);
}

void inserirReserva(Reserva **lista, int *capacidade, int *qtd) {
    if (*qtd >= *capacidade) {
        *lista = expandirCapacidade(*lista, capacidade);
    }

    Reserva nova;
    nova.id = gerarIdUnico(*lista, *qtd);

    printf("\n--- Nova Reserva ---\n");
    printf("ID do Laboratorio: ");
    scanf(" %[^\n]", nova.laboratorio);
    printf("Nome do Solicitante: ");
    scanf(" %[^\n]", nova.solicitante);
    
    printf("Data (DD/MM/AAAA): ");
    scanf(" %s", nova.data);
    if (!validarData(nova.data)) {
        printf("Data invalida. Tente novamente!!\n");
        return;
    }

    printf("Horario inicial(HH:MM): ");
    scanf(" %s", nova.horario);
    if (!validarHorario(nova.horario)) {
        printf("Horario invalido. Tente novamente!!\n");
        return;
    }
    printf("Horario final(HH:MM): ");
    scanf(" %s", nova.horario_fim);
    if (!validarHorario(nova.horario_fim)) {
        printf("Horario invalido. Tente novamente!!\n");
        return;
    }

    if (!verificarDisponibilidadelab(*lista, *qtd, nova.laboratorio)) {
        printf("Laboratorio indisponivel! Tente outro!!\n");
        return;
    }if (!verificarDisponibilidadedata(*lista, *qtd, nova.data)) {
        printf("Data indisponivel! Tente outra data!!\n");
        return;
    }
    if (!verificarDisponibilidadehora(*lista, *qtd, nova.horario, nova.horario_fim)) {
        printf("Horario indisponivel! Tente outro horario!!\n");
        return;
    }

    (*lista)[*qtd] = nova;
    (*qtd)++;
    printf("Reserva realizada! ID: %d| Solicitande: %s| Data: %s| Horario: %s ate %s\n", nova.id, nova.solicitante, nova.data, nova.horario, nova.horario_fim);
    
}

void listarReservas(Reserva *lista, int qtd) {
    if (qtd == 0) {
        printf("\nNenhuma reserva cadastrada.\n");
        return;
    }
    printf("\nID | Laboratorio | Solicitante | Data       | Hora\n");
    printf("---|-------------|-------------|------------|-----\n");
    for (int i = 0; i < qtd; i++) {
        printf("%-2d | %-11s | %-11s | %-10s | %s\n", 
            lista[i].id, lista[i].laboratorio, lista[i].solicitante, lista[i].data, lista[i].horario);
    }
}

void atualizarReserva(Reserva *lista, int qtd) {
    int id;
    printf("Digite o ID da reserva: ");
    scanf("%d", &id);
    
    int idx = buscarPorId(lista, qtd, id);
    if (idx == -1) {
        printf("Reserva nao encontrada.\n");
        return;
    }
    
    printf("Novo Solicitante: ");
    scanf(" %[^\n]", lista[idx].solicitante);
    printf("Atualizado!\n");
}

void removerReserva(Reserva *lista, int *qtd) {
    if (*qtd == 0){
       printf ("Nenhuma reserva para remover.\n");
        return;
    }
   
     listarReservas(lista, *qtd);
     printf ("Informe o ID da reserva que deseja remover (ou -1 para SAIR): \n");
    int id, encontrado = 0;
    scanf("%d", &id);
    if (id == -1){
        printf ("Saindo da opcao de remover!!\n");
        return;
    }
   for (int i = 0; i < *qtd; i++){
    if (lista[i].id == id){
        encontrado = 1;
    for (int j =i; j< *qtd -1; j++){
        lista [j] = lista[j+1]; // Sobrescreve o elemento a ser removido
    }
    
    (*qtd)--;
    printf ("Reserva de ID %d removida com sucesso.\n", id);
    break;
    }
   }
   if (!encontrado){
    printf ("ID %d nao encontrado na lista. Tente novamente.\n", id);
   }
}