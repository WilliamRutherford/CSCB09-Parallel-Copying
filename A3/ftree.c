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
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>

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

/*
struct TreeNode *generate_ftree(const char *fname) {
    // Your implementation here.
    struct TreeNode *root =(struct TreeNode *) malloc(sizeof(struct TreeNode));
    struct stat  *currentfile = malloc(sizeof(struct stat));
    struct dirent *dir_contents;
    DIR *directory;

//     // if no root exists;
//     if (lstat(fname, currentfile) < 0){
//         printf("lstat: the name '%s' is not a file or directory \n", fname);
//      return root;
//     }
//     // trim fname
//     char *filename = strrchr(fname, '/');
//     if (filename != NULL)
//     {
//             root->fname = strdup(filename + 1);

//     } else {
//             root->fname = strdup(fname);

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
*/

/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */

/*
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    // Your implementation here.
    void print_ftree_helper(struct TreeNode *root, int depth);
    print_ftree_helper(root, depth);
}

// void print_ftree_helper(struct TreeNode *root, int depth) {
//     printf("%*s", depth * 2, "");
//     depth += 1;
//     // if node is a directory
//     if (root->hash == NULL && root->fname != NULL){
//         printf("==== %s (%o) ==== \n", root->fname, root->permissions);
//         // loop through all the contents of the root directory
//      while ((root->contents) != NULL){  
//        print_ftree_helper(root->contents, depth);
//        root->contents = root->contents -> next;
//      }
//     // if node is a file/link
//     } else if (root->hash != NULL && root->fname != NULL){
//     printf("%s (%o)\n", root->fname, root->permissions);
//  }
//     return; 
// }
*/

