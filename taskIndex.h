//
// Created by pepos on 11/22/2023.
//

#ifndef TASKS_TASKINDEX_H
#define TASKS_TASKINDEX_H
#include <iostream>
#include <map>
class TaskIndex{
public:
    std::map<int ,std::streampos> indexMap;
    void saveTaskIndex(std::string& filename);
    void loadTaskIndex(std::string& filename);
};
#endif //TASKS_TASKINDEX_H
