#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void displaySelectedWords(char **arr, int k){
    for (int i=0; i<k; i++){
        if (arr[i]) printf("%s\n", arr[i]);
    }
}

int get_me_random_n_words(int argc, char *argv[], int maxLen, char **arr){
    if (argc==3){
        printf("maxLen: %d\n", maxLen);
        char *filename = argv[1];
        int k = atoi(argv[2]);
        FILE *fp = fopen(filename, "r");
        // k is the number of words that we want to choose
        // Create the reservoir
        for (int i=0; i<k+1; i++){
            arr[i] = (char *) malloc(sizeof(char)*maxLen);
        }
        int i = 0;
        // Add first k items
        while (i<k){
            // fgets(arr[i], maxLen, fp);
            fscanf(fp, "%s\n", arr[i]);
            // printf("Here1\n");
            i++;
        }
        srand(time(NULL));

        // printf("Here2\n");
        char *temp = (char *) malloc(sizeof(char)*maxLen);
        while (!feof(fp)){
            int j = rand() % (i+1);
            // printf("feof\n");
            if (j<k){
                // printf("1\n");
                fscanf(fp, "%s\n", arr[j]);
                if (!(*arr)[j]) break;
            } else{
                // printf("2\n");
                fscanf(fp, "%s\n", temp);
                // printf("%s\n", temp);
                if (!temp) break;
            }
            i++;
        }

        // printf("Outside\n");
        displaySelectedWords(arr, k);
        // return arr;
    } else{
        printf("Need 2 arguments --- <word_file>.txt <number_of_words>\n");
    }
    return 0;
}
