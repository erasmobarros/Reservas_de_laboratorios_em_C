#include "reservas.h"
#include <ctype.h>

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int gerarIdUnico(Reserva *lista, int qtd) {
    int maxId = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].id > maxId)
            maxId = lista[i].id;
    }
    return maxId + 1;
}

int validarid(char *laboratorio) {
    for (int i = 0; laboratorio[i] != '\0'; i++) {
        if (!isdigit(laboratorio[i]) ) {
            return 0; 
        }
    }
    return 1;
}
int validarLab(char *laboratorio) {
    for (int i = 0; laboratorio[i] != '\0'; i++) {
        if (!isalnum(laboratorio[i]) && !isspace(laboratorio[i]))
            return 0;
    }
    return 1;
}

int buscarPorId(Reserva *lista, int qtd, int id) {
    for (int i = 0; i < qtd; i++)
        if (lista[i].id == id)
            return i;
    return -1;
}

int validarnome(char *solicitante) {
   for (int i =0; solicitante[i] != '\0'; i++) {
        if (!isalpha(solicitante[i]) && !isspace(solicitante[i])) {
            return 0; 
        }
    }
    return 1;
}

int validarData(char *data) {
    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;

    int d, m, a;
    if (sscanf(data, "%d/%d/%d", &d, &m, &a) != 3)
        return 0;

    if (d < 1 || d > 31) return 0;
    if (m < 1 || m > 12) return 0;
    if (a < 2024) return 0;

    return 1;
}

int validarHorario(char *horario) {
    if (strlen(horario) != 5) return 0;
    if (horario[2] != ':') return 0;

    int hh, mm;
    if (sscanf(horario, "%d:%d", &hh, &mm) != 2)
        return 0;

    if (hh < 0 || hh > 23) return 0;
    if (mm < 0 || mm > 59) return 0;

    return 1;
}
int validarHorariofim(char *horario_fim) {
    if (strlen(horario_fim) != 5) return 0;
    if (horario_fim[2] != ':') return 0;
    if (horario_fim[0] < '0' || horario_fim[0] > '2' || horario_fim[1] < '0' || horario_fim[1] > '9') return 0;
    return 1;
}
int horaParaMinutos(char *h){
    int hh = 0, mm = 0;
    sscanf(h,"%d:%d", &hh, &mm);
    return (hh * 60) + mm;
}

int horarioConflita(Reserva *lista, int qtd,
                    char *lab, char *data,
                    char *inicio, char *fim,
                    int ignorar)
{
    int iniNovo = horaParaMinutos(inicio);
    int fimNovo = horaParaMinutos(fim);

    for(int i=0; i<qtd; i++){
        if(i == ignorar) continue;

        if(strcmp(lista[i].laboratorio, lab) == 0 &&
           strcmp(lista[i].data, data) == 0){

            int ini = horaParaMinutos(lista[i].horario);
            int fim = horaParaMinutos(lista[i].horario_fim);

            if(iniNovo < fim && fimNovo > ini) {
                printf("\n[!] CONFLITO ENCONTRADO:\n");
                printf("O %s ja esta reservado por '%s' das %s as %s no dia %s.\n", 
                       lista[i].laboratorio, lista[i].solicitante, 
                       lista[i].horario, lista[i].horario_fim, lista[i].data);
                return 1; 
            }
        }
    }
    return 0;
}
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
    return temp;
}

void salvarDados(Reserva *lista, int qtd) {
    FILE *arquivo = fopen("dados_reservas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        fprintf(arquivo, "%d;%s;%s;%s;%s;%s\n", 
            lista[i].id, lista[i].laboratorio, lista[i].solicitante, 
            lista[i].data, lista[i].horario, lista[i].horario_fim);
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
        sscanf(linha, "%d;%[^;];%[^;];%[^;];%[^;];%[^\n]", 
            &r->id, r->laboratorio, r->solicitante, r->data, r->horario, r->horario_fim);
        
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
    while (1) {
        printf("ID do Laboratorio: ");
        scanf(" %[^\n]", nova.laboratorio);

        if (!validarid(nova.laboratorio)) {
            printf("ID invalido! Digite apenas numeros.\n");
        } else {
            break;
        }
    }
    while (1) {
        printf("Nome do Solicitante: ");
        scanf(" %[^\n]", nova.solicitante);

        if (!validarnome(nova.solicitante)) {
            printf("Nome invalido! Use apenas letras.\n");
        } else {
            break;
        }
    }
    while (1) {
        printf("Data (DD/MM/AAAA): ");
        scanf("%s", nova.data);

        if (!validarData(nova.data)) {
            printf("Data invalida! Use formato DD/MM/AAAA.\n");
        } else {
            break;
        }
    }
    while (1) {
        printf("Horario inicial (HH:MM): ");
        scanf("%s", nova.horario);

        if (!validarHorario(nova.horario)) {
            printf("Horario invalido!\n");
        } else {
            break;
        }
    }
    while (1) {
        printf("Horario final (HH:MM): ");
        scanf("%s", nova.horario_fim);

        if (!validarHorario(nova.horario_fim)) {
            printf("Horario invalido!\n");
            continue;
        }

        if (horaParaMinutos(nova.horario_fim) <= horaParaMinutos(nova.horario)) {
            printf("Horario final deve ser maior que o inicial!\n");
            continue;
        }

        break;
    }
    if (horarioConflita(*lista, *qtd,
                        nova.laboratorio,
                        nova.data,
                        nova.horario,
                        nova.horario_fim,
                        -1)) {

        printf("Reserva nao realizada devido a conflito.\n");
        return;
    }

    (*lista)[*qtd] = nova;
    (*qtd)++;

    printf("\nReserva realizada com sucesso!\n");
    printf("ID: %d | Lab: %s | Data: %s | %s ate %s\n",
           nova.id, nova.laboratorio, nova.data, nova.horario, nova.horario_fim);
}


