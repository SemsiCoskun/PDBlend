#include "readDataIntoIntArray.h"


void readDataIntoIntArray
(
    const std::string& filename, const int& size, int* array
) 
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    int index = 0;
    while (std::getline(file, line) && index < size) 
    {
        try {
            array[index] = std::stoi(line);  // Convert line to integer and store in array
            ++index;
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid number format in file at line " << index + 1 << std::endl;
            array[index] = 0.0;  // Optionally set a default value on error
        }
    }

    file.close();
}