#!/bin/sh

clear

gcc -Wall -c ./"$1"/process_management/process_management.c
gcc -Wall -c ./"$1"/c_file_utils/c_files_utils.c
gcc -Wall -c ./"$1"/directory_utils/directory_utils.c
gcc -Wall -c ./"$1"/general_utils/general_utils.c
gcc -Wall -c ./"$1"/main.c



gcc -Wall -o macOS_executable main.o process_management.o directory_utils.o c_files_utils.o general_utils.o

rm main.o
rm process_management.o
rm directory_utils.o
rm c_files_utils.o
rm general_utils.o