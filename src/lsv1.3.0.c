#define _GNU_SOURCE      // add this as the first line

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // getopt
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define SPACING 2

// Enum to represent display mode
typedef enum {
    MODE_DEFAULT,
    MODE_LONG,
    MODE_HORIZONTAL
} display_mode_t;

// Function to get terminal width
int get_terminal_width() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
        return 80;  // fallback
    return w.ws_col;
}

// Print in down-then-across format
void print_vertical(char **files, int count, int max_len) {
    int width = get_terminal_width();
    int col_width = max_len + SPACING;
    int cols = width / col_width;
    if (cols == 0) cols = 1;
    int rows = (count + cols - 1) / cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int idx = c * rows + r;
            if (idx < count) {
                printf("%-*s", col_width, files[idx]);
            }
        }
        printf("\n");
    }
}

// Print in horizontal format (-x)
void print_horizontal(char **files, int count, int max_len) {
    int width = get_terminal_width();
    int col_width = max_len + SPACING;
    int pos = 0;

    for (int i = 0; i < count; i++) {
        if (pos + col_width > width) {
            printf("\n");
            pos = 0;
        }
        printf("%-*s", col_width, files[i]);
        pos += col_width;
    }
    printf("\n");
}

// Print long listing (-l)
void print_long(char **files, int count) {
    struct stat st;
    struct passwd *pw;
    struct group *gr;
    char timebuf[80];

    for (int i = 0; i < count; i++) {
        if (stat(files[i], &st) == -1) continue;

        // file type and permissions
        printf( (S_ISDIR(st.st_mode)) ? "d" : "-");
        printf( (st.st_mode & S_IRUSR) ? "r" : "-");
        printf( (st.st_mode & S_IWUSR) ? "w" : "-");
        printf( (st.st_mode & S_IXUSR) ? "x" : "-");
        printf( (st.st_mode & S_IRGRP) ? "r" : "-");
        printf( (st.st_mode & S_IWGRP) ? "w" : "-");
        printf( (st.st_mode & S_IXGRP) ? "x" : "-");
        printf( (st.st_mode & S_IROTH) ? "r" : "-");
        printf( (st.st_mode & S_IWOTH) ? "w" : "-");
        printf( (st.st_mode & S_IXOTH) ? "x" : "-");

        printf(" %2ld", st.st_nlink);

        pw = getpwuid(st.st_uid);
        gr = getgrgid(st.st_gid);
        printf(" %s %s", pw->pw_name, gr->gr_name);
        printf(" %5ld", st.st_size);

        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&st.st_mtime));
        printf(" %s %s\n", timebuf, files[i]);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    display_mode_t mode = MODE_DEFAULT;

    // parse options
    while ((opt = getopt(argc, argv, "lx")) != -1) {
        switch (opt) {
            case 'l':
                mode = MODE_LONG;
                break;
            case 'x':
                mode = MODE_HORIZONTAL;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-x]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // open current directory
    DIR *dir = opendir(".");
    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    int capacity = 100;
    int count = 0;
    int max_len = 0;

    char **files = malloc(sizeof(char*) * capacity);
    if (!files) exit(EXIT_FAILURE);

    // read files
    while ((entry = readdir(dir)) != NULL) {
        if (count >= capacity) {
            capacity *= 2;
            files = realloc(files, sizeof(char*) * capacity);
        }
        files[count] = strdup(entry->d_name);
        int len = strlen(entry->d_name);
        if (len > max_len) max_len = len;
        count++;
    }
    closedir(dir);

    // choose display
    if (mode == MODE_LONG) {
        print_long(files, count);
    } else if (mode == MODE_HORIZONTAL) {
        print_horizontal(files, count, max_len);
    } else {
        print_vertical(files, count, max_len);
    }

    // free memory
    for (int i = 0; i < count; i++) free(files[i]);
    free(files);

    return 0;
}
