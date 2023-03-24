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
from scipy.stats import linregress




Chosen          = [1, 2, 3, 4]
Structure       = ['Sph', 'RLM', 'WLM', 'BWLM']
direct          = "X"
Rate            = [1, 3, 10, 30]
Strain          = [1]


Time1           = []
Strain1         = []
Pxx1            = []
RangeStrain1    = []
Range1          = []

Time2           = []
Strain2         = []
Pxx2            = []
RangeStrain2    = []
Range2          = []

RelaxRate       = []
CurveArea       = []
FitData         = []


timeStep       = 30
timeIdx        = 0
strainIdx      = 1
pxxIdx         = 4
rangeTimeIdx   = 1
rangeXIdx      = 2
rangeYIdx      = 3
convDefRate    = 1e-6

rootDir = "/media/KaceHDD1/Struct/"


# WriteFolderName   =  "/mnt/f/Struct/ImagesVideos/"
WriteFolderName   =  "/media/KaceHDD1/Struct/ImageVideos/UnLoad_Images/"

countFreq  = 0
countTime  = 1
avePxx     = 0

RlxTime    = 3
freqStrain = 0.1

simulationType1 = 'UnLoad'
simulationType2 = 'Load'


def func(x, a, b, c):
	return a * np.exp(-b * x) + c


def GetSlope (X, Y):
	slope, intercept, r_value, p_value, std_err = linregress(X, Y)
	return slope

