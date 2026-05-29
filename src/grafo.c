#include <stdio.h>
#include <stdlib.h>
#include "../include/grafo.h"

// Função auxiliar interna para alocar um novo nó na memória
NoAresta* criar_no(int destino, float peso) {
    NoAresta* novo_no = (NoAresta*)malloc(sizeof(NoAresta));
    if (novo_no == NULL) {
        printf("Erro fatal: Falha na alocação de memória para o nó!\n");
        exit(1);
    }
    novo_no->destino = destino;
    novo_no->peso = peso;
    novo_no->prox = NULL;
    return novo_no;
}

Grafo* inicializar_grafo(int num_vertices, char tipo, int valorado) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) {
        printf("Erro fatal: Falha na alocação de memória para o grafo!\n");
        exit(1);
    }
    
    grafo->num_vertices = num_vertices;
    grafo->num_arestas = 0;
    grafo->tipo = tipo;
    grafo->valorado = valorado;
    
    // Aloca o vetor de ponteiros para as listas de adjacência
    grafo->vetor_adjacencias = (NoAresta**)malloc(num_vertices * sizeof(NoAresta*));
    for (int i = 0; i < num_vertices; i++) {
        grafo->vetor_adjacencias[i] = NULL;
    }
    
    return grafo;
}

void liberar_grafo(Grafo* grafo) {
    if (grafo != NULL) {
        for (int i = 0; i < grafo->num_vertices; i++) {
            NoAresta* atual = grafo->vetor_adjacencias[i];
            while (atual != NULL) {
                NoAresta* temp = atual;
                atual = atual->prox;
                free(temp);
            }
        }
        free(grafo->vetor_adjacencias);
        free(grafo);
    }
}

void adicionar_aresta(Grafo* grafo, int origem, int destino, float peso) {
    // Insere o nó no INÍCIO da lista da origem (Complexidade O(1))
    NoAresta* novo_no = criar_no(destino, peso);
    novo_no->prox = grafo->vetor_adjacencias[origem];
    grafo->vetor_adjacencias[origem] = novo_no;
    
    // Se for um Grafo Não Dirigido (G), precisamos adicionar a aresta de "volta"
    // Evitamos adicionar duas vezes caso seja um auto-loop (origem == destino)
    if (grafo->tipo == 'G' && origem != destino) {
        NoAresta* no_volta = criar_no(origem, peso);
        no_volta->prox = grafo->vetor_adjacencias[destino];
        grafo->vetor_adjacencias[destino] = no_volta;
    }
    
    grafo->num_arestas++;
}

Grafo* ler_grafo_arquivo(const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nome_arquivo);
        return NULL;
    }
    
    int num_v, num_a, valorado;
    char tipo;
    
    // Lê a primeira linha com as definições do grafo
    fscanf(arquivo, "%d %d %c %d", &num_v, &num_a, &tipo, &valorado);
    
    Grafo* grafo = inicializar_grafo(num_v, tipo, valorado);
    
    int origem, destino;
    float peso = 0.0;
    
    // Lê as arestas das linhas subsequentes
    for (int i = 0; i < num_a; i++) {
        if (valorado == 1) {
            fscanf(arquivo, "%d %d %f", &origem, &destino, &peso);
        } else {
            fscanf(arquivo, "%d %d", &origem, &destino);
        }
        adicionar_aresta(grafo, origem, destino, peso);
    }
    
    fclose(arquivo);
    return grafo;
}

void salvar_grafo_arquivo(Grafo* grafo, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel criar o arquivo %s\n", nome_arquivo);
        return;
    }
    
    // Escreve a 1ª linha
    fprintf(arquivo, "%d %d %c %d\n", grafo->num_vertices, grafo->num_arestas, grafo->tipo, grafo->valorado);
    
    // Itera sobre as listas para escrever as arestas
    for (int i = 0; i < grafo->num_vertices; i++) {
        NoAresta* atual = grafo->vetor_adjacencias[i];
        while (atual != NULL) {
            // Em grafos não dirigidos (G), a aresta i-j também existe como j-i.
            // Para não duplicar a impressão no arquivo de saída, imprimimos apenas se i <= destino.
            if (grafo->tipo == 'D' || (grafo->tipo == 'G' && i <= atual->destino)) {
                if (grafo->valorado == 1) {
                    fprintf(arquivo, "%d %d %.2f\n", i, atual->destino, atual->peso);
                } else {
                    fprintf(arquivo, "%d %d\n", i, atual->destino);
                }
            }
            atual = atual->prox;
        }
    }
    
    fclose(arquivo);
    printf("Grafo salvo com sucesso no arquivo: %s\n", nome_arquivo);
}

