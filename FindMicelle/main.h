#ifndef MAIN_H
#define MAIN_H


struct atom
{
	int  id = 0;
	
	float xCorr = 0;
	float yCorr = 0;
	float zCorr = 0;
	
	std::string beadType = "";
};

struct cluster 
{
	int name     = 0; 
	int mainHy   = 0; 
	int mainCh   = 0; 
	int soluteHy = 0; 
	int soluteCh = 0; 
	
	std::vector <atom> content = {};
	
	std::string dataXyz      = ""; 
	std::string dataIndexDat = ""; 
	std::string dataIndexNdx = ""; 
};

struct mySystem 
{
	int mainBead  = 0;
	int mainChaB  = 0; 
	int mainChain = 0; 
	int soluteBead  = 0;
	int soluteChain = 0;
	int soluteChaB  = 0;
	int counterMainChain   = 0;
	int counterSoluteChain = 0;
	int surfacIndexSize    = 0;
	int chaB  = 0;
	int size  = 0;
	int cumm0 = 0;
	int cumm1 = 0;
	int cumm2 = 0;
	int cumm3 = 0;
	int cumm4 = 0;
	int cumm5 = 0;
	int countColumnIndex   = 0;
	int micelleNumber      = 0;
	int fileHeader         = 2;
	int countAllBead       = 0;
	int countCtaFat        = 0;
	int countLineInFile    = 0;
	int cutOffMicelleSize  = 100;
	int indexFileCol       = 15;
	
	float xBoxLength = 0;
	float yBoxLength = 0;
	float zBoxLength = 0;
	
	double r = 0;
	double rCut = 8.5*8.5;
	
	std::string allClusterInfo = "";
	std::string mainTop   = "";
	std::string soluteTop = "";
	std::string ReadFolder  = "";
	std::string WriteFolder = "";
	std::string rValue      = "";
	
	std::vector <atom> AllAtoms = {};
	
	std::vector <cluster> AllClusters = {};
	
	std::vector <int> VarSurfacIndex;
}; 





#endif
