#include "reservas.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// FUNÇÕES AUXILIARES


// Limpa o buffer do teclado (stdin) para evitar pular leituras
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Gera um ID novo baseado no maior ID existente na lista + 1 e garante que cada reserva tenha um identificador único.
int gerarIdUnico(Reserva *lista, int qtd) {
    int maxId = 0;
    for (int i = 0; i < qtd; i++) {
        if (lista[i].id > maxId)
            maxId = lista[i].id;
    }
    return maxId + 1;
}

// Busca o índice de uma reserva no array baseada no seu ID numérico e retornando-1 se não encontrar..
int buscarPorId(Reserva *lista, int qtd, int id) {
    for (int i = 0; i < qtd; i++)
        if (lista[i].id == id)
            return i;
    return -1;
}

// Converte uma string de horário "HH:MM" para minutos totais desde a meia-noite. Ex: "01:30" vira 90 minutos, assim, facilitando comparações matemáticas de tempo.
int horaParaMinutos(char *h){
    int hh = 0, mm = 0;
    sscanf(h,"%d:%d", &hh, &mm);
    return (hh * 60) + mm;
}


// FUNÇÕES DE VALIDAÇÃO


// Verifica se a string de validar ID contém apenas dígitos numéricos.
int validarid(char *laboratorio) {
    for (int i = 0; laboratorio[i] != '\0'; i++) {
        if (!isdigit(laboratorio[i]) ) {
            return 0; 
        }
    }
    return 1;
}

// Verifica se o laboratório contém apenas números ou letras.
int validarLab(char *laboratorio) {
    for (int i = 0; laboratorio[i] != '\0'; i++) {
        if (!isalnum(laboratorio[i]) && !isspace(laboratorio[i]))
            return 0;
    }
    return 1;
}

// Verifica se o nome contém apenas letras e espaços.
int validarnome(char *solicitante) {
   for (int i =0; solicitante[i] != '\0'; i++) {
        if (!isalpha(solicitante[i]) && !isspace(solicitante[i])) {
            return 0; 
        }
    }
    return 1;
}

// Valida o formato tamanho e barras e se o dia/mês são reais.
int validarData(char *data) {
    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;

    int d, m, a;
    // Tenta extrair 3 inteiros da string
    if (sscanf(data, "%d/%d/%d", &d, &m, &a) != 3)
        return 0;

    if (d < 1 || d > 31) return 0;
    if (m < 1 || m > 12) return 0;

    return 1;
}

// Valida se o ano é igual ou posterior a 2026.
int validardataano(char *data) {
    int d, m, a;
    if (sscanf(data, "%d/%d/%d", &d, &m, &a) != 3)
        return 0;

    if (a < 2026)
        return 0;

    return 1;
}

// Valida formato HH:MM e se as horas e minutos estão corretos.
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


// LÓGICA DE CONFLITO


// Verifica se existe sobreposição de horários para o mesmo laboratório na mesma data. Assim, retornando 1 se houver conflito, 0 se estiver livre.
int horarioConflita(Reserva *lista, int qtd,
                    char *lab, char *data,
                    char *inicio, char *fim,
                    int ignorar)
{
    int iniNovo = horaParaMinutos(inicio);
    int fimNovo = horaParaMinutos(fim);

    for(int i=0; i<qtd; i++){
        if(i == ignorar) continue; // Pula a própria reserva se estiver editando

        // Verifica se é o mesmo laboratório e mesmo dia
        if(strcmp(lista[i].laboratorio, lab) == 0 &&
           strcmp(lista[i].data, data) == 0){

            int ini = horaParaMinutos(lista[i].horario);
            int fimReservaExistente = horaParaMinutos(lista[i].horario_fim);

            // Lógica de intersecção de intervalos: (InicioNovo < FimAntigo) E (FimNovo > InicioAntigo)
            
            if(iniNovo < fimReservaExistente && fimNovo > ini) {
                printf("\nConflito no horario:\n");
                printf("O %s ja esta reservado por '%s' das %s as %s no dia %s.\n", 
                       lista[i].laboratorio, lista[i].solicitante, 
                       lista[i].horario, lista[i].horario_fim, lista[i].data);
                return 1; 
            }
        }
    }
    return 0;
}

// GERENCIAMENTO DE MEMÓRIA E SISTEMA

// Aloca a memória inicial para o vetor dinâmico de Reservas.
Reserva* inicializarSistema(int *capacidade, int *qtd) {
    *capacidade = 10; // Começa com espaço para 10 reservas
    *qtd = 0;
    Reserva *lista = (Reserva*) malloc((*capacidade) * sizeof(Reserva));
    if (lista == NULL) {
        printf("Falha na alocacao de memoria.\n");
        exit(1);
    }
    return lista;
}

