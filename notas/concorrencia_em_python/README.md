# Concorrência em Python

Um programa Python padrão, utiliza-se de um processo, com uma thread da execução principal do programa, e outras threads que são funcionalidades externas ao código do programador (como o _garbage collector_). Cada um dos processos é associado a uma _GIL_ (_Global interpreter lock_). Dessa forma, apenas uma _thread_ de cada processo pode executar por vez.

A _GIL_ é uma decisão de projeto, feita para que o _garbage collector_ trabalhe em exclusão mútua com as _threads_ do processo. Ela é implementada com um semáforo binário na implementação do _CPython_.

No caso do Python, o escalonamento das _threads_ é realizado diretamente pelo interpretador, tirando do SO essa responsabilidade.

Dessa forma, Python possui um suporte a concorrência, mas sem a possibilidade de paralelismo entre threads.

# Multiprocessing em Python

Como vimos, no modelo _multithread_ do Python não temos um paralelismo real, mas podemos contornar isso utilizando _multiprocessing_, criando vários processos para executar instruções, sendo que esses processos podem rodar simultâneamente em _cores_ diferentes.

Uma desvantagem dessa abordagem é que a comunicação entre processos é custosa, utilizando filas, pipes ou áreas de memória compartilhada.

```python

from multiprocessing import Process

def func1():
    pass

def func2():
    pass

proc1 = Process(target=func1)
proc2 = Process(target=func2)

proc1.start()
proc2.start()
proc1.join()
proc2.join()
```

### Métodos úteis da classe multiprocessing.Process

1. run - executado quando o processo é criado
2. start - inicia um objeto Process (invocará o run)
3. join - aguarda o processo terminar sua execução
4. is_alive - retorna um valor booleano, indicando se o processo ainda está executando
5. terminate - encerra o processo

### Atributos úteis da classe multiprocessing.Process

1. name - nome do objeto que instanciou Process
2. pid - pid do processo gerado
