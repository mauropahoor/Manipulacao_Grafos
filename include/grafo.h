#ifndef GRAFO_H
#define GRAFO_H

/**
 * Estrutura que representa uma aresta ou arco saindo de um vértice.
 * Corresponde a um nó dentro da lista encadeada.
 */
typedef struct NoAresta {
    int destino;             /**< Vértice extremo da aresta/arco (vj) */
    float peso;              /**< Peso da aresta (apenas se o grafo for valorado) */
    struct NoAresta* prox;   /**< Ponteiro para a próxima aresta na lista deste vértice */
} NoAresta;

/**
 * Estrutura principal que representa o Grafo/Dígrafo.
 */
typedef struct Grafo {
    int num_vertices;        /**< Quantidade total de vértices */
    int num_arestas;         /**< Quantidade total de arestas/arcos */
    char tipo;               /**< 'G' para grafo não dirigido, 'D' para dirigido */
    int valorado;            /**< 1 para valorado, 0 para não valorado */
    
    NoAresta** vetor_adjacencias; /**< Ponteiro para o início da lista de adjacência de cada vértice */
} Grafo;

/**
 * @brief Inicializa um grafo com o número de vértices, tipo ('G' para grafo não dirigido, 'D' para dirigido) e se é valorado ou não.
 * @param num_vertices Numero de vértices do grafo
 * @param tipo Tipo do grafo: 'G' para não dirigido, 'D' para dirigido
 * @param valorado Se o grafo é valorado (1) ou não valorado (0)
 * @return Grafo* Retorna o grafo inicializado ou NULL em caso de falha na alocação de memória
 */
Grafo* inicializar_grafo(int num_vertices, char tipo, int valorado);

/**
 * @brief Adiciona uma nova aresta ao grafo.
 * @param grafo Grafo previamente inicializado
 * @param origem Aresta de origem (vi)
 * @param destino Aresta de destino (vj)
 * @param peso Peso da aresta (apenas se o grafo for valorado)
 */
void adicionar_aresta(Grafo* grafo, int origem, int destino, float peso);

/**
 * @brief Libera toda a memória alocada para o grafo, incluindo as listas de adjacências e a estrutura principal.
 * @param grafo Grafo previamente inicializado
 */
void liberar_grafo(Grafo* grafo);

/**
 * @brief Lê um arquivo .txt contendo as especificações de um grafo e constrói a estrutura de dados interna.
 * @param nome_arquivo Caminho ou nome do arquivo a ser lido (ex: "entrada.txt")
 * @return Grafo* Retorna o ponteiro para o grafo carregado na memória ou NULL em caso de erro na leitura
 */
Grafo* ler_grafo_arquivo(const char* nome_arquivo);

/**
 * @brief Salva o grafo atual em um arquivo .txt seguindo o formato padrão do sistema.
 * @param grafo Ponteiro para a estrutura Grafo previamente inicializada.
 * @param nome_arquivo Caminho ou nome do arquivo onde os dados serão salvos
 */
void salvar_grafo_arquivo(Grafo* grafo, const char* nome_arquivo);

/**
 * @brief Gera a matriz de adjacências para o grafo atual e a salva em um arquivo de texto.
 * @param grafo Ponteiro para a estrutura Grafo previamente inicializada.
 * @param nome_arquivo Caminho ou nome do arquivo onde a matriz será salva
 */
void gerar_e_salvar_matriz_adjacencia(Grafo* grafo, const char* nome_arquivo);

/**
 * @brief Calcula e imprime o grau de cada vértice do grafo.
 * @param grafo Grafo dirigido ou não dirigido previamente inicializado.
 */
void exibir_graus_dos_vertices(Grafo* grafo);

/**
 * @brief Calcula e imprime a árvore geradora mínima do grafo utilizando o Algoritmo de Prim.
 *
 * @param grafo Ponteiro para a estrutura Grafo previamente inicializada.
 * @warning A operação de Árvore Geradora Mínima só é aplicável a grafos NÃO dirigidos ('G') e VALORADOS (1).
 */
void arvore_geradora_minima_prim(Grafo* grafo);

/**
 * @brief Calcula e imprime o caminho mais curto a partir de um vértice de origem.
 *
 * Esta função utiliza o Algoritmo de Dijkstra para encontrar a menor distância 
 * entre o vértice de origem e todos os outros vértices alcançáveis no grafo.
 * Os resultados, incluindo as distâncias e os caminhos passo a passo, são 
 * impressos diretamente na saída padrão (console).
 *
 * @param grafo Ponteiro para a estrutura Grafo previamente inicializada.
 * @param origem ID do vértice de onde a busca será iniciada (0 até num_vertices - 1).
 *
 * @warning O algoritmo de Dijkstra não funciona com grafos que contêm arestas 
 * de pesos negativos. Caso detectado, a função abortará a execução.
 * @note Se o grafo não for valorado (`grafo->valorado == 0`), a função assumirá
 * automaticamente o peso 1.0 para todas as arestas.
 */
void caminho_mais_curto_dijkstra(Grafo* grafo, int origem);

/**
 * @brief Esta função utiliza o Algoritmo de Busca em Largura (BFS) para explorar o grafo e
 * imprime a ordem de visita dos vértices a partir de um vértice de origem.
 *
 * @param grafo Ponteiro para a estrutura Grafo previamente inicializada.
 * @param origem ID do vértice de onde a busca será iniciada
 *
 * @warning A função assume que o vértice de origem é válido (entre 0 e num_vertices - 1). 
 * Se um valor inválido for fornecido, a função imprimirá uma mensagem de erro e retornará sem realizar a operação.
 */
void busca_em_largura(Grafo* grafo, int origem);

/**
 * @brief Esta função utiliza o Algoritmo de Busca em Profundidade (DFS) para explorar o grafo de forma recursiva e
 * imprime a ordem de visita dos vértices a partir de um vértice de origem.
 *
 * @param grafo Ponteiro para a estrutura Grafo previamente inicializada.
 * @param origem ID do vértice de onde a busca será iniciada
 *
 * @warning A função assume que o vértice de origem é válido (entre 0 e num_vertices - 1). 
 * Se um valor inválido for fornecido, a função imprimirá uma mensagem de erro e retornará sem realizar a operação.
 */
void busca_em_profundidade(Grafo* grafo, int origem);

#endif // GRAFO_H