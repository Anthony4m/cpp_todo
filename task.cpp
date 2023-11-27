//
// Created by pepos on 11/19/2023.
//
#include "task.h"
#include "taskIndex.h"
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <utility>
#include <iostream>
#include <iomanip>

namespace tasks {
    const std::string binaryTaskFile = "tasks.dat";
    const std::string indexTaskFile = "tasks_index.dat";
    const std::string tasksIndexFile = "taskIndex.dat";
    int Task::lastId = 0;

    std::string statusToString(enum status s) {
        switch (s) {
            case status::TODO:
                return "Todo";
            case status::IN_PROGRESS:
                return "In Progress";
            case status::DONE:
                return "Done";
            default:
                return "Unknown";
        }
    }
    std::string priorityToString(enum priority p) {
        switch (p) {
            case priority::LOW:
                return "Low";
            case priority::HIGH:
                return "High";
            case priority::MEDIUM:
                return "Medium";
            default:
                return "Unknown";
        }
    }
    std::string format_time(time_t timestamp) {
        std::tm* localTime = std::localtime(&timestamp);
        char buffer[25];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %I:%M:%S %p", localTime);
        return buffer;
    }

    std::ostream& operator<<(std::ostream& os, const Task& task) {
        static bool headersPrinted = false;

        if (!headersPrinted) {
            os << "+----+------------------+---------------------------+-----------------------------+--------+-----------------+"
               << "\n| #  |       TITLE      | DESCRIPTION           | PRIORITY | STATUS | DUE DATE | CREATION DATE             "
               << "\n+----+------------------+----------------------------+-----------------------------+---------+----------------";
            headersPrinted = true;
        }

        os << "\n| " << std::setw(3) << task.id
           << " | " << std::setw(16) << task.title
           << " | " << std::setw(12) << task.description
           << " | " << std::setw(6) << priorityToString(task.priority)
           << " | " << std::setw(6) << statusToString(task.status)
           << " | " << std::setw(5) << task.due_date
           << " | " << std::setw(20) << format_time(task.creationDate)
           << " |";

        return os;
    }
    void Task::updateIndex() {
        TaskIndex index;
        index.loadTaskIndex(const_cast<std::string &>(tasksIndexFile));
        std::fstream dataFile(binaryTaskFile, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);
        if (dataFile.is_open()){
            // Get the current position in the main data file (tasks.dat)
            std::streampos position = dataFile.tellg();
            index.indexMap[id] = position;
            index.saveTaskIndex(const_cast<std::string &>(tasksIndexFile));
        }

    }

    void Task::createTask(std::string title, std::string description, enum priority, std::string due_date) {
        // Use 'this' to refer to the current instance
        *this = Task(std::move(title), std::move(description), priority, std::move(due_date));

        try {
            // Open the index file for reading and writing
            std::fstream indexFile(tasksIndexFile, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);

            if (indexFile.is_open()) {
                // Get the current position in the main data file (tasks.dat)
                std::streampos position = indexFile.tellg();

                // Open the main data file for writing in binary mode
                std::ofstream dataFile(binaryTaskFile, std::ios::binary | std::ios::app);

                if (dataFile.is_open()) {
                    cereal::BinaryOutputArchive archive(dataFile);
                    archive(*this);

                    // Update the index file with the position of the new task
                    indexFile.write(reinterpret_cast<const char*>(&position), sizeof(std::streampos));

                    // Close the files
                    dataFile.close();
                    indexFile.close();

                    // Update the index with the current task position
                    updateIndex();
                    std::cout<<"Task created\n";
                } else {
                    std::cerr << "Error: Unable to open the data file for saving tasks.\n";
                    indexFile.close();
                }
            } else {
                std::cerr << "Error: Unable to open the index file for reading and writing.\n";
            }
        } catch (const std::string& e) {
            // catch and handle the exception
            std::cout << "String exception raised!" << '\n';
            std::cout << "The exception has a value of: " << e << '\n';
        }
    }


    void Task::getTask(const int taskId) {

        TaskIndex index;
        // Load the index from the file
        index.loadTaskIndex(const_cast<std::string &>(tasksIndexFile));

        // Check if the task with the given title exists in the index
        auto it = index.indexMap.find(taskId);
        if (it != index.indexMap.end()) {

            std::ifstream dataFile(binaryTaskFile, std::ios::binary);

            if (dataFile.is_open()) {
                try {
                    // Set the file position to the stored position in the index
                    dataFile.seekg(it->second);

                    // Deserialize the task
                    cereal::BinaryInputArchive archive(dataFile);
                    Task task;
                    archive(task);

                    dataFile.close();
                    std::cout << task << std::endl;
                }catch (cereal::Exception exception){
                    std::cout<<exception.what();
                }
            } else {
                std::cerr << "Error: Unable to open the data file for loading tasks.\n";
            }
        } else {
            std::cerr << "Error: Task with title '" << title << "' not found.\n";
        }
    }


