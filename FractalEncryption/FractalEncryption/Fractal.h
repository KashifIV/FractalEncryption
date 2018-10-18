#pragma once
#ifndef __Fractal_h__
#define __Fractal_h__
#include <vector> 
using std::vector; 
using std::string; 
using std::pair; 
#define acc long double
struct Key {
	acc maxR;
	acc minR;
	acc maxI;
	acc minI;
	int iterations;
	Key() {}
	Key(acc maxr, acc minr, acc maxi, acc mini, int i) {
		maxR = maxr; 
		minR = minr; 
		maxI = maxi; 
		minI = mini; 
		iterations = i; 
	}
};
class Fractal
{
private: 	 
	pair<long double, long double> point; 
	//vector<vector<int>> depict; 
	int CalculatePixel(long double x, long double y);
	pair<long double, long double> FindPOI();
	long double GetX(long double x);
	long double GetY(long double y);
public:
	int depth;
	Fractal(int sx, int sy, int iter, long double maxr, long double minr, long double maxi, long double mini, int d = 0)
		:sizex(sx), sizey(sy), iterations(iter), maxR(maxr), minR(minr), maxI(maxi), minI(mini), depth(d)
	{
		point = std::make_pair((maxR - minR) / 2, (maxI - minI) / 2);
	}
	long double sizex, sizey, iterations;
	long double maxR, minR, maxI, minI;
	void CalculateFractal();
	void CalculateDepth();
	void CalculatePoint();
	vector<vector<int>> depict;
	void SetPoint(pair<long double, long double> p);
	void ZoomToDepth(int d); 
	vector<int> GetPrivateKey(int depth, pair< long double, long double> pt);
	void WriteToFile(vector<int> a);
	vector<int> GenerateEncryption(); 
	Key GetPublicKey();
	pair<long double, long double> GetPoint();
	void PrintIter(); 
	void DrawFractal(string filename); 
};

#endif 