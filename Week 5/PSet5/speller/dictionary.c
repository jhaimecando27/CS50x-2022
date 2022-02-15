// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];

// Size of dictionary, I hate this var name
int dict_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Loop through linked list and check
    int hash_val = hash(word);
    node *n = table[hash_val];
    while (n != NULL)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
        
        n = n->next;
    }
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /* REF: https://cs50.stackexchange.com/questions/19705/how-to-make-the-check-function-faster
     * I discover that my time in check is slow and I think it is because of the hash value
     * because it doesn't return that distinctful value for each word.
     * So in this REF I learned that I can use binary operators to tinker around the binary values
     * to generate more distinctful values. Ofcourse, it also depends on the N */
    unsigned int hash_val = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        hash_val = (hash_val << 1) ^ tolower(word[i]);
    }
    return hash_val % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    /* Open the file */
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    /* Tmp word */
    char word[LENGTH + 1];
    
    /* Add each word to hash table */
    while (fscanf(dict, "%s", word) != EOF)
    {
        /* Create new node */
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        
        /* Store the word in the node */
        strcpy(n->word, word);
        
        /* Assign hash value of the word */
        int hash_val = hash(word);
        
        /* If the linked list is empty make it the head */
        if (table[hash_val] == NULL)
        {
            n->next = NULL;
            table[hash_val] = n;
        }
        
        /* Else add it at the start */
        else  
        {
            n->next = table[hash_val];
            table[hash_val] = n;
        }
        
        /* For size() */
        dict_size++;
    }
    
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    /* Go through each bucket */
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
        if (table[i] == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}
