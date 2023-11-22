//
// Created by pepos on 11/19/2023.
//
#include "task.h"
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <utility>
#include <iostream>
#include <iomanip>

namespace tasks {
    const std::string binaryTaskFile = "tasks.dat";
    const std::string indexTaskFile = "tasks_index.dat";
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



    void Task::createTask(std::string title, std::string description, enum priority priority, std::string due_date) {
        // Use 'this' to refer to the current instance
        *this = Task(std::move(title), std::move(description), priority, std::move(due_date));

    std::fstream indexFile(indexTaskFile, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);

    if (indexFile.is_open()) {
        // Get the current position in the main data file (tasks.dat)
        std::streampos position = indexFile.tellg();

        std::ofstream dataFile(binaryTaskFile, std::ios::binary | std::ios::app);

        if (dataFile.is_open()) {
            try {
            // Serialize and write the new task to the main data file
            cereal::BinaryOutputArchive archive(dataFile);
            archive(*this);

            // Update the index file with the position of the new task
            indexFile.write(reinterpret_cast<const char *>(&position), sizeof(std::streampos));
        } catch (const std::exception& e) {
            std::cerr << "Exception caught: " << e.what() << '\n';
        }
            dataFile.close();
            indexFile.close();
        } else {
            std::cerr << "Error: Unable to open the data file for saving tasks.\n";
            indexFile.close();
        }
    } else {
        std::cerr << "Error: Unable to open the index file for reading and writing.\n";
    }

    }

    void Task::getTask(const std::string &title) {
        std::vector<Task> tasks;

            std::ifstream outFile(binaryTaskFile);
            if (outFile.peek() == std::ifstream::traits_type::eof()) {
                std::cerr << "Sorry: You have no tasks.\n";
                return;
            }
            if (outFile.is_open()) {
                try {
                // Create a stringstream to store deserialized data
                std::stringstream iss;
                // Read the file content into the stringstream
                iss << outFile.rdbuf();
                // Deserialize the tasks
                {
                    cereal::BinaryInputArchive archive(iss);
                    while (true) {
                        try {
                            Task task;
                            archive(task);
                            if (task.title == title) {
                                tasks.push_back(task);
                            }
                        } catch (const cereal::Exception &e) {
                            // Break the loop if there's nothing more to read
                            break;
                        }
                    }
                }
            } catch (const cereal::Exception &exception){
                std::cerr << "Exception: " << exception.what() << "\n";
            }
                // Close the file
                outFile.close();
            } else {
                std::cerr << "Error: Unable to open the file for loading tasks.\n";
            }

            for (const auto &el: tasks) {
                std::cout << el << std::endl;
            }

    }

    void Task::updateTask(const std::string &title) {
        // Implementation to update a task by title
    }

    void Task::deleteTask(std::string title) {
        // Implementation to delete a task by title
    }
} // namespace tasks
