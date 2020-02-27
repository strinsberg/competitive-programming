def part(A):
  partition = []
  temp = [0, 0, 0]
  for x in A:
    if x == 0:
      temp[0] += 1
    else:
      temp[2] = x
      partition.append(temp)
      temp = [0, x, 0]
  if A[len(A)-1] == 0:
    partition.append(temp)
  return partition

def num_even_odd(A):
  e, o = 0, 0
  for x in A:
    if x == 0:
      continue
    if x % 2 == 0:
      e += 1
    else:
      o += 1
  n = len(A)
  return n // 2 - e, n // 2 - o + (n % 2)

def parity(p):
  return p[1] % 2 == p[2] % 2

def end(p):
  return p[1] == 0 or p[2] == 0

def solve():
  n = int(input())
  A = [int(x) for x in input().strip().split()]

  partition = part(A)
  even, odd = num_even_odd(A)

  partition.sort()
  partition.sort(key=lambda p: end(p) or parity(p))
  print(partition)
 
  ans = 0 
  for p in partition:
    c, l, r = p
    
    if parity(p) or end(p):
      pass
    else:
      ans+=1
  
  print(ans)


solve()
