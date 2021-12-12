P, S = [int(x) for x in input().strip().split()]

people = []
for i in range(P):
    name, bid = [x for x in input().strip().split()]
    people.append((int(bid), name))

people.sort()
people.reverse()
group = []
total = 0
s = S
for p in people:
    if p[0] <= s:
        group.append(p[1])
        s -= p[0]
        total += p[0]

if total == S:
    print(len(group))
    if group:
        group.sort()
        print("\n".join(group))
else:
    print(0)


