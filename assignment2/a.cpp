#include "nanoflann.hpp"
using namespace nanoflann;

#include "KDTreeVectorOfVectorsAdaptor.h"

#include <ctime>
#include <cstdlib>
#include <iostream>

const int SAMPLES_DIM = 128;

typedef std::vector<std::vector<float>> my_vector_of_vectors_t;


void generateRandomPointCloud(my_vector_of_vectors_t& samples,
                              const size_t N = 1e7,
                              const size_t dim = 128,
                              const float max_range = 1.0)
{
    std::cout << "Generating " << N << " random points...";
    samples.resize(N);
    for (size_t i = 0; i < N; i++)
    {
        samples[i].resize(dim);
        for (size_t d = 0; d < dim; d++)
            samples[i][d] = max_range * (rand() % 1000) / (1000.0);
    }
    std::cout << "done\n";
}

void kdtree_demo(const size_t nSamples = 1e7, const size_t dim = 128)
{
    my_vector_of_vectors_t samples;

    const float max_range = 1.0;

    // Generate points:
    generateRandomPointCloud(samples, nSamples, dim, max_range);

    // Query point:
    std::vector<float> query_pt(dim);
    for (size_t d = 0; d < dim; d++)
        query_pt[d] = max_range * (rand() % 1000) / (1000.0);

    // construct a kd-tree index:
    // Dimensionality set at run-time (default: L2)
    // ------------------------------------------------------------
    std::cout << "Constructing Kd Tree" << std::endl;
    typedef KDTreeVectorOfVectorsAdaptor<my_vector_of_vectors_t, float> my_kd_tree_t;
    my_kd_tree_t mat_index(dim /*dim*/, samples, 20 /* max leaf */);

    std::cout << "Building Index" << std::endl;
    mat_index.index->buildIndex();

    std::cout << "Initializing Indexes" << std::endl;
    // do a knn search
    const size_t num_results = 3;
    std::vector<size_t> ret_indexes(num_results);
    std::vector<float> out_dists_sqr(num_results);

    std::cout << "Initializing Resultset" << std::endl;
    nanoflann::KNNResultSet<float> resultSet(num_results);
    resultSet.init(&ret_indexes[0], &out_dists_sqr[0]);

    std::cout << "Starting " << std::endl;
    mat_index.index->findNeighbors(resultSet, &query_pt[0], nanoflann::SearchParams(10));

    std::cout << "knnSearch(number or results=" << num_results << "): \n";
    for (size_t i = 0; i < num_results; i++)
        std::cout << "ret_index[" << i << "]=" << ret_indexes[i] << " out_dist_sqr=" << out_dists_sqr[i] << std::endl;
}

int main()
{
    // Randomize Seed
    srand(time(NULL));
    kdtree_demo(1e7 /* samples */, SAMPLES_DIM /* dim */);
}