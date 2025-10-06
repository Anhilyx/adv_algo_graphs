#include "matrice.h"
#include <fstream>
#include <stdexcept>

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
