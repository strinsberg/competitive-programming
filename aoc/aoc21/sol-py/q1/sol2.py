n = 2000
scan = []
for i in range(n):
    scan.append(int(input()))

ans = 0
last = 100000000000
for i in range(2, n):
    a = 0 if i-2 < 0  else scan[i-2]
    b = 0 if i-1 < 0 else scan[i-1]
    c = scan[i]

    s = a + b + c
    if s > last:
        ans += 1
    last = s

print(ans)
