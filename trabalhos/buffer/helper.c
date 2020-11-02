#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "buffer.h"

/***************************************************************
 *                          ATENÇÃO!                           *
 * Não altere esse arquivo! Ele será substituido na avaliação! *
 ***************************************************************/

void dump_buffer(buffer_t* b) {
    printf("  [");
    for (int i = 0; i < b->size; ++i)  {
        printf("%s%3d", i == 0 ? "" : " ",
                         b->data[(b->take_idx+i) % b->capacity]);
    }
    printf("]\n");
}

/***************************************************************
 *                          ATENÇÃO!                           *
 * Não altere esse arquivo! Ele será substituido na avaliação! *
 ***************************************************************/

int ler_comando(buffer_t* b) {
    printf(">");
    fflush(stdout);
    char cmd;
    if (scanf(" %c", &cmd) <= 0)
        return 0; //parar de ler
    cmd = tolower(cmd);
    if (cmd == 'q')
        return 0; //parar de ler
        
    if (cmd == 'r') {
        printf("  --> %d\n", take_buffer(b));
        dump_buffer(b);
    } else if (cmd == 'c') {
        int r = rand() % 1000;
        printf("  <-- %d\n", r);
        if (put_buffer(b, r) < 0) {
            printf("Não foi possível buffer_colocar, buffer cheio!\n");
        } else {
            dump_buffer(b);
        }
    }
    return 1; //ler denovo
}

/***************************************************************
 *                          ATENÇÃO!                           *
 * Não altere esse arquivo! Ele será substituido na avaliação! *
 ***************************************************************/
