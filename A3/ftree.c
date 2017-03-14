#include <stdio.h>
#include <stdlib.h>
// Add your system includes here.
#include "ftree.h"
#include "hash.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1+len2+1);
 
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);
    return result;
}

int indexfinder(const char* text, const char toFind){
    int index, length;
    index = 0;
    length = strlen(text);
    for (int i = 0; i<length; i ++){
        if (text[i] == toFind){
            index = i;
        }
    }
    return index;
}
/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) {
    // Your implementation here.
    struct TreeNode *root =(struct TreeNode *) malloc(sizeof(struct TreeNode));
    struct stat  *currentfile = malloc(sizeof(struct stat));
    struct dirent *dir_contents;
    DIR *directory;

    // if no root exists;
    if (lstat(fname, currentfile) < 0){
        printf("lstat: the name '%s' is not a file or directory \n", fname);
	    return root;
    }
    // trim fname
    char *filename = strrchr(fname, '/');
    if (filename != NULL)
    {
            root->fname = strdup(filename + 1);

    } else {
            root->fname = strdup(fname);

    }
    root->permissions = currentfile->st_mode & 0777;
    root->hash = NULL;
    root->contents = NULL;
    // if current root is a directory
    if (S_ISDIR(currentfile->st_mode)){
	    directory = opendir(fname);
        if (directory == NULL){
            fprintf(stderr, "Directory cannont be opened %s \n", root->fname);
            exit(1);
        }
        struct TreeNode *temp_root = malloc(sizeof(struct TreeNode));
	   	//if there is no files in directory
    	//go through each file
	    while ((dir_contents = readdir(directory)) != NULL){
            if (dir_contents->d_name[0] != '.'){ //exclude hidden files
                char *slash = "/";
                if (root->contents == NULL){
                    temp_root =  generate_ftree(concat(concat(fname,slash), dir_contents->d_name)); 
                    root->contents = temp_root;
                }
                else{
                    temp_root -> next = generate_ftree(concat(concat(fname,slash), dir_contents->d_name));
                    temp_root = temp_root->next;
                }
            }            
		}
        temp_root -> next = NULL;
   } else if (S_ISREG(currentfile->st_mode) || S_ISLNK(currentfile->st_mode)){ // if current root is a link/reg
        FILE *opened_file;
        opened_file = fopen(fname, "rb");
        if (opened_file){
            root->hash = hash(opened_file);
            fclose(opened_file);
            }
        else{
            fprintf(stderr, "File cannont be read %s \n", root->fname);
            exit(1);
            }
        }
        return root;
   }


/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    // Your implementation here.
    void print_ftree_helper(struct TreeNode *root, int depth);
    print_ftree_helper(root, depth);
}

void print_ftree_helper(struct TreeNode *root, int depth) {
    printf("%*s", depth * 2, "");
    depth += 1;
    // if node is a directory
    if (root->hash == NULL && root->fname != NULL){
        printf("==== %s (%o) ==== \n", root->fname, root->permissions);
        // loop through all the contents of the root directory
	    while ((root->contents) != NULL){  
		  print_ftree_helper(root->contents, depth);
		  root->contents = root->contents -> next;
	    }
    // if node is a file/link
    } else if (root->hash != NULL && root->fname != NULL){
	   printf("%s (%o)\n", root->fname, root->permissions);
	}
    return; 
}

int copy_ftree(const char *src, const char *dest) {
    // if source or destinatoin does not exist
    result = fork();
    struct stat *sourcefile = malloc(sizeof(struct stat));
    struct stat *destinationfile = malloc(sizeof(struct stat));
    DIR directory;
    struct dirent *dir_contents;
    if (lstat(src, sourcefile) < 0 || lstat(dest, destinationfile) < 0){
        return -1;
    }
    // if destination is not a directory, give error
    if (!(S_ISDIR(destinationfile -> st_mode)){
        return -1;
    } //if source is a file
    if (S_ISREG(sourcefile -> st_mode)){
        directory = opendir(dest);
        while((dir_contents = readdir(directory)) != NULL){
            if (strcmp(dir_contents -> d_name, src){ // if they have the same name
                struct stat *file2 = malloc(sizeof(struct stat));
                lstat(dir_contents -> d_name, file2); //need actual file path will do later
                FILE *f1 = fopen(src, 'rb');
                FILE *f2 = fopen(dir_contents ->d_name, 'rb');
                if (f1 & f2){
                    if (!((sourcefile->st_size == file2->st_size) && (strcmp(hash(f1),hash(f2))))){ // if they dont have same size and hash
                        fclose(f2);
                        f2 = fopen(dir_contents -> d_name, 'wb');
                        rewind(f1);
                        void buffer = malloc(100);
                        while((fread(buffer, 1, sizeof(buffer), f1) != 0){
                            fwrite(buffer, 1, sizeof(buffer), f2);
                        }

                    } 

                }
            else{ // file does not exists in destination directory
                FILE *f1 = fopen(src, 'rb');
                FILE *f2 = fopen
            }
            
        }
    }
}
