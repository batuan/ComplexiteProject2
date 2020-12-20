1. mini1: CNF reading
use: ./mini1 dimas.txt affect.txt
    - dimas.txt: file with format DIMACS CNF
    - affect.txt: input file of affectation of variables.
ex:
./mini1 ./TestData/mini1/test1.txt ./TestData/mini1/affect.txt
clause: 2 variable: 3
(x1 ∨ ¬x3) ∧ (¬x1 ∨ x2 ∨ x3).
affec: 1 1 0
verify: 1

2. mini2: Independent Set to SAT
use: ./mini2 graph.txt k
    - graph.txt: graph file with input is DIMACS CNF
    - k size of Zonde Vide to find

    python3 displayzonevide.py sat.txt size edges
    - sat.txt: sat solution using minisat
    - size: size of zone Vide
    - edges: number of edges in graph
    
ref: http://huntaj.stu.cofc.edu/web/AHuntExtraCredit310.pdf

mini3: Sudoku to sat
use: ./mini3 Sudoku.txt size
    - Sudoku.txt: input file of sudoku with format in the TestData
    - size: size of sudolu 3 for 9*9, 4 for 16*16, .....

    python3 display_sudoku.py sat.txt size
    - sat.txt: sat solution with using minisat
    - size: sudoku size
ref: https://people.mpi-sws.org/~joel/publications/sudoku05.pdf
