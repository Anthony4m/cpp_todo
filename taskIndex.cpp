//
// Created by pepos on 11/22/2023.
//
#include "taskIndex.h"
#include "cereal/archives/binary.hpp"
#include <fstream>

void TaskIndex::saveTaskIndex(std::string &filename) {
std::ofstream outFile(filename,std::ios::binary);
    if (outFile.is_open()) {

        for (const auto &entry: indexMap) {
            outFile.write(reinterpret_cast<const char *>(&entry.first), sizeof(int ));
            outFile.write(reinterpret_cast<const char *>(&entry.second), sizeof(std::streampos));
        }
        outFile.close();
    } else{
        std::cerr<<"Error: Unable to open the index file for writing.\n";
    }
}

void TaskIndex::loadTaskIndex(std::string &filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (infile.is_open()){
        while (!infile.eof()){
            int id;
            std::streampos position;

            infile.read(reinterpret_cast<char*>(&id), sizeof (int ));
            infile.read(reinterpret_cast<char*>(&position), sizeof (std::streampos));

            if (infile.eof()){
                break;
            }
            indexMap[id] = position;
        }
        infile.close();
    } else{
        std::cerr<<"Error: Unable to open the index file for reading.\n";
    }
}

void TaskIndex::removeIndex(std::string& filename, int taskId) {
    // Open the file in binary mode
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);

    if (file.is_open()) {
        // Read and modify the index map
        indexMap.erase(taskId);

        // Now, write the modified index map back to the file
        // (Note: This approach assumes that the entire map can fit into memory)
        file.seekp(0);

        // Truncate any remaining content (if the new data is smaller than the old data)
        file.seekp(file.tellp());
        file.write("", 1);  // Write an empty character to truncate the file

        // Close the file
        file.close();
    } else {
        std::cerr << "Error: Unable to open the file for reading and writing.\n";
    }
}







