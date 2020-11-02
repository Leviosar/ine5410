#ifndef __BUFFER_H_
#define __BUFFER_H_

typedef struct buffer_s {
    /// Array onde devem ser colocados os elementos. No init_buffer(),
    /// esse ponteiro é incializado para uma região que consegue
    /// armazenar capacity elementos. No destroy_buffer(), essa região
    /// deve ser liberada
    int* data;
    /// indice onde irá ocorrer a próxima inserção (put)
    int put_idx;
    /// indice onde irá ocorrer a próxima remoção (take)
    int take_idx;
    /// Quantos elementos cabem em data
    int capacity;
    /// Quantos elementos estão em data
    int size;
} buffer_t;

void init_buffer(buffer_t* b, int capacity);
void destroy_buffer(buffer_t* b);
int take_buffer(buffer_t* b);
int put_buffer(buffer_t* b, int val);
void dump_buffer(buffer_t* b);


#endif /*__BUFFER_H_*/
