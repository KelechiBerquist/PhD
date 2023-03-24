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
	TruncArray   =  ThisArray[(ThisArray[:, strainIdx] >=0) & (ThisArray[:, strainIdx] <=emodCutOff)]
	XEmod        =  TruncArray [:, strainIdx]
	YEmod        =  TruncArray [:, pxxIdx]
	slope, intercept, r_value, p_value, std_err = linregress(XEmod, YEmod)
	return slope

def GetTimeStrainPxx (ThisArray):
	ThisArray  =  np.array(ThisArray)
	Data       =  []
	avePxx     =  0
	countFreq  =  1
	countTime  =  1

	for line in ThisArray:
		if (line[strainIdx] > (countTime)*freq):
			Data.append([int(line[timeIdx]),   round((countTime)*freq, 2),   10*avePxx/countFreq])
			avePxx     =  0
			countFreq  =  1
			countTime  += 1

		if (line[strainIdx] <= (countTime)*freq):
			avePxx     += line[pxxIdx]
			countFreq  += 1

	Data = np.array(Data)
	return Data



# /media/KaceHDD1/DifferentStructures/1/X/Load/MicelleFixLoad-Rate1-Spn1-En1.dat  /media/KaceHDD1/Struct/1/X/Load/MicelleFixLoad-Rate0.3-Spn1-En1.dat
Chosen            =   [1, 2, 3, 4]
Structure         =   ['Sph', 'RLM', 'WLM', 'BWLM']
Rate1             =   []
Rate2             =   [0.3, 3, 30]
# Rate2             =   [0.3,  30]
ContentFile1      =   []
ContentFile2      =   []
MaxPxx            =   []
MainRate          =   []
Emod              =   []
Pxx               =   []
Time              =   []
Strain            =   []
timeStep          =   30
timeIdx           =   0
strainIdx         =   1
pxxIdx            =   4
rangeTimeIdx      =   1
rangeXIdx         =   2
rangeYIdx         =   3
allIdx            =   9
freq              =   0.1
countFreq         =   0
countTime         =   1
avePxx            =   0
RlxTime           =   3
emodCutOff        =   0.1
direct            =   "X"
rootDir1          =   "/media/KaceHDD1/DifferentStructures/"
rootDir2          =   "/media/KaceHDD1/Struct/"
WriteFolderName   =   "/media/KaceHDD1/Struct/2018-06-13-Images/"
# rootDir = "/media/kelechi/8TB-Kay/DifferentStructures/"
# WriteFolderName   =  "/mnt/f/Struct/ImagesVideos/"





for chosen in Chosen:
	if chosen == 1 :
		# Rate1 = [0.2, 1, 10]
		Rate1 = [0.2, 1]
	else: 
		Rate1 = [0.1, 1]
		# Rate1 = [0.1, 1, 10]

	for rate in  Rate1:
		ReadFolderName    =  os.path.join(rootDir1, str(chosen), direct, "Load")
		ReadFileNameEnd   =  "Load-Rate" + str(rate) + "-Spn" + str(chosen) + "-En1"
		ReadFileName      =  os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd  + ".dat")
		ReadFile          =  open(ReadFileName, 'r')
		ThisFile1         =  ReadFile.readlines()
		ReadFile.close()
		ThatFile1         =  []
		for line in ThisFile1:
			if "# Fix" in line:
				continue
			else:
				ThatFile1.append([float(i) for i in line.split()])
		ContentFile1.append(ThatFile1)
		

	for rate in  Rate2:
		ReadFolderName    =  os.path.join(rootDir2, str(chosen), direct, "Load")
		ReadFileNameEnd   =  "Load-Rate" + str(rate) + "-Spn" + str(chosen) + "-En1"
		ReadFileName      =  os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd  + ".dat")
		ReadFile          =  open(ReadFileName, 'r')
		ThisFile2         =  ReadFile.readlines()
		ReadFile.close()
		ThatFile2         =  []
		for line in ThisFile2:
			if "# Fix" in line:
				continue
			else:
				ThatFile2.append([float(i) for i in line.split()])
		ContentFile2.append(ThatFile2)


