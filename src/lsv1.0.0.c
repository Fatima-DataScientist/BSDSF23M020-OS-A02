#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Function to read all filenames into dynamic array and track longest
char **read_directory(const char *dirname, int *file_count, int *max_len) {
    DIR *dp = opendir(dirname);
    if (!dp) {
        perror("opendir");
        return NULL;
    }

    struct dirent *entry;
    int size = 10; // initial array size
    char **files = malloc(sizeof(char*) * size);
    *file_count = 0;
    *max_len = 0;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue; // skip hidden files

        if (*file_count >= size) {
            size *= 2;
            files = realloc(files, sizeof(char*) * size);
        }

        files[*file_count] = strdup(entry->d_name);
        int len = strlen(entry->d_name);
        if (len > *max_len) *max_len = len;
        (*file_count)++;
    }

    closedir(dp);
    return files;
}

// Function to print files in "down then across" columns
void print_columns(char **files, int count, int max_len) {
    if (count == 0) return;

    // Get terminal width
    struct winsize w;
    int term_width = 80; // fallback
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) term_width = w.ws_col;

    int col_width = max_len + 2;          // 2 spaces padding
    int num_cols = term_width / col_width;
    if (num_cols == 0) num_cols = 1;
    int num_rows = (count + num_cols - 1) / num_cols; // ceiling division

    // Print "down then across"
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            int index = col * num_rows + row;
            if (index < count) {
                printf("%-*s", col_width, files[index]);
            }
        }
        printf("\n");
    }
}

// Core do_ls function
void do_ls(const char *dirname) {
    int count, max_len;
    char **files = read_directory(dirname, &count, &max_len);
    if (!files) return;

    // Sort alphabetically (qsort)
    qsort(files, count, sizeof(char*), (int (*)(const void*, const void*))strcmp);

    printf("%s:\n", dirname);
    print_columns(files, count, max_len);

    // Free memory
    for (int i = 0; i < count; i++) free(files[i]);
    free(files);
}

// Main
int main(int argc, char *argv[]) {
    const char *dir = ".";
    if (argc > 1) dir = argv[1];

    do_ls(dir);
    return 0;
}

