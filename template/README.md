**Group:** 8

**Members:**
- Louis Diomande (dioma004)
- Tyler Yantes (yante009)
- Jonathan Olaleye (olale013)

**Tested On:csel-kh1250-25.cselabs.umn.edu**

**Contributions:**
- git repo creation, leaf_process (Louis)
- ouput setup, root_process (Tyler)
- makefile updates, nonleaf_process (Jonathan)

**Makefile Changes:**
- added output/ to clean script
- added a script to create output/ folders
- added scripts to diff between output/ and expected/

**Process Tree Pseudocode**
 nonleaf_process.c:

```
    tree_data = ""

    for entry in dir
        pipe = create_pipe()
        fork()
        if parent
            tree_data += read_from_pipe()
        else
            if entry.type == dir
                exec("nonleaf_process", entry.path, pipe.write)
            else if entry.type == file
                exec("leaf_process", entry.path, pipe.write)
    
    send_to_parent(tree_data)
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
