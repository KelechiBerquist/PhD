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




# Chosen          = [1, 2, 3, 4]
Chosen            =   [1, 2, 3]
Structure         =   ['Sph', 'RLM', 'WLM', 'BWLM']
Rate              =   [1, 3, 10, 30]
SimType           =   ['Load', 'UnLoad']
RevTime           =   []
Time              =   []
RevStrain         =   []
Strain            =   []
Pxx               =   []
Emod              =   []
RelaxRate         =   []
CurveArea         =   []
FitData           =   []

timeStep          =    40
timeIdx           =    0
strainIdx         =    1
pxxIdx            =    4
convDefRate       =    1e-6
countFreq         =    0
countTime         =    1
avePxx            =    0
RlxTime           =    3
emodCutOff        =    0.2
freqStrain        =    0.05
Encore            =    3
strain            =    1
convDefRate       =    1e-6

direct            =   "X"
rootDir           =   "/media/KaceHDD1/Struct/"
WriteFolderName   =   "/media/KaceHDD1/Struct/ImageVideos/Cycle_Images/"



# ****************************************************
def func(x, a, b, c):
	return a * np.exp(-b * x) + c
# 
# 
def GetSlope (X, Y):
	slope, intercept, r_value, p_value, std_err = linregress(X, Y)
	return slope
# 
# ****************************************************


for chosen in Chosen:
	ReadFolderName    =  os.path.join(rootDir, str(chosen), direct, 'Cycle')

	for rate in  Rate:
		freq  = math.ceil(freqStrain/(timeStep*rate*1e-6))
	
		for simType in  SimType:
			for encore in  range (Encore):
				# /media/KaceHDD1/Struct/2/X/Cycle/MicelleFixUnLoad-Rate1-Spn2-S3.dat
				
				ReadFileNameEnd   =  simType + "-Rate" + str(rate) + "-Spn" + str(chosen) + "-En" + str(encore+1)
				ReadFileName      =  os.path.join(ReadFolderName, "MicelleFix" + ReadFileNameEnd + ".dat")
				ReadFile          =  open(ReadFileName, 'r')
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
					elif (rate*convDefRate*timeStep*float(line.split()[timeIdx]) <= strain):
						if (float(line.split()[timeIdx]) > (countTime)*freq):
							ThisPxx.append(10*avePxx/countFreq )
							ThisTime.append(timeStep*convDefRate*int(line.split()[timeIdx]))
							ThisStrain.append(round(float(line.split()[strainIdx]), 2) )

							if (rate*convDefRate*timeStep*float(line.split()[timeIdx]) <= emodCutOff):
								X.append(rate*convDefRate*timeStep*float(line.split()[timeIdx]))
								Y.append(10*avePxx/countFreq)

							avePxx     =  0
							countFreq  =  1
							countTime  += 1

						if (float(line.split()[timeIdx]) <= (countTime)*freq):
							avePxx     += float(line.split()[pxxIdx])
							countFreq  += 1

				ThisTime             =   np.array(ThisTime)
				ThisStrain           =   np.array(ThisStrain)
				ThisPxx              =   np.array(ThisPxx)

				ThisRevTime          =   ThisTime[-1]*np.ones(len(ThisTime))     - ThisTime
				ThisRevStrain        =   ThisStrain[-1]*np.ones(len(ThisStrain)) - ThisStrain

				if simType == 'Load':
					Emod.append(GetSlope (X, Y))
				Time.append(ThisTime)
				Strain.append(ThisStrain)
				Pxx.append(ThisPxx)
				RevTime.append(ThisRevTime)
				RevStrain.append(ThisRevStrain)

				# ThisTime.reverse()
				# ThisStrain.reverse()
			# TheFunc = LoadPxx[:minLength] - UnLoadPxx[:minLength]
			# CurveArea.append(np.trapz(TheFunc, x=LoadStrain[:minLength]))	
				
				

