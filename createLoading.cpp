#include "createLoading.h"

void createLoading
(
    const double& finalLoad, const int& totalLoadingStep,
    std::vector<double>& appliedLoading
)
{
    if (numCycles > 0.1) // cyclic loading via linear functions
    {
        double period = totalLoadingStep / numCycles;
        int cycleNum = static_cast<int>(ceil(numCycles));
        std::vector<double> loadStart(cycleNum, 0.0); // start loadings for each cycle
        std::vector<double> loadEnd(cycleNum, 0.0);   // end loadings for each cycle
        std::vector<double> loadMax(cycleNum, 0.0);   // max loadings for each cycle
        // The unloading points can be gathered from the experimental loading protocol
        loadMax[0] = 0.09e-3;
        loadMax[1] = 0.135e-3;
        loadMax[2] = 0.17e-3;
        loadMax[3] = 0.21e-3;
        loadMax[4] = 0.25e-3;
        loadMax[cycleNum - 1] = finalDisplacement;
        //
        for (int i = 0; i < totalNumLoadStep; i++)
        {
            int currentCycle = (static_cast<double> (i)) / period;
            double startTimeStep = currentCycle * period;

            double startingLoad = loadStart[currentCycle];
            double endingLoad = loadEnd[currentCycle];
            double maximumLoad = loadMax[currentCycle];

            if (i <= startTimeStep + 0.5*period) // loading part
            {
                appliedLoading[i] = 1.0e-6*finalLoad + startingLoad + (i - startTimeStep) * (maximumLoad - startingLoad) / (0.5*period);
            } else
            {
                appliedLoading[i] = 1.0e-6*finalLoad + maximumLoad - (i - startTimeStep - 0.5*period) * (maximumLoad - endingLoad) / (0.5*period);
            }        
        }
    } else // linearly increasing load
    {
        for (int i = 0; i < totalLoadingStep; i++)
        {

            appliedLoading[i] = finalLoad * (static_cast<double> (i)) / (static_cast<double> (totalLoadingStep)); 
        }
    }
    // end
}