def toBin(n):
    if n >= 0:
        return str(bin(n))[2:]
    else:
        return str(bin(2 ** 32 + n))[2:]

with open ("binary.in", "r") as f:
    lines = f.readlines()
    f.close()

with open ("binary.out", "r") as f:
    lines2 = f.readlines()
    f.close()

n = 0;
for i in range(0,len(lines)):
    a = toBin(int(lines[i]))
    b = int(lines2[i])
    if int(a) == b:
        print "Yes!"
    else:
        print "NO!"
        n += 1
print n
