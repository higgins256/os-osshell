#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
#include <list>

void allocateArrayOfCharArrays(char ***array_ptr, size_t array_length, size_t item_size);
void freeArrayOfCharArrays(char **array, size_t array_length);
void splitString(std::string text, char d, char **result);
std::string findExecutables(char **path_list, std::string command);

int main (int argc, char **argv)
{

    // Get list of paths to binary executables
    // `os_path_list` supports up to 16 directories in PATH, 
    //     each with a directory name length of up to 64 characters
    char **os_path_list;
    allocateArrayOfCharArrays(&os_path_list, 16, 64);
    char* os_path = getenv("PATH");
    splitString(os_path, ':', os_path_list);

    //welcome message
    printf("Welcome to the OSShell! Please enter you commands ('exit' to quite).\n");

    // Example code for how to loop over NULL terminated list of strings

    std::string choice;
    int i = 0;
    int trueCount = 0;

    while (os_path_list[i] != NULL)
    {
        if(os_path_list[i] == choice )
        {
            trueCount++;
        }
        i++;
    }

    //user input to determine wich path they are taking
    std::vector<std::string> history;
    int counter = 0;
    int size = 128;

    while(counter <= size)
    {
      std::cin >> choice;
      history.push_back(choice);
      if(choice == "history")
      {
          for(int i = 0; i < history.size(); i++)
             std::cout << history.at(i) << "\n";
      }
      else if(choice == "exit")
      {
         exit(0);
      }
      else
      {
        std::string fullPath = findExecutables(os_path_list, choice);
        std::cout << fullPath;
      }
      counter++;
    }

    // Allocate space for input command lists
    // `command_list` supports up to 32 command line parameters, 
    //     each with a parameter string length of up to 128 characters
    char **command_list;
    allocateArrayOfCharArrays(&command_list, 32, 128);

    // Repeat:
    //  Print prompt for user input: "osshell> " (no newline)
    //  Get user input for next command
    //  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error command not found" (do include newline)

    // Free allocated memory
    freeArrayOfCharArrays(os_path_list, 16);
    freeArrayOfCharArrays(command_list, 32);

    return 0;
}

void print(std::list<std::string> const &list)
{
   for(auto const& i: list){
         std::cout << i << "\n";
   }

}

std::string findExecutables(char **path_list, std::string command)
{
    int i = 0;
    std::string returnString = "";
    while(path_list[i] != NULL)
    {
        if(path_list[i] == command)
        {
             returnString = returnString + command;
             // what do we do with fork and execute fork();
             // how do I use the getenv()
             //execv(path_list, command);
        }
        if(path_list[i] != command)
        {
             returnString = "<" + command +">: Error command not found";
        }

    }
    return returnString;
}




/*
   array_ptr: pointer to list of strings to be allocated
   array_length: number of strings to allocate space for in the list
   item_size: length of each string to allocate space for
*/
void allocateArrayOfCharArrays(char ***array_ptr, size_t array_length, size_t item_size)
{
    int i;
    *array_ptr = new char*[array_length];
    for (i = 0; i < array_length; i++)
    {
        (*array_ptr)[i] = new char[item_size];
    }
}

/*
   array: list of strings to be freed
   array_length: number of strings in the list to free
*/
void freeArrayOfCharArrays(char **array, size_t array_length)
{
    int i;
    for (i = 0; i < array_length; i++)
    {
        delete[] array[i];
    }
    delete[] array;
}

/*
   text: string to split
   d: character delimiter to split `text` on
   result: NULL terminated list of strings (char **) - result will be stored here
*/
void splitString(std::string text, char d, char **result)
{
    int i;
    std::vector<std::string> list;
    std::stringstream ss(text);
    std::string token;
    
    while (std::getline(ss, token, d))
    {
        list.push_back(token);
    }

    for (i = 0; i < list.size(); i++)
    {
        strcpy(result[i], list[i].c_str());
    }
    result[list.size()] = NULL;
}
