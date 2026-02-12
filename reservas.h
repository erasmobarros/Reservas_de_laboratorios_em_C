#ifndef RESERVAS_H
#define RESERVAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char laboratorio[50];
    char solicitante[50];
    char data[11];
    char horario[6];
    char horario_fim[6];
} Reserva;

Reserva* inicializarSistema(int *capacidade, int *qtd);
Reserva* expandirCapacidade(Reserva *lista, int *capacidade);
void salvarDados(Reserva *lista, int qtd);
void carregarDados(Reserva **lista, int *capacidade, int *qtd);
void liberarMemoria(Reserva *lista);

void inserirReserva(Reserva **lista, int *capacidade, int *qtd);
void listarReservas(Reserva *lista, int qtd);
void atualizarReserva(Reserva *lista, int qtd);
void removerReserva(Reserva *lista, int *qtd);

void limparBuffer();

#endif