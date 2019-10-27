#include "headers.h"

bool logginWithUsername(char* username) {
     
    FILE *configFile;
    
    configFile = fopen(CONFIG_FILE, "r");
    if(configFile == NULL)
    {
        exit(-1);
    }

    char *usernameInFile = NULL;
    size_t length = 0;
    size_t read;
    while((read = getline(&usernameInFile, &length, configFile)) != -1)
    {
        if(strcmp(username, removeNewlineFromString(usernameInFile)) == 0)
        {
            fclose(configFile);
            return true;
        }
    }    
    fclose(configFile);
    return false;
}

char* getFileInfos(char *filaPath) {
    struct stat statBuffer;
    stat(filaPath, &statBuffer);
    char *aux = malloc(sizeof(char));
    sprintf(aux, "File path:              %s\nLast file modification: %sPermisions:             %lo (octal)\nFile size:              %lld bytes\nBlocks allocated:       %lld\n", filaPath, ctime(&statBuffer.st_mtime), ((unsigned long) statBuffer.st_mode),  (long long) statBuffer.st_size,  (long long) statBuffer.st_blocks);
    return aux;
}