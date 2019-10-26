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

void writeFileInfos(char *filaPath, char *response) {
    struct stat statBuffer;
    stat(filaPath, &statBuffer);
    char *aux;
    sprintf(response, "File path:                %s \n", filaPath);
    sprintf(aux, "Last file modification:   %s", ctime(&statBuffer.st_mtime));
    strcat(response, aux);
    sprintf(aux, "Permisions:               %lo (octal)\n", ((unsigned long) statBuffer.st_mode));
    strcat(response, aux);
    sprintf(aux, "File size:                %lld bytes\n",  (long long) statBuffer.st_size);
    strcat(response, aux);
    sprintf(aux, "Blocks allocated:         %lld\n",  (long long) statBuffer.st_blocks);
    strcat(response, aux);
}