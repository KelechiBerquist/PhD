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
ContentFile       =   []
MaxPxx            =   []
Rate              =   [0.1, 0.3, 1, 3, 10, 30]
Emod              =   []
EmodMicelle       =   []
Pxx               =   []
Time              =   []
SyStrain          =   []
MiStrain          =   []
timeStep          =   30
timeIdx           =   0
syStrainIdx       =   1
pxxIdx            =   4
miStrainIdx       =   3
allIdx            =   9
freq              =   0.06
countFreq         =   0
countTime         =   1
avePxx            =   0
RlxTime           =   3
emodCutOff        =   0.2
legendCols        =   3
convDefRate       =   1e-6
direct            =   "X"
rootDir           =   "/media/KaceHDD1/Struct/"
WriteFolderName   =   "/media/KaceHDD1/Struct/ImageVideos/Load_Images"
# rootDir = "/media/kelechi/8TB-Kay/DifferentStructures/"
# WriteFolderName   =  "/mnt/f/Struct/ImagesVideos/"





for chosen in Chosen:
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

		# ThisTime             =   np.array(ThisTime)
		# ThisStrain           =   np.array(ThisStrain)
		# ThisPxx              =   np.array(ThisPxx)

		
		MaxPxx.append(np.max(ThisPxx))
		Emod.append(GetSlope (X, Y))
		Time.append(ThisTime)
		SyStrain.append(ThisStrain)
		Pxx.append(ThisPxx)


		# StrainLoad-Rate0.1-Spn4-Y.dat
		ReadFolderName    =  "/media/KaceHDD1/Struct/ImageVideos/Load_Struct/"
		ReadFileNameEnd   =  "Load-Rate" + str(rate) + "-Spn" + str(chosen) + "-" + direct
		ReadFileName      =  os.path.join(ReadFolderName, "Strain" + ReadFileNameEnd  + ".dat")
		ReadFile          =  open(ReadFileName, 'r')
		ThisFile          =  ReadFile.readlines()
		ReadFile.close()
		ThatFile          =  []
		XMicelle          =  []
		aveStr            =  0
		countFreq         =  1
		countTime         =  1
		forPrint          =  ""

		for line in ThisFile:
			if "# Fix" in line:
				continue
			else:
				if (round(float(line.split()[miStrainIdx]), 2) % freq == 0):
					ThatFile.append(round(float(line.split()[miStrainIdx]), 2))

				# if (round(float(line.split()[miStrainIdx]), 2) <= 0.12):
				if (round(float(line.split()[miStrainIdx]), 2) <= emodCutOff):
					XMicelle.append(round(float(line.split()[miStrainIdx]), 2))
					forPrint += str(round(float(line.split()[miStrainIdx]), 2)) + "  "
		MiStrain.append(ThatFile)

		# print(len(XMicelle), len(Y), forPrint)
		if (rate > 0.3 and chosen > 1):
			# print(len(XMicelle), len(ThisPxx[:len(XMicelle)]), forPrint)
			EmodMicelle.append(GetSlope (XMicelle, ThisPxx[:len(XMicelle)]))




			
			
MaxPxx            =   np.array(MaxPxx)
Rate              =   np.array(Rate)
Emod              =   np.array(Emod)
EmodMicelle       =   np.array(EmodMicelle)
Time              =   np.array(Time)
SyStrain          =   np.array(SyStrain)
MiStrain          =   np.array(MiStrain)
Pxx               =   np.array(Pxx)

MaxPxx.shape      =   (len(Chosen), len(Rate), -1)
Emod.shape        =   (len(Chosen), len(Rate), -1)
EmodMicelle.shape =   (len(Chosen)-1, len(Rate)-2, -1)
Time.shape        =   (len(Chosen), len(Rate), -1)
SyStrain.shape    =   (len(Chosen), len(Rate), -1)
MiStrain.shape    =   (len(Chosen), len(Rate), -1)
Pxx.shape         =   (len(Chosen), len(Rate), -1)

print(MaxPxx.shape,  Rate.shape, Emod.shape, EmodMicelle.shape, MiStrain.shape, Time.shape, SyStrain.shape, Pxx.shape)