// Aumenta o tamanho do vetor dinâmico quando ele fica cheio.
Reserva* expandirCapacidade(Reserva *lista, int *capacidade) {
    int novaCapacidade = *capacidade + 10;
    // Tenta realocar para o novo tamanho
    Reserva *temp = (Reserva*) realloc(lista, novaCapacidade * sizeof(Reserva));
    
    if (temp == NULL) {
        printf("Nao foi possivel expandir a memoria.\n");
        return lista; // Retorna a lista original se falhar
    }
    
    *capacidade = novaCapacidade;
    return temp;
}

void liberarMemoria(Reserva *lista) {
    if (lista != NULL) free(lista);
}


// PERSISTÊNCIA DE DADOS (ARQUIVOS)

// Grava todas as reservas no arquivo de texto.
void salvarDados(Reserva *lista, int qtd) {
    FILE *arquivo = fopen("dados_reservas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        // Formato CSV separado por ponto e vírgula para facilitar leitura.
        fprintf(arquivo, "%d;%s;%s;%s;%s;%s\n", 
            lista[i].id, lista[i].laboratorio, lista[i].solicitante, 
            lista[i].data, lista[i].horario, lista[i].horario_fim);
    }
    fclose(arquivo);
    printf("Dados salvos com sucesso!\n");
}

// Lê o arquivo de texto e reconstrói a lista de reservas na memória.
void carregarDados(Reserva **lista, int *capacidade, int *qtd) {
    FILE *arquivo = fopen("dados_reservas.txt", "r");
    if (arquivo == NULL) return; // Se o arquivo não existir ainda, apenas retorna

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Se a lista estiver cheia, expande antes de adicionar
        if (*qtd >= *capacidade) {
            *lista = expandirCapacidade(*lista, capacidade);
        }
        
        Reserva *r = &(*lista)[*qtd];
        // Parse da string usando ';' como delimitador
        sscanf(linha, "%d;%[^;];%[^;];%[^;];%[^;];%[^\n]", 
            &r->id, r->laboratorio, r->solicitante, r->data, r->horario, r->horario_fim);
        
        (*qtd)++;
    }
    fclose(arquivo);
}


// FUNÇÕES CRUD


