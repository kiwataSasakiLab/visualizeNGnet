#include "visualizeNGnet.h"

visualizeNGnet::visualizeNGnet() {
    this->TotalGaussianNumber = 0;
    this->r = 0.0;
    cout << "Error: You need to set TotalGaussainNumber and covariance value " << endl;
    exit(EXIT_FAILURE);
}

visualizeNGnet::visualizeNGnet(int TotalGaussianNumber, double r) {
    this->TotalGaussianNumber = TotalGaussianNumber;
    this->r = r;
}

visualizeNGnet::~visualizeNGnet(){}

void visualizeNGnet::read_csv(const char* fn, int num_row, int num_col, double** data) {
    FILE* fp;
    fp = fopen(fn, "r");
    if(fp == NULL) {
        cout << "error " << fn << endl;
    }
    else {
        for(int i = 0; i < num_row; i++) {
            for(int j = 0; j < num_col; j++) {
                if(j == num_col-1) {
                    fscanf(fp, "%lf", &data[i][j]);
                }
                else{
                    fscanf(fp, "%lf,", &data[i][j]);
                }
            }
        }
    }
    fclose(fp);
}

void visualizeNGnet::Paraview_Materialconf(const char* fn, int TotalNodeNumber, int TotalElementNumber, double** NodeData, int** ElementData) {
	FILE* fp = fopen(fn, "w");
    if(fp == NULL) {
        cout << "error at opening " << fn << endl;
    }
    else {
        fprintf(fp, "# vtk DataFile Version 2.0\n");
        fprintf(fp, "Title Data\n");
        fprintf(fp, "ASCII\n");
        fprintf(fp, "DATASET UNSTRUCTURED_GRID\n");
        fprintf(fp, "POINTS %d float\n",TotalNodeNumber);
        for(int i=0; i<TotalNodeNumber; i++) {
            fprintf(fp, "%lf %lf %lf\n",NodeData[i][0], NodeData[i][1], 0.0);
        }
        fprintf(fp, "CELLS %d %d\n", TotalElementNumber, 4*TotalElementNumber);
        for(int i=0; i<TotalElementNumber; i++) {
            fprintf(fp, "%d ", 3);
            fprintf(fp, "%d %d %d\n", ElementData[i][1], ElementData[i][2], ElementData[i][3]);
        }
        fprintf(fp, "CELL_TYPES %d\n", TotalElementNumber);
        for(int i = 0; i < TotalElementNumber; i++) {
            fprintf(fp, "5\n");
        }
		fprintf(fp, "CELL_DATA %d\n", TotalElementNumber);
		fprintf(fp, "SCALARS Material int\n");
		fprintf(fp, "LOOKUP_TABLE default\n");
		for(int ele = 0; ele < TotalElementNumber; ele++) {
			fprintf(fp, "%d\n", ElementData[ele][0]);
		}
    }
    fclose(fp);
}

void visualizeNGnet::make_viewer(const char* myuFile, const char* vtkFile) {
    //  myufile must include (N x 2) data such as {{x1, y1},{x2,y2},...,{xN,yN}}.
    double pi = 3.14;
    double** myu = new double*[TotalGaussianNumber]();
    for(int i = 0; i < TotalGaussianNumber; i++) {
        myu[i] = new double[2]();
    }
    read_csv(myuFile, TotalGaussianNumber, 2, myu);

    int pn = 200;                                           //  Number of points per gaussians.
    int TotalNodeNumber     = (1+pn)*TotalGaussianNumber;
    int TotalElementNumber  = pn*TotalGaussianNumber;

    double** NodeData = new double*[TotalNodeNumber]();
    int** ElementData = new int*[TotalElementNumber]();
    for(int i = 0; i < TotalNodeNumber; i++) {
        NodeData[i] = new double[2]();
    }
    for(int i = 0; i < TotalElementNumber; i++) {
        ElementData[i] = new int[4]();
    }
    for(int i = 0; i < TotalGaussianNumber; i++) {
        NodeData[(pn+1)*i][0] = myu[i][0];
        NodeData[(pn+1)*i][1] = myu[i][1];
        for(int j = 1; j < pn+1; j++) {
            NodeData[(pn+1)*i + j][0] = myu[i][0] + r*cos((j-1)*2.0*pi/pn);
            NodeData[(pn+1)*i + j][1] = myu[i][1] + r*sin((j-1)*2.0*pi/pn);
        }
    }
    for(int i = 0; i < TotalGaussianNumber; i++) {
        for(int j = 0; j < pn; j++) {
            if( j == pn-1) {
                ElementData[i*pn + j][0] = i;
                ElementData[i*pn + j][1] = (pn+1)*i;
                ElementData[i*pn + j][2] = (pn+1)*i + (j+1)%(pn+1);
                ElementData[i*pn + j][3] = (pn+1)*i + 1;
            }
            else {
                ElementData[i*pn + j][0] = i;
                ElementData[i*pn + j][1] = (pn+1)*i;
                ElementData[i*pn + j][2] = (pn+1)*i + (j+1)%(pn+1);
                ElementData[i*pn + j][3] = (pn+1)*i + (j+2)%(pn+1);
            }
        }
    }
    Paraview_Materialconf(vtkFile, TotalNodeNumber, TotalElementNumber, NodeData, ElementData);
    
    for(int i = 0; i < TotalGaussianNumber; i++) {
        delete[] myu[i];
    }
    delete[] myu;
    for(int i = 0; i < TotalNodeNumber; i++) {
        delete[] NodeData[i];
    }
    delete[] NodeData;
    for(int i = 0; i < TotalElementNumber; i++) {
        delete[] ElementData[i];
    }
    delete[] ElementData;
}