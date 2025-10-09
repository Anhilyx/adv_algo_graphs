import os
import random

MATRICE_SIZE = 50
CLUSTER_SIZE = 10
CLUSTER_EDGE_CHANCE = 0.2
OUTSIDE_EDGE_CHANCE = 0.001


def create_matrice(
        matrice_size: int = MATRICE_SIZE,
        cluster_size: int = CLUSTER_SIZE,
        cluster_edge_chance: float = CLUSTER_EDGE_CHANCE,
        outside_edge_chance: float = OUTSIDE_EDGE_CHANCE
) -> list[list[str]]:
    """
    Creates a matrice representing a graph, with a few parameters for better control over its structure.
    The graph will have clusters (sub-matrices with higher edge density) and some random edges outside of clusters.
    Args:
        matrice_size (int): The size of the matrice (number of nodes in the graph).
        cluster_size (int): The size of each cluster (sub-matrix).
        cluster_edge_chance (float): The probability of an edge existing between two nodes within a cluster.
        outside_edge_chance (float): The probability of an edge existing between two nodes outside of clusters.
    Returns:
        list[list[str]]: The created matrice as a list of lists of strings.
    """

    # Create an empty matrice
    matrice = [["0" for _ in range(matrice_size)] for _ in range(matrice_size)]

    # Create "clusters" (sub-matrices with higher edge density)
    for c in range(matrice_size // cluster_size):
        for i in range(c * cluster_size, (c + 1) * cluster_size):
            for j in range(c * cluster_size, (c + 1) * cluster_size):
                if i != j and random.random() < cluster_edge_chance:
                    matrice[i][j] = "1"

    # Add some random edges outside of clusters
    for i in range(matrice_size):
        for j in range(matrice_size):
            if i != j and matrice[i][j] == "0" and random.random() < outside_edge_chance:
                matrice[i][j] = "1"

    # Return the created matrice
    return matrice


def print_matrice(
        matrice: list[list[str]],
        print: bool = True
) -> str:
    """
    Prints the matrice in a readable format.
    Args:
        matrice (list[list[str]]): The matrice to print.
        print (bool): Whether to print the matrice or not.
    Returns:
        str: The matrice as a string.
    """

    # Create the string representation of the matrice
    new_matrice = f"{len(matrice)}\n"
    for line in matrice:
        new_matrice += " ".join(line) + "\n"
    
    # Print the matrice if required
    if print:
        print(new_matrice)

    # Return the string representation of the matrice
    return new_matrice


def save_matrice(
        matrice: list[list[str]],
        path: str = f"{os.path.dirname(os.path.abspath(__file__))}/size_{MATRICE_SIZE}/matrice.txt"
) -> None:
    """
    Saves the matrice to a file.
    Args:
        matrice (list[list[str]]): The matrice to save.
        path (str): The path to the file where the matrice will be saved.
    """

    with open(path, "w") as f:
        f.write(print_matrice(matrice, print=False))


if __name__ == "__main__":
    matrice = create_matrice()
    save_matrice(matrice)