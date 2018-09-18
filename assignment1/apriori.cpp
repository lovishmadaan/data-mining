#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

bool check_valid(vector<int> &c, vector<vector<int> > &F) {
	for(int i = 0; i < c.size() - 2; i++) {
		bool present = false;
		for(int j = 0; j < F.size(); j++) {
			int ind = 0, k = 0;
			for(; k < c.size(); k++) {
				if(k != i) {
					if(c[k] != F[j][ind]) {
						break;
					}
					ind++;
				}
			}
			if(k == c.size()) {
				present = true;
				break;
			}
		}
		if(!present) {
			return false;
		}
	}
	return true;
}

void cand_gen(vector<vector<int> > &F, vector<vector<int> > &C) {
	for(int i = 0; i < F.size(); i++) {
		for(int j = i + 1; j < F.size(); j++) {
			vector<int> f1 = F[i], f2 = F[j];
			int k = 0;
			for(; k < f1.size() - 1; k++) {
				if(f1[k] != f2[k]) {
					break;
				}
			}
			if(k == f1.size() - 1) {
				if(f1.back() != f2.back()) {
					if(f1.back() > f2.back()){
						swap(f1, f2);
					}

					vector<int> c(f1);
					c.push_back(f2.back());
					if(check_valid(c, F)) {
						C.push_back(c);
					}
				}
			}
		}
	}

	return;
}

int main(int argc, char *argv[]) {
	clock_t start, end;
	start = clock();
	string line;
	ifstream fin;
	fin.open(argv[1], ifstream::in);
	map<int, int> M;
	M.clear();
	int T = 0, max_len = 0;
	while (getline(fin, line)) {
		istringstream is(line);
		vector<int> set = vector<int>(istream_iterator<int>(is), istream_iterator<int>());
		for(int i = 0; i < set.size(); i++) {
			if(M.find(set[i]) == M.end()) {
				M[set[i]] = 1;
			} else {
				M[set[i]]++;
			}
		}
		T++;
		if(set.size() > max_len) {
			max_len = set.size();
		}
	}
	fin.close();
	map<int, int> :: iterator it = M.begin();
	vector<vector<int> >C1(M.size());
	int ind = 0;
	for(; it != M.end(); it++) {
		C1[ind++] = vector<int>(1, it->first);
	}

	float x = atof(argv[3]);
	vector<vector<vector<int> > > Fsets;
	vector<vector<vector<int> > > Csets;
	Csets.push_back(C1);
  
	// cout << T << endl;
	// cout << x / 100 << endl;

	vector<vector<int> > temp;
	for(int i = 0; i < Csets[0].size(); i++) {
		if(M[Csets[0][i][0]] * 100 >= x * T) {
			temp.push_back(Csets[0][i]);
			// cout << Csets[0][i][0] << " " << (1.0 * M[Csets[0][i][0]]) / T << endl;
		}
	  
	}
	Fsets.push_back(temp);
	
	// for(int i = 0; i < Fsets[0].size(); i++)
	// 	cout << Fsets[0][i][0] << " " << M[Fsets[0][i][0]] << endl;

	while(Fsets.back().size() != 0 && Fsets.size() < max_len) {
		temp.clear();
		cand_gen(Fsets.back(), temp);

		// cout << "Yeah\n\n";
		// for(int i = 0; i < temp.size(); i++) {
		// 	for(int j = 0; j < temp[i].size(); j++) {
		// 		cout << temp[i][j] << " ";
		// 	}
		// 	cout << endl;
		// }

		Csets.push_back(temp);

		vector<int> count(temp.size(), 0);
		fin.open("retail.dat", ifstream::in);
		while(getline(fin, line)) {
			istringstream is(line);
			vector<int> set = vector<int>(istream_iterator<int>(is), istream_iterator<int>());
			for(int i = 0; i < temp.size(); i++) {
				// if(temp[i].size() == 3)
					// cout << i << " " << (set.size() >= temp[i].size() && includes(set.begin(), set.end(), temp[i].begin(), temp[i].end()) ? "True" : "False") << endl;
				if(set.size() >= temp[i].size() && includes(set.begin(), set.end(), temp[i].begin(), temp[i].end())) {
					count[i]++;
				}
		   }
		}
		fin.close();

		Fsets.push_back(vector<vector<int> >());
		for(int i = 0; i < count.size(); i++) {
			// cout << count[i] << " ";
			if(count[i] * 100 >= x * T) {
				Fsets.back().push_back(temp[i]);
			}
		}
		// cout << endl;
	}
	ofstream writeout;
	writeout.open(argv[2]);
	for(int i = 0; i < Fsets.size(); i++) {
		for(int j = 0; j < Fsets[i].size(); j++) {
			for(int k = 0; k < Fsets[i][j].size() - 1; k++) {
				writeout << Fsets[i][j][k] << " ";
			}
			writeout << Fsets[i][j][Fsets[i][j].size() - 1] << endl;
		}
	}
	writeout.close();

	end = clock();
	cout << (float) (end - start) / CLOCKS_PER_SEC << endl;

	return 0;
}