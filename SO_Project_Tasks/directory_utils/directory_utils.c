//
//  directory_utils.c
//  Proiect_SO
//
//  Created by Andrei Karolyi on 05.11.2022.
//

#include "directory_utils.h"

//shared variables (global to this .c only):
struct stat lstat_result;

typedef struct DIR_stack{
    DIR *stack[100];
    char name[100][100];
    unsigned int dir_pointer;
    unsigned int char_pointer;
}DIR_stack;
DIR_stack dir;
const char possible_options[6]="nuadc";


static void check_no_of_args(const int argc, const char* argv4);
static void lstat_checking(const char *relative_path);
static void directory_checking(const char *directory_path);
static void open_directory(const char* directory_path);
static void close_directories(void);
static int is_c_file(const char name[]);
static int check_for_dot_refs(char name[]);
static void check_if_symlink_exists(const char*);
static void create_symlink_for_c_files_under_100kb(const char path_of_c_file[] , const char c_file_name[]);
static void options_execution(const char*, const char*);
static void find_c_files_and_execute_printf(const char root_realtive_path[], const unsigned int step, const char options[]);
void search_c_files(const char input_name[], const char options[], const int argc, const char argv4[] );
void print_directories_names(void);


static void check_no_of_args(const int argc, const char* argv4)
{
    if(!((argc>=2 && argc <=3) || (strcmp(argv4, "DEBUG")==0 && argc==4)))
        ERROR_custom("Number of Arguments do not match\n");
}

static void lstat_checking(const char *relative_path) {
    assert(relative_path != NULL);
    
    if(lstat(relative_path, &lstat_result)!=0)
        ERROR_perror();
}

static void directory_checking(const char *directory_path)
{
    assert(directory_path != NULL);
    
    lstat_checking(directory_path);
    
    if( S_ISDIR(lstat_result.st_mode) ==0)
        ERROR_custom("\"%s\" is not a directory\n", directory_path);
}

static void open_directory(const char* directory_path)
{
    assert( directory_path != NULL);
    directory_checking(directory_path);
    
    if( (dir.stack[dir.dir_pointer++] = opendir(directory_path)) == NULL)
        ERROR_custom("Cannot open directory\n");
}

static void close_directories(void)
{
    for(int i=0;i<dir.dir_pointer;i++)
        closedir(dir.stack[i]);
    dir.dir_pointer=0; //dir_pointer must be reseted
}

static int is_c_file(const char name[])
{
    lstat_checking(name);
    return name[ strlen(name)-1 ]=='c' && name[ strlen(name)-2 ]=='.' && S_ISREG(lstat_result.st_mode);
}

static int check_for_dot_refs(char name[])
{
    if(strcmp(".", name)==0 || strcmp("..", name)==0)
        return 1;
    return 0;
}

static void create_symlink_for_c_files_under_100kb(const char path_of_c_file[] , const char c_file_name[])
{
    float size_in_kilobytes= lstat_result.st_size / 1024; //conversion from bytes to kilobytes
    
    char path_without_c_extension[100]="./";
    check_if_symlink_exists(c_file_name);

    strncat(path_without_c_extension, c_file_name, strlen(c_file_name)-2);
    
    if(size_in_kilobytes < 100)
    {
        if( symlink(path_of_c_file, path_without_c_extension) != 0)
            ERROR_perror();
    }
}

static void check_if_symlink_exists(const char c_file_name[])
{
    char name_without_c[100]="";
    char path_to_be_unlinked[100]="./";
    
    strncpy(name_without_c, c_file_name, strlen(c_file_name) - 2);
    strcat(path_to_be_unlinked, name_without_c);
    
    if( unlink(path_to_be_unlinked)!=0)
    {
        char str[100];
        strcpy(str, strerror(errno));
        if(strcmp("No such file or directory", str)!=0)
            ERROR_perror();
    }
}

