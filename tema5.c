
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
                int fifo = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
                int stats;        
                
                waitpid(childPid, &stats, 0);
                if(!WIFEXITED(stats)) 
                {
                    perror("child not exited normally");
                    exit(0);
                }
                int exitStatus = WEXITSTATUS(stats);
                switch (exitStatus)
                {
                    case COMMAND_LOGN: 
                    {
                        char loginResponse;
                        close(loginResponsPipe[1]);
                        read(loginResponsPipe[0], &loginResponse, 1);
                        switch (loginResponse - '0')
                        {
                            case 0:
                                printf("\nLoggin failed, please try aggain \n\n");
                                loggedIn = false;
                                break;
                            case 1: 
                                printf("\nYou successfully logged in, now you can use other commands \n\n");
                                loggedIn = true;
                                break;
                            case 2:
                                printf("\nYou are allready logged in \n\n");
                                break;
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
                        read(socketPair[0], myfindResponse, lenght);
                        printf("\n%s\n", myfindResponse); 
                        close(socketPair[0]);
                        break;
                    }
                    case COMMAND_MYSTAT:
                    {
                        char myStatResponse[1024] = "";                       
                        int lenght = getResponseLenghtFromPrefix(fifo);
                        read(fifo, myStatResponse, lenght);
                        close(fifo);
                        printf("\n%s\n", myStatResponse);
                        break;
                    }
                    case COMMAND_LOGGOUT: 
                    {
                        if(loggedIn)
                        {
                            printf("\nYou logged out\n\n");
                            loggedIn = false;
                            break;
                        }
                        {
                            printf("\nYou haven't logged in\n\n");
                            break;
                        }
                        break;
                    }
                    case COMMAND_QUIT:
                    {
                        printf("\nQuitting...\n\n");
                        exit(0);
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
                        bool login = false;
                        close(loginResponsPipe[0]);
                        if(loggedIn == false)
                        {
                            login = logginWithUsername(argument);                       
                            write(loginResponsPipe[1], (login ? "1" : "0"), 1);
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
                        char *response = findStatsOfFile(argument, true, loggedIn);
                        char *prefixedResponse = buildResponsePrefixedByLenght(response);
                        write(socketPair[1], prefixedResponse, strlen(prefixedResponse));
                        close(socketPair[1]);
                        exit(COMMAND_MYFIND);
                    }
                    case COMMAND_MYSTAT:
                    {
                        char *response = findStatsOfFile(argument, false, loggedIn);
                        mkfifo(FIFO_PATH, 0666);
                        int fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
                        char *prefixedResponse = buildResponsePrefixedByLenght(response);
                        write(fd, prefixedResponse, strlen(prefixedResponse));
                        close(fd);
                        exit(COMMAND_MYSTAT);
                    }
                    case COMMAND_QUIT:
                    {
                        exit(COMMAND_QUIT);
                    }
                    case COMMAND_LOGGOUT:
                    {
                        exit(COMMAND_LOGGOUT);                        
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