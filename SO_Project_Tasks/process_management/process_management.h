//
//  process_management.h
//  SO_Project_Tasks
//
//  Created by Andrei Karolyi on 30.11.2022.
//

#ifndef process_management_h
#define process_management_h

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include <errno.h> /** for perror() and error handling functions*/

#include <fcntl.h> /** for opening constants (bits masks)*/

#include <sys/stat.h> /** for lstat() function */

#include <dirent.h>

#include <assert.h> /** for assert() macro */

#include <unistd.h> /** for symlink() function*/

#include <sys/types.h>
#include <sys/uio.h>

#include "../general_utils/general_utils.h"

void execute_options_process(char *new_relative_path, const char *options, const char c_file_name[], void (*option_execution_def)(const char *, const char *, const char *) );
void compile_c_file_process_simple_variant(const char *c_file_path, const char c_file_name[], const char options[]);
void compile_c_file_process_modified_variant(const char *c_file_path, const char c_file_name[], const char options[] );
void wait_for_child_loop(pid_t child_PID, char*);

#endif /* process_management_h */
