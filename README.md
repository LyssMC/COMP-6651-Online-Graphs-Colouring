# COMP-6651-Online-Graphs-Colouring
Implementation of Greedy Algorithms for colouring online graphs, study of their performance.


## How to run the project

Open a terminal and navigate to the project directory.

Compile the program using:

```bash
gcc -Wall -O2 main.c graph.c generator.c firstfit.c cbip.c -o graphprog -lm
```

Run the executable and save the output to a CSV file:

```bash
./graphprog > results.csv
```