#include "writeTimingInfo.h"

using namespace std;

void writeTimingInfo
(
    const std::string outputPath, const std::string fileName, const int64_t& duration
) 
{
    // Duration is in milliseconds
    int hours = (duration / (1000*60*60)) % 24;
    int minutes = (duration / (1000*60)) % 60;
    int seconds = (duration / 1000) % 60;
    
    std::cout << std::endl;
    std::cout << "Time elapsed is " << duration << " milliseconds." << std::endl;
    std::cout << hours << " hours, " << minutes << " minutes, and " << seconds << " seconds." << std::endl;
    

    std::string fullPath = outputPath + fileName;

    std::ofstream file(fullPath);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Failed to open file for writing: " + fullPath);
    }

    file << "Time elapsed is " << duration << " milliseconds." << std::endl;
    file << hours << " hours, " << minutes << " minutes, and " << seconds << " seconds." << std::endl;
    file.close();
}
