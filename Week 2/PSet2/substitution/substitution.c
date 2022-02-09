#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    /* #########################
     * #  Verify the Argument  #
     * ######################### */

    /* Variable that will be verified */
    string key = argv[1];

    /* 1. Check num of Argument/s */
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    /* 2. Check length of the Argument */
    if ()


    /* ########################
     * #  Getting User Input  #
     * ######################## */

    string plain_text = get_string("plaintext: ");

    
    /* ##################
     * #  Substitution  #
     * ################## */

    /* Output text */
    string cipher_text = plain_text;

    /* Process of substitution */
    for (int i = 0; i < strlen(plain_text); ++i)
    {
        if (isupper(plain_text[i]) != 0)  
        {
            cipher_text[i] = key[((int) plain_text[i]) - 65];
        }
        else if (islower(plain_text[i]) != 0)
        {
            cipher_text[i] = tolower(key[((int) plain_text[i]) - 97]);
        }
    }

    printf("ciphertext: %s\n", cipher_text);
    return 0;
}
