#include "magic.h"

class visualizeNGnet {
private:
    int TotalGaussianNumber;
    double r;

public:
    visualizeNGnet();
    visualizeNGnet(int TotalGaussianNumber, double r);
    ~visualizeNGnet();
    
    void read_csv(const char* fn, int num_row, int num_col, double** data);
    void Paraview_Materialconf(const char* fn, int TotalNodeNumber, int TotalElementNumber, double** NodeData, int** ElementData);
    void make_viewer(const char* myuFile, const char* vtkFile);
};