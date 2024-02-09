//-----------------------------------------------
#include "visualizeNGnet.h"

//-----------------------------------------------
int main() {
    int TotalGaussianNumber = 54;       //  Number of Gassian basis functions.
    double r = 2.0 * 1e-3;              // corvariance
    string fn_myu = "myu.csv";
    string fn_vtk = "Gauusian.vtk";
    
    visualizeNGnet visualizeNGnet(TotalGaussianNumber, r);
    visualizeNGnet.make_viewer(fn_myu.c_str(), fn_vtk.c_str());
    
    return 0;
}
