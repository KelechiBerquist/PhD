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
timeIdx           =   0
totalEnergyIdx    =   1
pairEnergyIdx     =   2
bondEnergyIdx     =   3
angleEnergyIdx    =   4
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


			for line in ThisFile:
				if ( int(line.split()[timeIdx])%(SkipStep[Rate.index(rate)]*dumpSkip) == 0):
					# debug += ('{:6.2f}'.format(int(line.split()[timeIdx])%(SkipStep[Rate.index(rate)]*dumpSkip)) + " "  + '{:6.2f}'.format(float(line.split()[timeIdx])*timeStep*rate*convDeformRate) + "   " + line)

					
					ThatStrain.append(round(float(line.split()[timeIdx])*timeStep*rate*convDeformRate,2))
					ThatTotalEnergy.append(0.004184*float(line.split()[totalEnergyIdx]))
					ThatPairEnergy.append(0.004184*float(line.split()[pairEnergyIdx]))
					ThatBondEnergy.append(0.004184*float(line.split()[bondEnergyIdx]))
					ThatAngleEnergy.append(0.004184*float(line.split()[angleEnergyIdx]))

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



# print(Strain[0, 0, 0])
# print(0.95*np.min(np.min(np.min(BondEnergy[0,0]))), 1.05*np.max(np.max(np.max(BondEnergy[0,0]))))
# print(0.95*np.min(np.min(BondEnergy[0,0])), 1.05*np.max(np.max(BondEnergy[0,0])))
# print(np.min(np.min(Strain[0,0])), np.max(np.max(Strain[0,0])))
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

		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=0.97*ymaxThis)
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))	
		plt.ylabel('Total Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "TotalEnergy-Sp" + str(Chosen[i]) + "-" + str(Direct[j]) + ".png"))
		plt.show()
		plt.close(fig)


		#For Plotting PairEnergy Against Strain For Each Rate and Each Direction
		fig = plt.figure(j, figsize=(3.5, 3.5))
		plt.subplot(111)
		for k in range (len(Rate)):
			plt.plot(Strain[i,j,k][0], PairEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))

		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=0.97*ymaxThis)
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))
		plt.ylabel('Pair Potential Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "PairEnergy-Sp" + str(Chosen[i]) + "-" + str(Direct[j]) + ".png"))
		plt.show()
		plt.close(fig)


		#For Plotting BondEnergy Against Strain For Each Rate and Each Direction
		fig = plt.figure(j, figsize=(3.5, 3.5))
		plt.subplot(111)
		for k in range (len(Rate)):
			plt.plot(Strain[i,j,k][0], BondEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))

		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=1.02*ymaxThis)
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))
		plt.ylabel('Bond Potential Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "BondEnergy-Sp" + str(Chosen[i]) + "-" + str(Direct[j]) + ".png"))
		plt.show()
		plt.close(fig)


		#For Plotting AngleEnergy Against Strain For Each Rate and Each Direction
		fig = plt.figure(j, figsize=(3.5, 3.5))
		plt.subplot(111)
		for k in range (len(Rate)):
			plt.plot(Strain[i,j,k][0], AngleEnergy[i,j,k][0], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		
		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=1.02*ymaxThis)
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))
		plt.ylabel('Angle Potential Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "AngleEnergy-Sp" + str(Chosen[i]) + "-" + str(Direct[j]) + ".png"))
		plt.show()
		plt.close(fig)
# 
# 
# 
# 
# 
# 
#For Plotting TotalEnergy Against Strain For Select Rate and Each Direction
for i in range (len(Chosen)):
	# for k in range (0, len(Rate), 2):
	for k in range (len(Rate)):
		fig = plt.figure(k, figsize=(3.5, 3.5))
		plt.subplot(111)
		plt.plot(Strain[i,0,k][0], TotalEnergy[i,0,k][0], 'o', linewidth=2, label=Direct[0]+' Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		plt.plot(Strain[i,1,k][0], TotalEnergy[i,1,k][0], '^', linewidth=2, label=Direct[1]+' Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))

		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=0.97*ymaxThis)
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))	
		plt.ylabel('Total Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "TotalEnergy-Sp" + str(Chosen[i]) + "-R" + str(k) + "-XY" + ".png"))
		plt.show()
		plt.close(fig)


		#For Plotting PairEnergy Against Strain For Select Rate and Each Direction
		fig = plt.figure(k, figsize=(3.5, 3.5))
		plt.subplot(111)
		plt.plot(Strain[i,0,k][0], PairEnergy[i,0,k][0], 'o', linewidth=2, label=Direct[0]+' Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		plt.plot(Strain[i,1,k][0], PairEnergy[i,1,k][0], '^', linewidth=2, label=Direct[1]+' Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))

		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=0.99*ymaxThis)
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))	
		plt.ylabel('Pair Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "PairEnergy-Sp" + str(Chosen[i]) + "-R" + str(k) + "-XY" + ".png"))
		plt.show()
		plt.close(fig)


		#For Plotting BondEnergy Against Strain For Select Rate and Each Direction
		fig = plt.figure(k, figsize=(3.5, 3.5))
		plt.subplot(111)
		plt.plot(Strain[i,0,k][0], BondEnergy[i,0,k][0], 'o', linewidth=2, label= Direct[0]+' Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		plt.plot(Strain[i,1,k][0], BondEnergy[i,1,k][0], '^', linewidth=2, label= Direct[1]+' Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))

		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=1.02*ymaxThis)
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))	
		plt.ylabel('Bond Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "BondEnergy-Sp" + str(Chosen[i]) + "-R" + str(k) + "-XY" + ".png"))
		plt.show()
		plt.close(fig)


		#For Plotting AngleEnergy Against Strain For Select Rate and Each Direction
		fig = plt.figure(k, figsize=(3.5, 3.5))
		plt.subplot(111)
		plt.plot(Strain[i,0,k][0], AngleEnergy[i,0,k][0], 'o', linewidth=2, label= Direct[0]+' Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		plt.plot(Strain[i,1,k][0], AngleEnergy[i,1,k][0], '^', linewidth=2, label= Direct[1]+' Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))

		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=1.02*ymaxThis)
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))	
		plt.ylabel('Angle Energy (MJ/mol)')
		plt.xlabel('Strain')
		plt.legend(bbox_to_anchor=(0.05, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "AngleEnergy-Sp" + str(Chosen[i]) + "-R" + str(k) + "-XY" + ".png"))
		plt.show()
		plt.close(fig)