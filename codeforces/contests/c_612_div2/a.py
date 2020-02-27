t = int(input())

for i in range(t):
  n = int(input())
  s = input()

  l = 0
  count = 0
  start = False
  for c in s:
    if c == 'A':
      l = max(l, count)
      count = 0
      start = True
    elif start:
      count += 1
  l = max(l, count)
 
  print(l)
