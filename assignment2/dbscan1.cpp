#include"preprocess.h"
#include"RangeTree.h"
#include<cfloat>
#include<queue>

namespace RT = RangeTree;
using namespace std;

vector<int> dbscan(vector<vector<float> > &data, int tau, float eps);

int main(int argc, char* argv[]) {
    string file = argv[1];
    int tau = atoi(argv[2]);
    float eps = atof(argv[3]);
    vector<vector<float> > data = read_file(file);
    int n = data.size();

    vector<int> clusters = dbscan(data, tau, eps);
    ofstream fw("out_dbscan2.txt");
    for(int i = 0; i < n; i++) {
        fw << clusters[i] << endl;
    }
    return 0;
}

vector<int> dbscan(vector<vector<float> > &data, int tau, float eps) {
    // -2 represents noise
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

    vector<int> cluster(n, -1);
    vector<bool> visited(n, false);
    int cluster_val = -1;

    for(int i = 0; i < n; i++) {
        if(cluster[i] != -1) {
            continue;
        }

        cout << i << endl;
        vector<float> lower(d), upper(d);
        for(int j = 0; j < d; j++) {
        	lower[j] = data[i][j] - eps;
        	upper[j] = data[i][j] + eps;
        }
        vector<RT::Point<float,int> > box = rtree.pointsInRange(lower, upper, withLower, withUpper);

        queue<int> ball;
        for(int j = 0; j < box.size(); j++) {
            if(box[j].value() == i) {
            	continue;
            } else if(distance(data[i], box[j].asVector()) < eps2) {
            	ball.push(box[j].value());
            }
        }

        visited[i] = true;
        if(ball.size() < tau) {
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
                	vector<float> lower(d), upper(d);
        			for(int j = 0; j < d; j++) {
        				lower[j] = data[p][j] - eps;
        				upper[j] = data[p][j] + eps;
        			}
        			vector<RT::Point<float,int> > temp_box = rtree.pointsInRange(lower, upper, withLower, withUpper);
                    vector<int> temp_ball;
                    for(int j = 0; j < temp_box.size(); j++) {
                 		if(temp_box[j].value() == p) {
                			continue;
            			} else if(distance(data[p], temp_box[j].asVector()) < eps2) {
            				temp_ball.push_back(temp_box[j].value());
            			}
                    }

                    cluster[p] = cluster_val;

                    if(temp_ball.size() >= tau) { // Core point
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