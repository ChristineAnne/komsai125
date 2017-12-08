#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <dirent.h>
#include <windows.h>
#include <conio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>

/* gets the current directory and returns it in a char array*/
char* getCurrentDirectory(){
    char cwd[50];
    getcwd(cwd, sizeof(cwd));
    return cwd;
}

/*prints the path of the current directory*/
void printCurrent(){
    char current[50];
    strcpy(current, getCurrentDirectory());
    printf("%s> ", current);
}

/*opens a folder in the current directory or go back one directory*/
void cdCom(char *newDir){
    char current[50];

    // copy the current directory to variable 'current' and append the new folder/directory to 'current'
    strcpy(current, getCurrentDirectory());
    strcat(current, "\\");
    strcat(current, newDir);

    // change directory according to the path in 'current'
    int success = chdir(current);
    if(success != 0){
        printf("The system cannot find the file specified.\n");
    }
}

/*changes the directory according to the path in 'newDir'*/
void chdirCom(char *newDir){
    int success = chdir(newDir);
    if(success != 0){
        printf("The system cannot find the file specified.\n");
    }
}

/*clears the screen*/
void clsCom(){
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if(hStdOut == INVALID_HANDLE_VALUE){
        return;
    }

    // get number of cells in current buffer
    if(!GetConsoleScreenBufferInfo(hStdOut, &csbi)){
        return;
    }
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    // fill buffer with spaces
    if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count)){
        return;
    }

    //move the cursor home
    SetConsoleCursorPosition(hStdOut, homeCoords);
}

/*creates a new instance of the program*/
void cmdCom(){
    printf("Microsoft Windows [Version 6.3.9600]\n(c) 2013 Microsoft Corporation. All rights reserved.\n");
}

/*copies the specified file in the current directory to another directory specified by 'path'*/
void copyCom(char *file, char *path, char *current, int mode){ // if(copy) mode = 1 else if(move) mode = 2
    FILE *source, *target;
    char ch;
    int success = 0;
    char cur[strlen(current)];
    strcpy(cur, current);

    // opens the file to be copied and change the directory according to 'path'
    source = fopen(file, "r");
    if(source == NULL){
        printf("The system cannot find the file specified.\n");
        return;
    }
    chdir(path);

    // make a new file and copy the contents of 'source' unto it
    // TODO: error checking if file already exists
    target = fopen(file, "w");
    if(target != NULL){
        while((ch = fgetc(source)) != EOF ){
            fputc(ch, target);
        }
        success = 1;
    }
    fclose(source);
    fclose(target);

    if(mode == 1){
        if(success == 0){
            printf("The system cannot find the file specified.\n");
        }else{
            printf("\t1 file(s) copied.\n");
        }
    }else if(mode == 2){
        if(success == 0){
            printf("The system cannot find the file specified.\n");
        }else{
            printf("\t1 file(s) moved.\n");
        }
    }

    chdir(cur);
}

/*displays the current date*/
void dateCom(){
    char buffer[32];
    struct tm *ts;
    size_t last;
    time_t timestamp = time(NULL);

    ts = localtime(&timestamp);
    last = strftime(buffer, 32, "%A", ts);
    buffer[last] = '\0';

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("\nThe current date is: %s %d/%d/%d\n", buffer, tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);

    char newDate[50];
    printf("Enter the new date: (mm-dd-yy) ");
    gets(newDate);
    printf("A required privilege is not held by the client.\n");
}

/*deletes a file from the current directory*/
void delCom(char *file){
    int ret = remove(file);
    char cwd[50];
    strcpy(cwd, getCurrentDirectory());
    if(ret != 0){
        // if delete was not successful
        printf("Could Not Find %s\\%s\n", cwd, file);
    }
}

/*displayes all the contents of the current directory*/
void dirCom(){
    DIR *dp;
    char current[50];
    strcpy(current, getCurrentDirectory());
    struct dirent *dirp;
    dp = opendir(current);

    printf(" Volume in drive %c has no label.\n", current[0]);
    if(current[0] == 'C' || current[0] == 'c'){
        printf(" Volume Serial Number is F855-D696\n\n");
    }else if(current[0] == 'd' || current[0] == 'D'){
        printf(" Volume Serial Number is DE61-226C\n\n");
    }
    printf(" Directory of %s\n\n", current);

    while((dirp = readdir(dp)) != NULL){ // prints every content of the current path
        printf("\t%s\n", dirp->d_name);
    }
    closedir(dp);
}

/*makes a new directory*/
void mkdirCom(char *folder){
    struct stat st = {0};
    if (stat(folder, &st) == -1) {
        mkdir(folder);
    }
}

/*moves a file from one directory to the directory specified by 'path'*/
void moveCom(char *file, char *path, char *current){
    char cur[strlen(current)];
    strcpy(cur, current);
    copyCom(file, path, cur, 2);
    chdir(cur);
    delCom(file);
}

/*renames a file with 'newname'*/
void renameCom(char *oldname, char *newname){
   int success = rename(oldname, newname);
   if(success != 0){
        printf("The system cannot find the file specified.\n");
   }
}

/*removes a directory specified by 'path'*/
void rmdirCom(char *path){
    int status = rmdir(path);
    if(status != 0){
        printf("The system cannot find the file specified.\n");
    }
}

