# Prints out a double half-pyramid of a specifid height
from cs50 import get_int

# Prompts the user
while True:
    num = get_int("Height: ")
    if num < 9 and num > 0:
        break

# Prints the double half-pyramid
for i in range(1, num + 1):
    print(" " * (num - i), end="")
    print("#" * i, end="")
    print("  ", end="")
    print("#" * i)
