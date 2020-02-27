n = int(input())
moves = input()

l = 0
r = 0

for i in moves:
  if i == 'L':
    l += 1
  else:
    r += 1

print(r + l + 1)