int copy_ftree(const char *src, const char *dest) {
    // if source or destinatoin does not exist
    struct stat *sourcefile = malloc(sizeof(struct stat));
    struct stat *destinationfile = malloc(sizeof(struct stat));
    DIR *directory;
    DIR *src_directory;
    struct dirent *dir_contents;
    //printf("INFO: source : %s   dest : %s \n", src, dest);    

    char *src_name = strrchr(src, '/');
    if( src_name == NULL ){

        src_name = strdup(src);

    } else {

        src_name += sizeof(char);
    }
    //printf("src_name is %s \n", src_name);

    const char *dest_file = concat(concat(dest, "/"), src_name);
    //printf("dest_file is %s \n", dest_file);
    FILE *f1 = fopen(src, "rb");
    FILE *f2 = NULL;
    //printf("INFO: src_name: %s  dest_file: %s \n", src_name, dest_file);
    if (lstat(src, sourcefile) < 0 || lstat(dest, destinationfile) < 0){
        exit(-1);
    }
    // if destination is not a directory, give error
    if (!(S_ISDIR(destinationfile -> st_mode))){
        perror("destination is not a directory.");
        exit(-1);
    } //if source is a file
    if (S_ISREG(sourcefile -> st_mode)){
   // printf("INFO: %s is regular file \n", src);
        bool writefile = true;
        directory = opendir(dest);
	if( directory == NULL ){
	    //printf("failed to open directory.\n");
	    perror("failed to open a directory.\n");
	    exit(-1);

	}
        while((dir_contents = readdir(directory)) != NULL){
	    //printf("currently comparing %s and %s \n", src_name, dir_contents->d_name);
            if (strcmp(dir_contents -> d_name, src_name) == 0){ // if they have the same name           
        	//printf("the two files have the same name \n");
        	struct stat *file2 = malloc(sizeof(struct stat));
                lstat(concat(concat(dest, "/"), dir_contents -> d_name), file2); //uses actual file path
                f2 = fopen(concat(concat(dest, "/"), dir_contents ->d_name), "rb");
                if (f1 && f2){ //succesfully open the files
		    //printf("source file size: %d file 2 size: %d \n", sourcefile->st_size, file2->st_size);
                    if (((sourcefile->st_size == file2->st_size) && (strcmp(hash(f1),hash(f2))== 0))){ // if they have same size and hash
                        writefile = false;
			//printf("file already exists \n");
			
                    }
                } else {
                    perror("Error when opening files of the same name \n");
                    exit(-1);
                }
            fclose(f2);
           }
        }
        if (writefile){
        
	    //printf("writing file\n");
	    f2 = fopen(dest_file, "wb");
        
	    if( f2 == NULL ) {

                perror("destination file is null \n");
		exit(-1);
            } else {

	    //printf("rewinding src file\n");
                rewind(f1);
            //void *buffer = malloc(100);
            //printf("code reached point where it starts to copy file data \n");
                /*while((fread(buffer, 1, sizeof(buffer), f1) != 0)){
                    fwrite(buffer, 1, sizeof(buffer), f2); // rewrtie the entire fucken file
            
            */
	    //printf("about to write %s to dest\n", src);
                int curr;
                while((curr = fgetc(f1)) != EOF){
                    //printf("going to write data \n");
                    fwrite(&curr, 1, 1, f2);
                    //printf("wrote some to file \n");

                } 

	    fclose(f1);
	    fclose(f2);

            }
	
        //printf("file was successfully copied \n");
        }
        // change permission of rewrittened file
        
        chmod(dest_file, sourcefile -> st_mode); //uses file path
    //if source is directory
    } else if (S_ISDIR(sourcefile->st_mode)){
        bool makedir = true;
        directory = opendir(dest);
        while((dir_contents = readdir(directory)) != NULL){ // check if directory already exists
            if (dir_contents -> d_name[0] != '.'){
		    //printf("TEST: contents: %s src_name: %s \n", dir_contents->d_name, src_name);
                if ((strcmp(dir_contents -> d_name, src_name) == 0) && (dir_contents -> d_type == DT_DIR)){ // if they have the same name and they are both directories
                    makedir = false;
                    //printf("don't need to make folder %s because %s already exists\n", src_name, dir_contents -> d_name);
                } else if (strcmp(dir_contents -> d_name, src_name) == 0){
                //printf("BAD: %s is the same as %s \n", dir_contents -> d_name, src_name);
                    perror("there is a file in destination that has the same name as source directory but is not a directory");
                    exit(-1);
		}
            }
        }
        closedir(directory);
        if (makedir){
            //printf("making directory %s \n", dest_file);
            mkdir(dest_file, sourcefile ->st_mode);
        }
        src_directory = opendir(src);
        // open source directory and read contents
        directory = opendir(dest_file); //need actual file path
        int parent_forks = 0;
        while ((dir_contents = readdir(src_directory)) != NULL ){ //read all contents in source directory
        //printf("currently on %s \n", dir_contents->d_name);
            if (dir_contents -> d_name[0] != '.'){
                if (dir_contents -> d_type ==  DT_REG){
                    //printf("recursive call on %s in src, regular file \n", dir_contents ->d_name);
                    copy_ftree(concat(concat(src,"/"),dir_contents -> d_name), dest_file ); //need actual file path for both arguements
                } else if (dir_contents -> d_type == DT_DIR){ // if sub directory in source directory is also a directory0
                    //printf("recursive call on %s in src, directory \n", dir_contents -> d_name);
                    int result = fork();
                    if (result == -1){
                        perror ("Fork failed");
                        exit(-1);
                    } else if (result == 0){
                        int child_i = copy_ftree(concat(concat(src,"/"),dir_contents -> d_name), dest_file);

                        exit(child_i);
                    } else if (result > 0){
                        parent_forks ++;
                    }
                }
            }
        }

        int child_processes = 0;
	int errors = 0;
        for(int j = 0; j < parent_forks; j++){ //wait 3 times
            pid_t pid;
        int status;
        if ((pid = wait(&status) == -1)){ // if wait command fails
                perror("Wait failed");
		exit(-1);
            } else{
                if (WIFEXITED(status)){ //child terminated
                    child_processes += WEXITSTATUS(status);
		    if(WEXITSTATUS(status) < 0){

			errors++;

		    }
                    //printf("Child %d terminated with %d \n", pid, WEXITSTATUS(status));
                } else if(WIFSIGNALED(status)){ //child recieved signal to terminate
                    //printf("Child %d recieved signal %d to stop \n", pid, WTERMSIG(status));
                } else{
                    perror("undefined wait behaviour occurred");
		    exit(-1);
                }
            }
        }
	//printf("%s -- parent forks: %d sum of child exit codes: %d \n", src, parent_forks, child_processes);    
        if(errors > 0){
		return -(child_processes + 1);

	} else {
	    return child_processes + 1;
        }
    }

    return 1; //exit(0) makes entire program exit causing it to only be able to read one element in a directory
}
