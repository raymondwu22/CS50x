from cs50 import get_string

sentence = get_string("Text: ")
words = sentence.split(" ")
letters = list(sentence)
sentence_count = [char for char in letters if char in ['.', '?', '!']]
letters = [letter for letter in letters if letter.isalpha()]
# print(sentence_count)
# print(f"letters {len(letters)}")
# print(f"words {len(words)}")
# print(f"sentence {len(sentence_count)}")
W = len(words)
L = len(letters) * 100 / W
S = len(sentence_count) * 100 / W

index = 0.0588 * L - 0.296 * S - 15.8
print(index)
if index < 1:
    print("Before Grade 1")
elif index < 16:
    print(f"Grade {round(index)}")
else:
    print("Grade 16+")