#For Plotting Max Stress Against Erate For Each Structure
for i in range (len(Chosen)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	plt.semilogx(Rate*RlxTime, MaxPxx[i], 'o', linewidth=2, label=Structure[i])
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Wi')
	plt.legend(bbox_to_anchor=(0.05, 0.90, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((-0.01+0.95*yminThis, 1.1*ymaxThis))
	plt.xlim((-0.01+0.95*xminThis, 1.05*xmaxThis))
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "MaxPxxWi-Sp" + str(Chosen[i]) + ".png"))
	# plt.show()
	plt.close(fig)




#For Plotting Max Stress Against Structure For Each Erate
for i in range (len(Rate)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	x = np.array(range(0,len(Structure)))
	plt.xticks(x, Structure, rotation=45)
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Structure')
	plt.scatter(x, MaxPxx[:,i], linewidth=2, label='Wi~O(' + '{:1.1f}'.format(Rate[i]*RlxTime)+')')
	plt.legend(bbox_to_anchor=(0.60, 0.90, .99, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "MaxPxxStruct-R" + str(i+1) + ".png"))
	# plt.show()
	plt.close(fig)




#For Plotting Emod Against Erate For Each Structure
for i in range (len(Chosen)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	plt.semilogx(Rate*RlxTime, Emod[i], 'o', linewidth=2, label=Structure[i])
	plt.ylabel('Elastic Modulus (MPa)')
	plt.xlabel('Wi')
	plt.legend(bbox_to_anchor=(0.05, 0.88, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "EmodWi-Sp" + str(Chosen[i]) + ".png"))
	# plt.show()
	plt.close(fig)




#For Plotting Max Emod Against Structure For Each Erate
for i in range (len(Rate)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	x = np.array(range(0,len(Structure)))
	plt.xticks(x, Structure, rotation=45)
	plt.ylabel('Emod (MPa)')
	plt.xlabel('Structure')
	plt.scatter(x, Emod[:,i], linewidth=2, label='Wi~O(' + '{:1.1f}'.format(Rate[i]*RlxTime)+')')
	plt.legend(bbox_to_anchor=(0.60, 0.88, .99, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "EmodStruct-R" + str(i+1) + ".png"))
	# plt.show()
	plt.close(fig)




#For Plotting Stress Against SyStrain For Each Structure
for i in range (len(Chosen)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	for j in range (len(Rate)):
		# for k in range (len(Time[i,j])):
			# plt.plot(SyStrain[i,j,k], Pxx[i,j,k], '-', linewidth=2, label='Wi~O(' + '{:1.1f}'.format(Rate[0,j][0]/RlxTime)+')')
		plt.plot(SyStrain[i,j], Pxx[i,j], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[j]*RlxTime))
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Box Strain')
	plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "PxxStrain-Sp" + str(Chosen[i]) + ".png"))
	# plt.show()
	plt.close(fig)




#For Comparing Stress Release Mechanism For Each Structure at each Wi
for j in range (len(Rate)):
	fig = plt.figure(j, figsize=(3.5, 3.5))
	plt.subplot(111)
	for i in range(len(Chosen)):
		# for k in range(len(Time[i,j])):
		plt.plot(SyStrain[i,j], Pxx[i,j], '-', linewidth=2, label=Structure[i])
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Box Strain')
	plt.xticks(rotation=45)
	plt.legend(bbox_to_anchor=(0., 0.95, 1., .110), loc=1, ncol=legendCols, mode="expand", borderaxespad=0., fontsize = 6)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "PxxStrain-R" + str(j+1) + ".png"))
	# plt.show()
	plt.close(fig)










# #For Plotting Emod Against Erate For Each Structure
# for i in range (1, len(Chosen), 1):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	plt.semilogx(Rate[2:]*RlxTime, EmodMicelle[i-1], 'o', linewidth=2, label=Structure[i])
# 	plt.ylabel('Elastic Modulus (MPa)')
# 	plt.xlabel('Wi')
# 	plt.legend(bbox_to_anchor=(0.05, 0.88, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
# 	yminThis, ymaxThis = plt.ylim()
# 	xminThis, xmaxThis = plt.xlim()
# 	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
# 	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "EmodMiWi-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)



