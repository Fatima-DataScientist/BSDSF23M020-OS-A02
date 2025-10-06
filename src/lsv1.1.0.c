/*
* Programming Assignment 02: lsv1.1.0
* Description:
* This version adds support for the -l option (long listing format)
* Usage Examples:
*   $ ./bin/lsv1.1.0
*   $ ./bin/lsv1.1.0 -l
*   $ ./bin/lsv1.1.0 -l /home /etc
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

extern int errno;

void do_ls(const char *dir);
void do_ls_long(const char *dir);

int main(int argc, char *argv[])
{
    int opt;                // for getopt()
    int long_listing = 0;   // flag for -l

    // Parse command line options
    while ((opt = getopt(argc, argv, "l")) != -1)
    {
        switch (opt)
        {
        case 'l':
            long_listing = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-l] [directory...]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // If no directory given, use current directory
    if (optind == argc)
    {
        if (long_listing)
            do_ls_long(".");
        else
            do_ls(".");
    }
    else
    {
        for (int i = optind; i < argc; i++)
        {
            printf("Directory listing of %s:\n", argv[i]);
            if (long_listing)
                do_ls_long(argv[i]);
            else
                do_ls(argv[i]);
            puts("");
        }
    }

    return 0;
}

/* Original short listing version */
void do_ls(const char *dir)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    errno = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        printf("%s\n", entry->d_name);
    }

    if (errno != 0)
        perror("readdir failed");

    closedir(dp);
}

/* Long listing (-l) version */
void do_ls_long(const char *dir)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        perror("opendir");
        return;
    }

    char path[1024];
    struct stat sb;

    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue; // skip hidden files

        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
        if (lstat(path, &sb) == -1)
        {
            perror("lstat");
            continue;
        }

        // File type and permissions
        printf((S_ISDIR(sb.st_mode)) ? "d" : "-");
        printf((sb.st_mode & S_IRUSR) ? "r" : "-");
        printf((sb.st_mode & S_IWUSR) ? "w" : "-");
        printf((sb.st_mode & S_IXUSR) ? "x" : "-");
        printf((sb.st_mode & S_IRGRP) ? "r" : "-");
        printf((sb.st_mode & S_IWGRP) ? "w" : "-");
        printf((sb.st_mode & S_IXGRP) ? "x" : "-");
        printf((sb.st_mode & S_IROTH) ? "r" : "-");
        printf((sb.st_mode & S_IWOTH) ? "w" : "-");
        printf((sb.st_mode & S_IXOTH) ? "x" : "-");

        // Number of hard links
        printf(" %2ld", sb.st_nlink);

        // Owner and group names
        struct passwd *pw = getpwuid(sb.st_uid);
        struct group *gr = getgrgid(sb.st_gid);
        printf(" %-8s %-8s", pw->pw_name, gr->gr_name);

        // File size
        printf(" %8ld", sb.st_size);

        // Modification time
        char timebuf[64];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&sb.st_mtime));
        printf(" %s", timebuf);

        // File name
        printf(" %s\n", entry->d_name);
    }

    closedir(dp);
}

