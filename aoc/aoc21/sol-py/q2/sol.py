import sys

def update(com, amt, pos):
    if com == "forward":
        return pos[0]+amt, pos[1]
    elif com == "up":
        return pos[0], pos[1]-amt
    else:
        return pos[0], pos[1]+amt

pos = (0,0)
for line in sys.stdin.readlines():
    com, amt = line.strip().split()
    pos = update(com, int(amt), pos)

print(pos[0] * pos[1])
