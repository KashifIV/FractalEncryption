#include <vector>
#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "Fractal.h"
#include "bitmap_image.hpp"

#define inf 0x3f3f3f
using namespace std; 

// Contains positioning and how interesting a certain area is. 
struct InterestBox {
	pair<long double, long double> position;
	double interestRating;
	
};
// Calculates the value for the number of escape itterations
// at point (X,Y), MandleBrot Formula: f(z) = z^2 + c
int Fractal::CalculatePixel(long double x, long double y)
{
	double zx = 0;
	double zy = 0;
	int maxiter = iterations;
	int i = 0;
	int value = -1;
	while ((zx * zx) + (zy * zy) < 4.0f && i < maxiter)
	{
		double temp = (zx * zx) - (zy * zy) + x;
		zy = 2 * zx * zy + y;
		zx = temp;
		i++;
	}
	if (i == iterations)
		value = -1;
	else
		value = i;
	return value;
}
//Returns the current point the fractal is in the 
// Complex plane
pair<long double, long double> Fractal::GetPoint()
{
	return point; 
}

// Sets hte current point of the fractal in the complex plane
void Fractal::SetPoint(pair<long double, long double> p)
{ 
	point = p; 
	CalculatePoint(); 

}
// Using the imaginary number, returns the position of the point
// on the screen based on pixels
long double Fractal::GetY(long double y)
{
	return y * ((maxI - minI) / double(sizey)) + minI;
}
// Using the real number, returns the position of the point on
// the screen based on pixels 
long double Fractal::GetX(long double x)
{
	return x * ((maxR - minR) / double(sizex)) + minR;
}
// Calculates the depth by manipulating the real and 
// Imaginary bounds of the fractal 
void Fractal::CalculateDepth()
{
		long double power = maxI - minI;
		power /= 6;
		maxI -= power;
		minI += power;
		power = maxR - minR;
		power /= 6;
		minR += power;
		maxR -= power;
}
//Given a specific Depth (arbitrary) the plane will zoom in
// Note: the center of the fractal (position) will be the anchor point 
void Fractal::ZoomToDepth(int d) {
	depth = d; 
	for (int i = 1; i < d; i++)
	{
		if (i % 5 == 0)
			iterations *= 1.5; 
		CalculateDepth(); 
	}
	CalculateFractal(); 
}
// Using the position in the variable "point", center the fractal such 
// that the point is in the center
void Fractal::CalculatePoint()
{
	long double lenX = maxR - minR;
	long double lenY = maxI - minI;
	maxR = lenX / 2 + point.first; 
	minR = point.first-lenX / 2;
	maxI = lenY / 2 + point.second; 
	minI = point.second-lenY / 2;
}
// Find a random point of interest in the fractal and return 
// its position. 
pair<long double, long double> Fractal::FindPOI()
{
	int widthCut = 19; 
	int heightCut = 15; 
	vector<InterestBox> boxes; 

	for (int i = 0; i < widthCut; i++)
	{
		for (int j = 0; j < heightCut; j++)
		{
			int lowX = (sizex/widthCut)*i; 
			int highX = (sizex/widthCut)*(i + 1); 
			int lowY = (sizey / heightCut)*j; 
			int highY = (sizey / heightCut)*(j + 1); 
			int low = inf;
			int high = -2; 
			for (int y = lowY; y < highY; y++)
			{
				for (int x = lowX; x < highX; x++)
				{
					if (depict[y][x] == -1) {
						depict[y][x] = iterations; 
					}
					if (depict[y][x] > high) {
						high = depict[y][x];
					}
					else if (depict[y][x] < low && depict[y][x] != iterations) {
						low = depict[y][x];
					}
					if (depict[y][x] == iterations)
						depict[y][x] = -1; 
				}
			}
			if (low == inf)
				low = iterations; 
			InterestBox a; 
			a.interestRating = high - low; 
			a.position = std::make_pair(GetX(long double(rand()%(highX-lowX)) + lowX), GetY(long double(rand()%(highY - lowY)) + lowY));
			
			if (a.interestRating > int(double(iterations)*0.9)) {
				boxes.push_back(a);
			}
		}
	}
	cout << "-------------------------" << endl; 
	srand(time(NULL)); 
	if (boxes.size() == 0)
	{
		iterations *= 2; 
		CalculateFractal(); 
		return FindPOI(); 
	}
	return boxes[rand() % boxes.size()].position; 
}
// Construct a 2D vector "depict" that contains the iterations for all pixel
// values that should be on screen 
void Fractal::CalculateFractal()
{ 
	vector<int> a; 
	depict.clear(); 
	for (int y = 0; y < sizey; y++)
	{
		depict.push_back(a); 
		for (int x = 0; x < sizex; x++)
		{
			depict[y].push_back(CalculatePixel(GetX(long double(x)), GetY(long double(y))));
		}
	}
}
//Print all the values in the "Depict" vector
void Fractal::PrintIter()
{
	for (int i = 0; i < sizey; i++)
	{
		for (int j = 0; j < sizex; j++)
		{
			cout << depict[i][j] << '\t'; 
		}
		cout << endl; 
	}
}
//Draw the fractal to the file filename.bmp
void Fractal::DrawFractal(string filename)
{
	bitmap_image img(sizex, sizey);

	//frac->PrintIter(); 
	bitmap_image fractal(sizex, sizey);

	fractal.clear();
	int max = -2; 
	for (int i = 0; i < sizey; i++)
	{
		for (int j = 0; j < sizex; j++)
		{
			int temp = (int)(depict[i][j] / (double)iterations * 255 * 3);
			int b = 255;
			temp -= b;
			if (temp < 0)
				b -= abs(temp);
			int g = 255;
			temp -= g;
			if (temp < 0)
				g -= abs(temp);
			int r = 255;
			temp -= r;
			if (temp < 0)
				r -= abs(temp);
			fractal.set_pixel(j, i, r, g, b); 
		}
	}
	fractal.save_image(filename + ".bmp");
}
//Generates a Key comprising of the current position and number of iterations used, 
//to decrypt the file. Note: All 4 edges of the plane are used to retain precision 
Key Fractal::GetPublicKey() {
	srand(time(NULL));  
	int d = depth; 
	CalculateFractal();
	for (int i = 1; i < d; i++) {
		if (i % 5 == 0)
			iterations *= 1.5;
		SetPoint(FindPOI());
		cout << GetPoint().first << " " << GetPoint().second << std::endl;
		cout << iterations << " " << i << std::endl;
		CalculateDepth();
		CalculateFractal();
	}
	Key a(maxR, minR, maxI, minI, iterations); 
	return a; 
}
//Generate encryption using the iteration values for each pixel in frame, 
// the function will also remove any repition between pixels 
vector<int> Fractal::GenerateEncryption()
{
	int temp = -1; 
	vector<int> a; 
	for (int i = 0; i < sizey; i++)
	{
		for (int j = 0; j < sizex; j++)
		{
			if (depict[i][j] != temp && depict[i][j] != 0) {
				a.push_back(depict[i][j]); 
				temp = depict[i][j]; 					
			}
		}
	}
	return a; 
}
//Using a vector of integers, print all values to a file, 
// used for debugging 
void Fractal::WriteToFile(vector<int> a)
{
	ofstream out ("Encrypt.txt", ofstream::out); 
	for (int i = 0; i < a.size(); i++)
	{
		out << a[i] << " "; 
	}
	out.close(); 
}