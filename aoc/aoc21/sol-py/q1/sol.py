n = 2000
scan = []
for i in range(n):
    scan.append(int(input()))

ans = 0
for i in range(1, n):
    if (scan[i] > scan[i-1]):
        ans += 1

print(ans)

