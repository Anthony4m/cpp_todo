//
// Created by pepos on 11/22/2023.
//
#include "taskIndex.h"
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
