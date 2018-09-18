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