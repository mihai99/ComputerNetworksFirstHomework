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

char* findStatsOfFile(char* fileName, bool searchForFile, bool isLoggedIn)
{
    char *response = malloc(sizeof(char));
    struct searchedFile file;
    file.found = false;
    if(isLoggedIn == false)
    {
        response = "Please log in to use other commands\n";
        return response;
    }
    else if(searchForFile)
    {
        serchForFile(removeNewlineFromString(fileName), "./", &file);           
    } 
    else if(!searchForFile)
    {
        strcpy(file.filePath, removeNewlineFromString(fileName));
    }    
    response = getFileInfos(file.filePath);
    
    return response;
}