static void find_c_files_and_execute_printf(const char root_realtive_path[], const unsigned int step, const char options[]) {
    struct dirent *dir_entry;
    char new_relative_path[100];
    open_directory(root_realtive_path);
    strcpy(new_relative_path, root_realtive_path);
    
        while((dir_entry=readdir(dir.stack[dir.dir_pointer-1])) != NULL)
        {
            strcat(new_relative_path, "/");
            strcat(new_relative_path, dir_entry->d_name);
            
            if( is_c_file(new_relative_path) && !check_for_dot_refs(dir_entry->d_name))
            {
                options_execution(new_relative_path, options);
                create_symlink_for_c_files_under_100kb(new_relative_path, dir_entry->d_name);
            }
            else if( S_ISDIR(lstat_result.st_mode) && !check_for_dot_refs(dir_entry->d_name))
            {
                strcpy(dir.name[dir.char_pointer++], new_relative_path);
            }
            strcpy(new_relative_path, root_realtive_path); // used to re-INIT new_relative_path buffer
        }
    if( step < dir.char_pointer-1 ){ // there is a -1 because otherwise it will overflow the buffer dir.name[]
        find_c_files_and_execute_printf(dir.name[step+1], step+1, options);
    }

    
}

static void check_for_invalid_options(const char options[])
{
    if(strlen(options)>6)
        ERROR_custom("Too many options given\n");
    for(int i=1; i < strlen(options); i++)
    {
        if(strchr(possible_options , options[i]) == NULL)
            ERROR_custom("Invalid option found: %c\n", options[i]);
    }
}

static void options_execution(const char c_file_path[], const char options[])
{
    if(options != NULL)
    {
        check_for_invalid_options(options);
        lstat_checking(c_file_path);
        if(strchr(options, 'n'))
        {
            printf("\nName of input file: %s\n", c_file_path);
        }
        if(strchr(options, 'u'))
        {
            printf("\nUser identifier (UID): %d\n", lstat_result.st_uid);
        }
        if(strchr(options, 'a'))
        {
            printf("\n------------------\n");
            printf("\nUSER:\n");
            
            printf("Read : ");
            if(S_IRUSR & lstat_result.st_mode)
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Write : ");
            if(S_IWUSR & lstat_result.st_mode)
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Execution : ");
            if(S_IXUSR & lstat_result.st_mode)
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("\nGROUP:\n");
            
            printf("Read : ");
            if(S_IRGRP & lstat_result.st_mode)
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Write : ");
            if(S_IWGRP & lstat_result.st_mode)
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Execution : ");
            if(S_IXGRP & lstat_result.st_mode)
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("\nOTHERS:\n");
            
            printf("Read : ");
            if(S_IROTH & lstat_result.st_mode)
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Write : ");
            if(S_IWOTH & lstat_result.st_mode)
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Execution : ");
            if(S_IXOTH & lstat_result.st_mode)
                printf("YES\n\n");
            else
                printf("NO\n\n");
            
            printf("------------------\n");
        }
        if(strchr(options, 'd'))
        {
            printf("\nNumber of bytes = %lld\n", lstat_result.st_size);
        }
        
        if(strchr(options, 'c'))
        {
            printf("\nNumber of Hard Links = %d\n", lstat_result.st_nlink);
        }
    }
    //options can be missed
}

void search_c_files(const char input_name[], const char options[], const int argc, const char argv4[] )
{
    assert(input_name != NULL);
    
    if(input_name == NULL)
        ERROR_custom("There is no directory.\n");
    
    check_no_of_args(argc, argv4);
    
    //create base relative path
    char relative_path[100]="./";
    strcat(relative_path, input_name);
    
    //JUSTIFICATION: index for char of relative paths should be 1 due to recursive calling -> first auto call woult do step+1 and skip entry 0
    
    dir.char_pointer=1;
    
    //open_directory(relative_path);
    
    find_c_files_and_execute_printf(relative_path, 0, options);
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
