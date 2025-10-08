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
    Matrice(int size, uint16_t** data, std::string* names = nullptr);

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
     * @brief Convert the matrix to a Markdown table representation.
     * @param path Optional path to save the Markdown table to a file. If empty, the table is not saved to a file.
     * @return A string containing the Markdown table representation of the matrix.
     */
    std::string toMarkdown(std::string path = "") const;

    /**
     * @brief Convert the matrix to an HTML table representation.
     * @param path Optional path to save the HTML table to a file. If empty, the table is not saved to a file.
     * @return A string containing the HTML table representation of the matrix.
     */
    std::string toHtml(std::string path = "") const;

    /**
     * @brief Get the size of the matrix (number of rows or columns).
     * @return The size of the matrix.
     */
    uint16_t getSize() const;

    /**
     * @brief Get the value at the specified row and column in the matrix.
     * @param from The row index.
     * @param to The column index.
     * @return The value at the specified position in the matrix.
     */
    uint16_t getEdge(uint16_t from, uint16_t to) const;

    /**
     * @brief Get the name of the node at the specified index.
     * If names are not set, returns the index+1 as a string.
     * @param index The index of the node.
     * @return The name of the node.
     */
    std::string getName(uint16_t index) const;

    /**
     * @brief Apply the Floyd-Warshall algorithm to find the shortest paths in the graph represented by the matrix.
     * @return A 2D array representing the shortest path distances between each pair of vertices.
     */
    Matrice* floydWarshall() const;

    /**
     * @brief Compute the clusters in the graph represented by the matrix, using the Kosaraju algorithm.
     * @return An array of clusters, where each cluster is represented by an array of node indexes.
     */
    std::vector<std::vector<uint16_t>> kosaraju() const;

    /**
     * @brief Create a new Matrice representing the clusters of the current Matrice.
     * Each cluster is represented as a single node, with edges between clusters if there are edges between any nodes in these clusters.
     * @return A new Matrice representing the clusters.
     */
    Matrice* clusterMatrice() const;

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
     * @brief An array of strings representing the names of the nodes.
     * If nullptr, nodes are unnamed and represented by their indexes+1.
     */
    std::string* names;

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
