#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void displaySelectedWords(char **arr, int k){
    for (int i=0; i<k; i++){
        if (arr[i]) printf("%s\n", arr[i]);
    }
}

char **get_me_random_n_words(int argc, char *argv[], int maxLen){
    if (argc==3){
        char *filename = argv[1];
        int k = atoi(argv[2]);
        FILE *fp = fopen(filename, "r");
        // k is the number of words that we want to choose
        // Create the reservoir
        char **arr = (char **) calloc(k+1, sizeof(char *));
        for (int i=0; i<k+1; i++){
            arr[i] = (char *) calloc(maxLen, sizeof(char));
        }
        int i = 0;
        // Add first k items
        while (i<k){
            // fgets(arr[i], maxLen, fp);
            fscanf(fp, "%s\n", arr[k]);
            // printf("Here1\n");
            i++;
        }
        srand(time(NULL));

        // printf("Here2\n");
        char *temp = (char *) malloc(sizeof(char)*maxLen);
        while (!feof(fp)){
            int j = rand() % (i+1);
            printf("feof\n");
            if (j<k){
                printf("1\n");
                fscanf(fp, "%[^\n]s", temp);
                printf("%s\n", temp);
                fseek(fp, 1, SEEK_SET);
                fseek(fp, 1, SEEK_SET);
                if (!arr[j]) break;
            } else{
                printf("2\n");
                fscanf(fp, "%[^\n]s", temp);
                fseek(fp, 1, SEEK_SET);
                fseek(fp, 1, SEEK_SET);
                printf("%s\n", temp);
                if (!arr[k]) break;
            }
            i++;
        }

        printf("Outside\n");
        displaySelectedWords(arr, k);
        return arr;
    } else{
        printf("Need 2 arguments --- <word_file>.txt <number_of_words>\n");
    }
}
