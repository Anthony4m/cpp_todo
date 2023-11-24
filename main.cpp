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
    myTask.getTask(3);
    return 0;
}
