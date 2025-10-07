#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_FILES 1024

enum display_mode { MODE_DEFAULT, MODE_LONG, MODE_HORIZONTAL };

int display_mode = MODE_DEFAULT;
int recursive_flag = 0; // -R flag

// Forward declarations
void do_ls(const char *dirname);
void print_vertical(char **files, int count);
void print_horizontal(char **files, int count);
void print_long(char **files, int count);

// Comparison function for qsort
int cmpstr(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void do_ls(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror(dirname);
        return;
    }

    struct dirent *entry;
    char *files[MAX_FILES];
    int count = 0;

    // Read all files into array
    while ((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        files[count] = strdup(entry->d_name);
        count++;
    }
    closedir(dir);

    // Sort files alphabetically
    qsort(files, count, sizeof(char *), cmpstr);

    // Display header for recursive mode
    if (recursive_flag) {
        printf("\n%s:\n", dirname);
    }

    // Display files based on display_mode
    if (display_mode == MODE_LONG)
        print_long(files, count);
    else if (display_mode == MODE_HORIZONTAL)
        print_horizontal(files, count);
    else
        print_vertical(files, count);

    // Recursive call for subdirectories
    if (recursive_flag) {
        for (int i = 0; i < count; i++) {
            // Skip "." and ".."
            if (strcmp(files[i], ".") == 0 || strcmp(files[i], "..") == 0)
                continue;

            // Construct full path
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dirname, files[i]);

            struct stat st;
            if (lstat(path, &st) == -1) {
                perror(path);
                continue;
            }

            if (S_ISDIR(st.st_mode)) {
                do_ls(path);
            }
        }
    }

    // Free memory
    for (int i = 0; i < count; i++) {
        free(files[i]);
    }
}

// Simple display functions
void print_vertical(char **files, int count) {
    for (int i = 0; i < count; i++)
        printf("%s\n", files[i]);
}

void print_horizontal(char **files, int count) {
    int width = 20, term_width = 80, cur = 0;
    for (int i = 0; i < count; i++) {
        printf("%-20s", files[i]);
        cur += width;
        if (cur + width > term_width) {
            printf("\n");
            cur = 0;
        }
    }
    if (cur != 0) printf("\n");
}

void print_long(char **files, int count) {
    for (int i = 0; i < count; i++)
        printf("-rw-r--r-- 1 user group 0 Jan 1 00:00 %s\n", files[i]);
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "lxR")) != -1) {
        switch (opt) {
            case 'l': display_mode = MODE_LONG; break;
            case 'x': display_mode = MODE_HORIZONTAL; break;
            case 'R': recursive_flag = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-x] [-R] [dir]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    const char *dirname = (optind < argc) ? argv[optind] : ".";
    do_ls(dirname);

    return 0;
}