ContentFile1          =  np.array(ContentFile1)
ContentFile2          =  np.array(ContentFile2)
ContentFile1.shape    =  (len(Chosen), len(Rate1), -1)
ContentFile2.shape    =  (len(Chosen), len(Rate2), -1)
# print(ContentFile1.shape, ContentFile2.shape)
# print(ContentFile1.dtype.name)
# print(ContentFile2.dtype.name)
# ContentFile1          =  ContentFile1.astype(float)
# ContentFile2          =  ContentFile2.astype(float)



for i in range (np.max( [len(ContentFile1), len(ContentFile2)] )):
	for j in range(np.max( [len(ContentFile1[0]), len(ContentFile2[0])] )):
		for k in range(np.max( [len(ContentFile1[0,0]), len(ContentFile2[0,0])] )):
			forTime   = 0
			forStrain = 1
			forPxx    = 2

			if (j < len(ContentFile1[i])):
				MainRate.append(Rate1[j])
				MaxPxx.append(GetMaxPxx (ContentFile1[i,j,k]))
				Emod.append(GetEmod (ContentFile1[i,j,k]))

				Data      = GetTimeStrainPxx (ContentFile1[i,j,k])
				Time.append(Data[:,forTime])
				Strain.append(Data[:,forStrain])
				Pxx.append(Data[:,forPxx])

			if (j < len(ContentFile2[i])):
				MainRate.append(Rate2[j])
				MaxPxx.append(GetMaxPxx (ContentFile2[i,j,k]))
				Emod.append(GetEmod (ContentFile2[i,j,k]))
				
				Data      = GetTimeStrainPxx (ContentFile2[i,j,k])
				Time.append(Data[:,forTime])
				Strain.append(Data[:,forStrain])
				Pxx.append(Data[:,forPxx])

			
			
MaxPxx           = np.array(MaxPxx)
MainRate         = np.array(MainRate)
Emod             = np.array(Emod)
Time             = np.array(Time)
Strain           = np.array(Strain)
Pxx              = np.array(Pxx)

MaxPxx.shape     = (len(Chosen), len(Rate1)+len(Rate2), -1)
MainRate.shape   = (len(Chosen), len(Rate1)+len(Rate2), -1)
Emod.shape       = (len(Chosen), len(Rate1)+len(Rate2), -1)
Time.shape       = (len(Chosen), len(Rate1)+len(Rate2), -1)
Strain.shape     = (len(Chosen), len(Rate1)+len(Rate2), -1)
Pxx.shape        = (len(Chosen), len(Rate1)+len(Rate2), -1)

# print(MaxPxx.shape,  MainRate.shape, Emod.shape, Time.shape, Strain.shape, Pxx.shape)






#For Plotting Max Stress Against Erate For Each Structure
for i in range (len(Chosen)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	plt.semilogx(MainRate[i]*RlxTime, MaxPxx[i], 'o', linewidth=2, label=Structure[i])
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Wi')
	plt.legend(bbox_to_anchor=(0.05, 0.90, .3, .99), loc=3, ncol=len(Rate2), borderaxespad=0., fontsize = 'xx-small')
	plt.xlim([0.01, 300])
	plt.ylim([np.min(MaxPxx[i])-0.5, np.max(MaxPxx[i])+0.5])
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "MaxPxxWi-Sp" + str(Chosen[i]) + ".png"))
	# plt.show()
	plt.close(fig)


#For Plotting Max Stress Against Structure For Each Erate
for i in range (len(MainRate[0])):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	x = np.array(range(0,len(Structure)))
	plt.xticks(x, Structure, rotation=45)
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Structure')
	plt.scatter(x, MaxPxx[:,i], linewidth=2, label='Wi~O(' + '{:1.1f}'.format(MainRate[:,i][0][0]/RlxTime)+')')
	plt.legend(bbox_to_anchor=(0.60, 0.90, .99, .99), loc=3, ncol=len(Rate2), borderaxespad=0., fontsize = 'xx-small')
	plt.xlim([x[0]-1, x[len(x)-1]+1])
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "MaxPxxStruct-R" + str(i+1) + ".png"))
	# plt.show()
	plt.close(fig)



