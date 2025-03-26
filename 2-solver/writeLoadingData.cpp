#include "writeLoadingData.h"

using namespace std;

void writeLoadingData
(
    const std::string outputPath, const double* appliedLoading
) 
{
    std::ofstream fileLoad(outputPath + "apliedLoading.csv");
    if (!fileLoad.is_open()) {
        throw runtime_error("Failed to open file for writing loading data!");
    }
    fileLoad << "LoadStep" << ',' << "DispLoad (mm)" << '\n';
    for (int i = 0; i < totalNumLoadStep; i++) {
        fileLoad << i << ',' << appliedLoading[i]*1000 << "\n";
    }
    fileLoad.close();
}