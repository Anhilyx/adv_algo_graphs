#ifndef MATRICE_H
#define MATRICE_H

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

/**
 * @brief A class representing a square matrix and providing graph algorithms.
 */
class Matrice
{
    
public:
    /******************************
    | Constructors and Destructor |
    ******************************/

    /**
     * @brief Construct a new Matrice object with given size and data.
     * @param size The size of the matrix (size x size).
     * @param data A 2D array of int32_t representing the matrix data.
     * @param names An optional array of strings representing the names of the nodes. If nullptr, nodes are unnamed and represented by their indexes+1.
     */
    Matrice(uint32_t size, int32_t** data, std::string* names = nullptr);

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

    /************
    | Operators |
    ************/

    /**
     * @brief Implicit conversion operator to convert Matrice to a string representation.
     */
    operator std::string() const;

    /**
     * @brief Implicit conversion operator to write the Matrice to an output stream.
     * @param os The output stream.
     * @param m The Matrice object.
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Matrice& m);

    /**********************
    | Getters and Setters |
    **********************/

    /**
     * @brief Get the size of the matrix (number of rows or columns).
     * @return The size of the matrix.
     */
    uint32_t getSize() const;

    /**
     * @brief Get the value at the specified edge (row and column) in the matrix.
     * @param from The row index.
     * @param to The column index.
     * @return The value at the specified position in the matrix.
     */
    int64_t getEdge(uint32_t from, uint32_t to) const;

    /**
     * @brief Get the name of the node at the specified index.
     * If names are not set, returns the index + 1.
     * @param index The index of the node.
     * @return The name of the node.
     */
    std::string getName(uint32_t index) const;

    /*************
    | Algorithms |
    *************/

    /**
     * @brief Apply the Floyd-Warshall algorithm to find the shortest paths in the graph represented by the matrix.
     * @return A 2D array representing the shortest path distances between each pair of vertices.
     */
    Matrice* floydWarshall() const;

    /**
     * @brief Compute the clusters in the graph represented by the matrix, using the Kosaraju algorithm.
     * @return A vector of clusters, where each cluster is represented by a vector of node indexes.
     */
    std::vector<std::vector<uint32_t>> kosaraju() const;

    /**
     * @brief Create a new Matrice representing the clusters of the current Matrice.
     * Each cluster is represented as a single node, with edges between clusters representing edges between any nodes in these clusters.
     * @return A new Matrice representing the clusters.
     */
    Matrice* clusterMatrice() const;

private:
    /*************
    | Attributes |
    *************/

    /**
     * @brief The size of the matrix (size x size).
     */
    uint32_t size;

    /**
     * @brief A 2D array of int64_t representing the matrix data.
     * Using int64 allow to store an int32_t (the original data) and an uint32_t (the size of the matrix).
     */
    int64_t** data;

    /**
     * @brief An array of C-strings representing the names of the nodes.
     * If nullptr, nodes are unnamed and represented by their indexes + 1.
     */
    std::string* names;

    /***********************
    | Private Constructors |
    ***********************/

    /**
     * @brief Construct a new Matrice object with given size and data.
     * @param size The size of the matrix (size x size).
     * @param data A 2D array of int64_t representing the matrix data.
     * @param names An optional array of strings representing the names of the nodes. If nullptr, nodes are unnamed and represented by their indexes+1.
     */
    Matrice(uint32_t size, int64_t** data, std::string* names = nullptr);

    /*****************
    | Sub-Algorithms |
    *****************/

    /**
     * @brief Helper function for depth-first search to find connected components.
     * This function automatically updates given variables.
     * @param id The current node index.
     * @param ordering The 2D array to store pre and post order numbers. For each node, pre-order is at index 0, post-order at index 1.
     * @param visited An array to keep track of visited nodes.
     * @param preCount A pointer to the count of nodes visited before the current node.
     * @param postCount A pointer to the count of nodes visited after the current node.
     */
    void dfs(uint32_t id, uint32_t** ordering, bool* visited,
             uint32_t* preCount = nullptr, uint32_t* postCount = nullptr) const;

    /**
     * @brief Helper function for depth-first search to collect nodes in a cluster.
     * @param id The current node index.
     * @param cluster The vector representing the current cluster.
     * @param visited An array to keep track of visited nodes.
     * @param preCount A pointer to the count of nodes visited before the current node.
     * @param postCount A pointer to the count of nodes visited after the current node.
     */
    void dfsCollect(uint32_t id, std::vector<uint32_t>* cluster, bool* visited,
                    uint32_t* preCount = nullptr, uint32_t* postCount = nullptr) const;
};

#endif // MATRICE_H
