#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int count_letters(string str);
int count_words(string str);
int count_sentences(string str);

int main(void)
{
    /* ########################
     * #  Getting User Input  #
     * ######################## */

    string text = get_string("Text: ");

    /* #####################
     * #  Get Grade Level  #
     * ##################### */

    /* Getting variables needed */
    int L = 0;
    int W = 0;
    int S = 0;

    for (int i = 0; i < strlen(text); ++i)
    {
        /* Count num of Letters */
        if (isalpha(text[i]) != 0)
        {
            ++L;
        }
        
        /* Count num of Words. Add 1 if it's at the end
         * cuz there is no space there. */
        if (text[i] == ' ' || i == strlen(text) - 1)
        {
            ++W;
        }
        
        /* Count number Sentences. */
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            ++S;
        }
    }

    /* Getting the average or L and S
     *
     *    L        ?
     *   ---  x  -----
     *    W      100.0
     * 
     * Cross multiply and divide. */
    float ave_L = (L * 100.0) / W;
    float ave_S = (S * 100.0) / W;

    /* Putting it All Together (Coleman-Liau index) */
    float index = 0.0588 * ave_L - 0.296 * ave_S - 15.8;


    /* ####################
     * #  Program Output  #
     * #################### */

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %0.0f\n", index);
    }
}
