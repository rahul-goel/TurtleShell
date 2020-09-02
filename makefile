all:
	gcc -g -c global_var.c
	gcc -g -c prompt.c
	gcc -g -c cd.c
	gcc -g -c ls.c
	gcc -g -c pwd.c
	gcc -g -c echo.c
	gcc -g -c execute.c
	gcc -g -c pinfo.c
	gcc -g main.c global_var.o prompt.o ls.o pwd.o echo.o execute.o pinfo.o cd.o
