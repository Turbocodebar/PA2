#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/utils.h"

#define WRITE (O_WRONLY | O_CREAT | O_TRUNC)
#define PERM (S_IRUSR | S_IWUSR)
char *output_file_folder = "output/final_submission/";

void redirection(char **dup_list, int size, char* root_dir){
    // TODO(overview): redirect standard output to an output file in output_file_folder("output/final_submission/")
    // TODO(step1): determine the filename based on root_dir. e.g. if root_dir is "./root_directories/root1", the output file's name should be "root1.txt"
    
    char filename[5000]; //not sure what to set this too
    snprintf(filename, sizeof(filename), "%s%s.txt", output_file_folder, extract_filename(root_dir));

    //TODO(step2): redirect standard output to output file (output/final_submission/root*.txt)
    
    int TEMP_STDOUT = dup(STDOUT_FILENO);
    int output_file = open(filename, WRITE, PERM);
    if(output_file == -1){
        perror("Failed to open output file");
        exit(-1);
    }
    dup2(output_file, STDOUT_FILENO);
    
    //TODO(step3): read the content each symbolic link in dup_list, write the path as well as the content of symbolic link to output file(as shown in expected)
    size_t link_size;
    char link_buffer[5000]; //not sure what to set this to either
    memset(link_buffer, 0, 5000);
    
    for(int i = 0; i < size; i++){
        link_size = readlink(dup_list[i], link_buffer, sizeof(link_buffer));
        if(link_size != -1){
            link_buffer[link_size] = '\0';      //Removes Null values from the string so printf can be used
            printf("%s --> %s\n", dup_list[i], link_buffer);
        }
    }

    //Close output_file and TEMP_STDOUT along with copy the original STDOUT back to STDOUT.
    fflush(stdout);
    dup2(TEMP_STDOUT, STDOUT_FILENO);
    close(TEMP_STDOUT);
    close(output_file);
}

void create_symlinks(char **dup_list, char **retain_list, int size) {
    //TODO(): create symbolic link at the location of deleted duplicate file
    //TODO(): dup_list[i] will be the symbolic link for retain_list[i]
    for(int i = 0; i < size; i++){
        symlink(retain_list[i], dup_list[i]);
    }

}

void delete_duplicate_files(char **dup_list, int size) {
    //TODO(): delete duplicate files, each element in dup_list is the path of the duplicate file
    for(int i = 0; i < size; i++){
        remove(dup_list[i]);
    }
}

// ./root_directories <directory>
int main(int argc, char* argv[]) {
    if (argc != 2) {
        // dir is the root_directories directory to start with
        // e.g. ./root_directories/root1
        printf("Usage: ./root <dir> \n");
        return 1;
    }

    //TODO(overview): fork the first non_leaf process associated with root directory("./root_directories/root*")

    char* root_directory = argv[1];
    char all_filepath_hashvalue[4098]; //buffer for gathering all data transferred from child process
    memset(all_filepath_hashvalue, 0, 4098);// clean the buffer. piazza post mentioned removing size of for the last element

    //TODO(step1): construct pipe

    int fd[2];
    if(pipe(fd) == -1){
        printf("failed to create pipe\n");
        return 1;
    }

    //TODO(step2): fork() child process & read data from pipe to all_filepath_hashvalue
    pid_t pid = fork();
    char fd_write[20];
    memset(fd_write, 0, 20);
    if(pid == 0){       //child process function, run nonleaf process using execl
        sprintf(fd_write, "%d", fd[1]);
        execl("./nonleaf_process", "./nonleaf_process", root_directory, fd_write, (char *) NULL);
        perror("Root Child Process Failure");
    }
    
    waitpid(pid, NULL, 0);
    read(fd[0], &all_filepath_hashvalue, sizeof(all_filepath_hashvalue));


    //TODO(step3): malloc dup_list and retain list & use parse_hash() in utils.c to parse all_filepath_hashvalue
    // dup_list: list of paths of duplicate files. We need to delete the files and create symbolic links at the location
    // retain_list: list of paths of unique files. We will create symbolic links for those files
    char **dup_list = (char **)malloc(50 * sizeof(char *));
    char **retain_list = (char **)malloc(50 * sizeof(char *));
    for(int i = 0; i < 10; i++){
        dup_list[i] = (char *)malloc(50 * sizeof(char));
        retain_list[i] = (char *)malloc(50 * sizeof(char));
    }

    //10 is the max number of files from assumptions

    int size = parse_hash(all_filepath_hashvalue, dup_list, retain_list);

    //TODO(step4): implement the functions
    delete_duplicate_files(dup_list,size);
    create_symlinks(dup_list, retain_list, size);
    redirection(dup_list, size, argv[1]);

    //TODO(step5): free any arrays that are allocated using malloc!!
    // for(int i = 0; i < size; i++){       done in template code
    //     free(dup_list[i]);
    //     free(retain_list[i]);
    // }
    free(dup_list);
    free(retain_list);

    return 0;
}
