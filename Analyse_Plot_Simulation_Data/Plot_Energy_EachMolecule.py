import os
import sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
# import matplotlib.pylab as plt
import matplotlib.image as mpimg
import numpy as np
from scipy.stats import linregress



Chosen            =   [1, 2, 3, 4]
Structure         =   ['Sph', 'RLM', 'WLM', 'BWLM']
ClusterSize       =   np.array([4581, 3617,  3787,  3726])
Rate              =   [0.1, 0.3, 1, 3, 10, 30]
SkipStep          =   np.array([6667, 2223, 667, 223, 67, 23])
Direct            =   ["X", "Y"]
ContentFile       =   []
AllStrain         =   np.arange(0, 5, 0.5)
timeIdx           =   0
posIdx            =   1
xIdx              =   2
yIdx              =   3
zIdx              =   4
typeIdx           =   5
totalEnergyIdx    =   6
pairEnergyIdx     =   7
bondEnergyIdx     =   8
angleEnergyIdx    =   9
freq              =   0.1
RlxTime           =   3
legendCols        =   3
fileCols          =   4
dumpTrj           =   0.02
dumpSkip          =   5
timeStep          =   30
nanosecToSec      =   1e9
femtosecToSec     =   1e15
convDeformRate    =   1e-6
rootDir           =   "/media/KaceHDD1/Struct/"
WriteFolderName    =  "/media/KaceHDD1/Struct/ImageVideos/Load_Energy"



Strain                 =   []
TotalEnergy            =   []
PairEnergy             =   []
BondEnergy             =   []
AngleEnergy            =   []



for chosen in Chosen:
	for direct in  Direct:
		for rate in  Rate:
			ReadFolderName        =  "/media/KaceHDD1/Struct/ImageVideos/Load_Energy"
			ReadFileNameEnd       =  "Load-Rate" + str(rate) + "-Spn" + str(chosen) + "-" + direct
			ReadFileName          =  os.path.join(ReadFolderName, "Energy" + ReadFileNameEnd  + ".dat")
			ReadFile              =  open(ReadFileName, 'r')
			ThisFile              =  ReadFile.readlines()
			ReadFile.close()
			ThatStrain            =  []
			ThatTotalEnergy       =  []
			ThatPairEnergy        =  []
			ThatBondEnergy        =  []
			ThatAngleEnergy       =  []

			debug                 =   ""
			debug2                =   ""

			thisTotalEnergy       =   0
			thisPairEnergy        =   0
			thisBondEnergy        =   0
			thisAngleEnergy       =   0
			countLine             =   0


			for line in ThisFile:
				countLine +=1
				# if ( int(line.split()[timeIdx])%(SkipStep[Rate.index(rate)]*dumpSkip) == 0 and line.split()[typeIdx] in ["Bd4", "Bd7"]):
				if ( int(line.split()[timeIdx])%(SkipStep[Rate.index(rate)]*dumpSkip) == 0):
					# debug += ('{:6.2f}'.format(int(line.split()[timeIdx])%(SkipStep[Rate.index(rate)]*dumpSkip)) + " "  + '{:6.2f}'.format(float(line.split()[timeIdx])*timeStep*rate*convDeformRate) + "   " + line)

					thisTotalEnergy   +=   float(line.split()[totalEnergyIdx])
					thisPairEnergy    +=   float(line.split()[pairEnergyIdx])
					thisBondEnergy    +=   float(line.split()[bondEnergyIdx])
					thisAngleEnergy   +=   float(line.split()[angleEnergyIdx])

					
					if (countLine%ClusterSize[Chosen.index(chosen)] == 0):
						ThatStrain.append(round(float(line.split()[timeIdx])*timeStep*rate*convDeformRate,2))
						ThatTotalEnergy.append(0.004184*thisTotalEnergy)
						ThatPairEnergy.append(0.004184*thisPairEnergy)
						ThatBondEnergy.append(0.004184*thisBondEnergy)
						ThatAngleEnergy.append(0.004184*thisAngleEnergy)

						thisTotalEnergy   =   0
						thisPairEnergy    =   0
						thisBondEnergy    =   0
						thisAngleEnergy   =   0

			Strain.append(ThatStrain)
			TotalEnergy.append(ThatTotalEnergy)
			PairEnergy.append(ThatPairEnergy)
			BondEnergy.append(ThatBondEnergy)
			AngleEnergy.append(ThatAngleEnergy)


