#include "reservas.h"

int main() {
    Reserva *minhaLista = NULL; 
    int qtd = 0;
    int capacidade = 0;
    int opcao;

    // Tenta carregar ao iniciar
    // Note: minhaLista recebe o retorno da função
    minhaLista = carregarArquivo(&qtd, &capacidade);

    do {
        printf("\n=== SISTEMA UFAPE SIMPLIFICADO ===\n");
        printf("1. Adicionar\n2. Listar\n3. Remover\n4. Salvar e Sair\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                // Atualizamos o ponteiro minhaLista com o retorno da função
                minhaLista = adicionarReserva(minhaLista, &qtd, &capacidade);
                break;
            case 2:
                listarReservas(minhaLista, qtd);
                break;
            case 3:
                removerReserva(minhaLista, &qtd);
                break;
            case 4:
                salvarArquivo(minhaLista, qtd);
                free(minhaLista); // Limpa memória antes de sair
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 4);

    return 0;
}
