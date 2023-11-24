#include <cereal/archives/binary.hpp>
#include <fstream>
#include "task.h"  // Include your Task class definition

int main() {
    // Create an instance of the Task class
    tasks::Task myTask;
//    myTask.createTask("Church Task", "This is a sample task", priority::HIGH, "2023-12-31");
//    myTask.createTask("School Task", "This is a sample task", priority::HIGH, "2023-12-31");
//    myTask.createTask("Work Task", "This is a sample task", priority::HIGH, "2023-12-31");
//    myTask.createTask("Home Task", "This is a sample task", priority::HIGH, "2023-12-31");
    myTask.getTask(2);
    myTask.updateTaskStatus(2,status::IN_PROGRESS);
    myTask.updateTaskPriority(2,priority::LOW);
    myTask.getTask(2);
//    myTask.deleteTask(2);
    return 0;
}
