def sub(val, amt):
  if val - amt < 0:
    val = 0
    return amt - val
  else:
    val -= amt
    return 0
  
def update_odds(odds, evens, count):
  left = sub(odds, count)
  if left:
    evens -= count
    return True
  return False

def update_evens(evens, odds, count):
  left = sub(evens, count)
  if left:
    odds -= count
    return True
  return False

# Start
n = int(input())
nums = [int(x) for x in input().strip().split()]

# find number of even and odd numbers missing
e, o = 0, 0
for i in range(n):
  if nums[i] == 0:
    continue
  if nums[i] % 2 == 0:
    e += 1
  else:
    o += 1
evens = n // 2 - e
odds = n // 2 - o + (n % 2)

# find how many gaps there are and what is on either side
pairs = []
last = -1
count = 0

for i in range(n):
  if nums[i] == 0:
    count += 1
  else:
    if last == -1 and i > 0:
      pairs.append((count, 0, nums[i] % 2, 1))
    elif i > 0:
      comp = 0 if nums[i] % 2 ==  last % 2 else 1
      pairs.append((count, comp, nums[i] % 2, 0))
    last = nums[i]
    count = 0

if nums[len(nums) - 1] == 0:
  pairs.append((count, 0, last % 2, 1))

# find out how many gaps can be filled without putting odds and evens together
ans = 0
pairs = sorted(pairs, key=lambda p: p[3])
pairs.sort()
pairs = sorted(pairs, key=lambda p: p[1])
print(pairs)
print(evens, odds)
for p in pairs:
  count, parity, odd, end = p
  if count == 0:
    ans += parity

  if odd:
    if update_odds(odds, evens, count):
      if end or parity:
        ans += 1
      else:
        ans += 2
  else:
    if update_evens(evens, odds, count):
      if end or parity:
        ans += 1
      else:
        ans += 2

# print answer
print(ans)
