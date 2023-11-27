//
// Created by pepos on 11/27/2023.
//

#include "repl.h"
#include <string>
#include <sstream>

void processCommand(const std::string &command, tasks::Task& task){
    std::istringstream iss(command);
    std::string action;
    std::string title;
    std::string description;
    std::string due_date;
    int taskpriority;
    iss>>action;

    if (action == "add" or action == "Add"){
        iss >> title >> description >> taskpriority>>due_date;
        task.createTask(title,description,static_cast<priority>(taskpriority),due_date);
        std::cout<<"List added\n";
    }else if (action == "delete") {
        int taskId;
        iss >> taskId;
        task.deleteTask(taskId);
    }else if (action == "list") {
        task.listTasks();
    } else if (action == "updateStatus") {
        int taskId, newStatus;
        iss >> taskId >> newStatus;
        task.updateTaskStatus(taskId, static_cast<status>(newStatus));
    } else if (action == "exit") {
        // Exit the REPL
        std::cout << "Exiting task manager. Goodbye!\n";
        exit(0);
    } else {
        std::cout << "Unknown command. Type 'help' for a list of commands.\n";
    }
}