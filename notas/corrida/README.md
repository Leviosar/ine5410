# Condições de corrida

Ao executar código paralelo, alguns cuidados devem ser tomados para que a concorrência não acabe afetando negativamente o resultado do programa. Existem diversos problemas que podem ser causados pelo mau uso dessa tecnologia, uma grande parte delas devido a problemas de sincronização.

Uma condição de corrida (__race condition__) é um problema de sincronização que ocorre quando duas ou mais threads atuam sobre um mesmo espaço de memória, mais especificamente com operações de leitura. Suponha o seguinte código escrito em linguagem C:

```c
#include <pthread.h>

void increment_counter(int* counter) {
    *counter += 1;
}

void decrement_counter(int* counter) {
    *counter -= 1;
}

int main() {
    int counter = 8;
    pthread_t decrement_thread, increment_thread;

    pthread_create(&decrement_thread, NULL, decrement_counter, (void *) &counter);
    pthread_create(&increment_thread, NULL, increment_counter, (void *) &counter);

    return 0;
}
```

Ao executar o código, qual será o valor final de `counter`? Se você disse 7, você está errado. Se você disse 9 você também está errado. A resposta correta é: não sabemos com exatidão o valor que estará guardado em memória após ambas as threads executarem. Isso acontece porque o SO pode dar/retirar tempo de CPU de uma ou outra thread por meio do escalonador, fazendo com que elas executem por partes e em ordem diferente.

Vamos supor 2 cenários distintos, no primeiro a thread `decrement_thread` é executada completamente e só depois do seu fim, a thread `increment_thread` ganha espaço na CPU e executa completamente, esse aqui seria nosso caso ideal, o valo final de `counter` seria 8.

No segundo cenário, a thread `decrement_thread` começa a executar, busca o valor `8` de `counter` em memória e antes de realizar a soma, o SO interrompe sua execução e realiza uma troca de contexto para `increment_thread`, que por sua vez executa normalmente, deixando o valor `9` como resultado em `counter`. Ao final da `increment_thread` o escalonador devolve a CPU para `decrement_thread`, que realiza a operação `8 -= 1` (pois o valor 8 estava salvo localmente para a thread) e guarda o valor `7` no endereço de memória de `counter`.

Tivemos um problema aqui, nós rodamos o mesmo código duas vezes com as mesmas variáveis iniciais e tivemos resultados distintos. Isso acontece porque as duas threads não estavam bem sincronizadas e uma sobrescreveu a ação da outra, fazendo com que basicamente a outra thread nunca tivesse *efetivamente* executado.

Existem mecanismo de controle de concorrência que podem ser utilizados para resolver esse tipo de problema, entre eles estão os [mutexes](./../mutexes/) e [semáforos](./../semaforos).