for chosen in Chosen:
	for rate in  Rate:
		freq  = math.ceil(freqStrain/(timeStep*rate*1e-6))


		for strain in  Strain:
			ReadFolderName    =  os.path.join(rootDir, str(chosen), direct, "Cycle")
			# /media/KaceHDD1/Struct/4/X/Cycle/SystemFixUnLoad-Rate3-Spn4-En2.dat
			# ReadFileNameEnd   =  simulationType1 + "-Rate" + str(rate) + "-Spn" + str(chosen) + "-S" + str(strain) + "-En" + str(1)
			ReadFileNameEnd   =  simulationType1 + "-Rate" + str(rate) + "-Spn" + str(chosen) + "-En" + str(1)
			
			UnLoadTime          = []
			UnLoadStrain        = []
			UnLoadPxx           = []
			OtherUnLoadPxx      = []
			OtherUnLoadStrain   = []
			OtherUnLoadTime     = []
			
			
			avePxx              =  0
			countFreq           =  1
			countTime           =  1


			ReadFileName = os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd + ".dat")
			ReadFile     = open(ReadFileName, 'r')

			for line in ReadFile:
				if "# Fix" in line:
					continue
				elif (rate*1e-6*timeStep*float(line.split()[timeIdx]) <= 10*strain):
					if (float(line.split()[timeIdx]) > (countTime)*freq):
						UnLoadPxx.append(10*avePxx/countFreq )
						UnLoadTime.append(int(line.split()[timeIdx]))
						UnLoadStrain.append(round(float(line.split()[strainIdx]), 2) )

						if (avePxx > 0):
							OtherUnLoadPxx.append(10*avePxx/countFreq )
							OtherUnLoadStrain.append(round(float(line.split()[strainIdx]), 2) )
							OtherUnLoadTime.append(timeStep*convDefRate*int(line.split()[timeIdx]))


						avePxx     =  0
						countFreq  =  1
						countTime  += 1

					if (float(line.split()[timeIdx]) <= (countTime)*freq):
						avePxx     += float(line.split()[pxxIdx])
						countFreq  += 1
			ReadFile.close()
			


			ReadFolderName    =  os.path.join(rootDir, str(chosen), direct, simulationType2)
			ReadFileNameEnd   =  simulationType2 + "-Rate" + str(rate) + "-Spn" + str(chosen) + "-En" + str(1)

			LoadTime          = []
			LoadStrain        = []
			LoadPxx           = []
			
			
			avePxx            =  0
			countFreq         =  1
			countTime         =  1


			ReadFileName = os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd + ".dat")
			ReadFile     = open(ReadFileName, 'r')

			for line in ReadFile:
				if "# Fix" in line:
					continue
				elif (rate*1e-6*timeStep*float(line.split()[timeIdx]) <= strain):
					if (float(line.split()[timeIdx]) > (countTime)*freq):
						LoadPxx.append(10*avePxx/countFreq )
						LoadTime.append(int(line.split()[timeIdx]))
						LoadStrain.append(round(float(line.split()[strainIdx]), 2) )

						avePxx      = 0
						countFreq   = 1
						countTime  += 1



					if (float(line.split()[timeIdx]) <= (countTime)*freq):
						avePxx     += float(line.split()[pxxIdx])
						countFreq  += 1
			ReadFile.close()
		
			LoadTime             =   np.array(LoadTime)
			LoadStrain           =   np.array(LoadStrain)
			LoadPxx              =   np.array(LoadPxx)

			UnLoadTime           =   np.array(UnLoadTime)
			UnLoadStrain         =   np.array(UnLoadStrain)
			UnLoadPxx            =   np.array(UnLoadPxx)
			OtherUnLoadPxx       =   np.array(OtherUnLoadPxx)
			OtherUnLoadStrain    =   np.array(OtherUnLoadStrain)
			OtherUnLoadTime      =   np.array(OtherUnLoadTime)
			

			minLength = np.min( np.array( [len(LoadPxx), len(UnLoadPxx)] ) )


			RealLoadTime       =   LoadTime[:minLength]
			RealLoadStrain     =   LoadStrain[:minLength]
			RealUnLoadTime     =   RealLoadTime[minLength-1]*np.ones(minLength) - UnLoadTime[:minLength] 
			RealUnLoadStrain   =   -np.sort(-RealLoadStrain, kind='mergesort') 

			

			if (len (OtherUnLoadPxx) > 0):
				Otherfunc = np.log(OtherUnLoadPxx/OtherUnLoadPxx[0])
				decayRate = GetSlope (OtherUnLoadTime, Otherfunc)
				RelaxRate.append(decayRate)
				

				fig = plt.figure(1, figsize=(3.5, 3.5))
				plt.subplot(111)
				plt.plot(OtherUnLoadTime, Otherfunc, 'o', linewidth=2, label="unload")
				plt.plot(OtherUnLoadTime, OtherUnLoadTime*decayRate, '--', linewidth=2, label="decay rate  " + '{:1.1f}'.format(-decayRate))
				yminThis, ymaxThis = plt.ylim()
				xminThis, xmaxThis = plt.xlim()
				plt.ylim((0.3+1.1*yminThis, 0.95*ymaxThis))
				plt.xlim((0.01+xminThis, 1.05*xmaxThis))
				plt.xticks(rotation=45)
				plt.legend(bbox_to_anchor=(0., 1.0, 1., .104), loc=3, ncol=len(Rate), mode="expand", borderaxespad=0., fontsize = 'xx-small')
				plt.tight_layout()
				plt.savefig(os.path.join(WriteFolderName, "FitUnLoad-Sp" + str(chosen) + "-R" + str(Rate.index(rate)+1) + "-S" + str(strain) + ".png"))
				plt.show()
				plt.close(fig)


			TheFunc = LoadPxx[:minLength] - UnLoadPxx[:minLength]
			CurveArea.append(np.trapz(TheFunc, x=LoadStrain[:minLength]))

		
			# fig = plt.figure(1, figsize=(3.5, 3.5))
			# plt.subplot(111)
			# plt.plot(RealLoadStrain, LoadPxx[:minLength], 'o', linewidth=2, label="loading")
			# plt.plot(RealUnLoadStrain, UnLoadPxx[:minLength], 'o', linewidth=2, label="unloading")

			# plt.ylabel('Stress (MPa)')
			# plt.xlabel('Strain')
			# plt.legend(bbox_to_anchor=(0., 1.0, 1., .104), loc=3, ncol=len(Rate), mode="expand", borderaxespad=0., fontsize = 'xx-small')
			# plt.xticks(rotation=45)
			# yminThis, ymaxThis = plt.ylim()
			# xminThis, xmaxThis = plt.xlim()
			# plt.ylim((0.95*yminThis, 1.1*ymaxThis))
			# plt.xlim((-0.1+xminThis, 1.05*xmaxThis))
			# plt.tight_layout()
			# if (strain != 1):
			# 	plt.savefig(os.path.join(WriteFolderName, "StressHyst-Sp" + str(chosen) + "-R" + str(Rate.index(rate)+1) + "-S" + str(strain) + ".png"))
			# # plt.show()
			# plt.close(fig)



Rate                   =    np.array(Rate)
CurveArea              =    np.array(CurveArea)
# RelaxRate              =    np.array(RelaxRate)
CurveArea.shape        =    (len(Chosen), len(Rate), len(Strain))
# RelaxRate.shape        =    (len(Chosen), len(Rate), len(Strain))


