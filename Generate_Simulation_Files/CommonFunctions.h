#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

//1
std::string itos(int i);

//2
std::string itosPW(int i, int precision, int width);

//3
std::string ftos(float i);

//4
std::string ftosPW(float i, int precision, int width);

//5
std::string dtos(double i);

//6
std::string dtosPW(double i, int precision, int width);

//7
float ReturnShortestDistance (float u1, float u2, float BoxLength); 

//8
int CheckClose (float x1, float y1, float z1, float x2, float y2, float z2, float xBoxLength, float yBoxLength, float zBoxLength, float rCut);

//9
std::string FormatString (std::string Text,  std::string Direction, int width);

#endif


