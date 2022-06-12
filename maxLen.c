#include <stdio.h>
#include <stdlib.h>

int maxLen(int argc, char *argv[]){
    int maxLen = 0;
    if (argc==3){
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            perror("Unable to open file\n");
            return 1;
        }
        int i = 0;
        while (!feof(fp)){
            int len = 0;
            char c;
            while (1){
                fscanf(fp, "%c", &c);
                if (feof(fp)) break;
                if (c=='\n' || c=='\r') break;
                len++;
            }
            if (len>maxLen){
                maxLen = len;
            }
        }
    } else{
        printf("Need 1 argument --- <word_file>.txt <out_file>.txt\n");
    }
    return maxLen;
}
