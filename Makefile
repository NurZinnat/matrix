FLAGS = -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
OPT = -O3

CC = g++

TTT = a.out mult.out

all: $(TTT)

mult.out: mult.o matrix_inverse.o matrix.o sub_matrix.o block.o reflection.o data.o
	$(CC) $(OPT) $(FLAGS) mult.o matrix_inverse.o matrix.o sub_matrix.o block.o reflection.o data.o -o mult.out


a.out: main.o matrix_inverse.o matrix.o sub_matrix.o block.o reflection.o data.o
	$(CC) $(OPT) $(FLAGS) main.o matrix_inverse.o matrix.o sub_matrix.o block.o reflection.o data.o -o a.out

main.o: main.cpp matrix_inverse.h matrix.h block.h sub_matrix.h reflection.h data.h
	$(CC) -c $(OPT) $(FLAGS) main.cpp

mult.o: mult.cpp matrix_inverse.h matrix.h block.h sub_matrix.h reflection.h data.h
	$(CC) -c $(OPT) $(FLAGS) mult.cpp

matrix_inverse.o: matrix_inverse.cpp matrix_inverse.h
	$(CC) -c $(OPT) $(FLAGS) matrix_inverse.cpp

matrix.o: matrix.cpp matrix.h
	$(CC) -c $(OPT) $(FLAGS) matrix.cpp

sub_matrix.o: sub_matrix.cpp sub_matrix.h
	$(CC) -c $(OPT) $(FLAGS) sub_matrix.cpp

block.o: block.cpp block.h
	$(CC) -c $(OPT) $(FLAGS) block.cpp

reflection.o: reflection.cpp reflection.h
	$(CC) -c $(OPT) $(FLAGS) reflection.cpp

data.o: data.cpp data.h
	$(CC) -c $(OPT) $(FLAGS) data.cpp

clean:
	rm -rf *.o $(TTT) *.d