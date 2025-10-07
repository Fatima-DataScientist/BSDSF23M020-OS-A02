#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_FILES 1024
#define SPACING 2

typedef enum { MODE_DEFAULT, MODE_LONG, MODE_HORIZONTAL } display_mode_t;

// Comparison function for qsort
int cmpstring(const void *a, const void *b) {
    const char * const *sa = (const char * const *)a;
    const char * const *sb = (const char * const *)b;
    return strcmp(*sa, *sb);
}

// Get terminal width
int get_terminal_width() {
    struct winsize ws;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        return 80;
    return ws.ws_col;
}

// Print vertically: "down then across"
void print_vertical(char **files, int count) {
    int max_len = 0;
    for(int i = 0; i < count; i++)
        if((int)strlen(files[i]) > max_len) max_len = strlen(files[i]);

    int term_width = get_terminal_width();
    int col_width = max_len + SPACING;
    int cols = term_width / col_width;
    if(cols < 1) cols = 1;
    int rows = (count + cols - 1) / cols;

    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            int idx = c*rows + r;
            if(idx < count) {
                printf("%-*s", col_width, files[idx]);
            }
        }
        printf("\n");
    }
}

// Print horizontally: "row-major" (for -x)
void print_horizontal(char **files, int count) {
    int max_len = 0;
    for(int i = 0; i < count; i++)
        if((int)strlen(files[i]) > max_len) max_len = strlen(files[i]);

    int term_width = get_terminal_width();
    int col_width = max_len + SPACING;
    int pos = 0;

    for(int i = 0; i < count; i++) {
        int len = (int)strlen(files[i]);
        if(pos + col_width > term_width) {
            printf("\n");
            pos = 0;
        }
        printf("%-*s", col_width, files[i]);
        pos += col_width;
    }
    printf("\n");
}

// Long listing
void print_long(char **files, int count) {
    struct stat st;
    for(int i = 0; i < count; i++) {
        if(stat(files[i], &st) == -1) {
            perror(files[i]);
            continue;
        }
        printf("%10lld %s\n", (long long)st.st_size, files[i]);
    }
}

// Main LS function
int main(int argc, char *argv[]) {
    display_mode_t mode = MODE_DEFAULT;
    int opt;

    while((opt = getopt(argc, argv, "lx")) != -1) {
        switch(opt) {
            case 'l': mode = MODE_LONG; break;
            case 'x': mode = MODE_HORIZONTAL; break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-x]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    DIR *dir = opendir(".");
    if(!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    char **files = malloc(sizeof(char*) * MAX_FILES);
    if(!files) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    int count = 0;
    while((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        if(entry->d_name[0] == '.') continue;  // skip hidden
        files[count] = strdup(entry->d_name);
        if(!files[count]) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        count++;
    }
    closedir(dir);

    qsort(files, count, sizeof(char*), cmpstring);

    // Call the correct display function
    switch(mode) {
        case MODE_LONG: print_long(files, count); break;
        case MODE_HORIZONTAL: print_horizontal(files, count); break;
        default: print_vertical(files, count); break;
    }

    // Free memory
    for(int i = 0; i < count; i++)
        free(files[i]);
    free(files);

    return 0;
}
