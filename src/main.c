/**
 * @file main.c
 * @brief Ponto de entrada do programa e interface de linha de comando (CLI).
 *
 * Este arquivo contém o menu interativo que permite ao usuário testar todas as
 * funcionalidades da biblioteca de grafos, garantindo a alocação e liberação
 * correta de memória durante a execução.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/grafo.h"

// Função auxiliar para limpar o buffer do teclado (evita bugs com o scanf)
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int opcao;
    Grafo* grafo_atual = NULL;
    char nome_arquivo[100];

    do {
        printf("\n========================================\n");
        printf("       MANIPULADOR DE GRAFOS EM C       \n");
        printf("========================================\n");
        printf("1. Ler grafo de um arquivo (.txt)\n");
        printf("2. Criar grafo manualmente\n");
        printf("3. Salvar grafo atual em arquivo\n");
        printf("4. Gerar e salvar Matriz de Adjacencias\n");
        printf("5. Exibir graus dos vertices\n");
        printf("6. Encontrar Arvore Geradora Minima (Prim)\n");
        printf("7. Caminho Mais Curto (Dijkstra)\n");
        printf("8. Busca em Largura (BFS)\n");
        printf("9. Busca em Profundidade (DFS)\n");
        printf("0. Sair\n");
        printf("========================================\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            printf("Entrada invalida. Digite um numero.\n");
            continue;
        }

        switch(opcao) {
            case 1: {
                printf("Digite o nome do arquivo de entrada (ex: data/entrada.txt): ");
                scanf("%s", nome_arquivo);
                
                // Se já existir um grafo carregado, liberamos a memória antes de carregar outro
                if (grafo_atual != NULL) {
                    liberar_grafo(grafo_atual);
                }
                
                grafo_atual = ler_grafo_arquivo(nome_arquivo);
                if (grafo_atual != NULL) {
                    printf("Grafo carregado com sucesso!\n");
                }
                break;
            }
            case 2: {
                // Coleta os metadados do grafo, inicializa a estrutura e insere as arestas uma a uma
                int num_v, valorado, num_arestas;
                char tipo;
                
                printf("Digite a quantidade de vertices: ");
                scanf("%d", &num_v);
                
                printf("Digite o tipo (G para nao dirigido, D para dirigido): ");
                scanf(" %c", &tipo);
                printf("O grafo e valorado? (1 para sim, 0 para nao): ");
                scanf("%d", &valorado);
                
                if (grafo_atual != NULL) {
                    liberar_grafo(grafo_atual);
                }
                
                grafo_atual = inicializar_grafo(num_v, tipo, valorado);
                
                printf("Quantas arestas/arcos deseja inserir agora? ");
                scanf("%d", &num_arestas);
                
                for (int i = 0; i < num_arestas; i++) {
                    int origem, destino;
                    float peso = 0.0;
                    
                    printf("\nAresta %d:\n", i + 1);
                    printf("  Vertice de origem: ");
                    scanf("%d", &origem);
                    printf("  Vertice de destino: ");
                    scanf("%d", &destino);
                    
                    // Validação para evitar inserção de arestas com vértices fora do intervalo permitido
                    if (origem < 0 || origem >= num_v || destino < 0 || destino >= num_v) {
                        printf("  Erro: Vertices invalidos! Devem ser entre 0 e %d. Tente novamente.\n", num_v - 1);
                        i--; // Volta o contador para o usuário refazer esta aresta
                        continue;
                    }

                    if (valorado == 1) {
                        printf("  Peso da aresta: ");
                        scanf("%f", &peso);
                    }
                    
                    adicionar_aresta(grafo_atual, origem, destino, peso);
                }
                printf("\nGrafo criado com sucesso!\n");
                break;
            }
            case 3: {
                // Previne falhas de segmentação (segfault) caso o usuário tente operar um grafo inexistente
                if (grafo_atual == NULL) {
                    printf("Erro: Nenhum grafo carregado no sistema!\n");
                    break;
                }
                printf("Digite o nome do arquivo para salvar (ex: data/saida.txt): ");
                scanf("%s", nome_arquivo);
                salvar_grafo_arquivo(grafo_atual, nome_arquivo);
                break;
            }
            case 4: {
                // Previne falhas de segmentação (segfault) caso o usuário tente operar um grafo inexistente
                if (grafo_atual == NULL) {
                    printf("Erro: Nenhum grafo carregado no sistema!\n");
                    break;
                }
                printf("Digite o nome do arquivo para a matriz (ex: data/matriz.txt): ");
                scanf("%s", nome_arquivo);
                gerar_e_salvar_matriz_adjacencia(grafo_atual, nome_arquivo);
                break;
            }
            case 5: {
                // Previne falhas de segmentação (segfault) caso o usuário tente operar um grafo inexistente
                if (grafo_atual == NULL) {
                    printf("Erro: Nenhum grafo carregado no sistema!\n");
                    break;
                }
                exibir_graus_dos_vertices(grafo_atual);
                break;
            }
            case 6: {
                // Previne falhas de segmentação (segfault) caso o usuário tente operar um grafo inexistente
                if (grafo_atual == NULL) {
                    printf("Erro: Nenhum grafo carregado no sistema!\n");
                    break;
                }
                arvore_geradora_minima_prim(grafo_atual);
                break;
            }
            case 7: {
                // Previne falhas de segmentação (segfault) caso o usuário tente operar um grafo inexistente
                if (grafo_atual == NULL) {
                    printf("Erro: Nenhum grafo carregado no sistema!\n");
                    break;
                }
                int origem;
                printf("Digite o vertice de origem (0 a %d): ", grafo_atual->num_vertices - 1);
                scanf("%d", &origem);
                caminho_mais_curto_dijkstra(grafo_atual, origem);
                break;
            }
            case 8: {
                // Previne falhas de segmentação (segfault) caso o usuário tente operar um grafo inexistente
                if (grafo_atual == NULL) {
                    printf("Erro: Nenhum grafo carregado no sistema!\n");
                    break;
                }
                int origem;
                printf("Digite o vertice de origem (0 a %d): ", grafo_atual->num_vertices - 1);
                scanf("%d", &origem);
                busca_em_largura(grafo_atual, origem);
                break;
            }
            case 9: {
                // Previne falhas de segmentação (segfault) caso o usuário tente operar um grafo inexistente
                if (grafo_atual == NULL) {
                    printf("Erro: Nenhum grafo carregado no sistema!\n");
                    break;
                }
                int origem;
                printf("Digite o vertice de origem (0 a %d): ", grafo_atual->num_vertices - 1);
                scanf("%d", &origem);
                busca_em_profundidade(grafo_atual, origem);
                break;
            }
            case 0: {
                printf("Encerrando o programa e limpando a memoria...\n");
                if (grafo_atual != NULL) {
                    liberar_grafo(grafo_atual);
                }
                break;
            }
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}