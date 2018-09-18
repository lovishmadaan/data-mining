#include"preprocess.h"

using namespace std;

int main(int argc, char* argv[]) {
    string file = "format.txt";
    vector<vector<float> > data = read_file(file);
    int n = data.size();
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < data[i].size(); j++) cout << data[i][j] << " ";
        cout << endl;
    }

    return 0;
}