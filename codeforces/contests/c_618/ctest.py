import random
import itertools
import math

def f(x,y):
  return (x|y) - y

def gen(A):
  for i in range(5):
    A.append(random.randint(0,64))

def test(i, A):
  if i == 1:
    return f(A[0],A[i])
  else:
    return f(test(i-1, A), A[i])


def is_on(i, x):
  return (x >> i) & 1 == 1

def times_is_on(i, A):
  ans = 0
  for x in A:
    ans += is_on(i, x)
  return ans

def get_msb(x):
  n = x + 0
  ans = 0
  while n > 0:
    n = n >> 1
    ans +=1
  return ans - 1

def solve(A):
  mx = max(A)
  msb = get_msb(mx)

  bit = 0
  while (msb >= 0):
    if times_is_on(msb, A) == 1:
      bit = msb
      break
    msb -= 1

  if bit != 0:
    for x in A:
      if is_on(bit, x):
        A.remove(x)
        A.insert(0,x)
  return A
  
    
B = []
gen(B)
seen = set()
for A in itertools.permutations(B):
  ans = test(len(A) - 1, A)
  if A[0] not in seen:
    print("f_val:",ans, "Ordering:", A)
    seen.add(A[0])

print("Solve:", solve(list(B)))
