# -*- coding: utf-8 -*-

from os import lseek
from intelhex import IntelHex

# +-> X
# |
# v
# Y

I = ['0000',
     '####']

J = ['###0',
     '00#0']

L = ['###0',
     '#000']

O = ['##00',
     '##00']

S = ['0##0',
     '##00']

T = ['0#00',
     '###0']

Z = ['##00',
     '0##0']

x_coord = bytearray()
y_coord = bytearray()

for i in [I, J, L, O, S, T, Z, I]:
    for x in range(4):
        for y in range(2):
            if i[y][x] == '#':
                x_coord.append(x)
                y_coord.append(y)

print(x_coord)
print(y_coord)

'''
MSB   LSB
0000 0000
| |  | \-- x
| |  \---- y
| \------- ~x-1
\--------- ~y-1
'''

hex = IntelHex()
for i in range(len(x_coord)):
    w = bytearray(4)
    w[0] = x_coord[i]
    w[1] = (y_coord[i]) << 2
    w[2] = ((~x_coord[i] - 1) & 0x3) << 4
    w[3] = ((~y_coord[i]) & 0x3) << 6
    wb = sum(w)
    hex.puts(i, bytes([wb]))
hex.write_hex_file('../proteus/figure.hex', byte_count = 8)

print(f'Writed {hex.maxaddr() + 1} bytes to figure.hex.')

# Y
# ^
# Y
# +-> X

rect = {}

rect['full'] = [
     '# # 0 0 0 0 0 0 # #',
     '# 0 # 0 # 0 # 0 # 0',
     '0 # 0 # 0 # 0 # 0 #',
     '# 0 # 0 # 0 # 0 # 0',
     '0 # 0 # 0 # 0 # 0 #',
     '# 0 # 0 # 0 # 0 # 0',
     '0 # 0 # 0 # 0 # 0 #',
     '# 0 # 0 # 0 # 0 # 0',
     '0 # 0 # 0 # 0 # 0 #',
     '# # 0 # 0 # 0 # 0 #',
     '# # 0 # 0 # 0 # 0 #',
     '0 # 0 # 0 # 0 # 0 0',
     '0 # 0 # 0 # 0 # 0 0',
     '# 0 # 0 # 0 # 0 # 0',
     '0 # 0 # 0 # 0 # 0 #',
     '# # # # # # # # # #',
     '# 0 # 0 # 0 # 0 # 0',
     '0 # 0 # 0 # 0 # 0 #',
     '# # # # # # # # # #',
     '0 # 0 # 0 # 0 # 0 #',
     ]

rect['empty'] = [
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 0 0 0 0 0 0 0',
     '0 0 0 # 0 0 0 0 0 0',
     '0 0 0 # 0 0 0 0 0 0',
     '0 0 0 # 0 0 0 0 0 0',
     '# # # # # # # 0 0 0',
     ]

for k, v in rect.items():
     dots = bytearray()
     v.reverse()
     for i in v:
          w = 0xFFFF
          i = ''.join(i.split())[::-1]
          for j in range(len(i)):
              w <<= 1
              w += 1 if i[j] == '0' else 0 # inverse
          wb = bytearray(2)
          wb[0] = w & 0xFF
          wb[1] = (w >> 8) & 0xFF
          dots.extend(wb)
     dots.extend([0xFF] * (64 - len(dots)))

     hex = IntelHex()
     hex.frombytes(dots)
     hex.write_hex_file(f'../proteus/k155ru5_{k}.hex', byte_count = 8)

     print(f'Writed {hex.maxaddr() + 1} bytes to k155ru5_{k}.hex.')
