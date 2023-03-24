import os
import sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
# import matplotlib.pylab as plt
import matplotlib.image as mpimg
import numpy as np
from scipy.stats import linregress



def GetMaxPxx (ThisArray):
	ThisArray = np.array(ThisArray)
	# print(np.max(ThisArray[:, pxxIdx]))
	return 10*np.max(ThisArray[:, pxxIdx])

def GetEmod (ThisArray):
	ThisArray    =  np.array(ThisArray)
	TruncArray   =  ThisArray[(ThisArray[:, syStrainIdx] >=0) & (ThisArray[:, syStrainIdx] <=emodCutOff)]
	XEmod        =  TruncArray [:, syStrainIdx]
	YEmod        =  TruncArray [:, pxxIdx]
	slope, intercept, r_value, p_value, std_err = linregress(XEmod, YEmod)
	return slope

def GetSlope (X, Y):
	slope, intercept, r_value, p_value, std_err = linregress(X, Y)
	return slope

def GetTimeSyStrainPxx (ThisArray):
	ThisArray  =  np.array(ThisArray)
	Data       =  []
	avePxx     =  0
	countFreq  =  1
	countTime  =  1

	for line in ThisArray:
		if (line[syStrainIdx] > (countTime)*freq):
			Data.append([int(line[timeIdx]),   round((countTime)*freq, 2),   10*avePxx/countFreq])

			Time.append(int(line[timeIdx]))
			SyStrain.append(Data[:,forSyStrain])
			Pxx.append(Data[:,forPxx])

			avePxx     =  0
			countFreq  =  1
			countTime  += 1

		if (line[syStrainIdx] <= (countTime)*freq):
			avePxx     += line[pxxIdx]
			countFreq  += 1

	Data = np.array(Data)
	return Data



# /media/KaceHDD1/DifferentStructures/1/X/Load/MicelleFixLoad-Rate1-Spn1-En1.dat  /media/KaceHDD1/Struct/1/X/Load/MicelleFixLoad-Rate0.3-Spn1-En1.dat
Chosen            =   [1, 2, 3, 4]
Structure         =   ['Sph', 'RLM', 'WLM', 'BWLM']
MaxPxx            =   []
Direct            =   ["X", "Y"]
Rate              =   [0.1, 0.3, 1, 3, 10, 30]
Pxx               =   []
Time              =   []
SyStrain          =   []
CurveArea         =   []
timeStep          =   30
timeIdx           =   0
syStrainIdx       =   1
pxxIdx            =   4
miStrainIdx       =   3
allIdx            =   9
freq              =   0.5
countFreq         =   0
countTime         =   1
avePxx            =   0
RlxTime           =   3
emodCutOff        =   0.2
legendCols        =   3
convDefRate       =   1e-6
# direct            =   "X"
rootDir           =   "/media/KaceHDD1/Struct/"
WriteFolderName   =   "/media/KaceHDD1/Struct/ImageVideos/CompareOrient"
# rootDir = "/media/kelechi/8TB-Kay/DifferentStructures/"
# WriteFolderName   =  "/mnt/f/Struct/ImagesVideos/"





for chosen in Chosen:
	for direct in  Direct:
		if (direct == "Y"):
			pxxIdx   =  5
		else:
			pxxIdx   =  4
		for rate in  Rate:
			ReadFolderName    =  os.path.join(rootDir, str(chosen), direct, "Load")
			ReadFileNameEnd   =  "Load-Rate" + str(rate) + "-Spn" + str(chosen) + "-En1"
			ReadFileName      =  os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd  + ".dat")
			ReadFile          =  open(ReadFileName, 'r')
			ThisFile          =  ReadFile.readlines()
			ReadFile.close()

			ThisTime          =  []
			ThisStrain        =  []
			X                 =  []
			Y                 =  []
			ThisPxx           =  []
			
			avePxx            =  0
			countFreq         =  1
			countTime         =  1

			for line in ThisFile:
				if "# Fix" in line:
					continue
				# elif (rate*convDefRate*timeStep*float(line.split()[timeIdx]) <= strain):
				else:
					if (round(timeStep*rate*convDefRate*int(line.split()[timeIdx]), 2) > round(countTime*freq, 2)):
						ThisPxx.append(10*avePxx/countFreq )
						ThisTime.append(timeStep*convDefRate*int(line.split()[timeIdx]))
						ThisStrain.append(round(float(line.split()[syStrainIdx]), 2) )

						if (rate*convDefRate*timeStep*float(line.split()[timeIdx]) <= emodCutOff):
							X.append(rate*convDefRate*timeStep*float(line.split()[timeIdx]))
							Y.append(10*avePxx/countFreq)

						# print(round(timeStep*rate*convDefRate*int(line.split()[timeIdx]), 2), round(countTime*freq, 2))
						# print(avePxx)
						avePxx     =  0
						countFreq  =  1
						countTime  += 1

					if (round(timeStep*rate*convDefRate*int(line.split()[timeIdx]), 2) <= round(countTime*freq, 2)):
						avePxx     += float(line.split()[pxxIdx])
						countFreq  += 1
						# print(avePxx)

			
			Time.append(ThisTime)
			SyStrain.append(ThisStrain)
			Pxx.append(ThisPxx)
		
			
Rate              =   np.array(Rate)
Time              =   np.array(Time)
SyStrain          =   np.array(SyStrain)
Pxx               =   np.array(Pxx)


Time.shape        =   (len(Chosen), len(Direct), len(Rate), -1)
SyStrain.shape    =   (len(Chosen), len(Direct), len(Rate), -1)
Pxx.shape         =   (len(Chosen), len(Direct), len(Rate), -1)

print(Rate.shape, Time.shape, SyStrain.shape, Pxx.shape)





#For Plotting Stress Against SyStrain For Each Structure
for i in range (len(Chosen)):
	for k in range (len(Rate)):
		TheFunc = np.abs(Pxx[i,0,k] - Pxx[i,1,k])
		CurveArea.append(np.trapz(TheFunc, x=SyStrain[i,0,k]))
		
		fig = plt.figure(k, figsize=(3.5, 3.5))
		plt.subplot(111)
		for j in range (len(Direct)):
			plt.plot(SyStrain[i,j,k], Pxx[i,j,k], 'o', linewidth=2, label=str(Direct[j])+'  Wi~' + '{:1.1f}'.format(Rate[k]*RlxTime))
		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim((0.95*yminThis, 1.1*ymaxThis))
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))
		plt.ylabel('Stress (MPa)')
		plt.xlabel('Box Strain')
		plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "Compare-Sp" + str(Chosen[i]) + "-R" + str(k+1) + ".png"))
		# plt.show()
		plt.close(fig)




CurveArea           =   np.array(CurveArea)
CurveArea.shape     =   (len(Chosen), len(Rate), -1)


#For Plotting Curve Area Against SyStrain For Each Structure
for i in range (len(Chosen)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	plt.semilogx(Rate*RlxTime, CurveArea[i], 'o', linewidth=2, label=Structure[i])
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
	plt.ylabel('Area Under Curve (MPa)')
	plt.xlabel('Wi')
	plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "AreaComp-Sp" + str(Chosen[i]) + ".png"))
	# plt.show()
	plt.close(fig)
