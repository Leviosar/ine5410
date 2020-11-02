# Exercício 1

Escreva um programa que receba dois números como argumento na linha de comando (use `argc` e `argv` da função main) que representam um número de threads (`n_threads`) e o número de incrementos (`n_loops`) que as `n_threads` farão em uma variável global. O programa deve criar `n_threads` threads que paralelamente incrementam `n_loops` vezes, em um laço, uma variável global chamada `contador_global`. Siga essas instruções detalhadas:

- O programa deve receber `n_threads` em argv[1] e `n_loops` em argv[2].
    - Isso já vem implementado no código inicial!
- A variável global deve se chamar `contador_global` e deve ser inicializada com 0.
- O programa deve criar `n_threads` threads.
- Cada thread deve realizar `n_loops` incrementos em `contador_global`.
    - `n_loops` incrementos significa fazer `++contador_global`  `n_loops` vezes
    - `n_loops` deve ser passado como argumento da função executada pela thread (via `pthread_create()`).
    - Não crie uma variável global para esse fim!
- A thread principal deve esperar o término de todas as outras threads (pthread_join()).
- Após o término das demais, a thread principal deve imprimir o valor do contador e o valor esperado, conforme esse exemplo (`n_threads`=1, `n_loops`=32023):

```
    Contador: 32023 
    Esperado: 32023
```

Execute este programa e vá aumentando o número de threads a cada execução (ex.: 2, 5, 10, 30, 50, 100, ...). Observe o que acontece com o valor do contador_global conforme o número de threads aumenta. Observe também o que acontece quando o n_loops aumenta. 

**Reflita sobre os seguintes pontos**:

1. Qual a relação entre n_threads e a diferença entre "Contador" e "Esperado"?
    - Aumentando a quantidade de threads, pode existir diferença entre os dois valores, porém para valores baixos de n_loops essa diferença diminui. 
2. Qual a relação entre n_loops e a diferença entre "Contador" e "Esperado"?
    - Aumentando a quantidade de loops, pode existir diferença entre os dois valores, porém para valores baixos de n_threads essa diferença diminui. 
3. Existe alguma regra ou padrão na diferença entre "Contador" e "Esperado"?
    - Não é propriamente um padrão, o problema é que ao usar uma variável global corremos o risco de duas threads alterarem o valor concorrentemente e isso causar uma contagem a menos no final do programa, isso poderia ser resolvido por um lock. Para valores maiores de threads e loops, a probabilidade de que aconteça uma alteração concorrente é maior.

# Exercício 2

Paralelize o programa dado, que realiza a soma de dois vetores. Esse programa recebe dois vetores em arquivos e mostra o resultado na saída:

```
./program 1 a b 
12 7 5 5 14 16 11 14 11 15
```

O primeiro argumento ("1" no exemplo acima) é o número de threads, e os dois últimos argumentos são os nomes dos arquivos que contém os vetores. O programador original implementou a leitura dos argumentos e a leitura dos vetores, mas desapareceu misteriosamente antes de efetivamente usar threads. Sua implementação deve criar o número de threads indicado na variável `n_threads` e deve distribuir as operações `c[i] = a[i] + b[i]` entre essas threads. 

**Dicas**:

- Evite variáveis globais. Use o último argumento de `pthread_create()` para que cada worker thread receba um ponteiro cujo struct apontado contém as informações relevantes para aquela thread.
- Os arquivos a e b do exemplo acima, assim como o resultado (arquivo c) estão no esqueleto fornecido.
- Você não deve gerar mais threads do que elementos! Se o usuário solicitar 20 threads, e os vetores possuírem apenas 10 elementos, então apenas 10 threads deverão ser criadas
- É possível gerar vetores aleatórios com um tamanho especifico usando uma sintaxe especial:
    - `./program 1 gen:2000000 gen:2000000`
- Cuidado para não criar vetores grandes demais e travar a sua máquina!

Execute o programa com várias escolhas para o número de threads e meça o speedup. Reflita sobre os seguintes pontos:

1. O speedup obtido está proximo do speedup ideal?
2. O programa escala, ou seja, o speedup aumenta se aumentarmos o número de threads?

### Benchmarks

Ao executar o programa de forma serial, temos o seguinte output:

![Benchmark](https://imgur.com/QMECcKU.png)

Ao executar o programa paralelizado com o número máximo de 1 thread, temos o seguinte output:

![Benchmark](https://imgur.com/x5GESqy.png)

Ao executar o programa paralelizado com o número máximo de 3 threads, temos o seguinte output:

![Benchmark](https://imgur.com/DdkCwb6.png)

Ao executar o programa paralelizado com o número máximo de 10 threads, temos o seguinte output:

![Benchmark](https://imgur.com/U0RiD38.png)

Ao executar o programa paralelizado com o número máximo de 12 threads, temos o seguinte output:

![Benchmark](https://imgur.com/8FB4gI5.png)

# Exercício 3

Em outro caso de programador desaparecido, você deve paralelizar um programa que calcula o produto escalar de dois vetores:

```
./program 1 a b 
Produto escalar: -9
```

O primeiro argumento ("1" é o número de threads a serem usadas para computação. Assim como no exercício 2, a sintaxe gen:N pode ser usada para testar sua implementação com vetores aleatórios grandes. Lembre que:

- O produto escalar é o somatório dos produtos entre os elementos pareados dos vetores, ou seja, `sum += a[i] * b[i]`, para todo elemento `i` dos vetores `a` e `b`.
- Os arquivos a e b, com vetores pequenos, estão incluídos no esqueleto fornecido.
- Evite globais onde possível.
