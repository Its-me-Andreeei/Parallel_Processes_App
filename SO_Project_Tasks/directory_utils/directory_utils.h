//
//  directory_utils.h
//  Proiect_SO
//
//  Created by Andrei Karolyi on 05.11.2022.
//

#ifndef directory_utils_h
#define directory_utils_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h> /** for perror() and error handling functions*/

#include <fcntl.h> /** for opening constants (bits masks)*/

#include <sys/stat.h> /** for lstat() function */

#include <dirent.h>

#include <assert.h> /** for assert() macro */

#include <unistd.h> /** for symlink() function*/

#include "../c_file_utils/c_files_utils.h"
#include "../general_utils/general_utils.h"

typedef struct DIR_stack{
    DIR *stack[100];
    char name[100][100];
    unsigned int dir_pointer;
    unsigned int char_pointer;
}DIR_stack;
extern DIR_stack dir;


void search_c_files(const char *, const char*, const int);
void print_directories_names(void);
void open_directory(const char* );
void close_directories(void);

#endif /* directory_utils_h */
