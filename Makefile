all: solution solution_pruned
	python ./compare_algos_plot.py solution solution_pruned

solution_pruned: ./solve_backtracking_pruned.exe
	./solve_backtracking_pruned.exe matrix solution_pruned

./solve_backtracking_pruned.exe:
	gcc solve_backtracking_pruned.c -o solve_backtracking_pruned.exe

solution: ./solve_backtracking.exe matrix
	./solve_backtracking.exe matrix solution

./solve_backtracking.exe:
	gcc solve_backtracking.c -o solve_backtracking.exe

matrix: ./generate_instances.exe
	./generate_instances.exe 4 20 1 100 matrix

./generate_instances.exe:
	gcc generate_instances.c -o generate_instances.exe

clean :
	rm matrix