    void Task::updateTaskStatus(int taskId, enum status newStatus) {
        TaskIndex index;
        index.loadTaskIndex(const_cast<std::string &>(tasksIndexFile));
        // Check if the task with the given title exists in the index
        auto it = index.indexMap.find(taskId);

        if (it != index.indexMap.end()) {
            // Open the main data file for reading and writing
            std::fstream dataFile(binaryTaskFile, std::ios::binary | std::ios::in | std::ios::out);

            if (dataFile.is_open()) {
                // Set the file position to the stored position in the index
                dataFile.seekp(it->second);
                // Deserialize the task
                cereal::BinaryInputArchive archive(dataFile);
                Task task;
                archive(task);
                // Update the task status
                task.status = newStatus;

                // Set the file position for writing
                dataFile.seekp(it->second);

                // Serialize and write the updated task
                cereal::BinaryOutputArchive archived(dataFile);
                archived(task);

                // Close the file
                dataFile.close();
            }
        }
    }
    void Task::updateTaskPriority(int taskId,enum priority newPriority) {
        TaskIndex index;
        index.loadTaskIndex(const_cast<std::string &>(tasksIndexFile));
        auto it = index.indexMap.find(taskId);

        if (it != index.indexMap.end()) {
            std::fstream dataFile(binaryTaskFile, std::ios::binary | std::ios::in | std::ios::out);

            if (dataFile.is_open()) {
                dataFile.seekp(it->second);
                // Deserialize the task
                cereal::BinaryInputArchive archive(dataFile);
                Task task;
                archive(task);
                // Update the task priority
                task.priority = newPriority;

                dataFile.seekp(it->second);

                // Serialize and write the updated task
                cereal::BinaryOutputArchive archived(dataFile);
                archived(task);

                // Close the file
                dataFile.close();
                std::cout<<"Task updated\n";
            }
        }
    }


    void Task::deleteTask(int taskId) {
        TaskIndex index;
        index.loadTaskIndex(const_cast<std::string &>(tasksIndexFile));

        auto it = index.indexMap.find(taskId);
        if (it != index.indexMap.end()) {
            std::fstream dataFile(binaryTaskFile, std::ios::binary | std::ios::in | std::ios::out);

            if (dataFile.is_open()) {
                std::ofstream tempfile("temp.dat", std::ios::binary);

                if (tempfile.is_open()) {
                    while (!dataFile.eof()) {
                        std::streampos currentPosition = dataFile.tellg();

                        Task task;
                        try {
                            cereal::BinaryInputArchive archive(dataFile);
                            archive(task);
                        } catch (const cereal::Exception& exception) {
                            std::cerr << exception.what();
                            break;
                        }

                        if (task.id != taskId) {
                            tempfile.seekp(currentPosition);
                            cereal::BinaryOutputArchive tempArchive(tempfile);
                            tempArchive(task);
                        }
                    }

                    dataFile.close();
                    tempfile.close();

                    std::remove(binaryTaskFile.c_str());
                    std::rename("temp.dat", binaryTaskFile.c_str());

                    index.removeIndex(const_cast<std::string &>(tasksIndexFile), taskId);
                    index.saveTaskIndex(const_cast<std::string &>(tasksIndexFile));

                    std::cout << "Task with ID " << taskId << " has been deleted.\n";
                } else {
                    std::cerr << "Error: Unable to open the temporary file.\n";
                }
            } else {
                std::cerr << "Error: Unable to open the data file for deleting tasks.\n";
            }
        } else {
            std::cerr << "Error: Task with ID " << taskId << " not found.\n";
        }
    }

    void Task::listTasks() {
        std::vector<Task> tasks;

        std::ifstream outFile(binaryTaskFile);
        if (outFile.is_open()) {

            std::stringstream iss;
            iss << outFile.rdbuf();
            {
                cereal::BinaryInputArchive  archive(iss);
                while (true) {
                    try {
                        Task task;
                        archive(task);
                        tasks.push_back(task);
                    } catch (const cereal::Exception &e) {
                        // Break the loop if there's nothing more to read
                        break;
                    }
                }
            }
            // Close the file
            outFile.close();
        }else {
            std::cerr << "Error: Unable to open the file for loading tasks.\n";
        }

        for(const auto &el: tasks){
            std::cout<<el<<std::endl;
        }
        }


} // namespace tasks
