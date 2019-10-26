#include <string.h>
#include <dirent.h>
struct searchedFile {
    bool found;
    char fileName[256];
    char filePath[256];
};

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
void buildResponsePrefixedByLenght(char *response){
    char responseAux[1024];
    strcpy(responseAux, response);
    sprintf(response, "%ld~%s", strlen(response), responseAux);
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