void gerar_e_salvar_matriz_adjacencia(Grafo* grafo, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel criar o arquivo da matriz %s\n", nome_arquivo);
        return;
    }
    
    int n = grafo->num_vertices;
    
    float** matriz = (float**)malloc(n * sizeof(float*));
    for (int i = 0; i < n; i++) {
        matriz[i] = (float*)calloc(n, sizeof(float)); 
    }
    
    // Preenchimento da matriz baseando-se na lista de adjacências
    for (int i = 0; i < n; i++) {
        NoAresta* atual = grafo->vetor_adjacencias[i];
        while (atual != NULL) {
            int j = atual->destino;
            
            // Em caso de arestas paralelas (múltiplas), a última lida vai sobrescrever.
            // Matrizes padrão não lidam bem com múltiplas arestas entre 2 mesmos nós.
            if (grafo->valorado == 1) {
                matriz[i][j] = atual->peso;
            } else {
                matriz[i][j] = 1.0; // 1 indica que a aresta existe
            }
            atual = atual->prox;
        }
    }
    
    // Escrita da matriz no arquivo formato txt
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grafo->valorado == 1) {
                fprintf(arquivo, "%.2f", matriz[i][j]);
            } else {
                fprintf(arquivo, "%d", (int)matriz[i][j]);
            }
            
            // Adiciona espaço entre colunas, mas evita espaço extra no final
            if (j < n - 1) fprintf(arquivo, " ");
        }
        fprintf(arquivo, "\n");
    }
    
    for (int i = 0; i < n; i++) {
        free(matriz[i]);
    }
    free(matriz);
    
    fclose(arquivo);
    printf("Matriz de adjacencias gerada e salva em: %s\n", nome_arquivo);
}

void exibir_graus_dos_vertices(Grafo* grafo) {
    if (grafo == NULL) return;

    printf("\n--- GRAUS DOS VERTICES ---\n");

    if (grafo->tipo == 'G') {
        for (int i = 0; i < grafo->num_vertices; i++) {
            int grau = 0;
            NoAresta* atual = grafo->vetor_adjacencias[i];
            
            while (atual != NULL) {
                grau++;
                // Na teoria dos grafos, um auto-loop (aresta para si mesmo) soma 2 ao grau
                if (atual->destino == i) {
                    grau++;
                }
                atual = atual->prox;
            }
            printf("Vertice %d: Grau %d\n", i, grau);
        }
    } 
    else if (grafo->tipo == 'D') {
        int n = grafo->num_vertices;
        
        int* grau_entrada = (int*)calloc(n, sizeof(int));
        int* grau_saida = (int*)calloc(n, sizeof(int));

        for (int i = 0; i < n; i++) {
            NoAresta* atual = grafo->vetor_adjacencias[i];
            
            while (atual != NULL) {
                grau_saida[i]++; // O vértice 'i' está saindo para algum lugar
                grau_entrada[atual->destino]++; // O vértice 'destino' está recebendo uma aresta
                
                atual = atual->prox;
            }
        }

        for (int i = 0; i < n; i++) {
            printf("Vertice %d: Grau de Entrada = %d | Grau de Saida = %d\n", 
                   i, grau_entrada[i], grau_saida[i]);
        }

        free(grau_entrada);
        free(grau_saida);
    }
    printf("--------------------------\n");
}

