/*
Simple, fast and lightweight (runs about 10,000 lines in a quarter second) .env file loader written in C++ for C++.
This was inspired by others single files libraries and Python's load_dotenv() function from dotenv.

HOW TO USE:
just use load_dotenv(FilePath) and you are done loading the variables.
*/

#ifndef DOTENV_H
#define DOTENV_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <ranges>

inline void load_dotenv(std::string FilePath = ".env") {
    std::ifstream File(FilePath);
    if (!File.is_open()) {
        std::cerr << "ERROR: Could not find file." << '\n';
        return;
    }
    std::string Line;
    std::vector<std::string> DataArray;
    while (std::getline(File, Line)) {
        Line.erase(std::remove(Line.begin(), Line.end(), ' '), Line.end());
        size_t Pos = Line.find('#');
        if (Pos != std::string::npos) {
            Line = Line.substr(0, Pos);
        }
        if (Line.empty()) {
            continue;
        }
        DataArray.clear();
        for (auto&& part : Line | std::views::split('=')) {
            DataArray.push_back(std::string(part.begin(), part.end()));
        }
        #if defined(_WIN32) || defined(_WIN64)
                _putenv_s(DataArray[0].c_str(), DataArray[1].c_str());
        #elif defined(__linux__)
                setenv(DataArray[0].c_str(), DataArray[1].c_str(), 1);
        #elif defined(__APPLE__) || defined(__MACH__)
                setenv(DataArray[0].c_str(), DataArray[1].c_str(), 1);
        #else
                std::cerr << "ERROR: Unkown OS." << '\n';
                File.close();
                return;
        #endif
    }
    File.close();
}

#endif // DOTENV_H

