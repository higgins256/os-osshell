#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
#include <list>

void splitString(std::string text, char d, std::vector<std::string>& result);
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result);
void freeArrayOfCharArrays(char **array, size_t array_length);
std::string findExecutables(std::vector<std::string> path_list, std::string command);

int main (int argc, char **argv)
{

    // Get list of paths to binary executables
    std::vector<std::string> os_path_list;
    char* os_path = getenv("PATH");
    splitString(os_path, ':', os_path_list);

    
    //user input to determine wich path they are taking
    std::string choice;
    std::vector<std::string> history;
    int counter = 0;
    int size = 128;
    printf("Welcome to OSShell! Please enter your commands('exit to quit)\n");
    while(counter <= size)
    {
      std::cin >> choice;
      history.push_back(choice);
      if(choice == "history")
      {
          for(int i = 0; i < history.size(); i++)
             std::cout << history.at(i) << "\n";
      }
      else if(choice == "clear")
      {
          history.clear();
      }
      else if(choice == "exit")
      {
         exit(0);
      }
      else
      {
        std::string fullPath = findExecutables(os_path_list, choice);
        if(fullPath == "")
        {
           std::cout << "<" + choice + ">: Error command not found\n";
        }
        else
        {
           fork();
           const char *charPath = fullPath.c_str();
           const char **argv = new const char* [os_path_list.size()+1];
           argv[0] = charPath;
           for(int j = 0; j < os_path_list.size()+1; j++)
           {
              argv[j+1] = os_path_list[j].c_str();
           }
           execv(charPath, (char **)argv);

           std::cout << "Please enter in another command\n";
        }
        std::cout << "Please enter in another command\n";
      }
      counter++;
    }

    std::vector<std::string> command_list; // to store command user types in, split into its variour parameters
    char **command_list_exec; // command_list converted to an array of character arrays
    // Repeat:
    //  Print prompt for user input: "osshell> " (no newline)
    //  Get user input for next command
    //  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error command not found" (do include newline)


    /************************************************************************************
     *   Example code - remove in actual program                                        *
     ************************************************************************************/
    // Shows how to split a command and prepare for the execv() function
    std::string example_command = "ls -lh";
    splitString(example_command, ' ', command_list);
    vectorOfStringsToArrayOfCharArrays(command_list, &command_list_exec);
    // use `command_list_exec` in the execv() function rather than looping and printing
    int i = 0;
    while (command_list_exec[i] != NULL)
    {
        printf("CMD[%2d]: %s\n", i, command_list_exec[i]);
        i++;
    }
    // free memory for `command_list_exec`
    freeArrayOfCharArrays(command_list_exec, command_list.size() + 1);
    printf("------\n");

    // Second example command - reuse the `command_list` and `command_list_exec` variables
    example_command = "echo \"Hello world\" I am alive!";
    splitString(example_command, ' ', command_list);
    vectorOfStringsToArrayOfCharArrays(command_list, &command_list_exec);
    // use `command_list_exec` in the execv() function rather than looping and printing
    i = 0;
    while (command_list_exec[i] != NULL)
    {
        printf("CMD[%2d]: %s\n", i, command_list_exec[i]);
        i++;
    }
    // free memory for `command_list_exec`
    freeArrayOfCharArrays(command_list_exec, command_list.size() + 1);
    printf("------\n");
    /************************************************************************************
     *   End example code                                                               *
     ************************************************************************************/


    return 0;
}

void print(std::list<std::string> const &list)
{
   for(auto const& i: list){
         std::cout << i << "\n";
   }

}

std::string findExecutables(std::vector<std::string> path_list, std::string command)
{
    int i = 0;
    std::string returnString = "";
    for(i = 0; i < path_list.size(); i++)
    {
        std::string fullPath = "";
        fullPath = path_list[i] + "/" + command;
        FILE * pfile;
        if(pfile = fopen(fullPath.c_str(), "r"))
        {
             fclose(pfile);
             return fullPath;
        }

    }
    return returnString;
}




/*
   text: string to split
   d: character delimiter to split `text` on
   result: vector of strings - result will be stored here
*/
void splitString(std::string text, char d, std::vector<std::string>& result)
{
    enum states { NONE, IN_WORD, IN_STRING } state = NONE;

    int i;
    std::string token;
    result.clear();
    for (i = 0; i < text.length(); i++)
    {
        char c = text[i];
        switch (state) {
            case NONE:
                if (c != d)
                {
                    if (c == '\"')
                    {
                        state = IN_STRING;
                        token = "";
                    }
                    else
                    {
                        state = IN_WORD;
                        token = c;
                    }
                }
                break;
            case IN_WORD:
                if (c == d)
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
            case IN_STRING:
                if (c == '\"')
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
        }
    }
    if (state != NONE)
    {
        result.push_back(token);
    }
}

/*
   list: vector of strings to convert to an array of character arrays
   result: pointer to an array of character arrays when the vector of strings is copied to
*/
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result)
{
    int i;
    int result_length = list.size() + 1;
    *result = new char*[result_length];
    for (i = 0; i < list.size(); i++)
    {
        (*result)[i] = new char[list[i].length() + 1];
        strcpy((*result)[i], list[i].c_str());
    }
    (*result)[list.size()] = NULL;
}

/*
   array: list of strings (array of character arrays) to be freed
   array_length: number of strings in the list to free
*/
void freeArrayOfCharArrays(char **array, size_t array_length)
{
    int i;
    for (i = 0; i < array_length; i++)
    {
        if (array[i] != NULL)
        {
            delete[] array[i];
        }
    }
    delete[] array;
}
