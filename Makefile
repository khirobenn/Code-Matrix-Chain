all: solution solution_pruned
	python ./compare_algos_plot.py solution solution_pruned

solution: ./solve_backtracking.exe matrix
	./solve_backtracking.exe matrix solution

matrix: ./generate_instances.exe
	./generate_instances.exe 4 17 1 100 matrix

./generate_instances.exe:
	gcc generate_instances.c -o generate_instances.exe

./solve_backtracking.exe:
	gcc solve_backtracking.c -o solve_backtracking.exe

solution_pruned: ./solve_backtracking_pruned.exe
	./solve_backtracking_pruned.exe matrix solution_pruned

./solve_backtracking_pruned.exe:
	gcc solve_backtracking_pruned.c -o solve_backtracking_pruned.exe


clean :
	rm matrix
