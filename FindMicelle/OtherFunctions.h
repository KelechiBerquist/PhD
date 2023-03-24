// This is start of the header guard.  WRITETOFILE_H can be any unique name.  By convention, we use the name of the header file.
#ifndef OTHERFUNCTIONS_H
#define OTHERFUNCTIONS_H


//1
void AssignmentSequence (mySystem &CurrSystem);


//2
void PopulateBeadTypeLipids (mySystem &CurrSystem);
 
 
//3
void ReadFileSequence (mySystem &CurrSystem, std::string line);


//4
void DataForWriting(mySystem &CurrSystem, int fix, cluster &tempCluster);


//5
void PlaceBead (mySystem &CurrSystem, int fix, cluster &tempCluster);


//6
void FixCharged (mySystem &CurrSystem, int fix, cluster &tempCluster);


//7
void FindMicelleAlgorithm (mySystem &CurrSystem);


//8
void SortClusters (mySystem &CurrSystem);


//9
void WriteToFiles (mySystem &CurrSystem); 
 
#endif
