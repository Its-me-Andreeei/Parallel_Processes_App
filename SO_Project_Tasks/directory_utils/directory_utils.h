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

#define ERROR_perror()                                              \
            do                                                      \
            {                                                       \
                char message[100];                                  \
                sprintf(message, "Error at line %d : ", __LINE__);  \
                perror(message);                                    \
                exit(EXIT_FAILURE);                                 \
            }while(0)


#define ERROR_custom(...)                                           \
            do                                                      \
            {                                                       \
                char message[100];                                  \
                sprintf(message, "Error at line %d : ", __LINE__);  \
                fprintf(stderr, "%s", message);                     \
                fprintf(stderr,__VA_ARGS__);                        \
                exit(EXIT_FAILURE);                                 \
            }while(0)


void search_c_files(const char *, const char*, const int, const char*);
void print_directories_names(void);

#endif /* directory_utils_h */
