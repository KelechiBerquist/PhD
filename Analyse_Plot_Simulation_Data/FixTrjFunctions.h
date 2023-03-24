#ifndef FIXTRJFUNCTIONS_H
#define FIXTRJFUNCTIONS_H


struct eachBead 
{
	int indx     =   0;
	int type     =   0;
	float rz     =   0;
	float ry     =   0;
	float rx     =   0;

	// std::string type = 0;
};

struct eachDump 
{
	std::unordered_map <int, eachBead > atomOrig;
	std::unordered_map <int, eachBead > atomFix;

	std::vector <float> BoxLength;
	std::vector <float> MinCoord          =   {1000,   1000,  1000};
	std::vector <float> MaxCoord          =   {-1000, -1000, -1000};
	std::vector <std::string> header      =   {};

};


struct eachCluster 
{
	int totalTimeDump                                      =   0;
	int totalBead                                          =   0;
	int micelleTotalBead                                   =   0;
	
	std::vector <eachDump> dump                            =   {};
	std::vector <int> VarSurfac                            =   {};
	std::vector <int> StepAtDump                           =   {};
	std::vector <std::string> Header                       =   {};
};



struct eachLayer 
{
	int chosen                            =   0;
	double rate                           =   0;
	double forRate                        =   0;
	std::string direction                 =   "";
	std::string writeTrj                  =   "";
	std::string writeEnergy               =   "";
	std::string writeTotalEnergy          =   "";
	std::string readFolderName            =   "";
	std::string writeFolderName1          =   "";
	std::string writeFolderName2          =   "";
	std::string readFileNameEnd           =   "";
	std::string writeFileNameEnd          =   "";
	std::string writeMinMax               =   "";
	std::string AllTmsd                   =   "";
	std::vector <int> StepAtDump          =   {};
};


float ShortDist(float x1, float x2, float x3);
void AddFix (int timeDmp, int indx, std::vector <float> Coords,  eachCluster &Item);


void GetEnergy(eachLayer &ThisLayer);
eachCluster GetData(eachLayer &ThisLayer, std::string FileBegin);
std::string WritePreTrj ( std::vector <std::string> HeaderVector, int ClusterSize);

void FindEachMicelle(eachLayer &ThisLayer, eachCluster &ThisClusterW);
void FixOtherStep(eachLayer &ThisLayer, eachCluster &ThisClusterW, std::vector <int> GdCluster);


void FindFixWriteMicelle(eachLayer &ThisLayer, eachCluster &ThisCluster1, eachCluster &ThisCluster2);

std::string WriteTraj (int timeDmp, int indx, eachLayer &ThisLayer, eachCluster &ThisCluster, std::vector <float> Cod);
std::string WriteEnergy (int timeDump, std::vector <float> Coords, std::string atomType);


void FileWriteTraj (eachLayer &ThisLayer);
void FileWriteTsmd (eachCluster &ThisCluster);
void FileWriteEnergy (eachLayer &ThisLayer);

void CalcDiffCoeff(eachCluster &ThisCluster);

#endif


// void GetData(eachCluster &ThisCluster);
