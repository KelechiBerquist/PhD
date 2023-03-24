import os
import matplotlib
# matplotlib.use('Agg')
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

import sys  
sys.path.append("/media/KaceHDD1/2018-06-14-CppPyCodes/LinearRegression")  
import LinearRegression as LinReg
import random



Chosen              =   [1, 2, 3, 4]
Structure           =   ['Sph', 'RLM', 'WLM', 'BWLM']
Rate                =   [3, 30]
Strain              =   [1, 3, 5]
SimType             =   ["Load", "UnLoad"]
timeStep            =   30
timeIdx             =   0
strainIdx           =   1
pxx1Idx             =   4
pxx2Idx             =   5
rangeTimeIdx        =   1
rangeXIdx           =   2
rangeYIdx           =   3
rootDir             =   "/media/KaceHDD1/Struct/"
# WriteFolderName   =    "/mnt/f/Struct/ImagesVideos/"
WriteFolderName     =   "/media/KaceHDD1/Struct/ImageVideos/FitData/"
WriteFileNameEnd    =   ''
ReadFolderName1     =   ''
ReadFolderName2     =   ''
ReadFileNameEnd     =   ''
stopTimeInNs        =   0.3
fit0                =   0
fit1                =   0.1
fit2                =   0.2
fit3                =   0.3
countFreq           =   0
countTime           =   1
avePxx              =   0
RlxTime             =   0.8
direct1             =   "X"
direct2             =   "Y"
TimeLoad            =   []
StrainLoad          =   []
PxxLoad             =   []
learnRate           =   0.01
thisLambda          =   0




def ReadFileGetData (stopTime, pxxIdx, ItemPxx, ItemStrain, ItemTime, ReadFileName):
	avePxx              =  0
	countFreq           =  1
	countTime           =  1
	ReadFile            = open(ReadFileName, 'r')

	for line in ReadFile:
		if "# Fix" in line:
			continue
		elif (float(line.split()[timeIdx]) <= stopTime):
		# elif (rate*1e-6*timeStep*float(line.split()[timeIdx]) <= strain):
			if (int(line.split()[timeIdx]) > (countTime)*freq):
				if avePxx > 0:
					ItemPxx.append(10*avePxx/countFreq )
					ItemTime.append(int(line.split()[timeIdx]))
					ItemStrain.append(round(rate*1e-6*timeStep*float(line.split()[timeIdx]), 2) )

				# print(rate, simType, (countTime)*freq, 10*avePxx/countFreq)

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
	X             =  timeStep*1e-6*np.array(UnLoadTime1)
	UnLoadTime1   =  timeStep*1e-6*np.array(UnLoadTime1)
	UnLoadPxx1    =  np.array(UnLoadPxx1)
	Y             =  np.log(UnLoadPxx1)

	# print(Y-UnLoadPxx1)
	# plt.scatter(X, Y, color='b', label='log(pxx)')
	# plt.scatter(X, UnLoadPxx1, color='r', label='pxx')
	# plt.legend()
	# plt.show()
	# plt.close()
	# print(X.shape)
	# print(Y.shape)
	# print(X)
	# print(Y)

	X.shape     =  (X.shape[0], 1)
	X1          =  X
	X1          =  np.hstack(( np.ones((X1.shape[0],1)), X1 ))


	Iteration, Cost, Theta = LinReg.LinearRegression (X1, Y, learnRate, thisLambda)

	plt.plot(Iteration[2:], Cost[2:], label='cost')
	plt.legend()
	plt.show()
	plt.close()


	print(Theta)

	H = X1.dot(Theta)

	for i in range (X.shape[1]):
		plt.scatter(X[:, i], Y, color='b', label='target')
		plt.scatter(X[:, i], H, color='r', label='fit')
		plt.legend()
		plt.show()
		plt.close()


	# # realTheta0  =  random.randint(1,1000)
	# # # realTheta1  =  random.randint(1,5)
	# # realTheta1  =  random.uniform(1.0, 5.0)
	# # X2          =  np.linspace(0, 12, 100)
	# # Y2          =  realTheta0*np.exp(-realTheta1*X2)
	# # Y21         =  np.log(Y2)

	# # X2.shape     =  (X2.shape[0], 1)
	# # # X21          =  (X2 - np.mean(X2))/np.std(X2)
	# # X21          =  X2
	# # X21          =  np.hstack(( np.ones((X21.shape[0],1)), X21 ))


	# # Iteration, Cost, Theta = LinReg.LinearRegression (X21, Y21, learnRate, thisLambda)
	# # plt.plot(Iteration, Cost, label='cost')
	# # plt.legend()
	# # plt.show()
	# # plt.close()


	# # print(realTheta0, realTheta1, Theta)
	# # # print(realTheta0, realTheta1, np.exp(Theta[0]), Theta)


	# # plt.scatter(X2, Y21, color='b', label='target')
	# # plt.scatter(X2, X21.dot(Theta), color='r', label='fit')
	# # # plt.scatter(X2, np.log(realTheta0*np.exp(Theta[1]*X2)), color='g', label='fit+')
	# # plt.legend()
	# # plt.show()
	# # plt.close()

	



	



	# X             =  np.linspace(0, fit1, 100)
	# X0            =  np.linspace(0, stopTimeInNs, 100)
	# X1            =  np.linspace(fit1, stopTimeInNs, 100)
	# X2            =  np.linspace(fit2, stopTimeInNs, 100)
	# X3            =  np.linspace(fit3, stopTimeInNs, 100)
	# FitPxx        =  lastPxx*np.exp(-X*Fit[countChosen, countStrain])


	# fig           =  plt.figure(1, figsize=(3.5, 3.5))

	# plt.subplot(111)
	# plt.plot(UnLoadTime1, UnLoadPxx1, 'o', linewidth=2, label='data')
	# plt.plot(X, FitPxx, linewidth=2, label='fit:' +  str(Fit[countChosen, countStrain]))
	
	# plt.xlabel('Time (ns)')
	# plt.ylabel('Stress (MPa)')
	# plt.legend(bbox_to_anchor=(0., 0.98, 1., .104), loc=3, ncol=3, mode="expand", borderaxespad=0., fontsize = 'xx-small')
	# plt.xticks(rotation=45)
	# plt.tight_layout()
	# plt.savefig(os.path.join(WriteFolderName, "Fit" + WriteFileNameEnd + ".png"))
	# plt.show()
	# plt.close(fig)


	UnLoadTime1               = []
	UnLoadStrain1             = []
	UnLoadPxx1                = []


