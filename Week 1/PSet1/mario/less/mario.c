#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    
    // input positive int between 1 and 8
    do
    {
        printf("Height: ");
        scanf("%i", &height);
    }
    while (height < 1 || height > 8);
    
    // Display pyramid
    for (int rows = 0; rows < height; rows++)
    {
        // Display inverted pyramid as space
        for (int spc = rows; spc < height - 1; spc++)
        {
            printf(" ");
        }
        
        // Display has to form pyramid
        for (int hash = 0; hash <= rows; hash++)
        {
            printf("#");
        }
        printf("\n");
    }
}
