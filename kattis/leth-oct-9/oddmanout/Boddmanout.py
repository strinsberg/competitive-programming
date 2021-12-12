from collections import *

n = int(input().strip())
for i in range(n):
    g = int(input().strip())
    guests = Counter(input().strip().split())
    guests = [x for x in guests.keys()
              if guests[x] == 1]
    print(f"Case #{i+1}: {' '.join(guests)}")
