def case():
    sounds = [x for x in input().strip().split()]
    say = set()
    while True:
        line = input().strip()
        if line == "what does the fox say?":
            break
        _, _, s = [x for x in line.split()]
        say.add(s)

    print(" ".join([s for s in sounds if s not in say]))

T = int(input().strip())
for i in range(T):
    case()


