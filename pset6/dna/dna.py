import csv
from sys import argv, exit

def main():
    # The program should require as its first command-line argument the name of a CSV file containing the STR counts for a list of individuals
    # second command-line argument the name of a text file containing the DNA sequence to identify.
    if len(argv) != 3:
        print("missing command-line argument")
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)
    csv_file = argv[1]
    txt_file = argv[2]

    # open the CSV file and read its contents into memory.
    # assume that the first row of the CSV file will be the column names.
    # The first column will be the word name and the remaining columns will be the STR sequences themselves.
    d = dict()
    str = set()
    with open(csv_file, "r") as file:
        reader = csv.DictReader(open(csv_file))
        for row in reader:
            for pair in row:
                if pair == 'name':
                    name = row[pair]
                    d[name] = {}
                else:
                    d[name].update({pair: int(row[pair])})
                    str.add(pair)
    forensic = dict()
    # open the DNA sequence and read its contents into memory.
    with open(txt_file, "r") as dna:
        dna_seq = dna.read()
        max_count = cur_count = length = 0
        for sequence in str:
            length = len(sequence)
            # iterate through dna sequence
            for i in range(len(dna_seq)):
                # if match found
                if dna_seq[i:i+length] == sequence:
                    cur_count += 1
                    # check for consecutive matches
                    while dna_seq[i+(length * (cur_count)):i+(length * (cur_count + 1))] == sequence:
                        cur_count += 1
                    max_count = max(max_count, cur_count)
                else:
                    cur_count = 0

            forensic[sequence] = max_count
            cur_count = 0
            max_count = 0

    # find matches in our dna database
    for person in d:
        if d[person] == forensic:
            print(person)
            exit(0)
    print('No match')
main()