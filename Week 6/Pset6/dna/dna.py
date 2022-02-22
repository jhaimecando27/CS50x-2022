from csv import DictReader, reader
from sys import argv


def main():

    # Check for command-line usage
    if len(argv) != 3:
        print("Usage: python dna.py DATABASE SEQUENCE")

    # Read database file into a variable
    db_path = argv[1]
    with open(db_path, 'r') as db_file:
        db_reader = reader(db_file)
        STRs = next(db_reader)[1:]

    with open(db_path, 'r') as db_file:
        db_reader = DictReader(db_file)
        dna_dict = list(db_reader)
    
    #  Read DNA sequence file into a variable
    seq_path = argv[2]
    with open(seq_path, 'r') as seq_file:
        seq = seq_file.read()

    # Find longest match of each STR in DNA sequence
    seq_data = {}
    for STR in STRs:
        seq_data[STR] = longest_match(seq, STR)

    # Check database for matching profiles
    for dna in dna_dict:
        # Count all STR that match
        STR_count = 0

        # Iterate all same STR in STRs
        for STR in STRs:
            if int(dna[STR]) == seq_data[STR]:
                STR_count += 1
        
        # Display if match found
        if STR_count == len(STRs):
            print(dna['name'])
            exit(0)

    print("No match")
    exit(1)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