void arvore_geradora_minima_prim(Grafo* grafo) {
    if (grafo == NULL) return;

    if (grafo->tipo == 'D' || grafo->valorado == 0) {
        printf("\nErro: A operacao de Arvore Geradora Minima so e aplicavel a grafos NAO dirigidos e VALORADOS.\n");
        return;
    }

    int n = grafo->num_vertices;
    
    int* pai = (int*)malloc(n * sizeof(int));
    
    float* chave = (float*)malloc(n * sizeof(float));
    
    int* incluido_na_agm = (int*)calloc(n, sizeof(int)); 

    for (int i = 0; i < n; i++) {
        chave[i] = 999999.0; // Simula infinito
        pai[i] = -1;
    }

    // A raiz da árvore será o vértice 0
    chave[0] = 0.0; 

    // A AGM terá exatamente V vértices
    for (int count = 0; count < n - 1; count++) {
        // Passo 1: Encontrar o vértice u com a menor chave, que ainda não está na AGM
        float min = 999999.0;
        int u = -1;

        for (int v = 0; v < n; v++) {
            if (incluido_na_agm[v] == 0 && chave[v] < min) {
                min = chave[v];
                u = v;
            }
        }
        
        // Se u continuou -1, o grafo é desconexo (não dá pra fazer AGM de todos os nós)
        if (u == -1) break; 

        // Passo 2: Adiciona o vértice escolhido u na AGM
        incluido_na_agm[u] = 1;

        // Passo 3: Atualiza as chaves e os pais dos vértices adjacentes a u
        NoAresta* atual = grafo->vetor_adjacencias[u];
        while (atual != NULL) {
            int v = atual->destino;
            float peso_uv = atual->peso;

            // Se o vizinho v não está na AGM e o peso da aresta u-v é menor que a chave atual de v
            if (incluido_na_agm[v] == 0 && peso_uv < chave[v]) {
                pai[v] = u;
                chave[v] = peso_uv;
            }
            atual = atual->prox;
        }
    }

    printf("\n--- ARVORE GERADORA MINIMA (AGM) ---\n");
    float peso_total = 0.0;
    
    printf("Aresta \tPeso\n");
    for (int i = 1; i < n; i++) {
        // Só imprime se o vértice foi realmente alcançado (grafo conexo)
        if (pai[i] != -1) { 
            printf("%d - %d \t%.2f\n", pai[i], i, chave[i]);
            peso_total += chave[i];
        }
    }
    printf("Peso Total da AGM: %.2f\n", peso_total);
    printf("------------------------------------\n");

    free(pai);
    free(chave);
    free(incluido_na_agm);
}

// Função interna para imprimir o caminho reconstruindo pelos pais
void imprimir_caminho(int destino, int* pai) {
    if (pai[destino] == -1) {
        printf("%d", destino);
        return;
    }
    imprimir_caminho(pai[destino], pai);
    printf(" -> %d", destino);
}


