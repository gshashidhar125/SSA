.phony: clean dump

main: main.o basic_block.o dominator.o frontier.o phi.o rename.o genir.h
	gcc -ggdb -g -o main main.o basic_block.o dominator.o frontier.o phi.o rename.o	genir.h

main.o: main.c genir.h
	gcc -c main.c

basic_block.o: basic_block.c genir.h
	gcc -c basic_block.c

dominator.o: dominator.c genir.h
	gcc -c dominator.c

frontier.o: frontier.c genir.h
	gcc -c frontier.c

phi.o: phi.c genir.h
	gcc -c phi.c

rename.o: rename.c genir.h
	gcc -c rename.c	

clean: 
	rm -f main *.o *.r *.dot *~ *.aux *.log *.gcov *.gcda *.gcno gmon.out

dump: 
	dot -Tps -o dump.ps cfg.dot semi.dot domi.dot DominanceFrontier.dot phi.dot rename.dot
