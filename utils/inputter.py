import sys
def bin2switches(num_to_input, prefix="A"):
    for n, digit in enumerate(reversed(str(num_to_input).strip()), start=1):
        if digit == "1":
            yield "+" + prefix + str(n)
        elif digit == "0":
            yield "-" + prefix + str(n)
        else:
            raise ValueError("Not valid Binary Number")


def int2bin(num):
    return bin(num)[2:]

def add(num1, num2, filename="num.in"):
    f = open(filename, 'w')
    for line in bin2switches(int2bin(num1), prefix="A"):
        f.write(line + '\n')
    for line in bin2switches(int2bin(num2), prefix="B"):
        f.write(line+ '\n')

if __name__ == "__main__":
    assert(len(sys.argv) == 3)
    add(int(sys.argv[1]), int(sys.argv[2]))
