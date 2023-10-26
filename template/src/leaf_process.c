#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/hash.h"
#include "../include/utils.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define WRITE (O_RDWR| O_CREAT | O_TRUNC)
#define PERM (S_IRUSR | S_IWUSR)
#define BUFSIZE 2056

char *output_file_folder = "output/inter_submission/";

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: Inter Submission --> ./leaf_process <file_path> 0\n");
        printf("Usage: Final Submission --> ./leaf_process <file_path> <pipe_write_end>\n");
        return -1;
    }

    //TODO(): get <file_path> <pipe_write_end> from argv[]
    char *file_path=argv[1];
    int pipe_write_end=atoi(argv[2]);
    // printf("leaf: %s\n", file_path);

    //TODO(): create the hash of given file
    char* hash_value=(char*)malloc(sizeof(char)*BUFSIZE);
    hash_data_block(hash_value,file_path);

    //TODO(): construct string write to pipe. The format is "<file_path>|<hash_value>"
    char WriteToPipe[BUFSIZE] = "";
    sprintf(WriteToPipe, "%s|%s|", file_path, hash_value);

    if(pipe_write_end==0){
        //TODO(inter submission)
        //TODO(overview): create a file in output_file_folder("output/inter_submission/root*") and write the constructed string to the file

        //TODO(step1): extract the file_name from file_path using extract_filename() in utils.c
        char* file_name = extract_filename(file_path);

        //TODO(step2): extract the root directory(e.g. root1 or root2 or root3) from file_path using extract_root_directory() in utils.c
        char* root_dir = extract_root_directory(file_path);

        //TODO(step3): get the location of the new file (e.g. "output/inter_submission/root1" or "output/inter_submission/root2" or "output/inter_submission/root3")
        char file_loc[BUFSIZE] = "";
        sprintf(file_loc, "%s%s/%s", output_file_folder, root_dir, file_name);

        //TODO(step4): create and write to file, and then close file
        FILE *fd = fopen(file_loc,"w+");
        if(fd==NULL){
            fprintf(stderr, "failed to open file %s\n", file_loc);
            exit(-1);
        }
        if (fputs(WriteToPipe, fd) == EOF) {
            fprintf(stderr, "failed to write to file %s\n", file_loc);
            exit(-1);
        }
        fclose(fd);

        //TODO(step5): free any arrays that are allocated using malloc!! Free the string returned from extract_root_directory()!! It is allocated using malloc in extract_root_directory()
        free(root_dir);

    }else{
        //TODO(final submission): write the string to pipe
        write(pipe_write_end, WriteToPipe, strlen(WriteToPipe));
        exit(0);
    }

    free(hash_value);
    hash_value=NULL;

    exit(0);
}
