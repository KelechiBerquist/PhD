#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#include "CommonFunctions.h"


//1
std::string itos(int i)
{
	std::stringstream s;
	s << i;
	return s.str();
}

//2
std::string itosPW(int i, int precision, int width)
{
	std::stringstream s;
	s  <<std::setw(width)<<std::fixed<<std::setprecision (precision) << i;
	return s.str();
}

//3
std::string ftos(float i)
{
	std::stringstream s;
	s << i;
	return s.str();
}

//4
std::string ftosPW(float i, int precision, int width)
{
	std::stringstream s;
	s  <<std::setw(width)<<std::fixed<<std::setprecision (precision) << i;
	return s.str();
}

//5
std::string dtos(double i)
{
	std::stringstream s;
	s << i;
	return s.str();
}

//6
std::string dtosPW(double i, int precision, int width)
{
	std::stringstream s;
	s  <<std::setw(width)<<std::fixed<<std::setprecision (precision) << i;
	return s.str();
}

//7
float ReturnShortestDistance (float u1, float u2, float BoxLength)
{
	float Min;
	
	std::vector <float> Dist = {u2-u1, u2-u1-BoxLength, u2-u1+BoxLength};
	
	Min = Dist[0];
	for (int a0 = 0;  a0 < Dist.size();  a0++) 
	{
		if (Min*Min > Dist[a0]*Dist[a0])
		{  
			Min = Dist[a0];  
		}
	}
	
	return Min;
}

//8
int CheckClose (float x1, float y1, float z1, float x2, float y2, float z2, float xBoxLength, float yBoxLength, float zBoxLength, float rCut)
{
	float dx, dy, dz, distanceCheck = 0.0;
	int   checkReturnValue = 9;
	
 	dx = ReturnShortestDistance(x1,x2,xBoxLength); 
 	
	distanceCheck += dx*dx; 
	
	if (distanceCheck <= rCut)
	{
		dy = ReturnShortestDistance(y1,y2,yBoxLength);
		distanceCheck += dy*dy; 
		
		if (distanceCheck <= rCut)
		{
			dz = ReturnShortestDistance(z1,z2,zBoxLength);
			distanceCheck += dz*dz; 
			
			if (distanceCheck <= rCut)
			{
				checkReturnValue = 2;
			}
		}
	}
	
	return checkReturnValue;
}



////9
std::string FormatString (std::string Text,  std::string Direction, int width)
{
	if (Direction == "left")
	{
		std::stringstream s;
		s <<std::left <<std::setw(width) << Text;
		return s.str();
	}
	else if (Direction == "right")
	{
		std::stringstream s;
		s  <<std::right <<std::setw(width) << Text;
		return s.str();
	}
	else if (Direction == "")
	{
		std::stringstream s;
		s  <<std::setw(width) << Text;
		return s.str();
	}
}

