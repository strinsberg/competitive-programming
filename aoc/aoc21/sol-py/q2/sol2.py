import sys

def update(com, amt, pos, aim):
    if com == "forward":
        return (pos[0]+amt, pos[1]+(aim*amt)), aim
    elif com == "up":
        return (pos[0], pos[1]), aim-amt
    else:
        return (pos[0], pos[1]), aim+amt

pos = (0,0)
aim = 0
for line in sys.stdin.readlines():
    com, amt = line.strip().split()
    pos, aim = update(com, int(amt), pos, aim)

print(pos[0] * pos[1])

