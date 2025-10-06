#ifndef MATRICE_H
#define MATRICE_H

#include <cstdint>
#include <string>

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

private:
    /**
     * @brief The size of the matrix (size x size).
     */
    uint16_t size;

    /**
     * @brief A 2D array of uint8_t representing the matrix data.
     */
    uint16_t** data;
};

#endif // MATRICE_H