# #For Plotting Stress Against MiStrain For Each Structure
# for i in range (len(Chosen)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	for j in range (len(Rate)):
# 		# plt.plot(SyStrain[i,j,k], Pxx[i,j,k], '-', linewidth=2, label='Wi~O(' + '{:1.1f}'.format(Rate[0,j][0]/RlxTime)+')')
# 		plt.plot(MiStrain[i,j,0], Pxx[i,j][:len(MiStrain[i,j,0])], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[j]*RlxTime))
# 	yminThis, ymaxThis = plt.ylim()
# 	xminThis, xmaxThis = plt.xlim()
# 	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
# 	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 	plt.ylabel('Stress (MPa)')
# 	plt.xlabel('Micelle Strain')
# 	plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
# 	plt.xticks(rotation=45)
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "PxxMiStrain-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)


# #For Plotting Stress Against MiStrain For Each Structure
# for i in range (len(Chosen)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	for j in range (len(Rate)):
# 		plt.plot(SyStrain[i,j][:len(MiStrain[i,j,0])],  MiStrain[i,j,0], '--', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[j]*RlxTime))
# 	yminThis, ymaxThis = plt.ylim()
# 	xminThis, xmaxThis = plt.xlim()
# 	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
# 	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 	plt.ylabel('Micelle Strain')
# 	plt.xlabel('Time (ns)')
# 	plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
# 	plt.xticks(rotation=45)
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "MiStrainTime-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)




# # #For Plotting Stress Against Time For Each Structure
# for i in range (len(Time)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	for j in range (len(Time[i])):
# 		for k in range (len(Time[i,j])):
# 			# plt.plot(Time[i,j,k]*timeStep*1e-6, Pxx[i,j,k], '-', linewidth=2, label='Wi~O(' + '{:1.1f}'.format(Rate[0,j][0]/RlxTime)+')')
# 			plt.plot(Time[i,j,k], Pxx[i,j,k], '-', linewidth=2, label='Wi~' + '{:1.1f}'.format(Rate[j]*RlxTime))
# 	yminThis, ymaxThis = plt.ylim()
# 	xminThis, xmaxThis = plt.xlim()
# 	plt.ylim((0.95*yminThis, 1.1*ymaxThis))
# 	plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 	plt.ylabel('Stress (MPa)')
# 	plt.xlabel('Time (ns)')
# 	plt.xticks(rotation=45)
# 	plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "PxxTime-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)












# 		ReadFolderName    =  os.path.join(rootDir, str(chosen), direct, "Load")
# 		ReadFileNameEnd   =  "Load-Rate" + str(rate) + "-Spn" + str(chosen) + "-En1"
# 		ReadFileName      =  os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd  + ".dat")
# 		ReadFile          =  open(ReadFileName, 'r')
# 		ThisFile          =  ReadFile.readlines()
# 		ReadFile.close()
# 		ThatFile         =  []
# 		for line in ThisFile:
# 			if "# Fix" in line:
# 				continue
# 			else:
# 				ThatFile.append([float(i) for i in line.split()])
# 		ContentFile.append(ThatFile)


# ContentFile          =  np.array(ContentFile)
# ContentFile.shape    =  (len(Chosen), len(Rate), -1)
# print(ContentFile1.shape, ContentFile2.shape)
# print(ContentFile1.dtype.name)
# print(ContentFile2.dtype.name)
# ContentFile1          =  ContentFile1.astype(float)
# ContentFile2          =  ContentFile2.astype(float)



# for i in range (ContentFile.shape[0] ):
# 	for j in range(ContentFile[i].shape[0]):
# 		for k in range(ContentFile[i,j].shape[0]):
# 			forTime     = 0
# 			forSyStrain = 1
# 			forPxx      = 2
# 			# Data        = GetTimeSyStrainPxx (ContentFile[i,j,k])
# 			# Time.append(Data[:,forTime])
# 			# SyStrain.append(Data[:,forSyStrain])
# 			# Pxx.append(Data[:,forPxx])
# 			MaxPxx.append(GetMaxPxx (ContentFile[i,j,k]))
# 			Emod.append(GetEmod (ContentFile[i,j,k]))

			
