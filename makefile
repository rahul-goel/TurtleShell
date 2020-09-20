all:
	gcc -g -c global_var.c
	gcc -g -c prompt.c
	gcc -g -c cd.c
	gcc -g -c ls.c
	gcc -g -c pwd.c
	gcc -g -c ctrl_c.c
	gcc -g -c echo.c
	gcc -g -c execute.c
	gcc -g -c execute_interface.c
	gcc -g -c pinfo.c
	gcc -g -c history.c
	gcc -g -c bg_proc_list.c
	gcc -g -c nightswatch.c
	gcc -g -c env_var.c
	gcc -g -c jobs.c
	gcc -g main.c global_var.o prompt.o ls.o pwd.o echo.o execute.o execute_interface.o pinfo.o cd.o ctrl_c.o history.o bg_proc_list.o nightswatch.o env_var.o jobs.o -ltinfo -lncurses -o TurtleShell
	rm -rf *.o