void inserirReserva(Reserva **lista, int *capacidade, int *qtd) {
    // Verifica capacidade antes de começar
    if (*qtd >= *capacidade) {
        *lista = expandirCapacidade(*lista, capacidade);
    }

    Reserva nova;
    nova.id = gerarIdUnico(*lista, *qtd);

    printf("\n--- Nova Reserva ---\n");
    
    // Loops while(1) forçam o usuário a digitar um dado válido para avançar
    while (1) {
        printf("ID do Laboratorio: ");
        scanf(" %[^\n]", nova.laboratorio);

        if (!validarLab(nova.laboratorio)) {
            printf("ID invalido! Digite apenas letras ou numeros.\n");
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
            printf("Data invalida! Use DD/MM/AAAA.\n");
            continue;
        }

        if (!validardataano(nova.data)) {
            printf("Ano deve ser 2026 ou posterior!\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Horario inicial (HH:MM): ");
        scanf("%s", nova.horario);

        if (!validarHorario(nova.horario)) {
            printf("Horario invalido! Use HH:MM. \n");
        } else {
            break;
        }
    }
    while (1) {
        printf("Horario final (HH:MM): ");
        scanf("%s", nova.horario_fim);

        if (!validarHorario(nova.horario_fim)) {
            printf("Horario invalido! Use HH:MM.\n");
            continue;
        }

        // Valida se hora final > hora inicial
        if (horaParaMinutos(nova.horario_fim) <= horaParaMinutos(nova.horario)) {
            printf("Horario final deve ser maior que o inicial!\n");
            continue;
        }
        break;
    }

    // Verifica conflito de horário no txt atual
    if (horarioConflita(*lista, *qtd,
                        nova.laboratorio,
                        nova.data,
                        nova.horario,
                        nova.horario_fim,
                        -1)) {

        printf("Reserva nao realizada devido a conflito.\n");
        return;
    }

    // Insere no vetor e incrementa contador
    (*lista)[*qtd] = nova;
    (*qtd)++;

    printf("\nReserva realizada com sucesso!\n");
    printf("\nID: %d | Laboratorio: %s | Nome Solicitante: %s | Data: %s | Horario: %s ate %s\n",
           nova.id, nova.laboratorio, nova.solicitante, nova.data, nova.horario, nova.horario_fim);
}

void listarReservas(Reserva *lista, int qtd) {
    if (qtd == 0) {
        printf("\nNenhuma reserva cadastrada.\n");
        return;
    }
    // Cabeçalho formatado
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
    printf("\nInforme o ID da reserva que deseja atualizar (ou -1 para SAIR): ");
    scanf("%d", &id);
    limparBuffer();
    if (id == -1){
        printf ("Saindo da opcao atualizar!!");
        return;
    }

    // Encontra a posição real no vetor
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

    char temp[50]; // Buffer temporário para validar antes de salvar

    switch(op){
        case 1: // Nome
        while (1) {
            printf("Novo nome: ");
            scanf(" %[^\n]", temp);

            if(!validarnome(temp)) {
                printf("Nome invalido! Digite apenas letras.\n");
                continue;
            }
            strcpy(lista[idx].solicitante, temp);
            break;
        }
        break;

        case 2: // Laboratório
        while (1) {
            printf("Novo Laboratorio: ");
            scanf(" %[^\n]", temp);

            if(!validarid(temp)) {
                printf("Laboratorio invalido! \n");
                continue;
            }

            // Verifica conflito usando o NOVO laboratório (temp) mas os DADOS ANTIGOS de data/hora
            if(horarioConflita(lista, qtd, temp,
               lista[idx].data,
               lista[idx].horario,
               lista[idx].horario_fim, idx)){ 
                printf("Conflito de horario!\n");
                continue;
            }

            strcpy(lista[idx].laboratorio, temp);
            break;
        }
        break;

        case 3: // Data
        while (1) {
            printf("Nova data (DD/MM/AAAA): ");
            scanf("%s", temp);

            if(!validarData(temp)) {
                printf("Data invalida! Use DD/MM/AAAA.\n");
                continue;
            }

            // Verifica conflito usando a NOVA data (temp)
            if(horarioConflita(lista, qtd,
               lista[idx].laboratorio, temp,
               lista[idx].horario,
               lista[idx].horario_fim, idx)){
                printf("Conflito de horario!\n");
                continue;
            }

            strcpy(lista[idx].data, temp);
            break;
        }
        break;

        case 4: // Hora Início
        while (1) {
            printf("Novo inicio (HH:MM): ");
            scanf("%s", temp);

            if(!validarHorario(temp)) {
                printf("Horario invalido!\n");
                continue;
            }
            // Garante que o novo início seja menor que o fim atual
            if(horaParaMinutos(temp) >= horaParaMinutos(lista[idx].horario_fim)) {
                printf("Inicio deve ser menor que o fim!\n");
                continue;
            }

            if(horarioConflita(lista, qtd,
               lista[idx].laboratorio,
               lista[idx].data,
               temp, // Novo início
               lista[idx].horario_fim, idx)){
                printf("Conflito de horario!\n");
                continue;
            }

            strcpy(lista[idx].horario, temp);
            break;
        }
        break;

        case 5: // Hora Fim
        while (1) {
            printf("Novo fim (HH:MM): ");
            scanf("%s", temp);

            if(!validarHorario(temp)) {
                printf("Horario invalido!\n");
                continue;
            }
            // Garante que o novo fim seja maior que o início atual
            if(horaParaMinutos(temp) <= horaParaMinutos(lista[idx].horario)) {
                printf("Fim deve ser maior que inicio!\n");
                continue;
            }

            if(horarioConflita(lista, qtd,
               lista[idx].laboratorio,
               lista[idx].data,
               lista[idx].horario,
               temp, idx)){ // Novo fim
                printf("Conflito de horario!\n");
                continue;
            }

            strcpy(lista[idx].horario_fim, temp);
            break;
        }
        break;

        case 6:
            printf("Saindo da opcao de atualizar!!\n");
            return;

        default:
            printf("Opcao invalida.\n");
            return;
    }

    printf("Atualizado!\n");
    printf ("ID: %d | Laboratorio: %s | Nome Solicitante: %s | Data: %s | %s ate %s\n",
           lista[idx].id, lista[idx].laboratorio, lista[idx].solicitante, lista[idx].data, lista[idx].horario, lista[idx].horario_fim);
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

   // Busca e remoção
   for (int i = 0; i < *qtd; i++){
        if (lista[i].id == id){
            encontrado = 1;
            // Shift à esquerda: move todos os elementos da frente uma casa para trás para preencher o buraco deixado pela remoção
            for (int j = i; j < *qtd - 1; j++){
                lista[j] = lista[j+1];
            }
            
            (*qtd)--; // Diminui o total de reservas
            printf ("Reserva de ID %d removida com sucesso.\n", id);
            break; // Sai do loop após remover
        }
   }

   if (!encontrado){
    printf ("ID %d nao encontrado na lista. Tente novamente.\n", id);
   }
}