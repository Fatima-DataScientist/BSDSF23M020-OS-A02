#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define COLOR_RESET   "\033[0m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_RED     "\033[0;31m"
#define COLOR_PINK    "\033[0;35m"
#define COLOR_REVERSE "\033[7m"

enum display_mode { DEFAULT, LONG, HORIZONTAL };

void print_colored(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) == -1) {
        perror("lstat");
        return;
    }

    if (S_ISDIR(st.st_mode)) 
        printf(COLOR_BLUE "%s" COLOR_RESET "  ", filename);
    else if (st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH)
        printf(COLOR_GREEN "%s" COLOR_RESET "  ", filename);
    else if (strstr(filename, ".tar") || strstr(filename, ".gz") || strstr(filename, ".zip"))
        printf(COLOR_RED "%s" COLOR_RESET "  ", filename);
    else if (S_ISLNK(st.st_mode))
        printf(COLOR_PINK "%s" COLOR_RESET "  ", filename);
    else if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode) || S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode))
        printf(COLOR_REVERSE "%s" COLOR_RESET "  ", filename);
    else
        printf("%s  ", filename);
}

int cmpfunc(const void *a, const void *b) {
    char * const *sa = (char * const *)a;
    char * const *sb = (char * const *)b;
    return strcmp(*sa, *sb);
}

// Horizontal display
void display_horizontal(char **files, int count) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col;
    int max_len = 0;
    for (int i = 0; i < count; i++)
        if ((int)strlen(files[i]) > max_len) max_len = strlen(files[i]);
    max_len += 2; // spacing
    int pos = 0;
    for (int i = 0; i < count; i++) {
        printf("%-*s", max_len, files[i]);
        pos += max_len;
        if (pos + max_len > width) { printf("\n"); pos = 0; }
    }
    if (pos != 0) printf("\n");
}

// Default vertical display
void display_vertical(char **files, int count) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col;
    int max_len = 0;
    for (int i = 0; i < count; i++)
        if ((int)strlen(files[i]) > max_len) max_len = strlen(files[i]);
    max_len += 2;

    int num_cols = width / max_len;
    if (num_cols < 1) num_cols = 1;
    int num_rows = (count + num_cols - 1) / num_cols;

    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            int idx = c * num_rows + r;
            if (idx < count)
                printf("%-*s", max_len, files[idx]);
        }
        printf("\n");
    }
}

// Long listing (simplified)
void display_long(char **files, int count) {
    for (int i = 0; i < count; i++)
        print_colored(files[i]), printf("\n");
}

int main(int argc, char *argv[]) {
    enum display_mode mode = DEFAULT;
    int opt;
    while ((opt = getopt(argc, argv, "lx")) != -1) {
        switch(opt) {
            case 'l': mode = LONG; break;
            case 'x': mode = HORIZONTAL; break;
            default: break;
        }
    }

    DIR *d = opendir(".");
    if (!d) { perror("opendir"); return 1; }

    struct dirent *entry;
    int capacity = 128, count = 0;
    char **files = malloc(sizeof(char*) * capacity);
    if (!files) { perror("malloc"); return 1; }

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue; // skip hidden
        if (count >= capacity) {
            capacity *= 2;
            files = realloc(files, sizeof(char*) * capacity);
            if (!files) { perror("realloc"); return 1; }
        }
        files[count] = strdup(entry->d_name);
        if (!files[count]) { perror("strdup"); return 1; }
        count++;
    }
    closedir(d);

    qsort(files, count, sizeof(char*), cmpfunc);

    switch(mode) {
        case LONG: display_long(files, count); break;
        case HORIZONTAL: display_horizontal(files, count); break;
        default: display_vertical(files, count); break;
    }

    for (int i = 0; i < count; i++) free(files[i]);
    free(files);

    return 0;
}
