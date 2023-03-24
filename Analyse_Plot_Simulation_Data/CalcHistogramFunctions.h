#ifndef CALCHISTOGRAMFUNCTIONS_H
#define CALCHISTOGRAMFUNCTIONS_H


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
	int stepAtDump    =   0;
	std::vector <eachBead> atom;
};

struct eachGroup 
{
	int totalBead    =   0;
	std::vector <eachDump> dump;
};

struct eachOrderedDump 
{
	int stepAtDump    =   0;
	std::unordered_map <int, eachBead> atom;
};


struct eachOrderedGroup 
{
	int totalBead    =   0;
	std::vector <eachOrderedDump> dump;
};


struct eachBin 
{
	float upperLimit     =   0;
	float lowerLimit     =   0;
	std::vector <float> freq;
};

struct eachType 
{
	int bead     =   0;
	std::vector <eachBin> bin;
};

struct eachHist 
{
	std::vector <eachType> type;
	float binWidth     =   0;
	int   binNumber    =   0;
	float startBin     =   0;
	float stopBin      =   0;
};


struct eachLayer 
{
	int chosen                            =   0;
	double rate                           =   0;
	double forRate                        =   0;
	double skipStrain                     =   0;
	double skipCheckFactor                =   0; //Calculated to account for different dump rates.
	std::string direction                 =   "";
	std::string writeTrj                  =   "";
	std::string writeEnergy               =   "";
	std::string writeTotalEnergy          =   "";
	std::string AllTmsd                   =   "";
	std::string AllStrain                 =   "";
	std::string BeadTypeForTmsd           =   "";
	std::string readFolderName            =   "";
	std::string writeFolderName1          =   "";
	std::string writeFolderName2          =   "";
	std::string writeFolderName3          =   "";
	std::string writeFolderName4          =   "";
	std::string readFileNameEnd           =   "";
	std::string writeFileNameEnd          =   "";
	std::vector <int> StepAtDump          =   {};
};


struct trackStrain 
{
	float ix        =   0;
	float iy        =   0;
	float iz        =   0;
	float fx        =   0;
	float fy        =   0;
	float fz        =   0;
	double dist     =   0;
};


eachGroup GetTraj(eachLayer &ThisLayer,  std::string Mid);
eachOrderedGroup GetOrderedTraj(eachLayer &ThisLayer,  std::string Mid);
void GetHistogram(eachLayer ThisLayer,  eachGroup Vec1,  eachGroup Vec2);
void ReadHistogram(int tDiff,  eachLayer ThisLayer,  eachHist MiniHistogram);
void CalcTsmd(eachLayer &ThisLayer,  eachOrderedGroup Vec1);
void FileWriteTsmd (eachLayer ThisLayer);

void GetStrain(eachLayer &ThisLayer,  eachGroup Vec1);
void FileWriteStrain (eachLayer ThisLayer);



#endif


// void GetHistogram(eachLayer ThisLayer,  eachGroup Vec1,  eachGroup Vec2);

// void ReadHistogram(int tDiff,   eachLayer ThisLayer,  std::map <int, std::map <float, std::vector <float> > > TheHistogram);


// void CalcDiffCoeff(eachLayer ThisLayer,  std::vector < std::map <int, std::vector <float> > > Vec1, std::vector <int> MiceIndex);

// void FileWriteTsmd (eachLayer &ThisLayer);


// std::vector < std::map <int, std::vector <float> > > GetTraj(eachLayer ThisLayer,  std::string Mid,  std::map <int, int> &BdType, std::vector <int> &ForIndex);

// void GetHistogram(eachLayer ThisLayer,  std::vector < std::map <int, std::vector <float> > > Vec1,  std::vector < std::map <int, std::vector <float> > > Vec2,  std::map <int, int> Type, std::vector <int> ThisIndex);

// void ReadHistogram(int tDiff,   eachLayer ThisLayer,  std::map <int, std::map <float, std::vector <float> > > TheHistogram);


// void CalcDiffCoeff(eachLayer ThisLayer,  std::vector < std::map <int, std::vector <float> > > Vec1, std::vector <int> MiceIndex);

// void FileWriteTsmd (eachLayer &ThisLayer);

// std::vector < std::map <int, std::vector <float> > > GetTraj(eachLayer ThisLayer,  std::string Mid, std::vector < std::map <int, std::vector <float> > > &BigVec);

// std::vector <std::map <std::string, std::map <float, std::vector <float> > > > GetHistogram(std::vector < std::map <int, std::vector <float> > > Vec1,  std::vector < std::map <int, std::vector <float> > > Vec2,  std::map <int, int> Type);

// void GetHistogram(eachLayer ThisLayer,  std::vector < std::map <int, std::vector <float> > > Vec1,  std::vector < std::map <int, std::vector <float> > > Vec2,  std::map <int, int> Type);
// void ReadHistogram(eachLayer ThisLayer,  std::vector <std::map <int, std::map <float, std::vector <float> > > > TheHistogram);

