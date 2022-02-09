#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    /* Get Input */
    long num = get_long("Number: ");


    /* ############################
     * #  Check length of Digits  #
     * ############################ */

    /* Get Length */
    int len = (int) log10(num) + 1;

    /* Exit if length is invalid */
    if (len != 13 && len != 15 && len && len != 16)
    {
        printf("INVALID\n");
        return 0;
    }


    /* ############################
     * #     Luhn's Algorithm     #
     * ############################ */

    long temp = num;
    int sum1 = 0;
    int sum2 = 0;
    int total;
    int new_last;
    int new_last1;
    int new_last2;

    do
    {
        /* Sum of every last digits */
        sum1 += temp % 10;

        /* Remove the last digit */
        temp /= 10;

        /* Get the next last digit and multiple by 2 */
        new_last = (temp % 10) * 2;

        /* Separate the digits of 2nd last digits */
        new_last1 = new_last % 10;
        new_last2 = new_last / 10;

        /* Sum of every next_last's digits (not sum of the product) */
        sum2 += new_last1 + new_last2;

        /* Remove the next last digit */
        temp /= 10;
    }
    while (temp > 0);

    total = sum1 + sum2;

    /* Exit if failed in Checksum */
    if (total % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }


    /* ############################
     * #       Find Credit        #
     * ############################ */


    /* Get 2 first digits first */
    long digits = num;

    while (digits >= 100)
    {
        digits /= 10;
    }

    int first = (int) digits / 10;
    int second = (int) digits % 10;

    /* Then find the Credit card */
    if (first == 3 && (second == 4 || second == 7) && len == 15)
    {
        printf("AMEX\n");
    }
    else if (first == 5 && (second > 0 && second < 6) && len == 16)
    {
        printf("MASTERCARD\n");
    }
    else if (first == 4 && (len == 13 || len == 16))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}
