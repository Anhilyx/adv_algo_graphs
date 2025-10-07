#include "matrice.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>

Matrice::Matrice(int size, uint16_t** data, std::string* names):
    size(size),
    data(data),
    names(names)
{}

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

std::string Matrice::toMarkdown(std::string path) const
{
    // Create the header row
    std::string result = "| |";
    for (uint16_t j = 0; j < size; ++j) {
        result += getName(j) + "|";
    }
    result += "\n|-|";
    for (uint16_t j = 0; j < size; ++j) {
        result += "-|";
    }

    // Create each row of the table
    result += "\n";
    for (uint16_t i = 0; i < size; ++i) {
        result += "|**" + getName(i) + "**|";
        for (uint16_t j = 0; j < size; ++j) {
            result += std::to_string(data[i][j]) + "|";
        }
        result += "\n";
    }

    // Optionally write to a file
    if (!path.empty()) {
        // Create the folder structure if it doesn't exist
        std::string folder = path.substr(0, path.find_last_of('/'));
        std::filesystem::create_directories(folder);

        // Write the result to the file
        std::ofstream file(path);
        if (file) {
            file << result;
        } else {
            throw std::runtime_error("Could not open file for writing");
        }
    }

    return result;
}

std::string Matrice::toHtml(std::string path) const
{
    // Create the base of the file
    std::string result =
        "<html>" \
            "<head>" \
                "<style>" \
                    "body {" \
                        "color: #fff;" \
                        "background-color: #222;" \
                    "}" \
                    "tr:first-child > td, td:first-child {" \
                        "font-weight: bold;" \
                        "font-size: 1.2rem;" \
                        "background-color: #0000;" \
                    "}" \
                    "td {" \
                        "width: 40px;" \
                        "height: 20px;" \
                        "text-align: center;" \
                        "background-color: #4F43;" \
                    "}" \
                    "td.null {" \
                        "color: #888;" \
                        "background-color: #F443;" \
                    "}" \
                "</style>" \
            "</head>" \
            "<body>" \
                "<table>" \
                    "<tbody>";

    // Create the header row
    result += "<td>";
    for (uint16_t j = 0; j < size; ++j) {
        result += "<td>" + getName(j) + "</td>";
    }
    result += "</tr>";

    // Create each row of the table
    for (uint16_t i = 0; i < size; ++i) {
        result += "<tr><td>" + getName(i) + "</td>";
        for (uint16_t j = 0; j < size; ++j) {
            result += std::string("<td") + (data[i][j] == 0 ? " class='null'" : "") + ">" + std::to_string(data[i][j]) + "</td>";
        }
        result += "</tr>";
    }

    // Create the end of the file
    result +=
                    "</tbody>" \
                "</table>" \
            "</body>" \
        "</html>";

    // Optionally write to a file
    if (!path.empty()) {
        // Create the folder structure if it doesn't exist
        std::string folder = path.substr(0, path.find_last_of('/'));
        std::filesystem::create_directories(folder);

        // Write the result to the file
        std::ofstream file(path);
        if (file) {
            file << result;
        } else {
            throw std::runtime_error("Could not open file for writing");
        }
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

std::string Matrice::getName(uint16_t index) const
{
    if (names) {
        return names[index];
    } else {
        return std::to_string(index + 1);
    }
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

    // Generate names for the clusters
    std::string* clusterNames = new std::string[clusterCount];
    for (uint16_t i = 0; i < clusterCount; i++) {
        clusterNames[i] = "{" + std::to_string(clusters[i][0]);
        for (size_t j = 1; j < clusters[i].size(); j++) {
            clusterNames[i] += "," + std::to_string(clusters[i][j]);
        }
        clusterNames[i] += "}";
    }

    return Matrice(clusterCount, clusterData, clusterNames);
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
