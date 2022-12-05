//
//  directory_utils.c
//  Proiect_SO
//
//  Created by Andrei Karolyi on 05.11.2022.
//

#include "directory_utils.h"

//shared variables (global to this .c only):

DIR_stack dir;

static void check_no_of_args(const int argc);
static void directory_checking(const char *directory_path);
void open_directory(const char* directory_path);
void close_directories(void);
void search_c_files(const char input_name[], const char options[], const int argc);
void print_directories_names(void); /** for debugging purpose */


static void check_no_of_args(const int argc)
{
    if( !(argc>=2 && argc <=3) )
        ERROR_custom("Number of Arguments do not match\n");
}

static void directory_checking(const char *directory_path)
{
    assert(directory_path != NULL);
    
    lstat_checking(directory_path);
    
    if( ! is_Directory())
        ERROR_custom("\"%s\" is not a directory\n", directory_path);
}

void open_directory(const char* directory_path)
{
    assert( directory_path != NULL);
    directory_checking(directory_path);
    
    if( (dir.stack[dir.dir_pointer++] = opendir(directory_path)) == NULL)
        ERROR_custom("Cannot open directory\n");
}

void close_directories(void)
{
    for(int i=0;i<dir.dir_pointer;i++)
        closedir(dir.stack[i]);
    dir.dir_pointer=0; //dir_pointer must be reseted
}

void search_c_files(const char input_name[], const char options[], const int argc)
{
    assert(input_name != NULL);
    
    if(input_name == NULL)
        ERROR_custom("There is no directory.\n");
    
    check_no_of_args(argc);
    
    //create base relative path
    char relative_path[100]="./";
    strcat(relative_path, input_name);
    
    //JUSTIFICATION: index for char of relative paths should be 1 due to recursive calling -> first auto call woult do step+1 and skip entry 0
    
    dir.char_pointer=1;
    
    find_c_files_and_execute_symlink(relative_path, 0, options);
    close_directories();
}


//FOR DEBUGGING PURPOSE -> Should be deleted after
void print_directories_names(void)
{
    for(int i=0;i<dir.char_pointer;i++)
    {
        printf("%s\n", dir.name[i]);
    }
}