/*displays the current time of today*/
void timeCom(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("The current time is: %d:%d:%d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Enter the new time: ");
    char newTime[50];
    gets(newTime);
    printf("A required privilege is not held by the client.\n");
}

/*displays the contents of 'file'*/
void typeCom(char *file){
    FILE *fptr;
    fptr = fopen(file, "r");
    if(fptr != NULL){
        char c;
        c = fgetc(fptr);
        printf("\n%s\n\n", file);
        while(c != EOF){
            printf ("%c", c);
            c = fgetc(fptr);
        }
    }else{
        // cannot find 'file' in current directory
        printf("The system cannot find the file specified.");
    }
    fclose(fptr);
    printf("\n");
}

void helpCom(){
    printf("CD\t\tDisplays the name of or changes the current directory.\n");
    printf("CHDIR\t\tChanges the current directory.\n");
    printf("CLS\t\tClears the screen.\n");
    printf("CMD\t\tStarts a new instance of the command interpreter.\n");
    printf("COPY\t\tCopies one or more files to another location.\n");
    printf("DATE\t\tDisplays or sets the date.\n");
    printf("DEL\t\tDeletes one or more files.\n");
    printf("DIR\t\tDisplays a list of files and subdirectories in a directory.\n");
    printf("MKDIR\t\tCreates a directory.\n");
    printf("MOVE\t\tMoves one or more files from one directory to another directory.\n");
    printf("RENAME\t\tRenames a file or files.\n");
    printf("RMDIR\t\tRemoves a directory.\n");
    printf("TIME\t\tDisplays or sets the system time.\n");
    printf("TYPE\t\tDisplays the contents of a text file.\n");
}

int main(){
    cmdCom();
    printf("\n");
    chdir("C:\\Users\\apple");

    int true = 0;
    while(true == 0){
        printCurrent();
        char string[50];
        gets(string);
        char commands[10][20];
        int i = 0, j = 0, counter = 0; // the counter is for how many arguments there are in 'commands'
        char *token = strtok(string, " ");

        // gets every word entered by user and stores it into 'commands'
        while (token != NULL) {
            strcpy(commands[i], token);
            i++;
            j++;
            counter++;
            token = strtok(NULL, " ");
        }

        int here = 0;

        if(strcmp(commands[0], "cd") == 0 || strcmp(commands[0], "CD") == 0){
            cdCom(commands[1]);
            here = 1;
        }
        if(strcmp(commands[0], "cd..") == 0 || strcmp(commands[0], "CD..") == 0){
            cdCom("..");
            here = 1;
        }
        if(strcmp(commands[0], "chdir") == 0 || strcmp(commands[0], "CHDIR") == 0){
            chdirCom(commands[1]);
            here = 1;
        }
        if(strcmp(commands[0], "cls") == 0 || strcmp(commands[0], "CLS") == 0){
            clsCom();
            here = 1;
        }
        if(strcmp(commands[0], "cmd") == 0 || strcmp(commands[0], "CMD") == 0){
            printf("\n");
            cmdCom();
            here = 1;
        }
        if(strcmp(commands[0], "copy") == 0 || strcmp(commands[0], "COPY") == 0){
            copyCom(commands[1], commands[2], getCurrentDirectory(), 1);
            here = 1;
        }
        if(strcmp(commands[0], "date") == 0 || strcmp(commands[0], "DATE") == 0){
            dateCom();
            here = 1;
        }
        if(strcmp(commands[0], "del") == 0 || strcmp(commands[0], "DEL") == 0){
            delCom(commands[1]);
            here = 1;
        }
        if(strcmp(commands[0], "dir") == 0 || strcmp(commands[0], "DIR") == 0){
            dirCom();
            here = 1;
        }
        if(strcmp(commands[0], "mkdir") == 0 || strcmp(commands[0], "MKDIR") == 0){
            mkdirCom(commands[1]);
            here = 1;
        }
        if(strcmp(commands[0], "move") == 0 || strcmp(commands[0], "MOVE") == 0){
            moveCom(commands[1], commands[2], getCurrentDirectory());
            here = 1;
        }
        if(strcmp(commands[0], "rename") == 0 || strcmp(commands[0], "RENAME") == 0){
            renameCom(commands[1], commands[2]);
            here = 1;
        }
        if(strcmp(commands[0], "rmdir") == 0 || strcmp(commands[0], "RMDIR") == 0){
            rmdirCom(commands[1]);
            here = 1;
        }
        if(strcmp(commands[0], "time") == 0 || strcmp(commands[0], "TIME") == 0){
            timeCom();
            here = 1;
        }
        if(strcmp(commands[0], "type") == 0 || strcmp(commands[0], "TYPE") == 0){
            i = 1;
            while(i < counter){
                typeCom(commands[i]);
                i++;
            }
            here = 1;
        }
        if(strcmp(commands[0], "help") == 0 || strcmp(commands[0], "HELP") == 0){
            helpCom();
            here = 1;
        }
        if(strcmp(commands[0], "exit") == 0 || strcmp(commands[0], "EXIT") == 0){
            return 0;
        }
        if(here == 0){
            printf("'%s' is not recognized as an internal or external command,\noperable program or batch file.\n", commands);
        }

        printf("\n");
    }
    return 0;
}
