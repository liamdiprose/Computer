import sys

filename = sys.argv[1]
d = dict()


for line in open(filename, 'r').readlines():
    outputName, val = line.split(':')
    d[outputName] = val.strip()

for keyval in reversed(sorted(d.items(), key=lambda x: int(x[0][1:]))):
    print(keyval[1], end='')
print()