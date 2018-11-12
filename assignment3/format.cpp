#include <iostream>


using namespace std;

int main(int argc, char* argv[]) {
    string line;
    ifstream fr;
    fr.open(argv[1], ifstream::in);

    ofstream fw;
    fw.open(argv[2]);
    while(getline(fr, line)) {
        cout << line << endl;
    }

    return 0;
}