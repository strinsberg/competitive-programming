def solve():
  n = int(input())
  a = [int(x) for x in input().strip().split()]

  for i in range(n - 1):
    if abs(a[i + 1] - a[i]) > 1:
      print("YES")
      print(i+1, i+2)
      return
    
  print("NO")



tests = int(input())

for i in range(tests):
  solve()
