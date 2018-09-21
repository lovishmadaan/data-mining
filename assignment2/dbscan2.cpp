#include"preprocess.h"
#include"nanoflann.hpp"

#include"KDTreeVectorOfVectorsAdaptor.h"
#include<cfloat>
#include<queue>

using namespace std;
using namespace nanoflann;

int cluster_val = -1;

vector<int> dbscan(vector<vector<float> > &data, int tau, float eps);

int main(int argc, char* argv[]) {
    string file = argv[1];
    int tau = atoi(argv[2]);
    float eps = atof(argv[3]);
    vector<vector<float> > data = read_file(file);
    int n = data.size();

    vector<int> clusters = dbscan(data, tau, eps);
    vector<vector<int> > group(cluster_val + 2, vector<int>());

    for(int i = 0; i < n; i++) {
    	if(clusters[i] == -2) {
    		group[cluster_val + 1].push_back(i);
    	} else {
    		group[clusters[i]].push_back(i);
    	}
    }
    
    ofstream fw("dbscan.txt");
    for(int i = 0; i <= cluster_val; i++) {
    	fw << "#" << i << endl;
        for(int j = 0; j < group[i].size(); j++) {
        	fw << group[i][j] << endl;
        }
    }

    if(group[cluster_val + 1].size() > 0) {
    	fw << "#outlier\n";
    	for(int j = 0; j < group[cluster_val + 1].size(); j++) {
    		fw << group[cluster_val][j] << endl;
    	}
    }
    return 0;
}

vector<int> dbscan(vector<vector<float> > &data, int tau, float eps) {
    // -2 represents noise
    float eps2 = eps * eps;
    int n = data.size();
    int d = data[0].size();

    typedef KDTreeVectorOfVectorsAdaptor<vector<std::vector<float>>, float> kd_tree_t;
    kd_tree_t kdtree(d, data, 20);

	kdtree.index->buildIndex();

    vector<int> cluster(n, -1);
    vector<bool> visited(n, false);

    SearchParams params;
	params.sorted = false;

    for(int i = 0; i < n; i++) {
        if(cluster[i] != -1) {
            continue;
        }
        
		vector<pair<size_t, float> >  ret_matches;

		const size_t nMatches = kdtree.index->radiusSearch(&data[i][0], eps, ret_matches, params);

        queue<int> ball;
        for(int j = 0; j < ret_matches.size(); j++) {
            if(ret_matches[j].first != i) {
            	ball.push(ret_matches[j].first);
            }
        }

        visited[i] = true;
        if(ball.size() < tau - 1) {
            cluster[i] = -2;
        } else {
            cluster_val++;
            cluster[i] = cluster_val;

            while(!ball.empty()) {
                int p = ball.front();
                ball.pop();

                if(cluster[p] == -2) {
                    cluster[p] = cluster_val;
                } else if(cluster[p] == -1) {
                	vector<pair<size_t, float> >  temp_box;
                	kdtree.index->radiusSearch(&data[p][0], eps, temp_box, params);

                	
                    vector<int> temp_ball;
                    for(int j = 0; j < temp_box.size(); j++) {
                 		if(temp_box[j].first != p) {
                			temp_ball.push_back(temp_box[j].first);
            			}
                    }

                    cluster[p] = cluster_val;

                    if(temp_ball.size() > tau) { // Core point
                        for(int k = 0; k < temp_ball.size(); k++) {
                        	if(!visited[temp_ball[k]]) {
                        		ball.push(temp_ball[k]);
                        		visited[temp_ball[k]] = true;
                        	}
                        }
                    }
                }
            }
        }
    }

    return cluster;
}