import csv
from sys import argv, exit
from cs50 import SQL


def main():
    # check for missing command-line argument
    if len(argv) != 2:
        print("python roster.py House")
        exit(1)

    # Create database
    # Schema: first | middle | last | house | birth
    db = SQL("sqlite:///students.db")

    house = argv[1]

    query = db.execute("SELECT first, last, middle, birth FROM students WHERE house=? ORDER BY last, first", house)

    for person in query:
        if person['middle'] == None:
            full_name = person['first'] + ' ' + person['last']
        else:
            full_name = person['first'] + ' ' + person['middle'] + ' ' + person['last']
        print(f'{full_name}, born {person["birth"]}')
    exit(0)


main()