import os
import random

from create_matrice import MATRICE_SIZE, print_matrice, save_matrice as _save_matrice

MATRICE_SIZE = MATRICE_SIZE if True else 50
MIN_VALUE = 1
MAX_VALUE = max(20, MATRICE_SIZE)


def create_install_costs(
        size: int = MATRICE_SIZE,
        min_value: int = MIN_VALUE,
        max_value: int = MAX_VALUE,
) -> list[list[str]]:
    """
    Creates a matrice with random installation costs.
    Args:
        size (int): The size of the matrice (number of nodes in the graph).
        min_value (int): The minimum value for the installation costs.
        max_value (int): The maximum value for the installation costs.
    Returns:
        list[list[str]]: The created matrice as a list of lists of strings.
    """
    
    # Create an empty matrice
    new_matrice = [[0 for _ in range(size)] for _ in range(size)]

    # Fill the matrice with random values between 1 and max(20, size)
    for i in range(size):
        for j in range(i, size):
            value = str(random.randint(min_value, max_value))
            new_matrice[i][j] = value
            new_matrice[j][i] = value

    return new_matrice


def save_matrice(
        matrice: list[list[str]],
        path: str = f"{os.path.dirname(os.path.abspath(__file__))}/size_{MATRICE_SIZE}/install_costs.txt"
) -> None:
    """
    Saves the matrice to a file.
    Args:
        matrice (list[list[str]]): The matrice to save.
        path (str): The path to the file where the matrice will be saved.
    """

    _save_matrice(matrice, path)


if __name__ == "__main__":
    matrice = create_install_costs()
    save_matrice(matrice)