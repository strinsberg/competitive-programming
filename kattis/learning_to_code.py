def sub(tk, ns):
  if tk[0] == "$":
    return ns[tk[2:len(tk)-1]]
  return tk

line = ["begin"]
cut = "\"`;"

ns = {}

while line[0] != "end.":
  line = [sub(x.strip(cut), ns) for x in input().strip(cut).split()]
  print(line)

  if line[0] == "var":
    val = " ".join(line[3:]).strip(cut)

    if val in ns:
      ns[line[1]] = ns[val]
    else:
      ns[line[1]] = val

  elif line[0] == "print":
    val = " ".join(line[1:]).strip(cut)

    if val in ns:
      print(ns[val])
    else:
      print(val)
