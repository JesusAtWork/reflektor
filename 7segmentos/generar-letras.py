import itertools
import struct

CHARACTERS = """
 _
|_|
|_|.
(*)

 _

(-)


 _
(_)

  |
  |
(1)
 _
 _|
|_
(2)
 _
 _|
 _|
(3)

|_|
  |
(4)
 _
|_
 _|
(5)
 _
|_
|_|
(6)
 _
  |
  |
(7)
 _
|_|
|_|
(8)
 _
|_|
  |
(9)
 _
| |
|_|
(0)



( )


   .
(.)
 _
|_|
| |
(A)

|_
|_|
(B)
 _
|
|_
(C)

 _|
|_|
(D)
 _
|_
|_
(E)
 _
|_
|
(F)
 _
|_
|_|
(G)

|_|
| |
(H)

|
|
(I)

  |
|_|
(J)
 _
|_
| |
(K)

|
|_
(L)
 _
| |
| |
(M)

 _
| |
(N)

 _
|_|
(O)
 _
|_|
|
(P)
 _
| |
|_|.
(Q)

 _
|
(R)
 _
|_
 _|
(S)

|_
|_
(T)


|_|
(U)

| |
|_|
(V)

|_|
|_|
(W)

|_|
| |
(X)

|_|
 _|
(Y)
 _
 _|
|_
(Z)"""

def enabled(line, pos, accepted):
    if len(line) > pos:
        return line[pos] == accepted
    else:
        return False

def grouper(n, iterable, fillvalue=None):
    "grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx"
    args = [iter(iterable)] * n
    return itertools.izip_longest(fillvalue=fillvalue, *args)

def build_segments():
    charmap = {}
    all_lines = CHARACTERS.split("\n")[1:]

    for l in grouper(4, all_lines):
        if len(l) < 3:
            break
        a = enabled(l[0], 1, "_") << 0
        b = enabled(l[1], 2, "|") << 1
        c = enabled(l[2], 2, "|") << 2
        d = enabled(l[2], 1, "_") << 3
        e = enabled(l[2], 0, "|") << 4
        f = enabled(l[1], 0, "|") << 5
        g = enabled(l[1], 1, "_") << 6
        h = enabled(l[2], 3, ".") << 7
        char = l[3][1]
        charmap[char] = a | b | c | d | e | f | g | h

    segments = [charmap["*"]] * 128
    for k, v in charmap.items():
        segments[ord(k)] = v

    return segments

segment_str = ", ".join("%d" % n for n in build_segments())

print "byte segments[128] = {%s};" % segment_str
