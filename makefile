all:
	gcc -c global_var.c
	gcc -c prompt.c
	gcc -c cd.c
	gcc -c ls.c
	gcc -c pwd.c
	gcc -c ctrl_c.c
	gcc -c echo.c
	gcc -c execute.c
	gcc -c pinfo.c
	gcc -c history.c
	gcc -c bg_proc_list.c
	gcc -c nightswatch.c
	gcc main.c global_var.o prompt.o ls.o pwd.o echo.o execute.o pinfo.o cd.o ctrl_c.o history.o bg_proc_list.o nightswatch.o -ltinfo -lncurses -o TurtleShell
	rm -rf *.o
