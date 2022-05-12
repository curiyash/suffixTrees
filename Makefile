menu: suffixTree.o menu.o
	gcc -o menu suffixTree.o menu.o
suffixTree.o: suffixTree.h suffixTree.c
	gcc -c suffixTree.c -o suffixTree.o
menu.o: menu.c suffixTree.h
	gcc -c menu.c -o menu.o