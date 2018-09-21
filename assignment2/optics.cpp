#include"preprocess.h"
#include"RangeTree.h"
#include<cfloat>
#include<set>

namespace RT = RangeTree;
using namespace std;

void optics(vector<vector<float> > &data, int tau, float eps);
void update(vector<int> &N, int p, float core_dist, set<pair<float, int> > &seeds, float eps, int tau);

vector<vector<float> > data;
vector<bool> visited;
vector<int> ordering;
vector<float> reach_dist;

float max_dist = 0.0;

int main(int argc, char* argv[]) {
    string file = argv[1];
    int tau = atoi(argv[2]);
    float eps = atof(argv[3]);
    data = read_file(file);
    int n = data.size();

    optics(data, tau, eps);
    ofstream fw("out_optics2.txt");
    for(int i = 0; i < n; i++) {
        if(reach_dist[ordering[i]] == -1) {
            fw << max_dist * 1.1 << " " << ordering[i] << endl;
        } else {
            fw << reach_dist[ordering[i]] << " " << ordering[i] << endl;
        }
    }
    return 0;
}

void optics(vector<vector<float> > &data, int tau, float eps) {
    float eps2 = eps * eps;
    int n = data.size();
    int d = data[0].size();

    // Make range tree
	vector<RT::Point<float,int> > points;
	for(int i = 0; i < n; i++) {
		points.push_back(RT::Point<float,int>(data[i], i));
	}

	RT::RangeTree<float,int> rtree(points);
	vector<bool> withLower(d, true), withUpper(d, true);

    visited = vector<bool>(n, false);
    reach_dist = vector<float>(n, -1.0);

    for(int i = 0; i < n; i++) {
        if(visited[i]) {
            continue;
        }

        vector<float> lower(d), upper(d);
        for(int j = 0; j < d; j++) {
        	lower[j] = data[i][j] - eps;
        	upper[j] = data[i][j] + eps;
        }
        vector<RT::Point<float,int> > box = rtree.pointsInRange(lower, upper, withLower, withUpper);

        vector<int> N;
        vector<float> tbs; 
        for(int j = 0; j < box.size(); j++) {
            if(box[j].value() == i) {
            	continue;
            } else if(distance(data[i], box[j].asVector()) < eps2) {
            	N.push_back(box[j].value());
                tbs.push_back(distance(data[i], box[j].asVector()));
            }
        }

        visited[i] = true;
        ordering.push_back(i);

        sort(tbs.begin(), tbs.end());

        if(N.size() < tau) {
            continue;
        } 
        
        float core_dist = tbs[tau - 1];
        cout << core_dist << endl;
        set<pair<float, int> > seeds;
        update(N, i, core_dist, seeds, eps, tau);

        while(!seeds.empty()) {
            set<pair<float, int> > :: iterator it = seeds.begin();
            int q = it->second;
            seeds.erase(it);

            vector<float> lower(d), upper(d);
            for(int j = 0; j < d; j++) {
                lower[j] = data[q][j] - eps;
                upper[j] = data[q][j] + eps;
            }
            vector<RT::Point<float,int> > box = rtree.pointsInRange(lower, upper, withLower, withUpper);

            vector<int> Np;
            vector<float> tbsp; 
            for(int j = 0; j < box.size(); j++) {
                if(box[j].value() == q) {
                    continue;
                } else if(distance(data[q], box[j].asVector()) < eps2) {
                    Np.push_back(box[j].value());
                    tbsp.push_back(distance(data[q], box[j].asVector()));
                }
            }

            visited[q] = true;
            ordering.push_back(q);

            sort(tbsp.begin(), tbsp.end());

            if(Np.size() >= tau) {
                float core_distp = tbsp[tau - 1];
                update(Np, q, core_distp, seeds, eps, tau);
            }
        }
    }

    return;
}

void update(vector<int> &N, int p, float core_dist, set<pair<float, int> > &seeds, float eps, int tau) {
    for(int i = 0; i < N.size(); i++) {
        int o = N[i];
        if(visited[o]) {
            continue;
        }

        float dist_p_o = distance(data[p], data[o]);
        float new_reach_dist = core_dist > dist_p_o ? core_dist : dist_p_o;

        if(new_reach_dist > max_dist) {
            max+dist = new_reach_dist;
        }
        if(reach_dist[o] == -1) {
            reach_dist[o] = new_reach_dist;
            seeds.insert(make_pair(new_reach_dist, o));
        } else if(new_reach_dist < reach_dist[o]) {
            seeds.erase(seeds.find(make_pair(reach_dist[o], o)));
            seeds.insert(make_pair(new_reach_dist, o));
            reach_dist[o] = new_reach_dist;
        }   
    }
    return;
}