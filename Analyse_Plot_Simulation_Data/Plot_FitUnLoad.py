import os
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import math
import scipy as scipy
from scipy import integrate as integrate
from scipy import stats as stats
from scipy.integrate import quad
from scipy.optimize import curve_fit
import scipy.fftpack




Chosen              = [1, 2, 3, 4]
Structure           = ['Sph', 'RLM', 'WLM', 'BWLM']
Rate                = [0.3, 3, 30]
Strain              = [1, 3, 5]
SimType             = ["Load", "UnLoad"]

timeStep            = 30
timeIdx             = 0
strainIdx           = 1
pxx1Idx             = 4
pxx2Idx             = 5
rangeTimeIdx        = 1
rangeXIdx           = 2
rangeYIdx           = 3

rootDir             = "/media/KaceHDD1/Struct/"
# WriteFolderName   =  "/mnt/f/Struct/ImagesVideos/"
WriteFolderName     =   "/media/KaceHDD1/Struct/ImageVideos/FitData/"
WriteFileNameEnd    =  ''
ReadFolderName1     =  ''
ReadFolderName2     =  ''
ReadFileNameEnd     =  ''


stopTimeInNs        = 0.6
fit0                = 0
fit1                = 0.1
fit2                = 0.2
fit3                = 0.3
countFreq           = 0
countTime           = 1
avePxx              = 0

RlxTime             = 0.8
direct1             = "X"
direct2             = "Y"

TimeLoad            = []
StrainLoad          = []
PxxLoad             = []

Fit = np.array([[24, 18, 14], [30, 24, 20], [28, 20, 22], [18, 18, 14]])

Fit0 = np.array([20, 28, 28, 20])
Fit1 = np.array([14, 14, 14, 14])
Fit2 = np.array([8, 8, 8, 8])
Fit3 = np.array([3, 5, 5, 5])

#For species 1 values: 
# Sp1-R3-S1: 14
# 8-12

# Tries: A_0=1, t_0 = 0.8. 
# This only remotely worked for the unloading of specie 1. It was a poor fit for Loading of all species and unloading of all species but specie 1.

# Fit = np.array([[1, 0.8], [1, 3], [1, 3], [1, 3]])
# Nope




def ReadFileGetData (stopTime, pxxIdx, ItemPxx, ItemStrain, ItemTime, ReadFileName):
	avePxx              =  0
	countFreq           =  1
	countTime           =  1

	ReadFile     = open(ReadFileName, 'r')

	for line in ReadFile:
		if "# Fix" in line:
			continue
		elif (float(line.split()[timeIdx]) <= stopTime):
		# elif (rate*1e-6*timeStep*float(line.split()[timeIdx]) <= strain):
			if (int(line.split()[timeIdx]) > (countTime)*freq):
				ItemPxx.append(10*avePxx/countFreq )
				ItemTime.append(int(line.split()[timeIdx]))
				ItemStrain.append(round(rate*1e-6*timeStep*float(line.split()[timeIdx]), 2) )

				avePxx     =  0
				countFreq  =  1
				countTime  += 1

				# if (int(line.split()[timeIdx])*timeStep*1e-6 <= fit1):
				# 	X1.append(int(line.split()[timeIdx]))
				# if (int(line.split()[timeIdx])*timeStep*1e-6 > fit1 and int(line.split()[timeIdx])*timeStep*1e-6 <= fit2):
				# 	X2.append(int(line.split()[timeIdx]))

			if (int(line.split()[timeIdx]) <= (countTime)*freq):
				avePxx     += float(line.split()[pxxIdx])
				countFreq  += 1
	ReadFile.close()


def PlotData (lastPxx, UnLoadPxx1, UnLoadTime1, WriteFolderName, WriteFileNameEnd):
	UnLoadTime1     = timeStep*1e-6*np.array(UnLoadTime1)
	UnLoadPxx1      = np.array(UnLoadPxx1)
	X               = np.linspace(0, fit1, 100)
	FitPxx          = lastPxx*np.exp(-X*Fit[countChosen, countStrain])


	
	fig = plt.figure(1, figsize=(3.5, 3.5))
	plt.subplot(111)

	plt.plot(UnLoadTime1, UnLoadPxx1, 'o', linewidth=2, label='data')
	# plt.plot(X, FitPxx, linewidth=2, label='fit:' +  str(Fit[countChosen, countStrain]))
	plt.xlabel('Time (ns)')
	plt.ylabel('Stress (MPa)')
	plt.legend(bbox_to_anchor=(0., 0.98, 1., .104), loc=3, ncol=3, mode="expand", borderaxespad=0., fontsize = 'xx-small')
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "Fit" + WriteFileNameEnd + ".png"))
	plt.show()
	plt.close(fig)


	UnLoadTime1               = []
	UnLoadStrain1             = []
	UnLoadPxx1                = []


countChosen = 0
for chosen in Chosen:
	countRate = 0
	for rate in  Rate:
		if rate != 0.3:
			countStrain = 0
			for strain in  Strain:
				LoadTime          =  []
				LoadStrain        =  []
				LoadPxx           =  []

				UnLoadTime        =  []
				UnLoadStrain      =  []
				UnLoadPxx         =  []
				

				WriteFileNameEnd  = str(chosen) + "-R" + str(countRate+1) + "-S" + str(strain)

				simType           =  'UnLoad'
				freq              =  300
				stopTime          =  stopTimeInNs/(timeStep*1e-6)

				ReadFolderName    =  os.path.join(rootDir, str(chosen), direct1, simType)
				ReadFileNameEnd   =  simType + "-Rate" + str(rate) + "-Spn" + str(chosen) + "-S" + str(strain)
				ReadFileName      =  os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd + ".dat")

				ReadFileGetData (stopTime, pxx1Idx, UnLoadPxx, UnLoadStrain, UnLoadTime, ReadFileName)

				PlotData (UnLoadPxx[0], UnLoadPxx, UnLoadTime, WriteFolderName, WriteFileNameEnd)

				countStrain+=1

		countRate += 1
	countChosen+=1

