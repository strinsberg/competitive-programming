n, k = [int(x) for x in input().strip().split()]

def match(a, b):
  s = []
  for i in range(k):
    if a[i] == b[i]:
      s.append(a[i])
    elif a[i] == 'S':
      if b[i] == 'E':
        s.append('T')
      elif b[i] == 'T':
        s.append('E')
    elif a[i] == 'E':
      if b[i] == 'S':
        s.append('T')
      elif b[i] == 'T':
        s.append('S')
    elif a[i] == 'T':
      if b[i] == 'S':
        s.append('E')
      elif b[i] == 'E':
        s.append('S')
  return "".join(s) 

ans = 0
cards = set()
for i in range(n):
  cards.add(input())
  
seen = set()
for c in cards:
  seen.add(c)
  for c2 in cards:
    if c == c2 or c2 in seen:
      continue
    s = match(c, c2)
    if s in cards:
      ans += 1

print(int(ans / 3))
