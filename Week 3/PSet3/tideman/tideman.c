/* Fun fact:
 * In PH most filipino probably pronounce tideman as tiedman.
 * It is because there's a famous powder detergent named tide
 * which pronounce as tied. XD
 */

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// User Function prototypes
bool cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

/* ###########################################################################
 * #                   Update ranks given a new vote                         #
 * ########################################################################### */

/* Update the vote */
bool vote(int rank, string name, int ranks[])
{
    // Loop through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Find the candidate
        if (strcmp(name, candidates[i]) == 0)
        {
            // Update Vote
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}


/* ###########################################################################
 * #              Update preferences given one voter's ranks                 #
 * ########################################################################### */

void record_preferences(int ranks[])
{
    // Loop through ranks (higher)
    for (int i = 0; i < candidate_count; i++)
    {
        // Loop through ranks (lower)
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Update preferences
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}


/* ###########################################################################
 * #     Record pairs of candidates where one is preferred over the other    #
 * ########################################################################### */

void add_pairs(void)
{
    // Loop through preferences
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Update pairs
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}


/* ###########################################################################
 * #            Sort pairs in decreasing order by strength of victory        #
 * ########################################################################### */

/* Bubble Sort
 *
 * I will use Bubble Sort because and at this very moment this is my level of
 * knowledge in Sorting Techiques.
 */
void sort_pairs(void)
{
    // Loop through preferences
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count - i; j++)
        {
            // Swap in decreasing order until sorted
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                pair temp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;
            }
        }
    }
    return;
}


/* ###########################################################################
 * #  Lock pairs into the candidate graph in order, without creating cycles  #
 * ########################################################################### */

void lock_pairs(void)
{
    // Loop through pairs
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}


/* ###########################################################################
 * #                      Print the winner of the election                   #
 * ########################################################################### 
 * 
 * Example:
 *
 *           Me      You      None    <- [x]
 *        ---------------------------
 *     Me | false    true     false
 *    You | true     false    false
 *   None | false    true     false
 *     ^
 *     |
 *    [y]
 *
 *
 * "None" is the winner because no one is locked to it in [x], thus using a
 * int variable, in my case the lockless, to count all false locked.
 * Top of that, we will reset the int variable if we're going to move to 
 * another candidate.
 */

void print_winner(void)
{
    // Loop through locked
    for (int x = 0; x < candidate_count; x++)
    {
        int lockless = 0; // lockless = no one is locked to it
        // Loop through losers
        for (int y = 0; y < candidate_count; y++)
        {
            // If lock is == to candidate_count then [x] is the winner
            if (!locked[y][x])
            {
                lockless++;
                if (lockless == candidate_count)
                {
                    printf("%s\n", candidates[x]);
                }
            }
        }
    }
    return;
}


/* ###########################################################################
 * #                              User Functions                             #
 * ########################################################################### */

bool cycle(int winner, int loser)
{
    // Base case. Sign that it make a cycle
    if (loser == winner)
    {
        return true;
    }
    
    // Loop through pairs
    for (int i = 0; i < candidate_count; i++)
    {
        /* If the loser of loser of the winner is locked
         * Then we should check that loser if they are equal to the winner
         * which means its a cycle, if not repeat the process until its empty */
        if (locked[loser][i])
        {
            if (cycle(winner, i))
            {
                return true;
            }
        }
    }
    return false;
}

