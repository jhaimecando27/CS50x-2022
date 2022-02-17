# Prints out a half-pyramid of a specified height
from cs50 import get_int

# Prompts the user
while True:
    num = get_int("Height: ")
    if num < 9 and num > 0:
        break

# Prints the pyramid
for i in range(1, num + 1, 1):
    print(" " * (num - i), end="")
    print("#" * i)