void listarReservas(Reserva *lista, int qtd) {
    if (qtd == 0) {
        printf("\nNenhuma reserva cadastrada.\n");
        return;
    }
    printf("\nID | Laboratorio | Solicitante | Data       | Inico      | Fim \n");
    printf("---|-------------|-------------|------------|------------|--------\n");
    for (int i = 0; i < qtd; i++) {
        printf("%-2d | %-11s | %-11s | %-10s | %-11s| %s\n", 
            lista[i].id, lista[i].laboratorio, lista[i].solicitante, lista[i].data, lista[i].horario, lista[i].horario_fim);
    }
}

void atualizarReserva(Reserva *lista, int qtd){
    if(qtd==0){
        printf("Nada cadastrado\n");
        return;
    }

    listarReservas(lista, qtd);

    int id;
    printf("ID da reserva que deseja atualizar: ");
    scanf("%d", &id);
    limparBuffer();

    int idx = buscarPorId(lista, qtd, id);
    if(idx == -1){
        printf("Nao encontrado\n");
        return;
    }

    int op;
    printf ("\n=== Escolha uma opcao para atualizar ===\n");
    printf("\n1. Atualizar Nome\n2. Atualizar Laboratorio\n3. Atualizar Data\n4. Atualizar Hora Inicio\n5. Atualizar Hora Fim\n6. Sair\nOpcao: ");
    scanf("%d", &op);
    limparBuffer();

    char temp[50];

    switch(op){
        case 1:
            printf("Novo nome: ");
            scanf(" %[^\n]", temp);
            if(!validarnome(temp)) {
                printf("Nome invalido!\n");
                return;
            }
            strcpy(lista[idx].solicitante, temp);
            break;

        case 2:
            printf("Novo lab: ");
            scanf(" %[^\n]", temp);
            if(!validarLab(temp)) {
                printf("Laboratorio invalido!\n");
                return;
            }

            if(horarioConflita(lista, qtd, temp,
               lista[idx].data,
               lista[idx].horario,
               lista[idx].horario_fim, idx)){
                printf("Operacao cancelada.\n");
                return;
            }

            strcpy(lista[idx].laboratorio, temp);
            break;

        case 3:
            printf("Nova data (DD/MM/AAAA): ");
            scanf("%s", temp);
            if(!validarData(temp)) {
                printf("Data invalida!\n");
                return;
            }

            if(horarioConflita(lista, qtd,
               lista[idx].laboratorio, temp,
               lista[idx].horario,
               lista[idx].horario_fim, idx)){
                printf("Operacao cancelada.\n");
                return;
            }

            strcpy(lista[idx].data, temp);
            break;

        case 4:
            printf("Novo inicio (HH:MM): ");
            scanf("%s", temp);
            if(!validarHorario(temp)) {
                printf("Horario invalido!\n");
                return;
            }

            if(horarioConflita(lista, qtd,
               lista[idx].laboratorio,
               lista[idx].data,
               temp,
               lista[idx].horario_fim, idx)){
                printf("Operacao cancelada.\n");
                return;
            }

            strcpy(lista[idx].horario, temp);
            break;

        case 5:
            printf("Novo fim (HH:MM): ");
            scanf("%s", temp);
            if(!validarHorario(temp)) {
                printf("Horario invalido!\n");
                return;
            }

            if(horarioConflita(lista, qtd,
               lista[idx].laboratorio,
               lista[idx].data,
               lista[idx].horario,
               temp, idx)){
                printf("Operacao cancelada.\n");
                return;
            }

            strcpy(lista[idx].horario_fim, temp);
            break;
            case 6:
            printf ("Saindo da opcao de atualizar!!\n");
            return;
        default:
            printf("Opcao invalida.\n");
            return;
    }

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