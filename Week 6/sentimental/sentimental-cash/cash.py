from cs50 import get_float


# Prompt User how many dollars the customer owed
while True:
    dollar = get_float("Change owed: ")
    if dollar > 0:
        break
    
# Count coins
coins = 0

# Convert dollar to cents
cents = dollar * 100

while cents > 0:
    # Calculate quarters to give the customer
    while cents >= 25:
        coins += 1
        cents -= 25
    
    # Calculate dimes to give the customer
    while cents >= 10:
        coins += 1
        cents -= 10
    
    # Calculate nickels to give the customer
    while cents >= 5:
        coins += 1
        cents -= 5

    # Calculate pennies to give the customer
    while cents >= 1:
        coins += 1
        cents -= 1

# Print total number of coins to give the customer
print(coins)
