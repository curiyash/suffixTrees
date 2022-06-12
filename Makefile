menu: dnaSuffixTree.o dnaUtilities.o queue.o maxLen.o menu.o lca.o stack.o
	gcc -o menu dnaSuffixTree.o dnaUtilities.o queue.o maxLen.o menu.o lca.o stack.o
dictionary: suffixTree.o utilities.o queue.o maxLen.o get_me_random_n_words.o dictionary.o
	gcc -o dictionary suffixTree.o utilities.o queue.o maxLen.o get_me_random_n_words.o dictionary.o
dna: dna.o dnaSuffixTree.o dnaUtilities.o queue.o maxLen.o lca.o stack.o
	gcc -o dna dna.o dnaSuffixTree.o dnaUtilities.o queue.o maxLen.o lca.o stack.o
dnaSuffixTree.o: dnaSuffixTree.c dnaSuffixTree.h
	gcc -c dnaSuffixTree.c -o dnaSuffixTree.o
dnaUtilities.o: dnaUtilities.h dnaUtilities.c
	gcc -c dnaUtilities.c -o dnaUtilities.o
lca: lca.o dnaSuffixTree.o dnaUtilities.o stack.o
	gcc -o lca dnaSuffixTree.o lca.o dnaUtilities.o stack.o
lca.o: lca.c lca.h
	gcc -c -lm lca.c -o lca.o
stack.o: stack.c stack.h
	gcc -c stack.c -o stack.o
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