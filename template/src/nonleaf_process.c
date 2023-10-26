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
    // printf("nonleaf: %s\n", dir_path);

    //TODO(step2): malloc buffer for gathering all data transferred from child process as in root_process.c
    char tree_data[BUFSIZE * 5] = "";
    memset(tree_data, 0, BUFSIZE * 5);

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
        memset(entry_path, 0, BUFSIZE);
        sprintf(entry_path, "%s/%s", dir_path, entry->d_name);

        int fd[2];
        if(pipe(fd) == -1){
            printf("failed to create pipe\n");
            return 1;
        }

        pid_t pid = fork();
        if (pid != 0) { // parent
            close(fd[1]); // close write

            //TODO(step5): read from pipe constructed for child process and write to pipe constructed for parent process
            char child_str[BUFSIZE] = "";
            memset(child_str, 0, BUFSIZE);
            read(fd[0], child_str, BUFSIZE);
            // printf("%s\n", child_str);
            strcat(tree_data, child_str);

            close(fd[0]); // done reading
            waitpid(pid, NULL, 0);
            close(fd[1]); // close child's write end after exit
        } else { // child
            close(fd[0]); // close read

            char child_write_end[20] = "";
            memset(child_write_end, 0, 20);
            sprintf(child_write_end, "%d", fd[1]);
            
            if (entry->d_type == DT_DIR) {
                // directory: spawn new nonleaf
                // printf("dir: %s\n", entry_path);
                char *argv[] = {"./nonleaf_process", entry_path, child_write_end, NULL};
                if (execvp(*argv, argv) < 0) {
                    printf("failed to run nonleaf for %s\n", entry_path);
                    return 1;
                }
            } else if (entry->d_type == DT_REG) {
                // regular file: spawn leaf
                // printf("file: %s\n", entry_path);
                char *argv[] = {"./leaf_process", entry_path, child_write_end, NULL};
                if (execvp(*argv, argv) < 0) {
                    printf("failed to run leaf for %s\n", entry_path);
                    return 1;
                }
            }
        }
    }
    closedir(dir);

    // write complete subtree data to parent pipe
    // printf("%s\n", tree_data);
    write(pipe_write_end, tree_data, strlen(tree_data));
}
