//
//  main.c
//
//
//  Created by Adam on 10/10/13.
//
//

#define MAX_LEN 255

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include "tokenizer.h"
#include "strmap.h"

void filetok(FILE *fp, StrMap *hash, char *filename) {
    
    if (fp == NULL) {
        printf("ERROR: Could not open file. %s\n", strerror(errno));
        exit(1);
    }
    
    
    char *word = malloc(sizeof(char) * 256);
    
    
    
    char *token = NULL;
    
    
    while (fscanf(fp, "%s", word) != EOF) {
        
        TokenizerT *tokenizer = TKCreate(" ", word);
        while ( (token = TKGetNextToken(tokenizer)) != NULL) {
            sm_put(hash, token, filename);
            
        }
    }
    printf("done\n");
    
}

void traverse(DIR *dir, char *path, StrMap *hashmap) {
  
    DIR *sub_dir;
    struct dirent *dp;
    char *new_path;
    
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_name[0] != '.') {
            
            new_path = (char*)malloc(sizeof(path) + sizeof(dp->d_name) + sizeof(char));
            strcpy(new_path, path);
            strcat(new_path, dp->d_name);
            
            
            if ((sub_dir = opendir(new_path)) != NULL) {
                strcat(new_path, "/");
                traverse(sub_dir, new_path, hashmap);
            }
            else {
                
                char *filename = dp->d_name;
                printf("%s\n", new_path);
                FILE *file = fopen(new_path, "r");
                filetok(file, hashmap, filename);
            }
            
            
        }
    }
    
}

int file_exist (char *filename)
{
    struct stat   buffer;
    return (stat (filename, &buffer) == 0);
}

int main(int argc, char **argv) {
    
    if (argc != 3) {
        printf("ERROR: Incorrect number of inputs.\n");
        exit(0);
    }
    
    
    
    char *index_file = argv[1];
    char input[MAX_LEN];
    
    if (file_exist(index_file)) {
        do {
            printf("There is already a file named \"%s\", would you like to overwrite this file? Enter yes/no:\n", index_file);
            scanf("%s", input);
            if (strcasecmp(input, "yes") == 0) {
                break;
            }
            else if (strcasecmp(input, "no") == 0) {
                do {
                    printf("Choose another name: \n");
                    scanf("%s", input);
                    index_file = input;
                } while(file_exist(index_file));
                break;
            }
        } while (1);
        
    }
    
    StrMap *hashmap = sm_new(40);
    
    DIR *dir;
    struct dirent *dp;
    
    if ((dir = opendir(argv[2])) == NULL) {     //if file
        FILE *fp = fopen(argv[2], "r");
        filetok(fp, hashmap, argv[2]);
        printf("done\n");
        fclose(fp);
        exit(0);
    }
    
    char *path = (char*)malloc(sizeof(argv[2]) + sizeof(char));
    path = argv[2];
    strcat(path, "/");
    traverse(dir, path, hashmap);
    
    int x = sm_get_count(hashmap);
    printf("THIS IS HTE COUNT: %d\n", x);
    
    return 0;
    
}