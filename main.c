#include "reservas.h"

int main() {
    int capacidade, qtd;
    int opcao;
    
    // Inicialização
    Reserva *lista = inicializarSistema(&capacidade, &qtd);
    carregarDados(&lista, &capacidade, &qtd);

    do {
        printf("\n=== SISTEMA DE RESERVAS DE LABORATORIO PARA INTRODUCAO A PROGRAMACAO ===\n");
        printf("1. Inserir\n2. Listar\n3. Atualizar\n4. Remover\n5. Salvar\n0. Sair\nOpcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
        }

        switch (opcao) {
            case 1: inserirReserva(&lista, &capacidade, &qtd); break;
            case 2: listarReservas(lista, qtd); break;
            case 3: atualizarReserva(lista, qtd); break;
            case 4: removerReserva(lista, &qtd); break;
            case 5: salvarDados(lista, qtd); break;
            case 0: 
                salvarDados(lista, qtd);
                liberarMemoria(lista);
                break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
