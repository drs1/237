import sys
for x in range(1,255):
    binary=bin(x)
    sys.stdout.write(binary[2:]+',')
