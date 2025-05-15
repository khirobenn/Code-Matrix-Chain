solutions/all: inemurate solutions/backtracking_solution.txt solutions/bt_pruned_solution.txt solutions/dp_solution.txt solutions/greedy1_solution.txt solutions/greedy2_solution.txt solutions/my_greedy_solution.txt
	python ./compare_algos_plot.py solutions/backtracking_solution.txt solutions/bt_pruned_solution.txt solutions/dp_solution.txt solutions/greedy1_solution.txt solutions/greedy2_solution.txt solutions/my_greedy_solution.txt

inemurate:
	gcc backtrack_inumerating.c -o backtrack_inumerating.exe

solutions/backtracking_solution.txt: ./solve_backtracking.exe matrix.txt
	./solve_backtracking.exe matrix.txt solutions/backtracking_solution.txt

matrix.txt: ./generate_instances.exe
	./generate_instances.exe 4 17 1 100 matrix.txt

./generate_instances.exe:
	gcc generate_instances.c -o generate_instances.exe

./solve_backtracking.exe:
	gcc solve_backtracking.c -o solve_backtracking.exe

solutions/bt_pruned_solution.txt: ./solve_backtracking_pruned.exe
	./solve_backtracking_pruned.exe matrix.txt solutions/bt_pruned_solution.txt

./solve_backtracking_pruned.exe:
	gcc solve_backtracking_pruned.c -o solve_backtracking_pruned.exe

solutions/dp_solution.txt : ./solve_dp.exe
	./solve_dp.exe matrix.txt solutions/dp_solution.txt

./solve_dp.exe:
	gcc solve_dp.c -o solve_dp.exe

solutions/greedy1_solution.txt : ./solve_greedy1.exe
	./solve_greedy1.exe matrix.txt solutions/greedy1_solution.txt

./solve_greedy1.exe:
	gcc solve_greedy1.c -o solve_greedy1.exe

solutions/greedy2_solution.txt : ./solve_greedy2.exe
	./solve_greedy2.exe matrix.txt solutions/greedy2_solution.txt

./solve_greedy2.exe:
	gcc solve_greedy2.c -o solve_greedy2.exe

solutions/my_greedy_solution.txt : ./solve_my_greedy.exe
	./solve_my_greedy.exe matrix.txt solutions/my_greedy_solution.txt

./solve_my_greedy.exe:
	gcc solve_my_greedy.c -o solve_my_greedy.exe

clean :
	rm matrix.txt *.exe solutions/*.txt *.png
