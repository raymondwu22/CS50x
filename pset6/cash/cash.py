from cs50 import get_float

change = get_float("Change Owed: ")
# If the user fails to provide a non-negative value, your program should re-prompt the user for a valid amount again and again until the user complies.
while True:
    if change > 0:
        break

PENNY = 1
NICKEL = 5
DIME = 10
QUARTER = 25
cents = change * 100
num_coins = 0

if cents >= 25:
    num_coins += cents // 25
    cents = cents % 25
    print(f'25: cents={cents} coins={num_coins}')

if cents >= 10:
    num_coins += cents // 10
    cents = cents % 10
    print(f'10: cents={cents} coins={num_coins}')

if cents >= 5:
    num_coins += cents // 5
    cents = cents % 5
    print(f'5: cents={cents} coins={num_coins}')

num_coins += cents
print(int(num_coins))