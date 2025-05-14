all: solution solution_pruned dp_solution greedy1_solution greedy2_solution
	python ./compare_algos_plot.py solution solution_pruned dp_solution greedy1_solution greedy2_solution

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

dp_solution : ./solve_dp.exe
	./solve_dp.exe matrix dp_solution

./solve_dp.exe:
	gcc solve_dp.c -o solve_dp.exe

greedy1_solution : ./solve_greedy1.exe
	./solve_greedy1.exe matrix greedy1_solution

./solve_greedy1.exe:
	gcc solve_greedy1.c -o solve_greedy1.exe

greedy2_solution : ./solve_greedy2.exe
	./solve_greedy2.exe matrix greedy2_solution

./solve_greedy2.exe:
	gcc solve_greedy2.c -o solve_greedy2.exe

clean :
	rm matrix
