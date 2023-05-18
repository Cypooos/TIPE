FILES = out/algos.o out/matrix_algos.o out/benchmark.o out/graphe.o
GCCFLAGS = -Iinclude -Wall -g -O3 -l pthread

main: $(FILES) out/main.o 
	gcc $(GCCFLAGS) -o out/prog out/main.o $(FILES)

test: $(FILES) out/test.o 
	gcc $(GCCFLAGS) -o out/prog_test out/test.o $(FILES)
	valgrind --leak-check=full --show-leak-kinds=all ./out/prog_test

test2: $(FILES) out/test.o 
	gcc $(GCCFLAGS) -o out/prog_test out/test.o $(FILES)
	./out/prog_test

out/%.o: src/%.c
	gcc $(GCCFLAGS) -o $@ -c $<

clean :
	rm -f out/*.o out/prog out/prog_test
