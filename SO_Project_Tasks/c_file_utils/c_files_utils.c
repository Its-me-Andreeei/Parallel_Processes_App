//
//  c_files_utils.c
//  SO_Project_Tasks
//
//  Created by Andrei Karolyi on 26.11.2022.
//

#include "c_files_utils.h"

void find_c_files_and_execute_symlink(const char root_realtive_path[], const unsigned int step, const char options[]);

static int is_c_file(const char name[]);
static int check_for_dot_refs(char name[]);
static void options_execution(const char c_file_path[], const char options[], const char c_file_name[]);
static void check_for_invalid_options(const char options[]);
static void check_if_symlink_exists_already_and_unlink(const char c_file_name[]);
static void create_symlink_for_c_files_under_100kb(const char path_of_c_file[] , const char c_file_name[]);
static void create_first_child_process(char *new_relative_path, const char *options, const char * c_file_name);
static void create_second_child_process(const char *c_file_path, const char c_file_name[]);


const char possible_options[NO_OF_POSSIBLE_OPTIONS]="nuadcgp";

static int is_c_file(const char name[])
{
    lstat_checking(name);
    return name[ strlen(name)-1 ]=='c' && name[ strlen(name)-2 ]=='.' && is_Regular_File();
}

static int check_for_dot_refs(char name[])
{
    if(strcmp(".", name)==0 || strcmp("..", name)==0)
        return 1;
    return 0;
}

static void create_first_child_process(char *new_relative_path, const char *options, const char c_file_name[]) {
    int child_PID;
    int child_status;
    
    if( (child_PID=fork()) <0 )
        ERROR_perror();
    if(child_PID == 0) /** child code here*/
    {
        options_execution(new_relative_path, options, c_file_name);
        
        exit(0); /** in order to put child on zombie state, with exit code 0*/
    }
    for(;;){
        int w = waitpid(child_PID, &child_status, WNOHANG); //No hang means it doesn't wait for it
        if( w == child_PID) //means change of state;  w == 0 means there is no change in child's state
        {
            if( WIFEXITED(child_status))
            {
                printf("Child process with PID (%d) exited with code (%d)\n", child_PID, WEXITSTATUS(child_status));
                break;
            }
            else if (WIFSIGNALED(child_status))
            {
                printf("Child process with PID (%d) terminated due to signal no. (%d)\n", child_PID, WTERMSIG(child_status));
                break;
            }
        }
        else if (w==-1)
        {
            ERROR_custom("There is no child process with PID (%d)\n", child_PID);
        }
    }
}

static void create_second_child_process(const char *c_file_path, const char c_file_name[]) {
    int child_PID;
    int child_status;
    
    char out_file_name[100];
    
    strcpy(out_file_name, "./");
    strncat(out_file_name, c_file_name, strlen(c_file_name)-2);
    strcat(out_file_name, "_out");
    
    if( (child_PID=fork()) <0 )
        ERROR_perror();
    if(child_PID == 0) /** child code here*/
    {
        execlp("gcc", "gcc", "-Wall", "-o", out_file_name, c_file_path, NULL);
        
        ERROR_custom("Could not overwrite fork() function with execlp(...) call\n");
    }
    for(;;){
        int w = waitpid(child_PID, &child_status, WNOHANG); //No hang means it doesn't wait for it
        if( w == child_PID) //means change of state;  w == 0 means there is no change in child's state
        {
            if( WIFEXITED(child_status))
            {
                printf("Child process with PID (%d) exited with code (%d)\n", child_PID, WEXITSTATUS(child_status));
                break;
            }
            else if (WIFSIGNALED(child_status))
            {
                printf("Child process with PID (%d) terminated due to signal no. (%d)\n", child_PID, WTERMSIG(child_status));
                break;
            }
        }
        else if (w==-1)
        {
            ERROR_custom("There is no child process with PID (%d)\n", child_PID);
        }
    }
}

void find_c_files_and_execute_symlink(const char root_realtive_path[], const unsigned int step, const char options[]) {
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
                create_first_child_process(new_relative_path, options, dir_entry->d_name);
                create_symlink_for_c_files_under_100kb(new_relative_path, dir_entry->d_name);
            }
            else if( is_Directory() && !check_for_dot_refs(dir_entry->d_name))
            {
                strcpy(dir.name[dir.char_pointer++], new_relative_path);
            }
            strcpy(new_relative_path, root_realtive_path); // used to re-INIT new_relative_path buffer
        }
    if( step < dir.char_pointer-1 ){ // there is a -1 because otherwise it will overflow the buffer dir.name[]
        find_c_files_and_execute_symlink(dir.name[step+1], step+1, options);
    }

}

static void options_execution(const char c_file_path[], const char options[], const char c_file_name[])
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
            printf("\nUser identifier (UID): %d\n", get_UID() );
        }
        if(strchr(options, 'a'))
        {
            printf("\n------------------\n");
            printf("\nUSER:\n");
            
            printf("Read : ");
            if(is_User_Read_Allowed())
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Write : ");
            if(is_User_Write_Allowed())
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Execution : ");
            if(is_User_Execute_Allowed())
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("\nGROUP:\n");
            
            printf("Read : ");
            if(is_Group_Read_Allowed())
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Write : ");
            if(is_Group_Write_Allowed())
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Execution : ");
            if(is_Group_Execute_Allowed())
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("\nOTHERS:\n");
            
            printf("Read : ");
            if(is_Other_Read_Allowed())
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Write : ");
            if(is_Other_Write_Allowed())
                printf("YES\n");
            else
                printf("NO\n");
            
            printf("Execution : ");
            if(is_Other_Execute_Allowed())
                printf("YES\n\n");
            else
                printf("NO\n\n");
            
            printf("------------------\n");
        }
        if(strchr(options, 'd'))
        {
            printf("\nNumber of bytes = %lld\n", get_file_Byte_size());
        }
        
        if(strchr(options, 'c'))
        {
            printf("\nNumber of Hard Links = %d\n", get_file_no_of_Hard_Links());
        }
        
        if(strchr(options, 'g'))
        {
            create_second_child_process(c_file_path, c_file_name);
        }
    }
    //options can be missed
}

static void check_for_invalid_options(const char options[])
{
    if(strlen(options)>NO_OF_POSSIBLE_OPTIONS)
        ERROR_custom("Too many options given\n");
    for(int i=1; i < strlen(options); i++)
    {
        if(strchr(possible_options , options[i]) == NULL)
            ERROR_custom("Invalid option found: %c\n", options[i]);
    }
}

static void create_symlink_for_c_files_under_100kb(const char path_of_c_file[] , const char c_file_name[])
{
    float size_in_kilobytes= get_file_Byte_size() / 1024; //conversion from bytes to kilobytes
    
    char path_without_c_extension[100]="./";
    check_if_symlink_exists_already_and_unlink(c_file_name);

    strncat(path_without_c_extension, c_file_name, strlen(c_file_name)-2);
    
    if(size_in_kilobytes < 100)
    {
        if( symlink(path_of_c_file, path_without_c_extension) != 0)
            ERROR_perror();
    }
}

static void check_if_symlink_exists_already_and_unlink(const char c_file_name[])
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
