#include"preprocess.h"
#include"nanoflann.hpp"

#include"KDTreeVectorOfVectorsAdaptor.h"
#include<cfloat>
#include<set>

using namespace std;
using namespace nanoflann;

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
    ofstream fw("optics.txt");
    for(int i = 0; i < n; i++) {
        if(reach_dist[ordering[i]] == -1) {
            fw << max_dist * 1.1 << " " << ordering[i] << endl;
        } else {
            fw << reach_dist[ordering[i]] << " " << ordering[i] << endl;
        }
    }
    fw.close();
    return 0;
}

void optics(vector<vector<float> > &data, int tau, float eps) {
    float eps2 = eps * eps;
    int n = data.size();
    int d = data[0].size();

    // Make range tree
	typedef KDTreeVectorOfVectorsAdaptor<vector<std::vector<float>>, float> kd_tree_t;
    kd_tree_t kdtree(d, data, 20);

    kdtree.index->buildIndex();

    SearchParams params;
    params.sorted = true;

    visited = vector<bool>(n, false);
    reach_dist = vector<float>(n, -1.0);

    for(int i = 0; i < n; i++) {
        if(visited[i]) {
            continue;
        }

        vector<pair<size_t, float> >  ret_matches;

        const size_t nMatches = kdtree.index->radiusSearch(&data[i][0], eps, ret_matches, params);

        vector<int> N;
        for(int j = 0; j < ret_matches.size(); j++) {
            if(ret_matches[j].first != i) {
            	N.push_back(ret_matches[j].first);
            }
        }

        visited[i] = true;
        ordering.push_back(i);

        if(N.size() < tau) {
            continue;
        } 
        
        float core_dist = ret_matches[tau - 1].second;
        set<pair<float, int> > seeds;
        update(N, i, core_dist, seeds, eps, tau);

        while(!seeds.empty()) {
            set<pair<float, int> > :: iterator it = seeds.begin();
            int q = it->second;
            seeds.erase(it);

            vector<pair<size_t, float> >  temp_box;

            const size_t temp_count = kdtree.index->radiusSearch(&data[q][0], eps, temp_box, params);

            vector<int> Np; 
            for(int j = 0; j < temp_box.size(); j++) {
                if(temp_box[j].first != q) {
                    Np.push_back(temp_box[j].first);
                }
            }

            visited[q] = true;
            ordering.push_back(q);

            if(Np.size() >= tau) {
                float core_distp = temp_box[tau - 1].second;
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
            max_dist = new_reach_dist;
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