#include <cereal/archives/binary.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include "task.h"
#include "repl.h"

int main() {
    // Create an instance of the Task class
    tasks::Task myTask;
    std::cout << "Task Manager REPL. Type 'help' for a list of commands.\n";
    while (true) {
        std::string command;
        std::cout << "> ";
        std::getline(std::cin, command);

        // Process the command
        processCommand(command, myTask);
    }
    return 0;
}
