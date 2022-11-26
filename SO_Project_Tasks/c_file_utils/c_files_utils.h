//
//  c_files_utils.h
//  SO_Project_Tasks
//
//  Created by Andrei Karolyi on 26.11.2022.
//

#ifndef c_files_utils_h
#define c_files_utils_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h> /** for perror() and error handling functions*/

#include <fcntl.h> /** for opening constants (bits masks)*/

#include <sys/stat.h> /** for lstat() function */

#include <dirent.h>

#include <assert.h> /** for assert() macro */

#include <unistd.h> /** for symlink() function*/

#include "../directory_utils/directory_utils.h"
#include "../general_utils/general_utils.h"

/** no of possible options + 1 ( \0 character ) **/
#define NO_OF_POSSIBLE_OPTIONS 6

void find_c_files_and_execute_symlink(const char *, const unsigned int, const char *);

#endif /* c_files_utils_h */
