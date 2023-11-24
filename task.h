//
// Created by pepos on 11/19/2023.
//
#pragma once
#include <ctime>
#include <string>
#include <utility>
#include "priority.h"
#include "status.h"
#include <iostream>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>

namespace tasks {
    class Task {
    private:
        static int lastId;
        int id;
        std::string title;
        std::string description;
        priority priority;
        status status;
        std::string due_date;
        time_t creationDate = time(0);

    public:
        // Default constructor
        Task() : id(++lastId), status(status::TODO), creationDate(time(0)) {}

        // Constructor for creating a task with specific details
        Task(std::string taskTitle, std::string taskDescription, enum priority taskPriority, std::string taskDueDate)
                : id(++lastId), title(std::move(taskTitle)), description(std::move(taskDescription)),
                  status(status::TODO), priority(taskPriority), due_date(std::move(taskDueDate)),
                  creationDate(time(0)) {}

        // Declare cereal serialization function as a friend
        friend class cereal::access;


        template <class Archive>
        void serialize(Archive &archive) {
            archive(id, title, description, priority, status, due_date, creationDate);
        }


        // Declare the << operator overload as a friend
        friend std::ostream& operator<<(std::ostream& os, const Task& task);
        void updateIndex();
        void createTask(std::string title, std::string description, enum priority priority, std::string due_date);
        void getTask(int taskId);
        void updateTaskStatus(int taskId,enum status newStatus);
        void updateTaskPriority(int taskId,enum priority newPriority);
        static void deleteTask(int taskId);
    };
} // namespace tasks
