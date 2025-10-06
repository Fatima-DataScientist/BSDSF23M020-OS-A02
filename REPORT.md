REPORT for Feature-2: Version 1.1.0 - Complete Long Listing Format

Q1. What is the crucial difference between the stat() and lstat() system calls? 
In the context of the ls command, when is it more appropriate to use lstat()?

Answer:
The stat() and lstat() system calls are both used to retrieve metadata (information) about files,
such as size, permissions, ownership, and timestamps. The main difference is how they handle symbolic links:

- stat() follows symbolic links — it gives information about the target file that the link points to.
- lstat() does not follow symbolic links — it gives information about the link itself, not the target.

In the context of the ls command, lstat() is more appropriate because ls -l needs to correctly display
information for symbolic links (like showing lrwxrwxrwx and the link name), instead of following the
link to another file. Using lstat() ensures the program lists the properties of the symbolic link itself.


Q2. The st_mode field in struct stat is an integer that contains both the file type (e.g., regular file, directory)
and the permission bits. Explain how you can use bitwise operators (like &) and predefined macros (like S_IFDIR or S_IRUSR)
to extract this information.

Answer:
The st_mode field is a set of bit flags where each bit (or group of bits) represents specific information about
the file — such as its type and access permissions. You can use bitwise AND (&) with predefined macros to test
which bits are set.

For example:
    if (S_ISDIR(sb.st_mode))
        printf("This is a directory.\n");

Here, the macro S_ISDIR() internally checks whether the bits in st_mode match the directory type (S_IFDIR).

Similarly, for permissions:
    if (sb.st_mode & S_IRUSR)
        printf("Owner has read permission.\n");
    if (sb.st_mode & S_IWGRP)
        printf("Group has write permission.\n");
    if (sb.st_mode & S_IXOTH)
        printf("Others have execute permission.\n");

Each macro (like S_IRUSR, S_IWGRP, S_IXOTH) represents a bit mask for a specific permission.
Using bitwise operators with these macros allows us to extract and interpret the file’s type and permission
bits from the integer st_mode value.
