//
//  general_utils.c
//  SO_Project_Tasks
//
//  Created by Andrei Karolyi on 26.11.2022.
//

#include "general_utils.h"

struct stat lstat_result;


void lstat_checking(const char *relative_path) {
    assert(relative_path != NULL);
    
    if(lstat(relative_path, &lstat_result)!=0)
        ERROR_perror();
}

int is_Directory()
{
    return S_ISDIR(lstat_result.st_mode) == 1;
}

int get_UID()
{
    return lstat_result.st_uid;
}

long long get_file_Byte_size()
{
    return lstat_result.st_size;
}

int get_file_no_of_Hard_Links()
{
    return lstat_result.st_nlink;
}

int is_User_Read_Allowed()
{
    return (S_IRUSR & lstat_result.st_mode) != 0;
}

int is_User_Write_Allowed()
{
    return (S_IWUSR & lstat_result.st_mode) != 0;
}

int is_User_Execute_Allowed()
{
    return (S_IXUSR & lstat_result.st_mode) != 0;
}

int is_Group_Read_Allowed()
{
    return (S_IRGRP & lstat_result.st_mode) != 0;
}

int is_Group_Write_Allowed()
{
    return (S_IWGRP & lstat_result.st_mode) != 0;
}

int is_Group_Execute_Allowed()
{
    return (S_IXGRP & lstat_result.st_mode) != 0;
}

int is_Other_Read_Allowed()
{
    return (S_IROTH & lstat_result.st_mode) != 0;
}

int is_Other_Write_Allowed()
{
    return (S_IWOTH & lstat_result.st_mode) != 0;
}

int is_Other_Execute_Allowed()
{
    return (S_IXOTH & lstat_result.st_mode) != 0;
}

int is_Regular_File()
{
    return S_ISREG(lstat_result.st_mode);
}
