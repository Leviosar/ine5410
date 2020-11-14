# OpenMP

É uma interface de programação, baseada em um modelo de programação paralela com memória compartilhada. Multilinguagem, com implementações em C, C++ e Fortran.

# Componentes

- Diretivas de compilação
- Biblioteca de execução
- Variáveis de ambiente

# Vantagens do OpenMP

Possui bom desempenho e escalabilidade, portabilidade e exige pouco esforço de programação. Também permite ao programador paralelizar um programa aos poucos.

# Fork and Join

No início de execução de um programa OpenMP, existe apenas uma única thread (_master thread_), mas ao atingir uma região paralela do código, cria N _worker threads_ no processo de "Fork".

Quando as _worker threads_ terminam de executar, elas chegam em uma barreira temporal implícita, esperando que as outras terminem, para então estarem sincronizadas e a execução continuar a partir da _master thread_, com todas as _worker threads_ sendo finalizadas.

# Utilização

O OpenMP está disponível no compilador `ifort` de Fortran, e também nos compiladores `gcc`, `g++` e `icc` de C e C++.

Para incluir a biblioteca em um código C, usamos `#include <omp.h>`, e para compilar no GCC precisamos utilizar a flag `-fopenmp`.

# Rotinas úteis

`omp_set_num_threads(int t)` - define o número total de threads a serem utilizadas em regiões paralelas.

`omp_get_num_threads(void)` - retorna o número de threads em execução numa região paralela (retorna 1 caso invocado fora de uma região paralela).

`omp_get_thread_num(void)` - retorna o ID de uma thread dentro de uma região paralela.

`omp_get_num_procs(void)` - retorna o número de núcleos de processamento disponíveis na máquina.

# Diretivas de compilação

Num programa OpenMP, especificamos diretivas de compilação para definir o paralelismo do programa, essas diretivas são interpretadas por cada compilador e são diferentes para cada linguagem da implementação.

- C/C++: #pragma omp {diretiva} {clausulas}
- Fortran: !$OMP {diretiva} {clausulas}

Um exemplo de diretiva na linguagem C, é a diretiva `parallel` que cria uma região paralela, feita da seguinte forma `#pragma omp parallel`. Veremos a seguir mais sobre ela.

# `Parallel`

Essa diretiva é uma das bases principais do OpenMP, ela indica o início de uma região paralela do código, o que fará com que o OpenMP crie _worker threads_ para executar o bloco de código abaixo da diretiva. Vamos dar uma olhadinha abaixo num Hello World que executa o `printf` em N threads paralelas, sendo N o número de núcleos de processamento da sua máquina.

```c
#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel
    printf("Hello World paralelo!\n");
    printf("Aqui voltou para a região normal\n");
}
```

Aqui, ao compilar e executar o código, o output gerado é:

```bash
[leviosar@marvin openmp]$ gcc hello_world_openmp.c -o hello -fopenmp
[leviosar@marvin openmp]$ ./hello
Hello World paralelo!
Hello World paralelo!
Hello World paralelo!
Hello World paralelo!
Aqui voltou para a região normal
[leviosar@marvin openmp]$ _
```

# Escopo de diretivas

Algumas diretivas só podem ser aplicadas a uma única linha de código, enquanto outras permitem um bloco de código completo. Como vimos antes, a diretiva `#pragma omp parallel` criou uma região paralela contendo apenas o código da minha primeira chamada a `printf`. E como faríamos pra fazer com que ela paralelizasse as duas partes do código? Simples, criamos um bloco


```c
#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel
    {
        printf("Primeiro!\n");
        printf("Segundo!\n");
    }
}
```

Nesse caso, o programa irá criar N threads para executar o código dentro do bloco, mas como essas threads estão sujeitas ao escalonamento do SO, não sabemos se todas elas irão executar em ordem (estatísticamente não vão). Um exemplo de saída que pode acontecer no caso acima é:

```
Primeiro!
Primeiro!
Segundo!
Segundo!
Primeiro!
Segundo!
Primeiro!
Segundo!
```

# Cláusulas

Existem uma porrada de cláusulas para serem utilizadas com cada uma das diretivas, modificando o comportamento delas para facilitar o uso mais "customizado" da biblioteca. Se você quiser ver todas elas fique a vontade pra passar horas lendo a documentação do OpenMP, mas por aqui vamos ver só algumas mais importantes.

### private()
    
- Cria uma cópia local das variáveis em cada _worker thread_
- Cópias locais NÃO são inicializadas
- Recebe como argumento uma lista de variáveis.

**Exemplo**:

```c
    int a = -1;
    #pragma omp parallel private(a)
    printf(“dentro = %d\n”, a);
    printf(“fora = %d\n”, a);
```

Nesse caso, como sabemos que a variável interna não será inicializada, supomos que o valor dela quando o `printf` de dentro é executado é um lixo de memória, enquanto no lado "de fora", temos o valor -1 definido na primeira linha.

### firstprivate()

