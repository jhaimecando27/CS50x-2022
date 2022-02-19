from sys import exit
from cs50 import get_int


def main():
    cardNo = get_card()

    if validate(str(cardNo)) == True:
        findCard(cardNo)
    else:
        print("INVALID")
        exit(1)


# Prompt for input
# Returns the card number otherwise it's invalid
def get_card():
    """Prompt for input, returns the card number otherwise it's invalid."""
    cardNo = get_int("Number: ")

    strlen = len(str(cardNo))
    if strlen != 13 and strlen != 15 and strlen != 16:
        print("INVALID")
        exit(0)

    return cardNo 


# Calculate the checksum of the card
# Returns true if valid otherwise false
def validate(cardNo):
    num_digits = len(cardNo)
    total = 0
    is_second = False
    
    # Luhn Algorithm
    for i in range(num_digits - 1, -1, -1):
        digit = ord(cardNo[i]) - ord('0')

        if is_second == True:
            digit *= 2

        total += digit // 10
        total += digit % 10

        is_second = not is_second

    if total % 10 == 0:
        return True
    else:
        return False


# Check for card length and starting digit
# Returns valid card's name otherwise invalid
def findCard(cardNo):
    num_digits = len(str(cardNo))
    
    while cardNo >= 100:
        cardNo /= 10

    # Gets first 2 numbers
    first = cardNo // 10
    second = int(cardNo % 10)
    
    # Finds the card
    if first == 3 and (second == 4 or second == 7):
        print("AMEX")
    elif first == 5 and (second > 0 and second < 6):
        print("MASTERCARD")
    elif first == 4 and (num_digits == 13 or num_digits == 16):
        print("VISA")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
