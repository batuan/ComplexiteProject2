import sys

square = 0
line = []
size = int(sys.argv[2])
edges = int(sys.argv[3])
def p(x, y):
    return (x-1)*edges + y

with open(sys.argv[1], 'r') as file:
    line = file.read()
    if "SAT" in line:
        print("S O L U T I O N\n- - - - - - - - -")
    solution = line.split()
    for i in range(1, size+1):
        for j in range(1, edges+1):
            if str(p(i, j)) in solution:
                print(j, end="  ")
    print()
