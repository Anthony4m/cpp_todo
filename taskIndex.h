#ifndef TASKS_TASKINDEX_H
#define TASKS_TASKINDEX_H

#include <iostream>
#include <map>
#include <cereal/types/map.hpp>

class TaskIndex {
public:
    std::map<int, std::streampos> indexMap;

    void saveTaskIndex(std::string& filename);
    void loadTaskIndex(std::string& filename);
    void removeIndex(std::string& filename, int taskId);
};

#endif //TASKS_TASKINDEX_H
