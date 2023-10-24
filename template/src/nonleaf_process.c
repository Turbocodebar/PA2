#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

#define BUFSIZE 2056

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./nonleaf_process <directory_path> <pipe_write_end> \n");
        printf("%d\n", argc);
        return 1;
    }
    //TODO(overview): fork the child processes(non-leaf process or leaf process) each associated with items under <directory_path>

    //TODO(step1): get <file_path> <pipe_write_end> from argv[]
    char* dir_path = argv[1];
    int pipe_write_end = atoi(argv[2]);
    printf("nonleaf: %s\n", dir_path);

    //TODO(step2): malloc buffer for gathering all data transferred from child process as in root_process.c


    //TODO(step3): open directory
    DIR* dir = opendir(dir_path);
    if (dir == NULL) {
        printf("failed to open directory %s\n", dir_path);
        return 1;
    }

    //TODO(step4): traverse directory and fork child process
    // Hints: Maintain an array to keep track of each read end pipe of child process
    struct dirent* entry;
    while((entry = readdir(dir)) != NULL){
        // skip . and ..
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }

        char entry_path[BUFSIZE] = "";
        sprintf(entry_path, "%s/%s", dir_path, entry->d_name);

        int fd[2];
        if(pipe(fd) == -1){
            printf("failed to create pipe\n");
            return 1;
        }

        pid_t pid = fork();
        if (pid != 0) { // parent
            // close write end, only read from child
            // close(fd[1]);
            // store read end in array of pipes to children
            // ?
        } else { // child
            // close read end, only write to parent
            // close(fd[0]);
            char proc_name[BUFSIZE] = "";
            if (entry->d_type == DT_DIR) {
                // directory: spawn new nonleaf
                // printf("dir: %s\n", entry_path);
                sprintf(proc_name, "./nonleaf_process");
            } else if (entry->d_type == DT_REG) {
                // regular file: spawn leaf
                // printf("file: %s\n", entry_path);
                sprintf(proc_name, "./leaf_process");
            }
            char *argv[] = {proc_name, entry_path, "0", NULL};
            if (execvp(*argv, argv) < 0) {
                printf("failed to run %s for %s\n", proc_name, entry_path);
                return 1;
            }
        }
    }
    closedir(dir);

    //TODO(step5): read from pipe constructed for child process and write to pipe constructed for parent process
}
