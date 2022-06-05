menu: suffixTree.o utilities.o queue.o maxLen.o get_me_random_n_words.o menu.o
	gcc -o menu suffixTree.o utilities.o queue.o maxLen.o get_me_random_n_words.o menu.o
get_me_random_n_words.o: get_me_random_n_words.c
	gcc -c get_me_random_n_words.c -o get_me_random_n_words.o
maxLen.o: maxLen.c
	gcc -c maxLen.c -o maxLen.o
suffixTree.o: suffixTree.h suffixTree.c
	gcc -c suffixTree.c -o suffixTree.o
utilities.o: utilities.h utilities.c
	gcc -c utilities.c -o utilities.o
queue.o: queue.h queue.c
	gcc -c queue.c -o queue.o
menu.o: menu.c suffixTree.h
	gcc -c menu.c -o menu.o