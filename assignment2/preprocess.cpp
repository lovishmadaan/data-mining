#include"preprocess.h"

using namespace std;

vector<vector<float> > read_file(string file) {
    ifstream fr(file);
    string line;
    vector<vector<float> > data;
    while(getline(fr, line)) {
        float f;
        vector<float> point;
        istringstream linestream(line);
        while(linestream >> f) {
            point.push_back(f);
        }
        data.push_back(point);
    }
    return data;
}

// TODO: Check overflow
float distance(vector<float> &x, vector<float> &y) {
    float sum = 0.0;
    int d = x.size();
    for(int i = 0; i < d; i++) {
        sum += (x[i] - y[i]) * (x[i] - y[i]);
    }
    return sum;
}

float distance(vector<float> &x, const vector<float> &y) {
    float sum = 0.0;
    int d = x.size();
    for(int i = 0; i < d; i++) {
        sum += (x[i] - y[i]) * (x[i] - y[i]);
    }
    return sum;
}