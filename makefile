all:
	gcc -g -c -Wall -Werror global_var.c
	gcc -g -c -Wall -Werror prompt.c
	gcc -g -c -Wall -Werror cd.c
	gcc -g -c -Wall -Werror ls.c
	gcc -g -c -Wall -Werror pwd.c
	gcc -g -c -Wall -Werror ctrl_c.c
	gcc -g -c -Wall -Werror echo.c
	gcc -g -c -Wall -Werror execute.c
	gcc -g -c -Wall -Werror pinfo.c
	gcc -g -c -Wall -Werror history.c
	gcc -g -c -Wall -Werror bg_proc_list.c
	gcc -g -c -Wall -Werror nightswatch.c
	gcc -g -Wall -Werror main.c global_var.o prompt.o ls.o pwd.o echo.o execute.o pinfo.o cd.o ctrl_c.o history.o bg_proc_list.o nightswatch.o -ltinfo -lncurses -o TurtleShell
	rm -rf *.o
