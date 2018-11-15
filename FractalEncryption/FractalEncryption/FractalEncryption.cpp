// FractalEncryption.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream> 
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdio>
#include <limits>
#include "Fractal.h"


using namespace std;

// Initialize the inital bounding variables for the fractal, screen size and number of iterations 
// Note: Consider increasing the screen dimensions 
void InitializeAutomatic(int &sizex, int &sizey, int &iterations, float &minreal, float &maxreal, float &mini, float &maxi)
{
	sizex = 1280;
	sizey = 720;
	iterations = 1000;
	minreal = -2.13f;
	maxreal = 1.33f;
	mini = -1.0f;
	maxi = 1.0f;
}
//If you'd like to implement your own default values they can be passed through the CLI by utilizing this function. 
void InitializeInput(int &sizex, int &sizey, int &iterations, float &minreal, float &maxreal, float &mini, float &maxi)
{
	cin >> sizex >> sizey >> iterations;
	cin >> minreal >> maxreal >> mini >> maxi;
	return;
}
// Encrypt the file to (filename)En.txt, passing in the vector of encryption values 
// Note: The file is encrypted using a binary read over. This can be used for files other then txts, however the
// decryption method can't decrypt from this state. 
// Support for files other then txt will hopefully be supported in a later update 
bool EncryptFile(string filename, vector<int> a)
{
	char ch;
	string newF = filename + "En" + ".txt";
	filename += ".txt";
	ofstream fn(newF.c_str());
	ifstream fl(filename.c_str(), ios::binary | ios::ate);
	int size = fl.tellg();
	fl.seekg(0, ios::beg);
	vector<char> buffer;
	buffer.resize(size);
	fl.read(buffer.data(), size);
	//ifstream fl(filename.c_str());
	//string buffer((std::istreambuf_iterator<char>(fl)),istreambuf_iterator<char>());
	cout << buffer.size() << endl; 
	long int counter = 0;
	for (int i = 0; i < buffer.size(); i++)
	{ 
		buffer[i] += (a[counter] % 94) + 32;
		fn << buffer[i];
		counter++;
		if (counter == a.size())
			counter = 0;		
	}
	fl.close();
	fn.close(); 
	return true; 
}
//Decrypt the file (filename)De.txt, passing in a vector of decryption values 
//Note: at the moment this method can only decrypt txts. 
bool DecryptFile(string filename, vector<int> a)
{
	char ch;
	string newF = filename + "De" + ".txt";
	filename += ".txt";
	ofstream fn(newF.c_str());
	/*ifstream fl(filename.c_str(), ios::binary | ios::ate);
	int size = fl.tellg();
	fl.seekg(0, ios::beg);
	vector<char> buffer;
	buffer.resize(size);
	fl.read(buffer.data(), size);*/
	ifstream fl(filename.c_str());
	string buffer((std::istreambuf_iterator<char>(fl)), istreambuf_iterator<char>());
	cout << buffer.size() << endl; 
	long int counter = 0;
	for (int i = 0; i < buffer.size(); i++)
	{ 		
		buffer[i] -= (a[counter] % 94) + 32;
		fn << buffer[i];
		counter++;
		if (counter == a.size()) { 
			counter = 0;
		}
	}
	fl.close();
	fn.close();
	return true;
}
// The CLI and controlling method for encrypting or decrypting a file 
void EncryptionMode() {
	int sizex, sizey, iterations;
	float minreal, maxreal, mini, maxi;
	cout << "Are you Encrypting or Decrypting? (e/d) : ";
	char en;
	cin >> en;
	if (en == 'e')
	{
		InitializeAutomatic(sizex, sizey, iterations, minreal, maxreal, mini, maxi);
		Fractal *frac = new Fractal(sizex, sizey, iterations, maxreal, minreal, maxi, mini, 4);
		string file;
		cout << endl << "What is the name of the file you'd like to encrypt? : ";
		cin >> file;
		cout << endl << "Maximum depth of your encryption? : ";
		cin >> frac->depth;
		cout << endl << "GENERATING FRACTAL";
		Key key = frac->GetPublicKey();
		cout << endl << "ENCRYPTING";
		if (EncryptFile(file, frac->GenerateEncryption()))
		{
			cout << endl << "Encryption Complete, would you like a copy of the Fractal used for your Encryption? (y/n):";
			cin >> en;
			if (en == 'y')
			{
				frac->DrawFractal(file + "Fractal");
				cout << endl << "The fractal was saved as " + file + "Fractal.bmp";
			}
			cout << endl << "Would you like your key saved to a file? (y/n): ";
			cin >> en;
			if (en == 'y')
			{
				ofstream out(file + "Key.txt");
				typedef std::numeric_limits< double > dbl; 
				out.precision(dbl::max_digits10); 
				out << key.maxR << " " << key.minR <<  " " << key.maxI << " " << key.minI << " " << key.iterations;
				out.close();
				cout << endl << "The File was Saved to " + file + "Key.txt" << endl;
			}

		}
		else
		{
			cout << endl << "Failed to open file" << endl;
		}

	}
	else {

		cout << endl << "What file would you like Decrypt: ";
		string file;
		cin >> file;
		cout << endl << "Please enter your keys: ";
		Key pt;
		cin >> pt.maxR >> pt.minR >> pt.maxI >> pt.minI >> pt.iterations;
		cout << endl << "FINDING DECRYPTION";
		InitializeAutomatic(sizex, sizey, iterations, minreal, maxreal, mini, maxi);
		Fractal *frac = new Fractal(sizex, sizey, pt.iterations, pt.maxR, pt.minR, pt.maxI, pt.minI, 1);
		frac->CalculateFractal(); 
		frac->DrawFractal("Decrypt"); 
		cout << endl << "DECRYPTING";
		if (DecryptFile(file, frac->GenerateEncryption()))
		{
			cout << endl << "Decryption Complete!" << endl;
		}
		else
			cout << endl << "Decryption failed" << endl;

	}

}
//Check if two vectors are the same, outputing any outliers 
//Note: this is primarily used for testing 
bool CheckSame(vector<vector<int>> a, vector<vector<int>> b) {
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = 0; j < a[i].size(); j++) {
			if (a[i][j] != b[i][j])
				cout << "(" << a[i][j] << ", " << b[i][j] << " )" << " (" << i << ", " << j << " )" << endl;
		}
	}
	return true; 
}
// Print the values for all main values of the fractal 
void PrintAllValues(Fractal *frac)
{
	cout << "--------------------------------------------------" << endl; 
	cout << "maxr = " << setprecision(20) << frac->maxR << endl; 
	cout << "maxi = " << setprecision(20) << frac->maxI << endl;
	cout << "minr = " << setprecision(20) << frac->minR << endl;
	cout << "minI = " << setprecision(20) << frac->minI << endl;
	cout << "depth = " << frac->depth << endl; 
	cout << "iterations = " << frac->iterations << endl; 
	cout << "--------------------------------------------------" << endl;
}
// Used for exploring and debugging issues related to the fractal 
void ExploreFractal()
{
	int sizex, sizey, iterations;
	float minreal, maxreal, mini, maxi;
	InitializeAutomatic(sizex, sizey, iterations, minreal, maxreal, mini, maxi);
	Fractal *frac = new Fractal(sizex, sizey, iterations, maxreal, minreal, maxi, mini, 8);
	Key  key = frac->GetPublicKey();
	vector<int> enc = frac->GenerateEncryption(); 
	frac->DrawFractal("First"); 
	PrintAllValues(frac); 
	Fractal *frac2 = new Fractal(sizex, sizey, frac->iterations, frac->maxR, frac->minR, frac->maxI, frac->minI, 1);	 
	frac2->CalculateFractal(); 
	frac2->DrawFractal("Second");
	PrintAllValues(frac2); 
	CheckSame(frac->depict, frac2->depict); 

}
int main()
{
	//ExploreFractal(); 
	EncryptionMode(); 
	return 0; 
}

