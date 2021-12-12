def expect(amount,l,k,t1,t2,h):
    f = amount / t1
    t3 = t1 - (l / f)
    ld = k * t3
    la = t2 * k
    #print(f,t3,ld,la)
    #print(abs(ld + la + h - amount))
    return ld + la + h - amount

def search(l,k,t1,t2,h):
    low = 0
    high = 100000000
    while high > low:
        mid = low + ((high - low) / 2)
        res = expect(mid,l,k,t1,t2,h)
        #print(high, low, mid, res)
        if abs(res) < 0.0000001:
            return mid
        elif res < 0:
            high = mid
        else:
            low = mid
    return -1

def solve():
    l,k,t1,t2,h = [float(x) for x in input().strip().split()]
    f1 = h
    f2 = h
    if h == l:
        f2 = search(l,k,t1,t2,h)
    elif h > l:
        f2 = search(l,k,t1,t2,h)
        f1 = f2

    print(f"{f1:0.6f} {f2:0.6f}")

solve()
