**Group:** 8

**Members:**
- Louis Diomande (dioma004)
- Tyler Yantes (yante009)
- Jonathan Olaleye (olale013)

**Tested On:csel-kh1250-25.cselabs.umn.edu**

**Contributions:**
- setting up the repo (Louis)
- creating output folder (Tyler)
- write file name and hash to files(Jonathan)

**Makefile Changes**
- added output/ to clean script
- added scripts to diff between output/ and expected/

**Process Tree Pseudocode**
 nonleaf_process.c:

```
    fork_child_process()

    if is_child_process():
        close_unused_pipe_end(read_pipe_fd)
        set_up_communication(write_pipe_fd, read_pipe_fd)
        exec("nonleaf_processs.c", path, read_pipe_fd)

```

root_process.c:

```
function redirection(dup_list, size, root_dir):
    output_filename = extract_filename(root_dir)

    open output_file as output_file_fd
    redirect stdout to output_file_fd

    for i in range(size):
        content = read_symbolic_link_content(dup_list[i])
        write content to output_filename

function create_symlinks(dup_list, retain_list, size):
    for i in range(size):
        create_symbolic_link(dup_list[i], retain_list[i])

function delete_duplicate_files(dup_list, size):
    for i in range(size):
        delete_file(dup_list[i])

function main(argv):
    if length of argv is not 2:
        print "Usage: ./root <dir>"
        return

    root_directory = argv[1]
    all_filepath_hashvalue = read_data_from_child_process(root_directory)

    dup_list, retain_list = parse_hash(all_filepath_hashvalue)

    delete_duplicate_files(dup_list, size)
    create_symlinks(dup_list, retain_list, size)
    redirection(dup_list, size, root_directory)

```
