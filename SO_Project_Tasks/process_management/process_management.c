//
//  process_management.c
//  SO_Project_Tasks
//
//  Created by Andrei Karolyi on 30.11.2022.
//

#include "process_management.h"

void execute_options_process(char *new_relative_path, const char *options, const char c_file_name[], void (*options_execution_def)(const char *, const char *, const char *) );
void compile_c_file_process_simple_variant(const char *c_file_path, const char c_file_name[], const char options[] );
void wait_for_child_loop(pid_t child_PID, char*);
void create_unique_name_for_symbolic_link(const char *c_file_name, char *path_without_c_extension);

static void launch_grep_as_process_without_waitPID_inside(pid_t *c2_child_PID, int pipe_c1_c2[2], int pipe_p_c2[2]);
static void launch_gcc_as_process_without_waitPID_inside(pid_t *c1_child_PID, const char *c_file_name, const char *c_file_path, char *out_file_name, int pipe_c1_c2[2], int pipe_p_c2[2]);
static void read_buffer_from_pipe(char *buffer, int *pipe_p_c2);
static float calculate_result(char buffer[]);
static int get_number_of_warnings(char buffer[]);
static void create_unique_name_for_out_file(const char *c_file_name, char *out_file_name);



static void create_unique_name_for_out_file(const char *c_file_name, char *out_file_name)
{
    strcpy(out_file_name, "./"); //overrides out_file_name if there is any content
    strncat(out_file_name, c_file_name, strlen(c_file_name)-2); //name without ".c"
    strcat(out_file_name, "_out");

    while(does_Regular_File_Exists(out_file_name))
    {
        strcat(out_file_name, "1");
    }
}

void create_unique_name_for_symbolic_link(const char *c_file_name, char *path_without_c_extension)
{
    strcpy( path_without_c_extension, "./");
    strncat(path_without_c_extension, c_file_name, strlen(c_file_name)-2);
    
    while(does_Symbolic_Link_Exists(path_without_c_extension))
    {
        strcat(path_without_c_extension, "_1");
    }
}

void compile_c_file_process_simple_variant(const char *c_file_path, const char c_file_name[], const char options[] )
{
    pid_t second_child_PID;
    char out_file_name[100];

    create_unique_name_for_out_file(c_file_name, out_file_name);
    
    /**create child process for compile**/
    if( (second_child_PID=fork()) <0 )
        ERROR_perror();
    if(second_child_PID == 0) /** child code here*/
    {
        execlp("gcc", "gcc", "-Wall", "-o", out_file_name, c_file_path, NULL); //execlp also closes pipe end
        ERROR_custom("Could not overwrite fork() function with execlp(...) call\n");
    }
    
    wait_for_child_loop(second_child_PID, NULL);
}

static void launch_gcc_as_process_without_waitPID_inside(pid_t *c1_child_PID, const char *c_file_name, const char *c_file_path, char *out_file_name, int pipe_c1_c2[2], int pipe_p_c2[2]) {
    if( (*c1_child_PID=fork()) <0 )
        ERROR_perror();
    if(*c1_child_PID == 0) /** child code here*/
    {
        close(pipe_p_c2[0]); //c1 does not participate to parent and c2 conversation through pipe 2
        close(pipe_p_c2[1]);
        
        close(pipe_c1_c2[0]); //close read end
        
        if(dup2(pipe_c1_c2[1], 2) < 0)
            ERROR_perror();
        
        create_unique_name_for_out_file(c_file_name, out_file_name);
        
        printf("warning");
        execlp("gcc", "gcc", "-Wall", "-o", out_file_name, c_file_path, NULL); //execlp also closes pipe ends
        ERROR_custom("Could not overwrite fork() function with execlp(...) call\n");
    }
}

static void launch_grep_as_process_without_waitPID_inside(pid_t *c2_child_PID, int pipe_c1_c2[2], int pipe_p_c2[2]) {
    if( (*c2_child_PID=fork()) <0 )
        ERROR_perror();
    if(*c2_child_PID == 0) /** child code here*/
    {
        close(pipe_c1_c2[1]); //close write end
        close(pipe_p_c2[0]); //c2 should only write to next pipe
        
        if(dup2(pipe_c1_c2[0], 0) <0 )
            ERROR_perror();
        
        if(dup2(pipe_p_c2[1], 1) <0 )
            ERROR_perror();
        
        execlp("grep", "grep", "-iE", "(warning|error)", NULL); //-i options means "no diff. between uppercase and lowercase letters
    }
}

