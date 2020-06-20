from cs50 import get_string


def main():
    credit_card_num = get_string("Number: ")
    while True:
        if len(credit_card_num) > 0:
            break
    is_credit = luhns(credit_card_num)

    if is_credit:
        print(card_type(credit_card_num))
    else:
        print("INVALID")


def luhns(card_num):
    card_copy = int(card_num)
    doubled = 0
    not_doubled = 0

    while card_copy > 0:
        not_doubled += card_copy % 10
        doubled = (doubled * 10) + ((card_copy % 100 // 10) * 2)
        card_copy = card_copy // 100

    product_sum = 0
    while doubled > 0:
        product_sum += doubled % 10
        doubled = doubled // 10

    product_sum += not_doubled
    last_digit = product_sum % 10

    return (last_digit == 0)


def card_type(card_num):
    nDigits = len(card_num)
    card = None
    prefix = 0
    if (nDigits == 13):
        prefix = int(card_num[0])
        if prefix == 4:
            card = "VISA"
        else:
            card = "INVALID"
    elif (nDigits == 15):
        prefix = int(card_num[0:2])
        if (prefix == 34 or prefix == 37):
            card = "AMEX"
        else:
            card = "INVALID"
    elif (nDigits == 16):
        prefix = int(card_num[0])
        if (prefix == 5):
            prefix = int(card_num[0:2])
            if (prefix == 51 or prefix == 52 or prefix == 53 or prefix == 54 or prefix == 55):
                card = "MASTERCARD"
            else:
                card = "INVALID"
        elif prefix == 4:
            card = "VISA"
    else:
        card = "INVALID"
    return card


main()
