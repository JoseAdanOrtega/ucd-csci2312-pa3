//
// Created by Jose on 3/28/2016.
//

#include <iostream>
#include <fstream>
#include <limits>
#include "KMeans.h"
#include "Exceptions.h"
#include "Cluster.h"

namespace Clustering
{


    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) :
            __dimensionality(dim), __k(k), __iFileName(filename), __maxIter(maxIter) {
        if (k == 0)
            throw ZeroClustersEx();

        std::ifstream iFile(filename.c_str());

        if (iFile.fail())
            throw DataFileOpenEx(filename);

        iFile.close();
    }

    KMeans::~KMeans() {
        for (int i = 0; i < __k; ++i) {
            delete __initCentroids[i];
        }

        delete [] __initCentroids;

        for (int i = 0; i < __k; ++i) {
            delete __clusters[i];
        }
        delete [] __clusters;
    }

//**********************************************************************************************

    unsigned int KMeans::getMaxIter() {
        return __maxIter;
    }

    unsigned int KMeans::getNumIters() {
        return __numIter;
    }

    unsigned int KMeans::getNumNonemptyClusters() {
        return __numNonempty;
    }

    unsigned int KMeans::getNumMovesLastIter() {
        return __numMovesLastIter;
    }

//**********************************************************************************************

    Cluster &KMeans::operator[](unsigned int u) {
        if (__k == 0) {
            throw ZeroClustersEx();
        }

        if (u >= __k) {
            throw DimensionalityMismatchEx(__k, u);
        }

        return *__clusters[u];
    }

    const Cluster &KMeans::operator[](unsigned int u) const {
        if (__k == 0) {
            throw ZeroClustersEx();
        }

        if (u >= __k) {
            throw DimensionalityMismatchEx(__k, u);
        }

        return *__clusters[u];
    }

//**********************************************************************************************
    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans) {

        Point hold(kmeans.__dimensionality);

        for (int i = 0; i < kmeans.__dimensionality; ++i) {
            hold[i] = std::numeric_limits<double>::max();
        }

        for (int i = 0; i < kmeans.__k; ++i) {
            if (!kmeans.__clusters[i]->centroid.equal(hold)) {
                os << *kmeans.__clusters[i] << std::endl;
            }
        }

        return os;
    }

    void KMeans::run() {
        int moves = 100;
        int iterator = 0;
        int nonempty = 0;

        while (moves > 0 && iterator < __maxIter) {
            moves = 0;

            for (int i = 0; i < __k; ++i) {
                for (int j = 0; j < __clusters[i]->getSize(); ++j)
                {
                    Cluster &c = *(__clusters[i]);

                    Point current_point(__dimensionality);
                    current_point = c[j];
                    int smallest_dist_index = 0;
                    double smallest_dist = current_point.distanceTo(*__initCentroids[0]);

                    for (int e = 0; e < __k; e++) {
                        if (current_point.distanceTo(*__initCentroids[e]) < smallest_dist) {
                            smallest_dist = current_point.distanceTo(*__initCentroids[e]);
                            smallest_dist_index = e;
                        }
                    }
                    Cluster::Move change_clusters(current_point, *__clusters[i], *__clusters[smallest_dist_index]);
                    change_clusters.perform();

                    for (int c = 0; c < __k; ++c) {
                        __clusters[c]->centroid.compute();
                    }
                    if (*__clusters[i] != *__clusters[smallest_dist_index]) {
                        moves++;
                    }
                }
            }

            iterator++;
        }

        Point inf(__dimensionality);

        for (int i = 0; i < __dimensionality; ++i) {
            inf[i] = std::numeric_limits<double>::max();
        }

        for (int i = 0; i < __k; ++i) {
            if (__clusters[i]->centroid.get() != inf) {
                ++nonempty;
            }
        }
        __numIter = iterator;
        __numMovesLastIter = moves;
        __numNonempty = nonempty;
    }

}