static void read_buffer_from_pipe(char *buffer, int *pipe_p_c2) {
    int index=0;
    char c;
    
    while(read(pipe_p_c2[0], &c, sizeof(char)) != 0)
    {
        buffer[index++]=c;
    }
    buffer[index]='\0';
}

static int get_number_of_warnings(char buffer[])
{
    char *p;
    char delim[]=" :.,'";
    int number_of_warnings=0;
    
    p=strtok(buffer, delim);
    while(p)
    {
        if(strcmp(p,"warning") ==0 )
            number_of_warnings++;
        
        p=strtok(NULL, delim);
    }
    
    return number_of_warnings;
}

static float calculate_result(char buffer[])
{
    float result;
    if(strcasestr(buffer, "error"))
        result=1;
    else{
        if(strcasestr(buffer, "warning")==NULL)
            result=10;
        else
        {   //there is at least 1 warning, no error
            int no_of_warnings=get_number_of_warnings(buffer);
            if(no_of_warnings > 10)
                result=2;
            else
                result=2+8.0*(10-no_of_warnings)/10;
        }
    }
    
    return result;
}

void compile_c_file_process_modified_variant(const char *c_file_path, const char c_file_name[], const char options[] )
{
    pid_t gcc_c1_child_PID;
    pid_t grep_c2_child_PID;
    
    char out_file_name[100];
    char buffer[1000];
    //FILE *file_descriptor;
    
    int pipe_c1_c2[2];
    int pipe_p_c2[2];
    
    if(pipe(pipe_c1_c2) < 0)
        ERROR_perror();
    
    if(pipe(pipe_p_c2) < 0)
        ERROR_perror();
    
    /**create child process for compile**/
    launch_gcc_as_process_without_waitPID_inside(&gcc_c1_child_PID, c_file_name, c_file_path, out_file_name, pipe_c1_c2, pipe_p_c2);
    
    /**create child process for reading and filtering from pipe**/
    launch_grep_as_process_without_waitPID_inside(&grep_c2_child_PID, pipe_c1_c2, pipe_p_c2);
    
    close(pipe_c1_c2[0]); //parent process does not participate in c1 and c2 's talking
    close(pipe_c1_c2[1]);
    
    close(pipe_p_c2[1]);
    
    wait_for_child_loop(gcc_c1_child_PID, "GCC may have found errors, please run again with -n options to see which file fails to compile");
    wait_for_child_loop(grep_c2_child_PID, "grep did not find any matching line");
    
    read_buffer_from_pipe(buffer, pipe_p_c2);
    printf("Code quality result: %0.2f points\n", calculate_result(buffer));
    
}

//-------------------------------------------------------------------------------------------
//UPDATE TO BE ADDED to handle more justifications for particular error codes using a struct
//-------------------------------------------------------------------------------------------
void wait_for_child_loop(pid_t child_PID, char *justification)
{
    int child_status;
    
    for(;;){
        int w = waitpid(child_PID, &child_status, WNOHANG); //No hang means it doesn't wait for it
        if( w == child_PID) //means change of state;  w == 0 means there is no change in child's state
        {
            if( WIFEXITED(child_status))
            {
                if(justification==NULL)
                    printf("Child process with PID (%d) exited with code (%d)\n", child_PID, WEXITSTATUS(child_status));
                else if(justification!=NULL && WEXITSTATUS(child_status)!=0)
                    printf("Child process with PID (%d) exited with code (%d) : %s\n", child_PID, WEXITSTATUS(child_status), justification);
                break;
            }
            else if (WIFSIGNALED(child_status))
            {
                if(WTERMSIG(child_status) != 5)
                {
                    printf("Child process with PID (%d) terminated due to signal no. (%d)\n", child_PID, WTERMSIG(child_status));
                    break;
                }
            }
        }
        else if (w==-1)
        {
            ERROR_custom("There is no child process with PID (%d)\n", child_PID);
        }
    }
}

void execute_options_process(char *new_relative_path, const char *options, const char c_file_name[], void (*options_execution_def)(const char *, const char *, const char *) )
{
    pid_t child_PID;
    
    if( (child_PID=fork()) <0 )
        ERROR_perror();
    if(child_PID == 0) /** child code here*/
    {
        options_execution_def(new_relative_path, options, c_file_name);
        
        exit(0); /** in order to put child on zombie state, with exit code 0*/
    }
    wait_for_child_loop(child_PID, NULL);
}
