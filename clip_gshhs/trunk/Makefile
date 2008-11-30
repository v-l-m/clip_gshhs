all:
	gcc -c read_gshhs.c
	gcc -c mercator.c
	gcc -c readgshhs_01.c

	gcc -o readgshhs readgshhs_01.o mercator.o read_gshhs.o -lgd -lpng