#For Plotting Emod Against Erate For Each Structure
for i in range (len(Chosen)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	plt.semilogx(MainRate[i]*RlxTime, Emod[i], 'o', linewidth=2, label=Structure[i])
	plt.ylabel('Elastic Modulus (MPa)')
	plt.xlabel('Wi')
	plt.legend(bbox_to_anchor=(0.05, 0.88, .3, .99), loc=3, ncol=len(Rate2), borderaxespad=0., fontsize = 'xx-small')
	plt.xlim([0.01, 300])
	plt.ylim([np.min(Emod[i])-0.5, np.max(Emod[i])+0.5])
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "EmodWi-Sp" + str(Chosen[i]) + ".png"))
	# plt.show()
	plt.close(fig)



#For Plotting Max Emod Against Structure For Each Erate
for i in range (len(MainRate[0])):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	x = np.array(range(0,len(Structure)))
	plt.xticks(x, Structure, rotation=45)
	plt.ylabel('Emod (MPa)')
	plt.xlabel('Structure')
	plt.scatter(x, Emod[:,i], linewidth=2, label='Wi~O(' + '{:1.1f}'.format(MainRate[:,i][0][0]/RlxTime)+')')
	plt.legend(bbox_to_anchor=(0.60, 0.88, .99, .99), loc=3, ncol=len(Rate2), borderaxespad=0., fontsize = 'xx-small')
	plt.xlim([x[0]-1, x[len(x)-1]+1])
	minY   = np.min(Emod[:,i])
	maxY   = np.max(Emod[:,i])
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "EmodStruct-R" + str(i+1) + ".png"))
	# plt.show()
	plt.close(fig)




#For Plotting Stress Against Strain For Each Structure
for i in range (len(Time)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	for j in range (len(Time[i])):
		for k in range (len(Time[i,j])):
			# plt.plot(Strain[i,j,k], Pxx[i,j,k], '-', linewidth=2, label='Wi~O(' + '{:1.1f}'.format(MainRate[0,j][0]/RlxTime)+')')
			plt.plot(Strain[i,j,k], Pxx[i,j,k], '-', linewidth=2, label='Wi~' + '{:1.2f}'.format(MainRate[0,j][0]/RlxTime))
	plt.ylim([np.min(np.min(MaxPxx[i]))-0.5, np.max(np.max(MaxPxx[i]))+0.05])
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Strain')
	plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=3, borderaxespad=0., fontsize=6)
	plt.tight_layout()
	plt.xlim([0., 4.51])
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "PxxStrain-Sp" + str(Chosen[i]) + ".png"))
	# plt.show()
	plt.close(fig)



# #For Plotting Stress Against Time For Each Structure
for i in range (len(Time)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	for j in range (len(Time[i])):
		for k in range (len(Time[i,j])):
			# plt.plot(Time[i,j,k]*timeStep*1e-6, Pxx[i,j,k], '-', linewidth=2, label='Wi~O(' + '{:1.1f}'.format(MainRate[0,j][0]/RlxTime)+')')
			plt.plot(Time[i,j,k]*timeStep*1e-6, Pxx[i,j,k], '-', linewidth=2, label='Wi~' + '{:1.2f}'.format(MainRate[0,j][0]/RlxTime))
	plt.ylim([np.min(np.min(MaxPxx[i]))-0.5, np.max(np.max(MaxPxx[i]))+0.5])
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Time (ns)')
	plt.xticks(rotation=45)
	plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=3, borderaxespad=0., fontsize=6)
	plt.tight_layout()
	plt.xticks(rotation=45)
	plt.savefig(os.path.join(WriteFolderName, "PxxTime-Sp" + str(Chosen[i]) + ".png"))
	# plt.show()
	plt.close(fig)



#For Comparing Stress Release Mechanism For Each Structure at each Wi
for j in range (len(Time[0])):
	fig = plt.figure(j, figsize=(3.5, 3.5))
	plt.subplot(111)
	for i in range(len(Time)):
		for k in range(len(Time[i,j])):
			plt.plot(Strain[i,j,k], Pxx[i,j,k], '-', linewidth=2, label=Structure[i])
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Strain')
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.legend(bbox_to_anchor=(0., 0.95, 1., .110), loc=1, ncol=len(Structure), mode="expand", borderaxespad=0., fontsize = 6)
	# plt.xlim([0.4, 9.51])
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "PxxStrain-R" + str(j+1) + ".png"))
	# plt.show()
	plt.close(fig)