Strain                 =   np.array(Strain)
TotalEnergy            =   np.array(TotalEnergy)
PairEnergy             =   np.array(PairEnergy)
BondEnergy             =   np.array(BondEnergy)
AngleEnergy            =   np.array(AngleEnergy)

# TotalEnergy            =   0.004184*TotalEnergy
# PairEnergy             =   0.004184*PairEnergy
# BondEnergy             =   0.004184*BondEnergy
# AngleEnergy            =   0.004184*AngleEnergy

print(Strain.shape)

Strain.shape           =   (len(Chosen),  len(Direct),  len(Rate),  -1)
TotalEnergy.shape      =   (len(Chosen),  len(Direct),  len(Rate),  -1)
PairEnergy.shape       =   (len(Chosen),  len(Direct),  len(Rate),  -1)
BondEnergy.shape       =   (len(Chosen),  len(Direct),  len(Rate),  -1)
AngleEnergy.shape      =   (len(Chosen),  len(Direct),  len(Rate),  -1)



# print(Strain[0, 0,0])
# print(TotalEnergy[0, 0,0])
# print(Strain[0, 0,-1])
# print(TotalEnergy[0, 0,-1])
# print(Strain.shape)
# print()
# print()


#For Plotting TotalEnergy Against Strain For Each Rate and Each Direction
for i in range (len(Chosen)):
	for j in range (len(Direct)):
		fig = plt.figure(j, figsize=(3.5, 3.5))
		plt.subplot(111)
		for k in range (len(Rate)):
			plt.plot(Strain[i,j,k][0], TotalEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		Yrange = np.array([1.01*np.min(np.min(np.min(TotalEnergy[i,j]))), 0.95*np.max(np.max(np.max(TotalEnergy[i,j])))])
		plt.ylim([np.min(Yrange),   np.max(Yrange)])
		plt.xlim([0., 5.1])		
		plt.ylabel('Total Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.tight_layout()
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "TotalEnergy-Sp" + str(Chosen[i]) + "-D" + str(Direct[j]) + ".png"))
		plt.show()
		plt.close(fig)


#For Plotting PairEnergy Against Strain For Each Rate and Each Direction
for i in range (len(Chosen)):
	for j in range (len(Direct)):
		fig = plt.figure(j, figsize=(3.5, 3.5))
		plt.subplot(111)
		for k in range (len(Rate)):
			plt.plot(Strain[i,j,k][0], PairEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		Yrange = np.array([1.01*np.min(np.min(np.min(PairEnergy[i,j]))), 0.95*np.max(np.max(np.max(PairEnergy[i,j])))])
		plt.ylim([np.min(Yrange),   np.max(Yrange)])
		plt.xlim([0., 5.1])
		plt.ylabel('Pair Potential Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.tight_layout()
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "PairEnergy-Sp" + str(Chosen[i]) + "-D" + str(Direct[j]) + ".png"))
		plt.show()
		plt.close(fig)


#For Plotting BondEnergy Against Strain For Each Rate and Each Direction
for i in range (len(Chosen)):
	for j in range (len(Direct)):
		fig = plt.figure(j, figsize=(3.5, 3.5))
		plt.subplot(111)
		for k in range (len(Rate)):
			plt.plot(Strain[i,j,k][0], BondEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		Yrange = np.array([0.9*np.min(np.min(np.min(BondEnergy[i,j]))), 1.1*np.max(np.max(np.max(BondEnergy[i,j])))])
		plt.ylim([np.min(Yrange),   np.max(Yrange)])
		plt.xlim([0., 5.1])
		plt.ylabel('Bond Potential Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.tight_layout()
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "BondEnergy-Sp" + str(Chosen[i]) + "-D" + str(Direct[j]) + ".png"))
		plt.show()
		plt.close(fig)


#For Plotting AngleEnergy Against Strain For Each Rate and Each Direction
for i in range (len(Chosen)):
	for j in range (len(Direct)):
		fig = plt.figure(j, figsize=(3.5, 3.5))
		plt.subplot(111)
		for k in range (len(Rate)):
			plt.plot(Strain[i,j,k][0], AngleEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		Yrange = np.array([0.99*np.min(np.min(np.min(AngleEnergy[i,j]))), 1.1*np.max(np.max(np.max(AngleEnergy[i,j])))])
		plt.ylim([np.min(Yrange),   np.max(Yrange)])
		plt.xlim([0., 5.1])
		plt.ylabel('Angle Potential Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.tight_layout()
		
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "AngleEnergy-Sp" + str(Chosen[i]) + "-D" + str(Direct[j]) + ".png"))
		plt.show()
		plt.close(fig)
