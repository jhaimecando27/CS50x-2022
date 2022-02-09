#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int start_s;
    // Prompt for start size
    do
    {
        start_s = get_int("Start size: ");
    }
    while (start_s < 9);

    // Prompt for end size
    int end_s;
    do
    {
        end_s = get_int("End size: ");
    }
    while (end_s < start_s);

    // TODO: Calculate number of years until we reach threshold
    int year = 0;
    while (start_s < end_s)
    {
        start_s += (start_s / 3) - (start_s / 4);
        ++year;
    }

    // Print number of years
    printf("Years: %i\n", year);
}
