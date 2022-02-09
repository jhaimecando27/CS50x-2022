#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Gets name
    string name = get_string("What is your name?\n");

    // Displays name
    printf("hello, %s\n", name);
}