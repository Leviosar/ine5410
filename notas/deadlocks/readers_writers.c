#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int reader_count = 0;
int data = 0;

sem_t write_lock, count_lock;

void write_f() {
    data += 1;
    sleep(1);
}

void read_f() {
    sleep(1);
}

void * reader(void* arg) {
    int index = *((int*) arg);

    sem_wait(&count_lock);

    reader_count += 1;

    if (reader_count == 1) {
        sem_wait(&write_lock);
    }

    sem_post(&count_lock);

    printf("Reader %d read the data: %d\n", index, data);
    read_f();

    sem_wait(&count_lock);
    
    reader_count -= 1;
    if (reader_count == 0)
        sem_post(&write_lock);        
    
    sem_post(&count_lock);
}

void * writer(void* arg) {
    int index = *((int*) arg);
    
    sem_wait(&write_lock);

    printf("Writer %d is writing data\n", index, index);
    write_f();

    sem_post(&write_lock);
    
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s writers readers\n", argv[0], argv[0]);
        return 0;
    }

    int writers_count = atoi(argv[1]);
    int readers_count = atoi(argv[2]);

    sem_init(&write_lock, 0, 1);
    sem_init(&count_lock, 0, 1);

    pthread_t readers[readers_count], writers[writers_count];
    int readers_args[readers_count], writers_args[writers_count]; 

    for (size_t i = 0; i < writers_count; i++) {
        writers_args[i] = i;
        pthread_create(&writers[i], NULL, writer, (void *) &writers_args[i]);
    }

    for (size_t i = 0; i < readers_count; i++) {
        readers_args[i] = i;
        pthread_create(&readers[i], NULL, reader, (void *) &readers_args[i]);
    }
    
    for (size_t i = 0; i < writers_count; i++)
        pthread_join(writers[i], NULL);    
    
    for (size_t i = 0; i < readers_count; i++)
        pthread_join(readers[i], NULL);    

    sem_destroy(&write_lock);
    sem_destroy(&count_lock);

    return 0;
}