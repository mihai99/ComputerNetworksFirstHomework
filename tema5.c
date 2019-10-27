
#include "commandHelpers.h"
#include "commandRunners.h"
#include "headers.h"

int main() {            

    bool loggedIn = false;
    bool quit = false;
    while (quit == false)
    {
        char command[100];
        printf("Give command: ");
        fgets(command, 100, stdin);
        struct command commandToExecute = getCommandType(removeNewlineFromString(command));       

        if(commandToExecute.type != INVALID_COMMAND)
        {
            int argumentPipe[2], loginResponsPipe[2], socketPair[2];
            pipe(argumentPipe);
            pipe(loginResponsPipe);
            socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair);
            int childPid = fork();

            if(childPid > 0) {
                //PARENT
                close(argumentPipe[0]);                
                write(argumentPipe[1], commandToExecute.argument, (strlen(commandToExecute.argument)+1));                
                wait(&childPid);  
                switch (commandToExecute.type)
                {
                    case COMMAND_LOGN: 
                    {
                        char loginResponse;
                        close(loginResponsPipe[1]);
                        read(loginResponsPipe[0], &loginResponse, 1);
                        switch (loginResponse - '0')
                        {
                            case 0:
                                printf("Loggin failed, please try aggain \n");
                                loggedIn = false;
                                break;
                            case 1: 
                                printf("You successfully logged in, now you can use other commands \n");
                                loggedIn = true;
                            case 2:
                                printf("You are allready logged in \n");
                            default:
                                break;
                        }
                        break;
                    }
                    case COMMAND_MYFIND:
                    {
                         close(socketPair[1]); 
                        char myfindResponse[1024] = "";
                        int lenght = getResponseLenghtFromPrefix(socketPair[0]);
                        read(socketPair[0], myfindResponse, 1000);
                        printf("\n%s\n", myfindResponse); 
                        close(socketPair[0]);
                        break;
                    }
                    default:
                        break;
                }      
            }
            else if(childPid == 0){
                //CHILD
                close(argumentPipe[1]);
                char argument[80];
                read(argumentPipe[0], argument, sizeof(argument));
                switch(commandToExecute.type)
                {
                    case COMMAND_LOGN: 
                    {
                        bool tryLogin = false;
                        close(loginResponsPipe[0]);
                        if(loggedIn == false)
                        {
                            tryLogin = logginWithUsername(argument);                       
                            write(loginResponsPipe[1], (tryLogin ? "1" : "0"), 1);
                        }
                        else
                        {
                            write(loginResponsPipe[1], "2", 1);
                        }                        
                        exit(COMMAND_LOGN);
                    }
                    case COMMAND_MYFIND: 
                    {
                        close(socketPair[0]); 
                        struct searchedFile file;
                        file.found = false;
                        serchForFile(removeNewlineFromString(argument), "./", &file);
                        char *response = "File not found \n";
                        
                        if(file.found == true) 
                        {            
                            response = getFileInfos(file.filePath);  
                        }
                        char *prefixedResponse = buildResponsePrefixedByLenght(response);
                        write(socketPair[1], prefixedResponse, strlen(prefixedResponse));
                        close(socketPair[1]);
                        exit(COMMAND_MYFIND);
                    }
                    default:
                    {
                        exit(INVALID_COMMAND);
                    }
                }

            }
        }   
        else
        {
            printf("invalid command, please try again \n");
        }
                   
    }  
    
 
}