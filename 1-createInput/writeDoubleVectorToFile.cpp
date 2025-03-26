#include "writeDoubleVectorToFile.h"


void writeDoubleVectorToFile
(
    const std::vector<double>& vec, const std::string& filename
) 
{
    // Open the file in write mode
    std::ofstream file(filename);

    // Check if the file opened successfully
    if (!file.is_open()) 
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Write each element in the vector to a new line in the file
    for (const auto& value : vec) 
    {
        file << std::setprecision(6) << value << "\n";
    }

    // Close the file
    file.close();
    std::cout << "Data written to " << filename << " successfully." << std::endl;
}