# print(CurveArea)			
# print()			
# print()			


# print(FitData)


# #For Plotting Area Against Strain For Each Structure and Erate
# for i in range (len(Chosen)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	for j in range (len(Rate)):
# 		plt.subplot(111)
# 		plt.plot(Strain, CurveArea[i,j], '-', linewidth=2, label='W~O(' + str(Rate[j]*RlxTime)+')')
# 		plt.ylabel('Area Under Curve (MPa)')
# 		plt.xlabel('Strain')
# 		# plt.legend(bbox_to_anchor=(0.005, 0.87, .2, .99), loc=3, ncol=2, borderaxespad=0., fontsize = 'xx-small')
# 		plt.legend(bbox_to_anchor=(0., 1.0, 1., .104), loc=3, ncol=len(Rate), mode="expand", borderaxespad=0., fontsize = 'xx-small')
# 	yminThis, ymaxThis = plt.ylim()
# 	xminThis, xmaxThis = plt.xlim()
# 	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
# 	plt.xlim((-0.1+xminThis, 1.05*xmaxThis))
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "AreaHyst-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)



# #For Plotting Area Against Strain For Each Structure and Erate
# for i in range (len(Rate)):
# 	x = np.array(range(0,len(Structure)))
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	for j in range (len(Chosen)):
# 		plt.subplot(111)

# 		plt.subplot(111)
# 		plt.plot(Strain, CurveArea[j,i], '-', linewidth=2, label=Structure[j])
# 		plt.ylabel('Area Under Curve (MPa)')
# 		plt.xlabel('Strain')
# 		# plt.legend(bbox_to_anchor=(0.005, 0.87, .2, .99), loc=3, ncol=2, borderaxespad=0., fontsize = 'xx-small')
# 		plt.legend(bbox_to_anchor=(0., 1.0, 1., .104), loc=3, ncol=len(Structure), mode="expand", borderaxespad=0., fontsize = 'xx-small')
# 	yminThis, ymaxThis = plt.ylim()
# 	xminThis, xmaxThis = plt.xlim()
# 	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
# 	plt.xlim((0.9*xminThis, 1.05*xmaxThis))
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "AreaHyst-R" + str(i+1) + ".png"))
# 	# plt.show()
# 	plt.close(fig)





# #For Plotting Area Against Strain For Each Structure and Erate
# for i in range (len(Chosen)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	for j in range (len(Rate)):
# 		plt.subplot(111)
# 		plt.plot(Strain, CurveArea[i,j], '-', linewidth=2, label='W~O(' + str(Rate[j]*RlxTime)+')')
# 		plt.ylabel('Area Under Curve (MPa)')
# 		plt.xlabel('Strain')
# 		# plt.legend(bbox_to_anchor=(0.005, 0.87, .2, .99), loc=3, ncol=2, borderaxespad=0., fontsize = 'xx-small')
# 		plt.legend(bbox_to_anchor=(0., 1.0, 1., .104), loc=3, ncol=len(Rate), mode="expand", borderaxespad=0., fontsize = 'xx-small')
# 	yminThis, ymaxThis = plt.ylim()
# 	xminThis, xmaxThis = plt.xlim()
# 	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
# 	plt.xlim((-0.1+xminThis, 1.05*xmaxThis))
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "AreaFull-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)



# #For Plotting Area Against Strain For Each Structure and Erate
# for i in range (len(Rate)):
# 	x = np.array(range(0,len(Structure)))
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	for j in range (len(Chosen)):
# 		plt.subplot(111)

# 		plt.subplot(111)
# 		plt.plot(Strain, CurveArea[j,i], '-', linewidth=2, label=Structure[j])
# 		plt.ylabel('Area Under Curve (MPa)')
# 		plt.xlabel('Strain')
# 		# plt.legend(bbox_to_anchor=(0.005, 0.87, .2, .99), loc=3, ncol=2, borderaxespad=0., fontsize = 'xx-small')
# 		plt.legend(bbox_to_anchor=(0., 1.0, 1., .104), loc=3, ncol=len(Structure), mode="expand", borderaxespad=0., fontsize = 'xx-small')
# 	yminThis, ymaxThis = plt.ylim()
# 	xminThis, xmaxThis = plt.xlim()
# 	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
# 	plt.xlim((0.9*xminThis, 1.05*xmaxThis))
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "AreaFull-R" + str(i+1) + ".png"))
# 	# plt.show()
# 	plt.close(fig)
# 		