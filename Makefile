solutions/all: solutions/backtracking_solution solutions/bt_pruned_solution solutions/dp_solution solutions/greedy1_solution solutions/greedy2_solution solutions/my_greedy_solution
	python ./compare_algos_plot.py solutions/backtracking_solution solutions/bt_pruned_solution solutions/dp_solution solutions/greedy1_solution solutions/greedy2_solution solutions/my_greedy_solution

solutions/backtracking_solution: ./solve_backtracking.exe matrix
	./solve_backtracking.exe matrix solutions/backtracking_solution

matrix: ./generate_instances.exe
	./generate_instances.exe 4 17 1 100 matrix

./generate_instances.exe:
	gcc generate_instances.c -o generate_instances.exe

./solve_backtracking.exe:
	gcc solve_backtracking.c -o solve_backtracking.exe

solutions/bt_pruned_solution: ./solve_backtracking_pruned.exe
	./solve_backtracking_pruned.exe matrix solutions/bt_pruned_solution

./solve_backtracking_pruned.exe:
	gcc solve_backtracking_pruned.c -o solve_backtracking_pruned.exe

solutions/dp_solution : ./solve_dp.exe
	./solve_dp.exe matrix solutions/dp_solution

./solve_dp.exe:
	gcc solve_dp.c -o solve_dp.exe

solutions/greedy1_solution : ./solve_greedy1.exe
	./solve_greedy1.exe matrix solutions/greedy1_solution

./solve_greedy1.exe:
	gcc solve_greedy1.c -o solve_greedy1.exe

solutions/greedy2_solution : ./solve_greedy2.exe
	./solve_greedy2.exe matrix solutions/greedy2_solution

./solve_greedy2.exe:
	gcc solve_greedy2.c -o solve_greedy2.exe

solutions/my_greedy_solution : ./solve_my_greedy.exe
	./solve_my_greedy.exe matrix solutions/my_greedy_solution

./solve_my_greedy.exe:
	gcc solve_my_greedy.c -o solve_my_greedy.exe

clean :
	rm matrix *.exe solutions/* *.png
