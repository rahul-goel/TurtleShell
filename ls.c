#include "header.c"

int is_hidden(char *path) {
    return path[0] == '.';
}

// Better do declare own due to GNU/POSIX clash
char *gnu_basename(char *path) {
    char *base = strrchr(path, '/');
    return base ? base+1 : path;
}

// input directory must be absolut path of the directory
// filename must be only the name of the file in that directory
void ls_normal_single(char *directory, char *filename) {
    char *abspath = (char *) malloc(sizeof(char) * (strlen(directory) + strlen(filename) + 5));

    strcpy(abspath, directory);
    strcat(abspath, "/");
    strcat(abspath, filename);

    struct stat file_stat;
    if (stat(abspath, &file_stat) == -1) {
        perror("Error");
        return;
    }

    if (S_ISDIR(file_stat.st_mode)) {
        printf(BBLU);
        printf("%s  ", filename);
        printf(WHT);
    }
    else if (file_stat.st_mode & S_IXUSR) {
        printf(BGRN);
        printf("%s  ", filename);
        printf(WHT);
    } else {
        printf("%s  ", filename);
    }
}

// input directory must be absolute path of the directory
void ls_normal_dir(char *directory, int hiddenflag) {
    struct dirent **list;
    // scandir places the list of (struct dirent) in the list.
    // and returns the size of the list
    // scandir may throw warning but linker takes care.
    int n = scandir(directory, &list, NULL, alphasort);
    if (n < 0) {
        perror("Error");
        return;
    } else {
        for (int i = 0; i < n ;i++) {
            if (hiddenflag) {
                if (!is_hidden(list[i]->d_name)) {
                    ls_normal_single(directory, list[i]->d_name);
                }
            } else {
                ls_normal_single(directory, list[i]->d_name);
            }
        }
    }
    printf("\n");
}

// input file must be absolute path of the file
void ls_normal_file(char *abspath, int hiddenflag) {
    char *filename = gnu_basename(abspath);
    char *directory = dirname(abspath);
    if (hiddenflag) {
        if (!is_hidden(filename)) {
            ls_normal_single(directory, filename);
        }
    } else {
        ls_normal_single(directory, filename);
    }
}


// input directory must be absolut path of the directory
// filename must be only the name of the file in that directory
void ls_l_single(char *directory, char *filename) {

    char *abspath = (char *) malloc(sizeof(char) * (strlen(directory) + strlen(filename) + 5));

    strcpy(abspath, directory);
    strcat(abspath, "/");
    strcat(abspath, filename);

    struct stat file_stat;
    if (stat(abspath, &file_stat) == -1) {
        perror("Error");
        return;
    }

    char perm[10];
    perm[0] = (S_ISDIR(file_stat.st_mode) ? 'd' : '-');
    perm[1] = ((S_IRUSR & file_stat.st_mode) ? 'r' : '-');
    perm[2] = ((S_IWUSR & file_stat.st_mode) ? 'w' : '-');
    perm[3] = ((S_IXUSR & file_stat.st_mode) ? 'x' : '-');
    perm[4] = ((S_IRGRP & file_stat.st_mode) ? 'r' : '-');
    perm[5] = ((S_IWGRP & file_stat.st_mode) ? 'w' : '-');
    perm[6] = ((S_IXGRP & file_stat.st_mode) ? 'x' : '-');
    perm[7] = ((S_IROTH & file_stat.st_mode) ? 'r' : '-');
    perm[8] = ((S_IWOTH & file_stat.st_mode) ? 'w' : '-');
    perm[9] = ((S_IXOTH & file_stat.st_mode) ? 'x' : '-');
    perm[10] = '\0';

    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);
    ssize_t size = file_stat.st_size;

    // permissions
    printf("%s ", perm);
    // no. of hardlinks
    printf("%-3lu ", file_stat.st_nlink);
    // user and group name
    printf("%-12s ", pw->pw_name);
    printf("%-12s ", gr->gr_name);
    // print file size
    printf("%-6lu ", size);
    // date and time
    char datetime[70];
    strftime(datetime, sizeof datetime, "%b %d %T", localtime(&(file_stat.st_ctime)));
    printf("%s ", datetime);
    // filename
    if (perm[0] != '-') {
        printf(BBLU);
        printf("%s ", filename);
        printf(WHT);
    } else if (perm[3] != '-') {
        printf(BGRN);
        printf("%s ", filename);
        printf(WHT);
    } else {
        printf("%s ", filename);
    }
    printf("\n");
}

// input directory must be absolute path of the directory
void ls_l_dir(char *directory, int hiddenflag) {
    struct dirent **list;
    int n = scandir(directory, &list, NULL, alphasort);
    if (n < 0) {
        perror("Error");
        return;
    }

    for (int i = 0; i < n; i++) {
        if (hiddenflag) {
            if (!is_hidden(list[i]->d_name)) {
                ls_l_single(directory, list[i]->d_name);
            }
        } else {
            ls_l_single(directory, list[i]->d_name);
        }
    }
}
// input file must be absolute path of the file
void ls_l_file(char *abspath, int hiddenflag) {
    char *filename = gnu_basename(abspath);
    char *directory = dirname(abspath);
    if (hiddenflag) {
        if (!is_hidden(filename)) {
            ls_l_single(directory, filename);
        }
    } else {
        ls_l_single(directory, filename);
    }
}

// input must be the abspath of the directory or the file
void ls_flag_wrapper(char *abspath, int hiddenflag, int listflag) {
    struct stat file_stat;
    if (stat(abspath, &file_stat) == -1) {
        perror("Error");
        return;
    }
    if (S_ISDIR(file_stat.st_mode)) {
        if (hiddenflag && listflag) {
            ls_l_dir(abspath, 1);
        } else if (hiddenflag && !listflag) {
            ls_normal_dir(abspath, 1);
        } else if (!hiddenflag && listflag) {
            ls_l_dir(abspath, 0);
        } else {
            ls_normal_dir(abspath, 0);
        }
    } else if (S_ISREG(file_stat.st_mode)) {
        if (hiddenflag && listflag) {
            ls_l_file(abspath, 1);
        } else if (hiddenflag && !listflag) {
            ls_normal_file(abspath, 1);
        } else if (!hiddenflag && listflag) {
            ls_l_file(abspath, 0);
        } else {
            ls_normal_file(abspath, 0);
        }
    }
}

/*
int main(int argc, char* argv[]) {
    getcwd(PWD, sizeof PWD);
    ls_flag_wrapper(argv[1], 1, 1);
    return 0;
}
*/

