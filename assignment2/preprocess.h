#ifndef PREPROCESS_H
#define PREPROCESS_H

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<math.h>
#include<fstream>
#include<sstream>

using namespace std;

vector<vector<float> > read_file(string file);
float distance(vector<float> &x, vector<float> &y);
float distance(vector<float> &x, const vector<float> &y);

#endif