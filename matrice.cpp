#include "matrice.h"

#include <fstream>
#include <stdexcept>

/******************************
| Constructors and Destructor |
******************************/

Matrice::Matrice(uint32_t size, int32_t** data, std::string* names):
    size(size),
    names(names)
{
    // Convert and copy the data to int64_t
    this->data = new int64_t*[size];
    for (uint32_t i = 0; i < size; i++) {
        this->data[i] = new int64_t[size];
        for (uint32_t j = 0; j < size; j++) {
            this->data[i][j] = data[i][j];
        }
    }
}

Matrice::Matrice(const std::string& path):
    names(nullptr)
{
    // Open the file
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }

    // Read the size of the matrix (first line of the file)
    file >> size;
    data = new int64_t*[size];  // Avoids inserting int64_t here, as it could lead to issues later on

    // Read the matrix data
    int32_t value;
    for (uint32_t i = 0; i < size; i++) {
        data[i] = new int64_t[size];
        for (uint32_t j = 0; j < size; j++) {
            file >> value;
            data[i][j] = value;
        }
    }

    // Free the resources
    file.close();
}

Matrice::~Matrice()
{
    // Free the data
    for (uint32_t i = 0; i < size; i++) {
        delete[] data[i];
    }
    delete[] data;

    // Free the names
    if (names) {
        delete[] names;
    }
}

/************
| Operators |
************/

