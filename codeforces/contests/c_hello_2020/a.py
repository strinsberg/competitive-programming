n, m = [int(x) for x in input().strip().split()]
S = [x for x in input().strip().split()]
T = [x for x in input().strip().split()]
q = int(input())

for i in range(q):
  y = int(input())
  s = y % len(S)
  t = y % len(T)

  print(S[s - 1] + T[t - 1])