- Cria uma cópia local das variáveis em cada _worker thread_
- Cópias locais SÃO inicializadas com o valor que a variável possuia antes da região paralela.
- Recebe como argumento uma lista de variáveis.

A função dessa cláusula é funcionar como uma `private`que inicializa o valor, caso o valor anterior precise ser usado por algum motivo.

### shared()

- Indica que as variáveis passadas são compartilhadas entre as _worker threads_
- Dessa forma, todas as _threads_ usarão a mesma instância das variáveis passadas
- Por não ser uma cópia local, mudanças em uma variável serão repassadas para outras threads

### reduction()

- Cria uma cópia localdas variáveis em cada thread
- Cópias locais são inicializadas com valor 0 ou 1, em função do operador de redução usado
- Ao final da região paralela ocorre uma operação de redução em todas as variáveis locais
- Os valores armazenados nas variáveis locais são copiados para as variáveis externasà região paralela
- Argumentos: um operador de redução (reduction-id) e uma lista de variáveis (list)

Mas o que diabos é um operador de redução? São operações de redução (ou seja, quando uma lista de entradas é reduzida para um único valor numérico) representados por operadores e funções da linguagem, na tabela abaixo você vê os operadores disponíveis na linguagem C.

![Tabela de operadores de redução](https://imgur.com/mDU6OKz.png)

# Diretiva `for`

- Utilizada dentro de uma região paralela ou através de uma construção combinada `parallel for`
- Permite paralelizar laços de maneira automática
- Iterações do laço são distribuídase executadas em paralelo pelas threads da região paralela

Podemos usar essa diretiva sempre que cada iteração do nosso `for` NÃO depender de iterações anteriores, um exemplo comum são operações vetoriais. Imagine que temos dois vetores A e B, de tamanho `N`, e queremos realizar a operação `C = A + B` que soma as entradas do vetor uma a uma. Criando um laço para isso, temos algo parecido com:

```c
for (i = 0; i < N; i++) {
    C[i] = A[i] + B[i];
}
```

Nesse caso, estamos usando apenas uma thread, que irá executar sequencialmente todos os passos do nosso laço. Mas nosso objetivo aqui é paralelizar o programa, então criaremos uma região paralela com a diretiva `parallel` e depois usaremos a diretiva `for` para que nosso laço seja executado de forma paralela pelas _worker threads_ da região.

```c
#pragma omp parallel
#pragma omp for
for (i = 0; i < N; i++) {
    C[i] = A[i] + B[i];
}
```

Ou então:

```c
#pragma omp parallel for
for (i = 0; i < N; i++) {
    C[i] = A[i] + B[i];
}
```

O número de iterações que cada _worker thread_ executa é balanceado pelo próprio OpenMP de forma oculta.

## Cláusulas da diretiva `for`

### schedule(kind[, chunk_size])

- Define como as iterações de um laço serão divididas entre as threads (estratégia de escalonamento)
- Iterações individuais podem ser agrupadas emblocos, denominados _chunks_
- Argumentos: estratégia de escalonamento (_kind_) e tamanho de blocos de iterações (_chunk\_size_)
    - _kind_: _static_, _dynamic_, _guided_ ou _auto_
    - _chunk\_size_: quantidade de iterações em cada bloco
    - _static_ e _dynamic_ possuem _chunk_ de tamanho fixo
    - _guided_ possui _chunk_ de tamanho variável

#### Estratégias de escalonamento

A diretiva `for` do OpenMP permite 3 tipos diferentes de estratégias de escalonamento: _static_, _dynamic_ e _guided_. Vamos falar um pouco sobre as diferenças entre elas. Para isso, usaremos como exemplo um laço de 40 iterações que será distribuido em 4 _threads_.

1. _Static_

No caso da _static_, cada _worker thread_ receberá um pacote de execução, irá fazer a computação necessária para finalizar esse pacote e esperar suas _threads_ irmãs. Caso ao final de todas as execuções ainda tenham sobrado pacotes, elas repetem o processo até que não existam mais pacotes.

![Static schedule](https://imgur.com/aIvV8d5.png)

2. _Dynamic_

Na estratégia _dynamic_, também teremos todas as threads pegando um pacote de execução, mas dessa vez ao finalizar seu pacote, caso ainda existam pacotes a serem processados a _thread_ irá pegar eles imediatamente, sem esperar que suas irmãs terminem. Isso é muito interessante para problemas onde cada iteração tem um custo computacional diferente, assim temos menos tempo de ociosidade.

![Dynamic schedule](https://imgur.com/A4Ewc6m.png)

3. _Guided_

A estratégia _guided_ utiliza pacotes de tamanho variável, alocando pacotes maiores no começo da execução e menores ao final, para reduzir a quantidade de acessos à pilha de pacotes de execução, já que essa é protegida por mecanismos de exclusão mútua e tem um certo custo ao ser acessada.

![Guided schedule](https://imgur.com/Ck9fV7P.png)