countChosen = 0
for chosen in Chosen:
	countRate = 0
	for rate in  Rate:
		countStrain = 0
		for strain in  Strain:
			LoadTime          =  []
			LoadStrain        =  []
			LoadPxx           =  []
			UnLoadTime        =  []
			UnLoadStrain      =  []
			UnLoadPxx         =  []
			


			# WriteFileNameEnd  = str(chosen) + "-R" + str(countRate+1) + "-t" + str(Fit[countChosen, 1]) + "-S" + str(strain)

			WriteFileNameEnd  = str(chosen) + "-R" + str(countRate+1) + "-S" + str(strain)

			# simType           =  'Load'
			# freqStrain        =  0.1
			# freq              =  math.ceil(freqStrain/(timeStep*rate*1e-6))
			# stopTime          =  math.ceil(strain/(timeStep*rate*1e-6))

			# ReadFolderName    =  os.path.join(rootDir, str(chosen), direct1, simType) 
			# ReadFileNameEnd   =  simType + "-Rate" + str(rate) + "-Spn" + str(chosen) + "-En1"
			# ReadFileName      =  os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd + ".dat")

			# ReadFileGetData (stopTime, pxx1Idx, LoadPxx, LoadStrain, LoadTime, ReadFileName)
			

			simType           =  'UnLoad'
			freq              =  125
			stopTime          =  stopTimeInNs/(timeStep*1e-6)

			ReadFolderName    =  os.path.join(rootDir, str(chosen), direct1, simType)
			ReadFileNameEnd   =  simType + "-Rate" + str(rate) + "-Spn" + str(chosen) + "-S" + str(strain)
			ReadFileName      =  os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd + ".dat")

			ReadFileGetData (stopTime, pxx1Idx, UnLoadPxx, UnLoadStrain, UnLoadTime, ReadFileName)
			PlotData (UnLoadPxx[0], UnLoadPxx, UnLoadTime, WriteFolderName, WriteFileNameEnd)

			countStrain+=1

		countRate += 1
	countChosen+=1
