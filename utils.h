#include "headers.h"

struct command
{
   int type;
   char *argument;
};

struct searchedFile {
    bool found;
    char fileName[256];
    char filePath[256];
};

struct command getCommandType(char* input) {
    struct command requiredCommand;
    requiredCommand.argument = "";
    requiredCommand.type = INVALID_COMMAND;
    if(strstr(input, "login :") != NULL){
        requiredCommand.type = COMMAND_LOGN;
        requiredCommand.argument = strtok(input+8, " ");
    }
    else if(strstr(input, "myfind") != NULL) {
        requiredCommand.type = COMMAND_MYFIND;
        requiredCommand.argument = strtok(input + 7, " ");
    }
    else if(strstr(input, "mystat") != NULL) {
        requiredCommand.type = COMMAND_MYSTAT;
        requiredCommand.argument = strtok(input + 7, " ");
    }
    else if(strstr(input, "quit") != NULL) {
        requiredCommand.type = COMMAND_QUIT;
        requiredCommand.argument = "";
    }
    else if(strstr(input, "logout") != NULL) {
        requiredCommand.type = COMMAND_LOGGOUT;
        requiredCommand.argument = "";
    }
    return requiredCommand;
}


char* removeNewlineFromString(char* input)
{
    return strtok(input, "\n");
}

int isDirectory(const char *path) {
   struct stat statsBuffer;
   if (stat(path, &statsBuffer) != 0)
       return 0;
   return S_ISDIR(statsBuffer.st_mode);
}

void serchForFile(char* fileName, char *basePath, struct searchedFile *searchedFile) {
   struct dirent *de;  
  
    DIR *directory = opendir(basePath); 
  
    if (directory == NULL)  
    { 
        printf("Could not open directory, %s \n", basePath); 
    } 
  
    while ((de = readdir(directory)) != NULL) 
    {
        if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0 && strcmp(de->d_name, "~"))
        {
            char newPath[256];
            strcpy(newPath,  basePath);
            strcat(newPath, de->d_name);
          
            if(strcmp(fileName, de->d_name) == 0)
            {
                searchedFile->found = true;
                strcpy(searchedFile->filePath, newPath);
                strcpy(searchedFile->fileName, de->d_name);
            } else if(isDirectory(newPath)) {
                strcat(newPath, "/");
                serchForFile(fileName, newPath, searchedFile);
            }
        }

    }
  
    closedir(directory);     
}
char* buildResponsePrefixedByLenght(char *response){
    char *responseAux = malloc(sizeof(char) * strlen(response));
    sprintf(responseAux, "%ld~%s", strlen(response), response);
    return responseAux;
}

int getResponseLenghtFromPrefix(int source) {
    char buf;
    int response = 0;
    while(read(source, &buf, 1))
    {
        if(buf == '~')
        {
            return response;
        }
        response = response*10 + (buf - '0');
    }
    return 0;
}

int getNumberlenght(int number) {
    int lenght = 1;
    while(number)
    {
        number /= 10;
        lenght ++;
    }
    return lenght;
}

void convetAccessRights(mode_t fileStMode, char rights[])
{
    rights[0] = fileStMode & S_IRUSR ? 'r' : '-';
    rights[1] = fileStMode & S_IWUSR ? 'w' : '-';
    rights[2] = fileStMode & S_IXUSR ? 'x' : '-';
    rights[3] = fileStMode & S_IRGRP ? 'r' : '-';
    rights[4] = fileStMode & S_IWGRP ? 'w' : '-';
    rights[5] = fileStMode & S_IXGRP ? 'x' : '-';
    rights[6] = fileStMode & S_IROTH ? 'r' : '-';
    rights[7] = fileStMode & S_IWOTH ? 'w' : '-';
    rights[8] = fileStMode & S_IXOTH ? 'x' : '-';
}

char* getFileInfos(char *filaPath) {
    struct stat statBuffer;
    int statResponse = stat(filaPath, &statBuffer);
    char *aux = malloc(sizeof(char));
    if(statResponse == 0)
    {
        char rightsString[9];
        convetAccessRights(statBuffer.st_mode, rightsString);
        sprintf(aux, "File path:              %s\nLast file modification: %sPermisions:             %lo (%s)\nFile size:              %lld bytes\nBlocks allocated:       %lld\n", filaPath, ctime(&statBuffer.st_mtime), ((unsigned long) statBuffer.st_mode), rightsString, (long long) statBuffer.st_size,  (long long) statBuffer.st_blocks);
        return aux;
    }
    else 
    {
        return "File not found\n";
    }
}
