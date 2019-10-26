
#include "headers.h"

struct command
{
   int type;
   char *argument;
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
    return requiredCommand;
}