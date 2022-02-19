# Computes the approximate grade level needed to comprehend some text
from cs50 import get_string

txt = get_string("Text: ")

L = 0
# For the last word in the txt
W = 1 
S = 0

for c in txt:
    if c.isalpha():
        L += 1
    if c == " ":
        W += 1
    if c == "." or c == "?" or c == "!":
        S += 1

# Caleman-Liau index
ave_L = (L * 100) / W
ave_S = (S * 100) / W
index = round((0.0588 * ave_L) - (0.296 * ave_S) - 15.8)

# Grade level
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade " + str(index))
