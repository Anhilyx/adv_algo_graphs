#include "matrice.h"
#include <fstream>
#include <stdexcept>
#include <vector>

Matrice::Matrice(int size, uint16_t** data):
    size(size),
    data(data)
{}

Matrice::Matrice(const std::string& path)
{
    // Open the file
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }

    // Read the size of the matrix (first line of the file)
    file >> size;
    data = new uint16_t*[size];

    // Read the matrix data
    for (int i = 0; i < size; ++i) {
        data[i] = new uint16_t[size];
        for (int j = 0; j < size; ++j) {
            file >> data[i][j];
        }
    }
}

Matrice::~Matrice()
{
    for (int i = 0; i < size; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

Matrice::operator std::string() const
{
    std::string result;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            result += std::to_string(data[i][j]);
            if (j < size - 1) {
                result += " ";
            }
        }
        result += "\n";
    }
    return result;
}

uint16_t Matrice::getSize()
{
    return size;
}

uint16_t Matrice::getEdge(uint16_t from, uint16_t to) const
{
    return data[from][to];
}

Matrice Matrice::floydWarshall()
{   
    // Create a new matrix to store the path lengths
    uint16_t** paths = new uint16_t*[size];
    for (uint16_t i = 0; i < size; i++) {
        paths[i] = new uint16_t[size];
        for (uint16_t j = 0; j < size; j++) {
            paths[i][j] = data[i][j] == 0 ? UINT16_MAX : data[i][j];
        }
    }

    // Compute the shortest paths
    for (uint16_t k = 0; k < size; k++) {
        for (uint16_t i = 0; i < size; i++) {
            if (i == k) continue;
            for (uint16_t j = 0; j < size; j++) {
                if (j == k || j == i) continue;

                if (
                    paths[i][k] < UINT16_MAX &&
                    paths[k][j] < UINT16_MAX &&
                    paths[i][k] + paths[k][j] < paths[i][j]
                ) {
                    paths[i][j] = paths[i][k] + paths[k][j];
                }
            }
        }
    }

    return Matrice(size, paths);
}

std::vector<std::vector<uint16_t>> Matrice::kosaraju() const
{
    // Create values for the pre/post ordering calculation
    uint16_t** ordering = new uint16_t*[size];
    bool* visited = new bool[size];
    for (uint16_t i = 0; i < size; i++) {
        visited[i] = false;
    }
    uint16_t preCount = 0;
    uint16_t postCount = 0;

    // Perform DFS until all nodes are visited
    while (preCount < size) {
        // Find the next unvisited node
        uint16_t next = 0;
        while (next < size && visited[next]) {
            next++;
        }

        // Perform a DFS from that node
        dfs(next, ordering, visited, &preCount, &postCount);
    }

    // Reset values for the cluster calculation
    std::vector<std::vector<uint16_t>> clusters;
    for (uint16_t i = 0; i < size; i++) {
        visited[i] = false;
    }
    preCount = 0;
    postCount = 0;

    // Perform DFS until all nodes are visited
    while (preCount < size) {
        // Find the next unvisited node (with the highest post number)
        uint16_t next = UINT16_MAX;
        uint16_t maxPost = 0;
        for (uint16_t i = 0; i < size; i++) {
            if (!visited[i] && ordering[i][1] >= maxPost) {
                next = i;
                maxPost = ordering[i][1];
            }
        }

        // Return if no more nodes
        if (next == UINT16_MAX) break;

        // Collect the cluster from that node
        std::vector<uint16_t> currentCluster;
        dfsCollect(next, currentCluster, visited, &preCount, &postCount);
        if (!currentCluster.empty()) {
            clusters.push_back(currentCluster);
        }
    }

    // Clean up
    for (uint16_t i = 0; i < size; i++) {
        delete[] ordering[i];
    }
    delete[] ordering;
    delete[] visited;

    return clusters;
}

Matrice Matrice::clusterMatrice()
{
    // Get the clusters using Kosaraju's algorithm
    std::vector<std::vector<uint16_t>> clusters = kosaraju();
    uint16_t clusterCount = clusters.size();

    // Create a new adjacency matrix for the clusters
    uint16_t** clusterData = new uint16_t*[clusterCount];
    for (uint16_t i = 0; i < clusterCount; i++) {
        clusterData[i] = new uint16_t[clusterCount];
        for (uint16_t j = 0; j < clusterCount; j++) {
            clusterData[i][j] = 0; // Initialize with no edges
        }
    }

    // Populate the cluster adjacency matrix
    for (uint16_t i = 0; i < clusterCount; i++) {
        for (uint16_t j = 0; j < clusterCount; j++) {
            if (i == j) continue; // No self-loops

            // Check if there is an edge from any node in cluster i to any node in cluster j
            for (uint16_t nodeFrom : clusters[i]) {
                for (uint16_t nodeTo : clusters[j]) {
                    clusterData[i][j] += data[nodeFrom][nodeTo] > 0 ? 1 : 0;
                }
            }
        }
    }

    return Matrice(clusterCount, clusterData);
}

void Matrice::dfs(uint16_t id, uint16_t** ordering, bool* visited,
                  uint16_t* preCount, uint16_t* postCount) const
{
    // Mark the node as visited and set its pre-order number
    visited[id] = true;
    ordering[id] = new uint16_t[2];
    ordering[id][0] = (*preCount)++;

    // Visit all the neighbors
    for (uint16_t i = 0; i < size; i++) {
        if (data[id][i] != 0 && !visited[i]) {
            dfs(i, ordering, visited, preCount, postCount);
        }
    }
    
    // Set the post-order number when all neighbors have been visited
    ordering[id][1] = (*postCount)++;
}

void Matrice::dfsCollect(uint16_t id, std::vector<uint16_t>& cluster, bool* visited,
                         uint16_t* preCount, uint16_t* postCount) const
{
    // Mark the node as visited and add it to the current cluster
    visited[id] = true;
    cluster.push_back(id);

    // Visit all the neighbors
    for (uint16_t i = 0; i < size; i++) {
        if ((data[i][id] != 0) && !visited[i]) {
            dfsCollect(i, cluster, visited, preCount, postCount);
        }
    }

    // Increment pre/post count, to check completion
    preCount++;
    postCount++;
}
