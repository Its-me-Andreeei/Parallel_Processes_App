//
//  main.c
//  Proiect_SO
//
//  Created by Andrei Karolyi on 03.11.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directory_utils/directory_utils.h"


int main(const int argc, const char * argv[]) {

    search_c_files(argv[1], argv[2], argc);
    
    print_directories_names();//For debugging only
        
    return 0;
}
