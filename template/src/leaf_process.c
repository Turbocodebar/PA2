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

static void makedir(const char *dir) {
        char temp_var[256];
        char *ptr = NULL;
        size_t ln;

        snprintf(temp_var, sizeof(temp_var),"%s",dir);
        ln = strlen(temp_var);
        if (temp_var[ln - 1] == '/')
            temp_var[ln - 1] = 0;
        for (ptr = temp_var + 1; *ptr; ptr++)
            if (*ptr == '/') {
                *ptr = 0;
                mkdir(temp_var, WRITE|PERM);
                *ptr = '/';
            }
        if(mkdir(temp_var, WRITE|PERM)!=0)
        {
            //fprintf(stderr, "Unable to open dir %s\n", temp_var);
        }
    }

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: Inter Submission --> ./leaf_process <file_path> 0\n");
        printf("Usage: Final Submission --> ./leaf_process <file_path> <pipe_write_end>\n");
        return -1;
    }
    //TODO(): get <file_path> <pipe_write_end> from argv[]
    char *file_path=argv[1];
    int pipe_write_end=atoi(argv[2]);

    //TODO(): create the hash of given file

    char* hash_value=(char*)malloc(sizeof(char)*BUFSIZE*7);
    
    hash_data_block(hash_value,file_path);
    printf("\n");
    //TODO(): construct string write to pipe. The format is "<file_path>|<hash_value>"
    char WriteToPipe[2056];
    
    printf("%s\n",WriteToPipe);
    strcat(WriteToPipe,file_path);
    strcat(WriteToPipe,"|");
    strcat(WriteToPipe,hash_value);
    strcat(WriteToPipe,"|");

    
    if(pipe_write_end==0){


        //TODO(inter submission)
        //TODO(overview): create a file in output_file_folder("output/inter_submission/root*") and write the constructed string to the file
        //TODO(step1): extract the file_name from file_path using extract_filename() in utils.c
        char file_name[1000]="";
        strcat(file_name,extract_filename(file_path));
        //TODO(step2): extract the root directory(e.g. root1 or root2 or root3) from file_path using extract_root_directory() in utils.c
        char root_dir[1000]="";
        strcat(root_dir, extract_root_directory(file_path));


        //TODO(step3): get the location of the new file (e.g. "output/inter_submission/root1" or "output/inter_submission/root2" or "output/inter_submission/root3")
        char file_loc[1000]="";
        strcat(file_loc,output_file_folder);
        strcat(file_loc,root_dir);
        makedir(file_loc);
        
        strcat(file_loc,file_name);

        //TODO(step4): create and write to file, and then close file
        

        //int fd =open(file_loc,WRITE,PERM);
        FILE *fd =fopen(file_loc,"w+");
        if(fd==NULL){
            fprintf(stderr, "Unable to open file %s\n", file_loc);
            exit(-1);
        }
        int results = fputs(WriteToPipe, fd);
        if (results == EOF) {
            // Failed to write do error code here.
        }
        fclose(fd);
        //TODO(step5): free any arrays that are allocated using malloc!! Free the string returned from extract_root_directory()!! It is allocated using malloc in extract_root_directory()

    }else{
        //TODO(final submission): write the string to pipe

        exit(0);

    }

    free(hash_value);
    //free(WriteToPipe);
    hash_value=NULL;
    //WriteToPipe=NULL;

    exit(0);
}
