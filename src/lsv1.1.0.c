#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *entry;
    char **files = NULL;
    int count = 0;
    int max_length = 0;

    // Open current directory
    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    // First pass: count files and store names
    while ((entry = readdir(dir)) != NULL) {
        files = realloc(files, sizeof(char*) * (count + 1));
        if (!files) {
            perror("realloc");
            closedir(dir);
            return 1;
        }
        files[count] = strdup(entry->d_name); // duplicate string
        if (!files[count]) {
            perror("strdup");
            closedir(dir);
            return 1;
        }
        int len = strlen(entry->d_name);
        if (len > max_length)
            max_length = len;
        count++;
    }
    closedir(dir);

    // Get terminal width
    struct winsize ws;
    int term_width = 80; // default
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
        term_width = ws.ws_col;

    // Calculate columns and rows
    int spacing = 2;
    int cols = term_width / (max_length + spacing);
    if (cols == 0) cols = 1;
    int rows = (count + cols - 1) / cols; // ceiling division

    // Print files in columns
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int index = c * rows + r;
            if (index < count) {
                printf("%-*s", max_length + spacing, files[index]);
            }
        }
        printf("\n");
    }

    // Free memory
    for (int i = 0; i < count; i++)
        free(files[i]);
    free(files);

    return 0;
}