void caminho_mais_curto_dijkstra(Grafo* grafo, int origem) {
    if (grafo == NULL) return;

    int n = grafo->num_vertices;

    // Verificação de segurança: Dijkstra não funciona com pesos negativos
    for (int i = 0; i < n; i++) {
        NoAresta* atual = grafo->vetor_adjacencias[i];
        while (atual != NULL) {
            if (grafo->valorado == 1 && atual->peso < 0) {
                printf("\nErro: O algoritmo de Dijkstra nao suporta arestas com pesos negativos.\n");
                return;
            }
            atual = atual->prox;
        }
    }

    if (origem < 0 || origem >= n) {
        printf("\nErro: Vertice de origem invalido.\n");
        return;
    }

    float* dist = (float*)malloc(n * sizeof(float)); // Guarda a menor distância conhecida
    int* pai = (int*)malloc(n * sizeof(int));        // Guarda o vértice anterior no caminho mais curto
    int* visitado = (int*)calloc(n, sizeof(int));    // 1 se a menor distância final já foi definida

    for (int i = 0; i < n; i++) {
        dist[i] = 999999.0; // Infinito
        pai[i] = -1;
    }
    dist[origem] = 0.0;

    for (int count = 0; count < n; count++) {
        // Passo 1: Encontrar o vértice não visitado com a menor distância
        float min_dist = 999999.0;
        int u = -1;

        for (int v = 0; v < n; v++) {
            if (!visitado[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }

        // Se u for -1, todos os vértices alcançáveis já foram visitados
        if (u == -1) break;

        // Passo 2: Marcar u como visitado
        visitado[u] = 1;

        // Passo 3: Relaxamento das arestas adjacentes a u
        NoAresta* atual = grafo->vetor_adjacencias[u];
        while (atual != NULL) {
            int v = atual->destino;
            // Se o grafo não for valorado, assumimos peso 1 para todas as arestas
            float peso_uv = (grafo->valorado == 1) ? atual->peso : 1.0; 

            // Se v não foi visitado e o novo caminho passando por u for mais curto
            if (!visitado[v] && dist[u] + peso_uv < dist[v]) {
                dist[v] = dist[u] + peso_uv;
                pai[v] = u;
            }
            atual = atual->prox;
        }
    }

    printf("\n--- CAMINHO MAIS CURTO (DIJKSTRA) A PARTIR DE %d ---\n", origem);
    printf("Vertice\t Distancia\t Caminho\n");
    for (int i = 0; i < n; i++) {
        if (dist[i] == 999999.0) {
            printf("%d \t Inalcancavel\t -\n", i);
        } else {
            printf("%d \t %.2f \t\t ", i, dist[i]);
            imprimir_caminho(i, pai);
            printf("\n");
        }
    }
    printf("---------------------------------------------------\n");

    free(dist);
    free(pai);
    free(visitado);
}

void busca_em_largura(Grafo* grafo, int origem) {
    if (grafo == NULL) return;

    int n = grafo->num_vertices;

    if (origem < 0 || origem >= n) {
        printf("\nErro: Vertice de origem invalido.\n");
        return;
    }

    int* visitado = (int*)calloc(n, sizeof(int));

    int* fila = (int*)malloc(n * sizeof(int));
    int inicio_fila = 0;
    int fim_fila = 0;

    printf("\n--- ORDEM DE VISITA (BUSCA EM LARGURA) ---\n");
    printf("Iniciando a partir do vertice %d...\n\n", origem);
    printf("Caminho: ");

    // 1. Marca a origem como visitada e a coloca na fila
    visitado[origem] = 1;
    fila[fim_fila] = origem;
    fim_fila++; // Aumenta o fim da fila

    // 2. Enquanto a fila não estiver vazia
    while (inicio_fila < fim_fila) {
        // Remove o primeiro elemento da fila (Dequeue)
        int u = fila[inicio_fila];
        inicio_fila++; 
        
        printf("%d ", u);

        // 3. Verifica todos os vizinhos de 'u'
        NoAresta* atual = grafo->vetor_adjacencias[u];
        while (atual != NULL) {
            int v = atual->destino;
            
            // Se o vizinho ainda não foi visitado
            if (visitado[v] == 0) {
                visitado[v] = 1;          // Marca como visitado para não ser colocado na fila de novo
                fila[fim_fila] = v;       // Coloca o vizinho no final da fila (Enqueue)
                fim_fila++;
            }
            atual = atual->prox;
        }
    }
    
    printf("\n------------------------------------------\n");

    free(visitado);
    free(fila);
}

// Função auxiliar recursiva que realmente executa a DFS
void dfs_recursiva(Grafo* grafo, int u, int* visitado) {
    // 1. Marca o vértice atual como visitado e imprime
    visitado[u] = 1;
    printf("%d ", u);

    // 2. Percorre todos os vizinhos do vértice atual
    NoAresta* atual = grafo->vetor_adjacencias[u];
    while (atual != NULL) {
        int v = atual->destino;
        
        // 3. Se o vizinho ainda não foi visitado, mergulhamos nele (chamada recursiva)
        if (visitado[v] == 0) {
            dfs_recursiva(grafo, v, visitado);
        }
        
        atual = atual->prox;
    }
}

void busca_em_profundidade(Grafo* grafo, int origem) {
    if (grafo == NULL) return;

    int n = grafo->num_vertices;

    if (origem < 0 || origem >= n) {
        printf("\nErro: Vertice de origem invalido.\n");
        return;
    }

    int* visitado = (int*)calloc(n, sizeof(int));

    printf("\n--- ORDEM DE VISITA (BUSCA EM PROFUNDIDADE) ---\n");
    printf("Iniciando a partir do vertice %d...\n\n", origem);
    printf("Caminho: ");

    // Dispara a chamada recursiva a partir da origem
    dfs_recursiva(grafo, origem, visitado);

    printf("\n\n-----------------------------------------------\n");

    free(visitado);
}