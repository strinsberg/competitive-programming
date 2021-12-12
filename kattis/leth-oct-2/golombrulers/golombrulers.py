import sys

def lengths(nums):
    lens = []
    for i in range(len(nums)):
        for j in range(i+1, len(nums)):
            lens.append(nums[j] - nums[i])
    return lens


def case(line):
    nums = [int(x) for x in line.strip().split()]
    nums.sort()
    top = nums[-1]
    lens = lengths(nums)
    unique = set(lens)
    if len(lens) > len(unique):
        print("not a ruler")
        return

    missing = [i for i in range(1,top) if i not in unique]
    if missing:
        print("missing ", end="")
        print(*missing)
    else:
        print("perfect")


lines = sys.stdin.read().strip().split("\n")
for line in lines:
    if line == " ":
        break
    case(line)