Rate             =   np.array(Rate)
Emod             =   np.array(Emod)
RevTime          =   np.array(RevTime)
Time             =   np.array(Time)
Strain           =   np.array(Strain)
RevStrain        =   np.array(RevStrain)
Pxx              =   np.array(Pxx)

Emod.shape       =   (len(Chosen), len(Rate), len(SimType)-1, Encore)
RevTime.shape    =   (len(Chosen), len(Rate), len(SimType), Encore)
Time.shape       =   (len(Chosen), len(Rate), len(SimType), Encore)
RevStrain.shape  =   (len(Chosen), len(Rate), len(SimType), Encore)
Strain.shape     =   (len(Chosen), len(Rate), len(SimType), Encore)
Pxx.shape        =   (len(Chosen), len(Rate), len(SimType), Encore)

print(RevTime.shape, Time.shape, Strain.shape, RevStrain.shape, Pxx.shape)
# print (RevStrain[0, 0, 0, 0])




# ORDER: (len(Chosen), len(Rate), len(SimType), Encore)
#For Plotting Area Against Strain For Each Structure and Erate
for i in range (len(Chosen)):
	# for j in range (len(Rate)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	for k in range (Encore):
		plt.semilogx(Rate*RlxTime, Emod[i, :, 0, k], 'o', linewidth=2, label='R:'+'{:1.0f}'.format(k+1))
	plt.ylabel('Elastic Modulus (MPa)')
	plt.xlabel('Wi')
	plt.legend(bbox_to_anchor=(0., 1.0, 1., .104), loc=3, ncol=len(Rate), mode="expand", borderaxespad=0., fontsize = 'xx-small')
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((0.90*yminThis, 1.1*ymaxThis))
	plt.xlim((0.90*xminThis, 1.05*xmaxThis))
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "Emod-Sp" + str(Chosen[i]) + ".png"))
	# plt.show()
	plt.close(fig)



# ORDER: (len(Chosen), len(Rate), len(SimType), Encore)
#For Plotting Pxx Against Strain For Each Structure and Erate Part1
for i in range (len(Chosen)):
	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	for k in range (Encore):
		for j in range (len(Rate)-2):
			plt.plot(Strain[i, j, 0, k],    Pxx[i, j, 0, k], '-', linewidth=2, label='W~' + str(Rate[j]*RlxTime)+',R:'+'{:1.0f}'.format(k+1))
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Strain')
	plt.legend(bbox_to_anchor=(0., 0.9, 1., .104), loc=3, ncol=len(Rate)-1, mode="expand", borderaxespad=0., fontsize = 'xx-small')
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((0.90*yminThis, 1.1*ymaxThis))
	plt.xlim((-0.01+xminThis, 1.1*xmaxThis))
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "TimeStrainLoad-Sp" + str(Chosen[i]) + '-PR1' +  ".png"))
	# plt.show()
	plt.close(fig)


	fig = plt.figure(i, figsize=(3.5, 3.5))
	plt.subplot(111)
	for k in range (Encore):
		for j in range (len(Rate)-2, len(Rate), 1):
			plt.plot(Strain[i, j, 0, k],    Pxx[i, j, 0, k], '-', linewidth=2, label='W~' + str(Rate[j]*RlxTime)+',R:'+'{:1.0f}'.format(k+1))
	plt.ylabel('Stress (MPa)')
	plt.xlabel('Strain')
	plt.legend(bbox_to_anchor=(0., 0.9, 1., .104), loc=3, ncol=len(Rate)-1, mode="expand", borderaxespad=0., fontsize = 'xx-small')
	yminThis, ymaxThis = plt.ylim()
	xminThis, xmaxThis = plt.xlim()
	plt.ylim((0.90*yminThis, 1.1*ymaxThis))
	plt.xlim((-0.01+xminThis, 1.1*xmaxThis))
	plt.xticks(rotation=45)
	plt.tight_layout()
	plt.savefig(os.path.join(WriteFolderName, "TimeStrainLoad-Sp" + str(Chosen[i]) + '-PR2' +  ".png"))
	# plt.show()
	plt.close(fig)


