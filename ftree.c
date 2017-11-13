#include <stdio.h>
#include <stdlib.h>
// Add your system includes here.
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "ftree.h"
#include "hash.h"
//#include "hash_functions.c"
#define MAX_NAME_SIZE 100
#define MAX_FILE_NUM 100


/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) {
    // Your implementation here.
    //get file  information
    struct stat buf;
    struct TreeNode *tn = malloc(sizeof(struct TreeNode));
    struct dirent *dp;
    char *currentpath = malloc(sizeof(char)*MAX_NAME_SIZE);
    if(getcwd(currentpath, MAX_NAME_SIZE+1) == NULL){
        perror("getcwd");
        exit(1);
    }

    int check = lstat(fname, &buf);
    //printf("%s\n", fname);
    //if the lstat not work
    if(check == -1){
      //printf("%s\n", fname);
      perror("lstat\n");
      exit(1);
    }
    //write the information into struct
    tn->fname = malloc(sizeof(char) * MAX_NAME_SIZE);
    strcpy(tn->fname, fname);
    tn->permissions = (buf.st_mode & 0777);
    //initialize to NULL
    tn->hash = NULL;
    tn->next =NULL;
    tn->contents = NULL;

    //find out it's a file or a dir
    //if it's a dir
    if(S_ISDIR(buf.st_mode)){
        DIR *dirp = opendir(fname);
        if(dirp == NULL) {
            perror("opendir\n");
            exit(1);
        }
        //printf("dir:%s\n", fname);
        //update the current working path
        strcat(currentpath,"/");
        strcat(currentpath, fname);

        struct TreeNode *curr;
        struct TreeNode *past;
        //read dir
        dp = readdir(dirp);
        while(dp != NULL){
            if(dp->d_name[0] != '.'){
                //set the current working dir to the open dir if it's
                //not already in there
                char ck[MAX_NAME_SIZE];
                if(getcwd(ck, MAX_NAME_SIZE+1) == NULL){
                    perror("getcwd\n");
                    exit(1);
                }
                if(strcmp(ck, currentpath) != 0){
                    int c = chdir(currentpath);
                    if(c != 0){
                        perror("chdir");
                        exit(1);
                    }
                }

                //first element in the dir
                if(tn->contents == NULL){
                    tn->contents = malloc(sizeof(struct TreeNode));
                    tn->contents = generate_ftree(dp->d_name);
                    curr = tn->contents;
                    //printf("contents creat\n" );
                }
                //other elements in the dir
                else if(tn->contents != NULL){
                    past = curr;
                    curr = generate_ftree(dp->d_name);
                    past->next = curr;
                    //printf("next creat\n" );
                }


            }
            dp = readdir(dirp);
        }
        chdir("..");
        closedir(dirp);
    }

    else if(S_ISREG(buf.st_mode)||S_ISLNK(buf.st_mode)){
        FILE *f = fopen(fname, "rb");
        if(f == NULL){
            perror("fopen");
            exit(1);
        }
        tn->hash = malloc (sizeof(char) * 8);
        strcpy(tn->hash, hash(f));
        //printf("file:%s\n", fname);

    }

    return tn;
}


/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    printf("%*s", depth * 2, "");

    // Your implementation here.

    if(root->fname != NULL && root->hash != NULL){
        printf("%s (%o)\n",root->fname, root->permissions);
    }
    else if(root->contents != NULL){
        printf("===== %s (%o) =====\n",root->fname, root->permissions);
        depth++;
	
        print_ftree(root->contents);
    }
    if(root->next != NULL){
        print_ftree(root->next);
	if(depth != 0){
	    depth--;
	}
    }
}
/*
int main(int argc, char const *argv[]) {
    if(argc == 2){
    struct TreeNode * t;
    t = generate_ftree(argv[1]);
    print_ftree(t);
    }
    else{
        printf("parameter\n" );
    }
    return 0;
}
*/
