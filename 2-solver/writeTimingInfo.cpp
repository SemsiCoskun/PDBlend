#include "writeTimingInfo.h"

using namespace std;

void writeTimingInfo
(
    const std::string outputPath, const int64_t& duration
) 
{
    // Duration is in milliseconds
    int hours = (duration / (1000*60*60)) % 24;
    int minutes = (duration / (1000*60)) % 60;
    int seconds = (duration / 1000) % 60;
    
    std::cout << std::endl;
    std::cout << "Time elapsed is " << duration << " milliseconds." << std::endl;
    std::cout << hours << " hours, " << minutes << " minutes, and " << seconds << " seconds." << std::endl;
    
    std::ofstream fileEnd(outputPath + "runEnd-Solver.txt");
    if (!fileEnd.is_open()) 
    {
        std::cerr << "Failed to open file for writing timing information" << std::endl;
        return;
    }
    
    fileEnd << "Time elapsed is " << duration << " milliseconds." << std::endl;
    fileEnd << hours << " hours, " << minutes << " minutes, and " << seconds << " seconds." << std::endl;
    fileEnd.close();
}
