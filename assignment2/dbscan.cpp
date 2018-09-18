#include"preprocess.h"
#include<cfloat>
#include<queue>

using namespace std;

vector<int> dbscan(vector<vector<float> > &data, int tau, float eps);

int main(int argc, char* argv[]) {
    string file = argv[1];
    int tau = atoi(argv[2]);
    float eps = atof(argv[3]);
    vector<vector<float> > data = read_file(file);
    int n = data.size();

    vector<int> clusters = dbscan(data, tau, eps);
    ofstream fw("out_dbscan.txt");
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
    vector<int> cluster(n, -1);
    int cluster_val = -1;

    for(int i = 0; i < n; i++) {
        if(cluster[i] != -1) {
            continue;
        }

        queue<int> ball;
        for(int j = 0; j < n; j++) {
            if(i == j) {
                continue;
            }

            if(distance(data[i], data[j]) < eps2) {
                ball.push(j);
            }

        }

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
                    vector<int> points;
                    for(int j = 0; j < n; j++) {
                        if(p == j) {
                            continue;
                        }

                        if(distance(data[p], data[j]) < eps2) {
                            points.push_back(j);
                        }
                    }

                    cluster[p] = cluster_val;

                    if(points.size() >= tau) { // Core point
                        for(int k = 0; k < points.size(); k++) {
                            ball.push(points[k]);
                        }
                    }
                }
            }
        }
    }

    return cluster;
}