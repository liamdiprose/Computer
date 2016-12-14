BITS_TO_GENERATE = 35
INPUT1_PREFIX = "A"
INPUT2_PREFIX = "B"
OUTPUT_PREFIX = "S"

NUM_STARTS_AT = 1

from yaml import dump

name = str(BITS_TO_GENERATE) + " bit adder"
filename = name.replace(" ", '').upper() + ".component"

input1 = [INPUT1_PREFIX + str(x) for x in range(NUM_STARTS_AT, BITS_TO_GENERATE+1)]
input2 = [INPUT2_PREFIX + str(x) for x in range(NUM_STARTS_AT, BITS_TO_GENERATE+1)]
inputs = input1 + input2
outputs = [OUTPUT_PREFIX + str(x) for x in range(NUM_STARTS_AT, BITS_TO_GENERATE+2)]

# Components
components = list()
components2 = dict()
components2["XOR1"] = "XOR"
components2["AND1"] = "AND"
for i in range(2, BITS_TO_GENERATE+1):
    components.append(("FA" + str(i),"FULLADDER"))
    components2["FA" + str(i)] = "FULLADDER"


m = dict()
m["name"] = name
m["id"] = name.replace(' ', '').upper()

m['inputs'] = inputs
m['outputs'] = outputs
m["components"] = components2

m["A1"] = {"XOR1": 1, "AND1": -1}
m["B1"] = {"XOR1": 2, "AND1": -1}
m["XOR1"] = {"OUT": "S1"}
m["AND1"] = {"FA2" : "Cin"}

for n, inp in enumerate(input1[1:], start=2):
    m[inp] = {"FA" + str(n): "A"}
for n, inp in enumerate(input2[1:], start=2):
    m[inp] = {"FA" + str(n): "B"}

for n, comp in enumerate(components, start=2):
    s = {"OUT" : "S" + str(n)}
    if n == len(components)+1:
        cout = {"OUT" : "S" + str(n+1)}
    else:
        cout = {"FA" + str(n+1) : "Cin"}
    m[comp[0]] = {"S": s, "Cout": cout}

f = open("components/" + filename, 'w')
f.write(dump(m))