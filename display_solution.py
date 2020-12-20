import sys

square = 0;
line = [];
size = int(sys.argv[2]);
square = size*size;

def p(x, y, z):
    return (((x - 1) * square) + (y - 1)) * square + z

with open(sys.argv[1], 'r') as file:
    line = file.read();
    if "SAT" in line:
        print("S O L U T I O N\n- - - - - - - - -")
    solution = line.split()
    for i in range(1, square+1):
        for j in range(1, square+1):
            for k in range(1, square+1):
                if str(p(i, j, k)) in solution:
                    if(k<10):
                        print(k, end="  ")
                    else:
                        print(k, end=" ")
        print()
