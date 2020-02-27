def xor(a):
  return 1

def addto(a, s, x):
  return "some answer"

tests = int(input())

for i in range(tests):
  n = int(input())
  a = [int(x) for x in input().strip().split()]

  s = sum(a)
  x = 2 * xor(a)

  if s == a:
    print("???")
  else:
    print(addto(a, s, x))

