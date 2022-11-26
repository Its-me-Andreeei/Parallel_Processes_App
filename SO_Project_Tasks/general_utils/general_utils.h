//
//  general_utils.h
//  SO_Project_Tasks
//
//  Created by Andrei Karolyi on 26.11.2022.
//

#ifndef general_utils_h
#define general_utils_h

#include <stdio.h>
#include <stdlib.h>

#include <errno.h> /** for perror() and error handling functions*/

#include <fcntl.h> /** for opening constants (bits masks)*/

#include <sys/stat.h> /** for lstat() function */

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

void lstat_checking(const char *); //to be integrated as a static function

int is_Directory(void);

int get_UID(void);
long long get_file_Byte_size(void);
int get_file_no_of_Hard_Links(void);

int is_Regular_File(void);

int is_User_Read_Allowed(void);
int is_User_Write_Allowed(void);
int is_User_Execute_Allowed(void);
int is_Group_Read_Allowed(void);
int is_Group_Write_Allowed(void);
int is_Group_Execute_Allowed(void);
int is_Other_Read_Allowed(void);
int is_Other_Write_Allowed(void);
int is_Other_Execute_Allowed(void);

#endif /* general_utils_h */
