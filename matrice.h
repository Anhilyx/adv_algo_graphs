#ifndef MATRICE_H
#define MATRICE_H

#include <cstdint>
#include <string>
#include <vector>

class Matrice
{
public:
    /**
     * @brief Construct a new Matrice object with given size and data.
     * @param size The size of the matrix (size x size).
     * @param data A 2D array of uint8_t representing the matrix data.
     */
    Matrice(int size, uint16_t** data);

    /**
     * @brief Construct a new Matrice object by reading from a file.
     * The file should contain the size of the matrix on the first line,
     * followed by the matrix data, with each row on a new line, and each value separated by spaces.
     * @param path The path to the file containing the matrix data.
     * @throws std::runtime_error if the file cannot be opened.
     */
    Matrice(const std::string& path);

    /**
     * @brief Destroy the Matrice object and free allocated memory.
     */
    ~Matrice();

    /**
     * @brief Implicit conversion operator to convert Matrice to a string representation.
     */
    operator std::string() const;

    /**
     * @brief Apply the Floyd-Warshall algorithm to find the shortest paths in the graph represented by the matrix.
     * @return A 2D array representing the shortest path distances between each pair of vertices.
     */
    Matrice floydWarshall();

    /**
     * @brief Compute the clusters in the graph represented by the matrix, using the Kosaraju algorithm.
     * @return An array of clusters, where each cluster is represented by an array of node indexes.
     */
    std::vector<std::vector<uint16_t>> kosaraju() const;

private:
    /**
     * @brief The size of the matrix (size x size).
     */
    uint16_t size;

    /**
     * @brief A 2D array of uint8_t representing the matrix data.
     */
    uint16_t** data;

    /**
     * @brief Helper function for depth-first search to find connected components.
     * @param id The current node index.
     * @param ordering The adjacency matrix representing the graph.
     * @param visited An array to keep track of visited nodes.
     * @param preCount A pointer to the count of nodes visited before the current node.
     * @param postCount A pointer to the count of nodes visited after the current node.
     * @return An array of node indexes in the connected component.
     */
    void dfs(uint16_t id, uint16_t** ordering, bool* visited,
             uint16_t* preCount, uint16_t* postCount) const;

    /**
     * @brief Helper function for depth-first search to collect nodes in a cluster.
     * @param id The current node index.
     * @param cluster The vector to store the collected cluster nodes.
     * @param visited An array to keep track of visited nodes.
     */
    void dfsCollect(uint16_t id, std::vector<uint16_t>& cluster, bool* visited,
                    uint16_t* preCount, uint16_t* postCount) const;
};

#endif // MATRICE_H