# 
# 
# 
# 
# 
# 
# #For Plotting TotalEnergy Against Strain For Each Rate and Each Direction
# for i in range (len(Chosen)):
# 	fig = plt.figure(j, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	for j in range (len(Direct)):
		
# 		for k in range (len(Rate), 2):
# 			plt.plot(Strain[i,j,k][0], TotalEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
# 		Yrange = np.array([1.05*np.min(np.min(np.min(TotalEnergy[i,j]))), 0.95*np.max(np.max(np.max(TotalEnergy[i,j])))])
# 		plt.ylim([np.min(Yrange),   np.max(Yrange)])
# 		plt.xlim([0., 5.1])		
# 		plt.ylabel('Total Energy (MJ/mol)')
# 		plt.xlabel('Strain')
# 		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
# 		plt.tight_layout()
# 		plt.xticks(rotation=45)
# 		plt.tight_layout()
# 		plt.savefig(os.path.join(WriteFolderName, "TotalEnergy-Sp" + str(Chosen[i]) + "-D" + str(Direct[j]) + ".png"))
# 		plt.show()
# 		plt.close(fig)


# #For Plotting PairEnergy Against Strain For Each Rate and Each Direction
# for i in range (len(Chosen)):
# 	for j in range (len(Direct)):
# 		fig = plt.figure(j, figsize=(3.5, 3.5))
# 		plt.subplot(111)
# 		for k in range (len(Rate)):
# 			plt.plot(Strain[i,j,k][0], PairEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
# 		Yrange = np.array([1.05*np.min(np.min(np.min(PairEnergy[i,j]))), 0.95*np.max(np.max(np.max(PairEnergy[i,j])))])
# 		plt.ylim([np.min(Yrange),   np.max(Yrange)])
# 		plt.xlim([0., 5.1])
# 		plt.ylabel('Pair Potential Energy (MJ/mol)')
# 		plt.xlabel('Strain')
# 		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
# 		plt.tight_layout()
# 		plt.xticks(rotation=45)
# 		plt.tight_layout()
# 		plt.savefig(os.path.join(WriteFolderName, "PairEnergy-Sp" + str(Chosen[i]) + "-D" + str(Direct[j]) + ".png"))
# 		plt.show()
# 		plt.close(fig)


# #For Plotting BondEnergy Against Strain For Each Rate and Each Direction
# for i in range (len(Chosen)):
# 	for j in range (len(Direct)):
# 		fig = plt.figure(j, figsize=(3.5, 3.5))
# 		plt.subplot(111)
# 		for k in range (len(Rate)):
# 			plt.plot(Strain[i,j,k][0], BondEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
# 		Yrange = np.array([0.95*np.min(np.min(np.min(BondEnergy[i,j]))), 1.05*np.max(np.max(np.max(BondEnergy[i,j])))])
# 		plt.ylim([np.min(Yrange),   np.max(Yrange)])
# 		plt.xlim([0., 5.1])
# 		plt.ylabel('Bond Potential Energy (MJ/mol)')
# 		plt.xlabel('Strain')
# 		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
# 		plt.tight_layout()
# 		plt.xticks(rotation=45)
# 		plt.tight_layout()
# 		plt.savefig(os.path.join(WriteFolderName, "BondEnergy-Sp" + str(Chosen[i]) + "-D" + str(Direct[j]) + ".png"))
# 		plt.show()
# 		plt.close(fig)


# #For Plotting AngleEnergy Against Strain For Each Rate and Each Direction
# for i in range (len(Chosen)):
# 	for j in range (len(Direct)):
# 		fig = plt.figure(j, figsize=(3.5, 3.5))
# 		plt.subplot(111)
# 		for k in range (len(Rate)):
# 			plt.plot(Strain[i,j,k][0], AngleEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
# 		Yrange = np.array([0.95*np.min(np.min(np.min(AngleEnergy[i,j]))), 1.05*np.max(np.max(np.max(AngleEnergy[i,j])))])
# 		plt.ylim([np.min(Yrange),   np.max(Yrange)])
# 		plt.xlim([0., 5.1])
# 		plt.ylabel('Angle Potential Energy (MJ/mol)')
# 		plt.xlabel('Strain')
# 		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
# 		plt.tight_layout()
		
# 		plt.xticks(rotation=45)
# 		plt.tight_layout()
# 		plt.savefig(os.path.join(WriteFolderName, "AngleEnergy-Sp" + str(Chosen[i]) + "-D" + str(Direct[j]) + ".png"))
# 		plt.show()
# 		plt.close(fig)