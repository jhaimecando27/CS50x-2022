#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int answer;

    // Get the appropriate input
    do
    {
        answer = get_int("Height: ");
    }
    while (answer < 1 || answer > 8);

    for (int i = 0; i < answer; ++i)
    {
        // Displays the num of space/s for the left pyramid
        for (int spc = answer - 1; spc > i; --spc)
        {
            printf(" ");
        }
        // Displays the left pyramid
        for (int left = 0; left <= i; ++left)
        {
            printf("#");
        }
        // Separators
        printf("  ");
        // Displays the right pyramid
        for (int right = 0; right <= i; ++right)
        {
            printf("#");
        }
        printf("\n");
    }
}