import csv
from sys import argv, exit
from cs50 import SQL


def main():
    # check for missing command-line argument
    if len(argv) != 2:
        print("python import.py characters.csv")
        exit(1)

    csv_file = argv[1]
    # Create database
    # Schema: first | middle | last | house | birth
    db = SQL("sqlite:///students.db")
    count = 0
    # db.execute("CREATE TABLE students (id INT, first TEXT, last TEXT, middle TEXT, house TEXT, birth NUMERIC, PRIMARY KEY(id))")
    with open(csv_file) as file:
        # Create DictReader
        reader = csv.DictReader(file)
        # Iterate over CSV file
        for row in reader:
            if row['name']:
                name = row['name'].split()
                if len(name) == 2:
                    middle = None
                else:
                    middle = name[1]
                first = name[0]
                last = name[-1]

            # Insert show by substituting values into each ? placeholder
            db.execute("INSERT INTO students (id, first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?, ?)",
                       count, first, middle, last, row["house"], row["birth"])
            count += 1
            print(row)
    exit(0)


main()