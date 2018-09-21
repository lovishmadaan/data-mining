#include"preprocess.h"
#include<cfloat>

using namespace std;

vector<int> kmeans(vector<vector<float> > &data, int k);

int main(int argc, char* argv[]) {
    string file = argv[1];
    int k = atoi(argv[2]);
    vector<vector<float> > data = read_file(file);
    int n = data.size();

    vector<int> clusters = kmeans(data, k);
    vector<vector<int> > group(k, vector<int>());

    for(int i = 0; i < n; i++) {
        group[clusters[i]].push_back(i);
    }
    
    ofstream fw("kmeans.txt");
    for(int i = 0; i < k; i++) {
        fw << "#" << i << endl;
        for(int j = 0; j < group[i].size(); j++) {
            fw << group[i][j] << endl;
        }
    }
    fw.close();
}

vector<int> kmeans(vector<vector<float> > &data, int k) {
    int n = data.size();
    int d = data[0].size();
    vector<int> cluster(n, -1);
    
    // Initialize Means:
    srand(time(NULL));
    vector<vector<float> > centers(k, vector<float>(d, 0.0));
    for(int r = 0; r < 10; r++) {
        for(int i = 0; i < k; i++) {
            int ind = rand() % n;
            for(int j = 0; j < d; j++)
                centers[i][j] += data[ind][j];
        }
    }
    
    for(int i = 0; i < k; i++) {
        int ind = rand() % n;
        for(int j = 0; j < d; j++)
            centers[i][j] /= 10;
    }
    
    // Run kmeans
    int iters = 200;
    int maxIter = iters;
    while(iters > 0) {
        // cout << maxIter + 1 - iters << endl;

        //Assign cluster
        for(int i = 0; i < n; i++) {
            float minDist = FLT_MAX;
            for(int j = 0; j < k; j++) {
                float dist = distance(data[i], centers[j]);
                if(dist < minDist) {
                    cluster[i] = j;
                    minDist = dist;
                }
            }
            // cout << "mindDist: " << minDist << endl;
        }

        //Update Centers
        vector<int> counts(k, 0);
        vector<vector<float> > sums(k, vector<float>(d, 0.0));
        for(int i = 0; i < n; i++) {
            counts[cluster[i]]++;
            for(int j = 0; j < d; j++) sums[cluster[i]][j] += data[i][j];
        }
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < d; j++) centers[i][j] = sums[i][j] / counts[i];
        }

        iters--;
    }

    return cluster;
}