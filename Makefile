menu: suffixTree.o utilities.o queue.o menu.o
	gcc -o menu suffixTree.o utilities.o queue.o menu.o
suffixTree.o: suffixTree.h suffixTree.c
	gcc -c suffixTree.c -o suffixTree.o
utilities.o: utilities.h utilities.c
	gcc -c utilities.c -o utilities.o
queue.o: queue.h queue.c
	gcc -c queue.c -o queue.o
menu.o: menu.c suffixTree.h
	gcc -c menu.c -o menu.o
geeks: geeks.o
	gcc -o geeksForGeeks geeksForGeeks.o
geeks.o: geeksForGeeks.c
	gcc -c geeksForGeeks.c -o geeksForGeeks.o