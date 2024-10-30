#include "mymemory.h"     // Inclui o arquivo de cabeçalho com as declarações de tipos e funções
#include <stdio.h>        // Biblioteca padrão para funções de entrada e saída
#include <stdlib.h>       // Biblioteca padrão para alocação e liberação de memória
#include <string.h>       // Biblioteca padrão para manipulação de strings

// Inicializa o pool de memória com o tamanho especificado
mymemory_t* mymemory_init(size_t size) {
    // Aloca espaço para a estrutura mymemory_t que gerencia o pool
    mymemory_t *memory = (mymemory_t *)malloc(sizeof(mymemory_t));
    if (!memory) return NULL; // Verifica se a alocação foi bem-sucedida; retorna NULL se falhou

    // Aloca o bloco de memória real que será gerenciado
    memory->pool = malloc(size);
    if (!memory->pool) {      // Verifica se a alocação foi bem-sucedida
        free(memory);         // Se falhou, libera a estrutura mymemory_t
        return NULL;
    }

    memory->total_size = size; // Armazena o tamanho total do pool
    // Cria o bloco inicial na lista de alocações
    memory->head = (allocation_t *)malloc(sizeof(allocation_t));
    if (!memory->head) {       // Verifica se a alocação foi bem-sucedida
        free(memory->pool);    // Se falhou, libera o bloco de memória
        free(memory);          // e a estrutura mymemory_t
        return NULL;
    }

    // Configura o bloco inicial como um bloco livre que cobre toda a memória
    memory->head->start = memory->pool;
    memory->head->size = size;
    memory->head->next = NULL; // Marca como último bloco da lista

    return memory; // Retorna o ponteiro para a estrutura de gerenciamento de memória
}

// Aloca um bloco de memória do tamanho solicitado
void* mymemory_alloc(mymemory_t *memory, size_t size) {
    allocation_t *current = memory->head; // Ponteiro para percorrer a lista de blocos
    allocation_t *prev = NULL;            // Ponteiro para armazenar o bloco anterior

    // Procura por um bloco livre com espaço suficiente
    while (current) {
        if (current->size >= size) {      // Se o bloco atual tem espaço suficiente
            void *allocated_memory = current->start; // Salva o endereço inicial do bloco

            // Se o bloco atual é maior que o necessário
            if (current->size > size) {
                // Cria um novo bloco para o espaço restante
                allocation_t *new_block = (allocation_t *)malloc(sizeof(allocation_t));
                new_block->start = (char *)current->start + size; // Define o início do novo bloco
                new_block->size = current->size - size;           // Define o tamanho restante
                new_block->next = current->next;                  // Conecta ao próximo bloco

                current->next = new_block;   // Atualiza o próximo bloco do atual para o novo bloco
            }
            
            current->size = size;            // Ajusta o tamanho do bloco alocado
            return allocated_memory;         // Retorna o endereço do bloco alocado
        }
        
        prev = current;                      // Armazena o bloco atual como o anterior
        current = current->next;             // Passa para o próximo bloco
    }
    
    return NULL; // Retorna NULL se não houver memória suficiente
}

// Libera um bloco de memória anteriormente alocado
void mymemory_free(mymemory_t *memory, void *ptr) {
    allocation_t *current = memory->head; // Ponteiro para percorrer a lista de blocos
    allocation_t *prev = NULL;            // Ponteiro para armazenar o bloco anterior

    // Procura pelo bloco que corresponde ao ponteiro fornecido
    while (current) {
        if (current->start == ptr) {     // Se o bloco atual corresponde ao ponteiro fornecido
            current->size = 0;           // Marca o bloco como liberado

            // Se o bloco não é o primeiro, remove da lista
            if (prev) {
                prev->next = current->next; // Conecta o bloco anterior ao próximo
                free(current);              // Libera o bloco atual
            } else {
                memory->head = current->next; // Atualiza o início da lista
                free(current);                // Libera o bloco atual
            }
            return;
        }
        
        prev = current;                    // Atualiza o bloco anterior
        current = current->next;           // Passa para o próximo bloco
    }
}

// Exibe as alocações atuais
void mymemory_display(mymemory_t *memory) {
    allocation_t *current = memory->head; // Ponteiro para percorrer a lista de blocos
    while (current) {
        // Imprime o endereço inicial e o tamanho do bloco atual
        printf("Start: %p, Size: %zu\n", current->start, current->size);
        current = current->next;          // Passa para o próximo bloco
    }
}

// Exibe estatísticas da memória
void mymemory_stats(mymemory_t *memory) {
    allocation_t *current = memory->head; // Ponteiro para percorrer a lista de blocos
    size_t total_allocated = 0;           // Variável para armazenar o total alocado
    size_t largest_free_block = 0;        // Variável para armazenar o maior bloco livre
    int free_fragments = 0;               // Contador de fragmentos livres

    while (current) {
        if (current->size > 0) {          // Se o bloco está alocado
            total_allocated += current->size; // Acumula o tamanho alocado
        } else {                          // Se o bloco está livre
            free_fragments++;             // Incrementa o contador de fragmentos livres
            if (current->size > largest_free_block) {
                largest_free_block = current->size; // Atualiza o maior bloco livre
            }
        }
        current = current->next;          // Passa para o próximo bloco
    }

    // Exibe as estatísticas de memória
    printf("Total allocated: %zu bytes\n", total_allocated);
    printf("Largest free block: %zu bytes\n", largest_free_block);
    printf("Free fragments: %d\n", free_fragments);
}

// Limpa todos os blocos de memória alocados e o pool
void mymemory_cleanup(mymemory_t *memory) {
    allocation_t *current = memory->head; // Ponteiro para percorrer a lista de blocos
    while (current) {
        allocation_t *temp = current;     // Armazena o bloco atual
        current = current->next;          // Passa para o próximo bloco
        free(temp);                       // Libera o bloco atual
    }

    free(memory->pool);                   // Libera o pool de memória real
    free(memory);                         // Libera a estrutura de gerenciamento de memória
}
