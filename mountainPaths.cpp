#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
// #include "functions.h"

using namespace std;

// function that prompts user for input
int ask_user(string const& prompt){
	int x;
	cout << prompt;
	cin >> x;
	cout << endl;
	return x;
}

// function that prompts user for input
string ask_user_string(string const& prompt){
	string filename;
	cout << prompt;
	cin >> filename;
	ifstream infile(filename);
	if(!infile.is_open())
		cout << "Error: Cannot open file." << endl;
	else
	return filename;
}

vector<vector<int>> valuesVector(int rows, int columns,string filename){
	vector<vector<int>> valuesVector (rows, vector<int>(columns));
	int number;
	int numberCount;
	ifstream infile(filename);
	while (infile >> number){
		numberCount++;
	}
	int userNumberCount = rows * columns;
	// if (userNumberCount!=numberCount){
	// 	cout << "Error: columns x rows don't match elements in file." << endl;
	// }
	// else{
		// stores values from file to vector
		infile.clear();
		infile.seekg(0, ios::beg);
		for (int i = 0; i < rows; i++){
			for (int j = 0; j < columns; j++){
				infile >> number;
				valuesVector.at(i).at(j) = number;
			}
		}
	//}
	return valuesVector;
}

int findMax(int rows, int columns, const vector<vector<int>>& v){
	int max = v.at(0).at(0);
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			if (v.at(i).at(j) > max){
				max = v.at(i).at(j);
			}
		}
	}
	return max;
}

int findMin(int rows, int columns, const vector<vector<int>>& v){
	int min = v.at(0).at(0);
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			if (v.at(i).at(j) < min){
				min = v.at(i).at(j);
			}
		}
	}
	return min;
}

void fillVector(vector<vector<int>>& r, const vector<vector<int>>& v, int rows, int columns, int min, int max){
	int color;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			color = ((static_cast<double>(v.at(i).at(j)-min)/static_cast<double>(max-min))*255.0);
			r.at(i).at(j) = color;
		}
	}
}

int colorPath(const vector<vector<int>>& heightMap, vector<vector<int>>& r, vector<vector<int>>& g, vector<vector<int>>& b, int color_r, int color_g, int color_b, int start_row){
	int i = start_row;
	int j = 0;
	int differenceUp;
	int differenceDown;
	int differenceFwd;
	int distance = 0;
	for (j = 0; j < heightMap[0].size();){
		r.at(i).at(j) = color_r;
		g.at(i).at(j) = color_g;
		b.at(i).at(j) = color_b;
		if (i > 0 && j+1 < heightMap[0].size()){
			differenceUp = heightMap.at(i).at(j)-heightMap.at(i-1).at(j+1);
			differenceUp = abs(differenceUp);
		}else{
			differenceUp = 100000;
		}
		if (i + 1 < heightMap.size() && j+1 < heightMap[0].size()){
			differenceDown = heightMap.at(i).at(j)-heightMap.at(i+1).at(j+1);
			differenceDown = abs(differenceDown);
		}else{
			differenceDown = 100000;
		}
		if (i < heightMap.size() && j+1 < heightMap[0].size()){
			differenceFwd = heightMap.at(i).at(j)-heightMap.at(i).at(j+1);
			differenceFwd = abs(differenceFwd);
		}else{
			differenceFwd = 100000;
		}
		if (differenceUp < differenceFwd && differenceUp < differenceDown){
			i--;
			j++;
			distance += differenceUp;
		}else if (differenceDown < differenceFwd && differenceDown < differenceUp){
			i++;
			j++;
			distance += differenceDown;
		}else if (differenceFwd < differenceUp && differenceFwd < differenceDown){
			j++;
			distance += differenceFwd;
		}else if (differenceFwd == differenceUp && differenceFwd == differenceDown){
			j++;
			distance += differenceDown;
		}else if (differenceFwd == differenceDown && differenceFwd < differenceUp){
			j++;
			distance += differenceDown;
		}else if (differenceFwd == differenceUp && differenceFwd < differenceDown){
			j++;
			distance += differenceFwd;
		}else if (differenceUp == differenceDown && differenceUp < differenceFwd){
			i++;
			j++;
			distance += differenceDown;
		}
	}
	return distance;
}

void write_file(int rows, int columns, string filename, const vector<vector<int>>& r, const vector<vector<int>>& g, const vector<vector<int>>& b){
	ofstream outputFile;
	filename += ".ppm";
	outputFile.open(filename);
	string p3 = "P3";
	outputFile << p3 << endl;
	outputFile << columns << " " << rows << endl;
	outputFile << "255" << endl;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			outputFile << r.at(i).at(j) << " ";
			outputFile << g.at(i).at(j) << " ";
			outputFile << b.at(i).at(j) << " ";
		}
		outputFile << endl;
	}
}

int main(){
	// asks for user input
	int row = ask_user("Enter number of rows: ");
	int column = ask_user("Enter number of columns: ");
	string infilename = ask_user_string("Enter filename: ");
	// stores values from file to 2D vector
	vector<vector<int>> v = valuesVector(row,column,infilename);
	// finds min and max
	int min = findMin(row, column, v);
	int max = findMax(row, column, v);
	// finds color
	vector<vector<int>> r (row, vector<int>(column));
	vector<vector<int>> g (row, vector<int>(column));
	vector<vector<int>> b (row, vector<int>(column));
	fillVector(r,v,row,column,min,max);
	fillVector(g,v,row,column,min,max);
	fillVector(b,v,row,column,min,max);
	// part 2
	int red = 252;
	int green = 25;
	int blue = 63;
	int distance = 0;
	vector<int> dist;
	for (int i = 0; i < row; i++){
		distance = colorPath(v,r,g,b,red,green,blue,i);
		dist.push_back(distance);
	}
	int minDist = 0;
	int minDistLocation = 0;
	minDist = dist.at(0);
	for (int i = 0; i < row; i++){
		if (minDist > dist.at(i)){
			minDist = dist.at(i);
			minDistLocation = i;
		}
	}
	red = 31;
	green = 253;
	blue = 13;
	distance = colorPath(v,r,g,b,red,green,blue,minDistLocation);
	// writes numbers to file
	write_file(row, column, infilename,r,g,b);
	// for (int i = 0; i < row; i++){
	// 	for (int j = 0; j < column; j++){
	// 		cout << r.at(i).at(j) << " ";
	// 		cout << g.at(i).at(j) << " ";
	// 		cout << b.at(i).at(j) << " ";
	// 	}
	// 	cout << endl;
	// }
}