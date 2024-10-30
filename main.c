#include <stdio.h>
#include <stdlib.h>  // Adicionado para uso de free e malloc
#include "mymemory.h"

int main() {
    // Inicializa a memória com um tamanho de 100 bytes.
    mymemory_t *memory = mymemory_init(100);
    if (!memory) {
        printf("Falha ao inicializar a memória.\n");
        return 1;
    }
    
    // Exibe o estado inicial da memória.
    printf("Estado inicial da memória:\n");
    mymemory_display(memory);

    // Aloca um bloco de 20 bytes.
    void *ptr1 = mymemory_alloc(memory, 20);
    if (ptr1) {
        printf("Alocado bloco de 20 bytes.\n");
        mymemory_display(memory);
    } else {
        printf("Falha ao alocar bloco de 20 bytes.\n");
    }

    // Aloca outro bloco de 30 bytes.
    void *ptr2 = mymemory_alloc(memory, 30);
    if (ptr2) {
        printf("Alocado bloco de 30 bytes.\n");
        mymemory_display(memory);
    } else {
        printf("Falha ao alocar bloco de 30 bytes.\n");
    }

    // Libera o primeiro bloco.
    mymemory_free(memory, ptr1);
    printf("Bloco de 20 bytes liberado.\n");
    mymemory_display(memory);

    // Libera o segundo bloco.
    mymemory_free(memory, ptr2);
    printf("Bloco de 30 bytes liberado.\n");
    mymemory_display(memory);

    // Libera a memória e finaliza.
    free(memory->pool);
    free(memory);
    return 0;
}