Matrice::operator std::string() const
{
    std::string result;
    for (uint32_t i = 0; i < size; ++i) {
        result += std::to_string(data[i][0]);
        for (uint32_t j = 1; j < size; ++j) {
            result += " " + std::to_string(data[i][j]);
        }
        result += "\n";
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Matrice& m)
{
    os << static_cast<std::string>(m);
    return os;
}

/**********************
| Getters and Setters |
**********************/

uint32_t Matrice::getSize() const
{
    return size;
}

int64_t Matrice::getEdge(uint32_t from, uint32_t to) const
{
    return data[from][to];
}

std::string Matrice::getName(uint32_t index) const
{
    if (names != nullptr) {
        return names[index];
    } else {
        return std::to_string(index + 1);
    }
}

/*************
| Algorithms |
*************/

Matrice* Matrice::floydWarshall() const
{   
    // Create a new matrix to store the path lengths
    int64_t** paths = new int64_t*[size];
    for (uint32_t i = 0; i < size; i++) {
        paths[i] = new int64_t[size];
        for (uint32_t j = 0; j < size; j++) {
            // Initialize paths with the current edge weights, using INT64_MIN for no edge (infinite length)
            paths[i][j] = data[i][j] == 0 ? INT64_MIN : data[i][j];
        }
    }

    // Compute the shortest paths
    for (uint32_t k = 0; k < size; k++) {
        for (uint32_t i = 0; i < size; i++) {
            if (i == k) continue;  // Skip self-loops
            for (uint32_t j = 0; j < size; j++) {
                if (j == k) continue;  // Skip self-loops
                if (paths[i][k] == INT64_MIN || paths[k][j] == INT64_MIN) continue;  // Skip "infinite" lengths

                // Update the path if a shorter one is found
                int64_t length = paths[i][k] + paths[k][j];
                if (length < paths[i][j] || paths[i][j] == INT64_MIN) {
                    paths[i][j] = length;
                }
            }
        }
    }

    return new Matrice(size, paths);  // Uses the private constructor (faster)
}

Matrice* Matrice::prim() const
{
    // Create a new matrix to store the MST
    int64_t** mstData = new int64_t*[size];
    for (uint32_t i = 0; i < size; i++) {
        mstData[i] = new int64_t[size];
        for (uint32_t j = 0; j < size; j++) {
            mstData[i][j] = 0;  // Initialize empty
        }
    }

    // Array to track included vertices in MST
    bool* inMST = new bool[size];
    for (uint32_t i = 0; i < size; i++) {
        inMST[i] = false;
    }

    // Start from the first vertex
    inMST[0] = true;

    for (uint32_t i = 1; i < size; i++) { // Start at 1 since the first vertex is already included
        int64_t minEdge = INT64_MAX;
        uint32_t from = UINT32_MAX;
        uint32_t to = UINT32_MAX;

        // Find the minimum edge connecting a vertex in MST to a vertex outside MST
        for (uint32_t i = 0; i < size; i++) {
            if (inMST[i]) {
                for (uint32_t j = 0; j < size; j++) {
                    if (!inMST[j] && data[i][j] != 0 && data[i][j] < minEdge) {
                        minEdge = data[i][j];
                        from = i;
                        to = j;
                    }
                }
            }
        }

        // Check that a valid edge was found (if not, the graph is not fully connected)
        if (from == UINT32_MAX || to == UINT32_MAX) break;

        // Add the found edge to the MST
        mstData[from][to] = minEdge;
        mstData[to][from] = minEdge;  // Add also the reverse edge, as it's an undirected graph
        inMST[to] = true;
    }

    delete[] inMST;

    return new Matrice(size, mstData);  // Uses the private constructor (faster)
}

std::vector<std::vector<uint32_t>> Matrice::kosaraju() const
{
    // Initialize all necessary variables
    uint32_t** ordering = new uint32_t*[size];
    bool* visited = new bool[size];
    for (uint32_t i = 0; i < size; i++) {
        visited[i] = false;
    }
    uint32_t* postCount = new uint32_t(0);

    // Perform the first DFS (until all nodes are visited)
    while (*postCount < size) {
        // Find the next unvisited node
        uint32_t next = 0;
        while (next < size && visited[next]) {
            next++;
        }

        // Perform a DFS from that node
        dfs(next, ordering, visited, nullptr, postCount);
    }

    // Initialize/reset values for the clusters calculation
    std::vector<std::vector<uint32_t>> clusters;
    for (uint32_t i = 0; i < size; i++) {
        visited[i] = false;
    }
    *postCount = 0;

    // Perform DFS until all nodes are visited
    while (*postCount < size) {
        // Find the next unvisited node (the one with the highest post number)
        uint32_t next = UINT32_MAX;
        uint32_t maxPost = 0;
        for (uint32_t i = 0; i < size; i++) {
            if (!visited[i] && ordering[i][1] >= maxPost) {  // '>=' is fine, since every post order is unique
                next = i;
                maxPost = ordering[i][1];
            }
        }

        // Collect the cluster from that node
        std::vector<uint32_t> currentCluster;
        dfsCollect(next, &currentCluster, visited, nullptr, postCount);
        clusters.push_back(currentCluster);
    }

    // Free the resources
    for (uint32_t i = 0; i < size; i++) {
        delete[] ordering[i];
    }
    delete[] ordering;
    delete[] visited;
    delete postCount;

    return clusters;
}

Matrice* Matrice::clusterMatrice() const
{
    // Get the clusters using Kosaraju's algorithm
    std::vector<std::vector<uint32_t>> clusters = kosaraju();
    uint32_t clustersCount = clusters.size();

    // Create a new adjacency matrix for the clusters
    int64_t** clustersData = new int64_t*[clustersCount];
    for (uint32_t i = 0; i < clustersCount; i++) {
        clustersData[i] = new int64_t[clustersCount];
        for (uint32_t j = 0; j < clustersCount; j++) {
            clustersData[i][j] = 0;  // Initialize empty
        }
    }

    // Populate the cluster adjacency matrix
    /* Note: the complexity here is in fact O(n^2), with n the number of nodes in the original graph. Below is an example of why:
           🟩🟩🟩🟦🟦
           🟩🟩🟩🟦🟦
           🟩🟩🟩🟦🟦
           🟥🟥🟥🟨🟨
           🟥🟥🟥🟨🟨
       In this example, the rows/columns are those of the original matrix, and each color represents a specific cluster.
       It's impossible to cross the same combination of nodes two times, leading to a n*n amount of combinations, and thus O(n^2) complexity. */
    for (uint32_t i = 0; i < clustersCount; i++) {
        for (uint32_t j = 0; j < clustersCount; j++) {
            if (i == j) continue;  // No self-loops

            // Check if there is an edge from any node in cluster i to any node in cluster j
            for (uint32_t nodeFrom : clusters[i]) {
                for (uint32_t nodeTo : clusters[j]) {
                    clustersData[i][j] += data[nodeFrom][nodeTo] > 0 ? 1 : 0;
                }
            }
        }
    }

    // Generate names for the clusters
    std::string* clustersNames = new std::string[clustersCount];
    for (uint32_t i = 0; i < clustersCount; i++) {
        clustersNames[i] = "{" + getName(clusters[i][0]);  // Start the name with the first node in the cluster, as cluster will always have at least one node
        uint32_t clusterSize = clusters[i].size();
        for (uint32_t j = 1; j < clusterSize; j++) {
            clustersNames[i] += "," + getName(clusters[i][j]);
        }
        clustersNames[i] += "}";
    }

    return new Matrice(clustersCount, clustersData, clustersNames);  // Uses the private constructor (faster and allows for int64_t data)
}

/***********************
| Private Constructors |
***********************/

Matrice::Matrice(uint32_t size, int64_t** data, std::string* names):
    size(size),
    data(data),
    names(names)
{}

/*****************
| Sub-Algorithms |
*****************/

void Matrice::dfs(uint32_t id, uint32_t** ordering, bool* visited,
                  uint32_t* preCount, uint32_t* postCount) const
{
    // Mark the node as visited
    visited[id] = true;
    // Set the pre-order
    ordering[id] = new uint32_t[2];
    if (preCount != nullptr) ordering[id][0] = (*preCount)++;

    // Visit all the unvisited neighbors
    for (uint32_t i = 0; i < size; i++) {
        if (data[id][i] != 0 && !visited[i]) {
            dfs(i, ordering, visited, preCount, postCount);  // Recursive call
        }
    }
    
    // Set the post-order only when all neighbors have been visited
    if (postCount != nullptr) ordering[id][1] = (*postCount)++;
}

void Matrice::dfsCollect(uint32_t id, std::vector<uint32_t>* cluster, bool* visited,
                         uint32_t* preCount, uint32_t* postCount) const
{
    // Mark the node as visited
    visited[id] = true;
    // Add the node to the current cluster
    cluster->push_back(id);

    // Visit all the unvisited neighbors
    for (uint32_t i = 0; i < size; i++) {
        if ((data[i][id] != 0) && !visited[i]) {
            dfsCollect(i, cluster, visited, preCount, postCount);  // Recursive call
        }
    }

    // Increment pre/post count. This is mainly useful to know when all nodes have been visited.
    if (preCount != nullptr) (*preCount)++;
    if (postCount != nullptr) (*postCount)++;
}
