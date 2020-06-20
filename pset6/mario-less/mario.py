from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break

brick = 1

for i in range(height):
    for j in range(height - brick):
        print(" ", end="")
    for k in range(brick):
        print("#", end="")
    print("\n", end="")
    brick += 1