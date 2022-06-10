menu: suffixTree.o utilities.o queue.o maxLen.o get_me_random_n_words.o menu.o
	gcc -o menu suffixTree.o utilities.o queue.o maxLen.o get_me_random_n_words.o menu.o
dictionary: suffixTree.o utilities.o queue.o maxLen.o get_me_random_n_words.o dictionary.o
	gcc -o dictionary suffixTree.o utilities.o queue.o maxLen.o get_me_random_n_words.o dictionary.o
dna: dna.o suffixTree.o utilities.o queue.o maxLen.o
	gcc -o dna dna.o suffixTree.o utilities.o queue.o maxLen.o
lca: lca.o suffixTree.o utilities.o
	gcc -o lca suffixTree.o lca.o utilities.o
lca.o: lca.c lca.h
	gcc -c lca.c -o lca.o
dictionary.o: dictionary.c
	gcc -c dictionary.c -o dictionary.o
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