# Simulazione dell'evoluzione di un'epidemia - Linee guida per la compilazione ed esecuzione del programma <!-- omit in toc -->

>Autori: Elena Calzolaio, Elena Mariotti, Giovanni Pedrelli

Le *translation units* coinvolte in questo progetto sono `main.cpp`, `population.cpp`, `population.hpp`, `test.cpp` e `graph.cpp` (quest'ultimo da compilarsi col framework ROOT CERN).

La compilazione ed esecuzione del programma avviene **da terminale** scrivendo i seguenti comandi:

```bash
g++ -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast -D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined main.cpp population.cpp
```

```bash
./a.out
```

Per eseguire i test occorre scrivere:

```bash
g++ -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast -D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined test.cpp population.cpp -o test.out
```

```bash
./test.out
```
È possibile anche compilare ed eseguire con **`CMake`** coi comandi:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -S . -Bbuild
```

```bash
cmake --build build
```

```bash
cmake --build build --target test
```

```bash
build/population
```

Per eseguire i test occorre scrivere:


```bash
build/population.t
```


Se nel proprio computer sono installati il framework **ROOT CERN** e un server grafico è possibile eseguire i seguenti comandi per ottenere la rappresentazione grafica dell'andamento dell'epidemia:

```bash
root .
```
```bash
.X graph.cpp
```

È stato utilizzato il version controller `Git` e una [repository GitHub remota](https://github.com/giopedro92/sir-model-exam) per la gestione del codice.

Eseguito il programma, viene chiesto all'utente di inserire i parametri del modello potendo scegliere se inserirli *1. Da file di configurazione* o *2. Da terminale*. Digitando sul terminale il numero, si sceglie l'opzione corrispondente.

Il file di configurazione è chiamato `ConfigFile.txt` e al suo interno l'utente può inserire i valori dei parametri del modello secondo le stesse regole dell'immissione da terminale. I valori numerici vanno inseriti al posto di quelli di default, prima del cancelletto. Tutto quello che verrà inserito dopo il cancelletto verrà ignorato. Esempio:

```bash
0.6   # beta, probabilità di contagio                                (tra 0 e 1 inclusi)
0.3   # gamma, inverso del tempo medio di risoluzione dell'infezione (tra 0 e 1 inclusi)
10000 # dimensione della popolazione                                 (intero positivo)
1000  # durata dell'epidemia in giorni                               (intero positivo)
35    # numero iniziale di infetti                                   (intero positivo)
3     # tempo di incubazione in giorni                               (intero positivo)
0.3   # mu, tasso di mortalità                                       (tra 0 e 1 inclusi)
```