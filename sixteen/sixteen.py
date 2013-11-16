from itertools import *

digits = r"""#
 - -
|\|/|
 - -
|/|\|
 - -
)
   -
  | |
    
  | |
 -  
(
 - -
| | |
 - -
  |  
    
@
 - -
    |
 -  
| | |
 - -
-


 - -


.




 -
=


 - -

 - -
&
 -
| |/
 - 
| |
 - -
<

   /|

   \|

_




 - -
*

 \|/
 - -
 /|\

[
   -
  |  
    
  |  
   -
]
 -  
  |  
    
  |  
 -  
0
 - -
|  /|

|/  |
 - -
1

   /|

    |

2
 - -
    |
   -
 /
 - -
3
 - -
   /
 -
   \
 - -
4

| |
 - -
  |

5
 - -
 \
   -
    |
 - -
6
 - -
|
 - -
|   |
 - -
7
 - -
   /
 -
 /

8
 - -
 \ /

 / \
 - -
9
 - -
|  /
 -
 /

A
 - -
|   |
 - -
|   |

B
 - -
 \  |
   -
 /  |
 - -
C
 - -
|

|
 - -
D
 - -
  | |

  | |
 - -
E
 - -
|
 - -
|
 - -
F
 - -
|
 - -
|

G
 - -
|
   -
|   |
 - -
H

|   |
 - -
|   |

I
 - -
  |

  |
 - -
J

    |

|   |
 - -
K

|  /
 -
|  \

L

|

|
 - -
M

|\ /|

|   |

N

|\  |

|  \|

O
 - -
|   |

|   |
 - -
P
 - -
|   |
 - -
|

Q
 - -
|   |

|  \|
 - -
R
 - -
|   |
 - -
|  \

S
 - -
|
 - -
    |
 - -
T
 - -
  |

  |

U

|   |

|   |
 - -
V

|  /

|/

W

| | |

|/ \|

X

 \ /

 / \

Y

 \ /

  |

Z
 - -
   /

 /
 - -
a
 - 
  | 
 - 
| | 
 - 
b

|
 - 
| |
 - 
c


 - 
|
 - 
d

  |
 - 
| |
 -
e
 -
| |
 -
|
 -
f
   -
  |
 - -
  |

g
 -
| |
 -
  |
 -
h

|
 -
| |

i


 -
  |
 - -
j

  |

| |
 -
k

  |/

  |\

l

|

|
 -
m


 - -
| | |

n


 -
| |

o


 -
| |
 -
p
 -
| |
 -
|

q
 -
| |
 -
  |

r


 -
|

s
 -
|
 -
  |
 -
t

  |
 - -
  |
   -
u



| |
 -
v



|/

w



|/ \|

x

 \ /

 / \

y

| |
 -
  |
 -
z


 -
 /
 -
 




"""

values = """
 1 6
DCBA9
 2 5
EF078
 3 4
""".split("\n")[1:-1]

lines = digits.split("\n")

def grouper(n, iterable, fillvalue=None):
    "Collect data into fixed-length chunks or blocks"
    args = [iter(iterable)] * n
    return izip_longest(fillvalue=fillvalue, *args)

table = {}

for g in grouper(6, lines):
    char = g[0]
    val = 0
    for y, line in enumerate(g[1:]):
        l = ""
        for x, c in enumerate(line.rstrip()):
            if c != " ":
                segment = int(values[y][x], 16)
                val += 1 << segment
    table[ord(char)] = val

for n in range(256):
    if n in table:
        v = table[n]
    else:
        v = table[ord("*")]
    print "%d," % v,