# # ORDER: (len(Chosen), len(Rate), len(SimType), Encore)
# #For Plotting Area Against Strain For Each Structure and Erate
# for i in range (len(Chosen)):
# 	for j in range (len(Rate)):
# 		fig = plt.figure(i, figsize=(3.5, 3.5))
# 		plt.subplot(111)
# 		for k in range (Encore):
# 			plt.plot(Strain[i, j, 0, k],    Pxx[i, j, 0, k], '-', linewidth=2, label='W~' + str(Rate[j]*RlxTime)+',R:'+'{:1.0f}'.format(k+1))
# 		plt.ylabel('Stress (MPa)')
# 		plt.xlabel('Strain')
# 		plt.legend(bbox_to_anchor=(0., 1.0, 1., .104), loc=3, ncol=len(Rate), mode="expand", borderaxespad=0., fontsize = 'xx-small')
# 		yminThis, ymaxThis = plt.ylim()
# 		xminThis, xmaxThis = plt.xlim()
# 		plt.ylim((0.90*yminThis, 1.1*ymaxThis))
# 		plt.xlim((-0.01+xminThis, 1.1*xmaxThis))
# 		plt.tight_layout()
# 		plt.savefig(os.path.join(WriteFolderName, "TimeStrainLoad-Sp" + str(Chosen[i]) + '-R' + str(j+3) +  ".png"))
# 		# plt.show()
# 		plt.close(fig)





# # ORDER: (len(Chosen), len(Rate), len(SimType), len(AccStrain))
# #For Plotting Area Against Strain For Each Structure and Erate
# for i in range (len(Chosen)):
# 	for j in range (len(Rate)):
# 		fig = plt.figure(j, figsize=(3.5, 3.5))
# 		plt.subplot(111)
# 		for k in range (len(AccStrain)):
# 			# for l in range (len(SimType)):
# 			plt.plot(Time[i, j, k, 0],    Pxx[i, j, k, 0], 'o', linewidth=2, label='Load ')
# 			# plt.plot(RevTime[i, j, k, 1], Pxx[i, j, k, 1], '.', linewidth=2, label='Unload ')

# 			# plt.plot(Strain[i, j, k, 0],    Pxx[i, j, k, 0], '-', linewidth=2, label='Load ')
# 			# plt.plot(RevStrain[i, j, k, 0], Pxx[i, j, k, 1], '-', linewidth=2, label='Unload ')

# 			# plt.plot(Strain[i, j, k, 0],    Pxx[i, j, k, 0], '-', linewidth=2, label='Load W~O(' + str(Rate[j]*RlxTime)+')')
# 			# plt.plot(RevStrain[i, j, k, 0], Pxx[i, j, k, 1], '-', linewidth=2, label='Unload W~O(' + str(Rate[j]*RlxTime)+')')
# 		plt.ylabel('Stress (MPa)')
# 		plt.xlabel('Time (ns')
# 		# plt.legend(bbox_to_anchor=(0.005, 0.87, .2, .99), loc=3, ncol=2, borderaxespad=0., fontsize = 'xx-small')
# 		plt.legend(bbox_to_anchor=(0., 1.0, 1., .104), loc=3, ncol=math.ceil(len(Rate)/2), mode="expand", borderaxespad=0., fontsize = 'xx-small')
# 		yminThis, ymaxThis = plt.ylim()
# 		xminThis, xmaxThis = plt.xlim()
# 		plt.ylim((0.90*yminThis, 1.1*ymaxThis))
# 		plt.xlim((-0.01+xminThis, 1.05*xmaxThis))
# 		plt.tight_layout()
# 		# plt.savefig(os.path.join(WriteFolderName, "AreaFull-Sp" + str(Chosen[i]) + ".png"))
# 		# plt.show()
# 		plt.close(fig)


