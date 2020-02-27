test = int(input())

for i in range(test):
  n, k1, k2 = [int(x) for x in input().strip().split()]
  p1 = [int(x) for x in input().strip().split()]
  p2 = [int(x) for x in input().strip().split()]

  m1 = max(p1)
  m2 = max(p2)

  if m1 > m2:
    print("YES")
  else:
    print("NO")
