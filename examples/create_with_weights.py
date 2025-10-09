import os
import random

from examples.create_matrice import MATRICE_SIZE, print_matrice, save_matrice as _save_matrice

MIN_VALUE = 1
MAX_VALUE = 10


def from_file(
        path: str = f"{os.path.dirname(os.path.abspath(__file__))}/size_{MATRICE_SIZE}/matrice.txt"
) -> list[list[str]]:
    """
    Reads a matrice from a file.
    Args:
        path (str): The path to the file containing the matrice.
    Returns:
        list[list[str]]: The matrice as a list of lists of strings.
    """

    # Read the matrice from the file
    with open(path, "r") as f:
        matrice = f.read()
    
    # Convert the matrice string to a list of lists of strings and return it
    return from_string(matrice)


def from_string(
        matrice: str
) -> list[list[str]]:
    """
    Converts a matrice string to a list of lists of strings.
    Args:
        matrice (str): The matrice as a string.
    Returns:
        list[list[str]]: The matrice as a list of lists of strings.
    """

    # Create the new matrice
    new_matrice = []
    size = int(matrice.split("\n")[0])
    lines = matrice.split("\n")[1:]

    # Convert each line to a list of strings and append it to the new matrice
    for line in lines:
        new_matrice.append(line.split(" "))

    # Return the new matrice
    return new_matrice


def add_weights(
        matrice: list[list[str]],
        min_value: int = MIN_VALUE,
        max_value: int = MAX_VALUE
) -> list[list[str]]:
    """
    Adds random weights to the edges in the given matrice string.
    Args:
        matrice (list[list[str]]): The matrice as a list of lists of strings.
        min_value (int): The minimum weight value.
        max_value (int): The maximum weight value.
    Returns:
        list[list[str]]: The matrice with weights as a list of lists of strings.
    """

    # Create a new matrice based on the given one
    new_matrice = []
    for i in range(len(matrice)):
        new_line = []
        for j in range(len(matrice[i])):
            if matrice[i][j] == "0":
                new_line.append("0")
            else:
                new_line.append(str(random.randint(min_value, max_value)))
        new_matrice.append(new_line)

    # Return the new matrice with weights
    return new_matrice


def save_matrice(
        matrice: list[list[str]],
        path: str = f"{os.path.dirname(os.path.abspath(__file__))}/size_{MATRICE_SIZE}/with_weights.txt"
) -> None:
    """
    Saves the matrice to a file.
    Args:
        matrice (list[list[str]]): The matrice to save.
        path (str): The path to the file where the matrice will be saved.
    """

    _save_matrice(matrice, path)


if __name__ == "__main__":
    matrice = from_file()
    matrice = add_weights(matrice)
    save_matrice(matrice)