# -*- coding: utf-8 -*-

from os import lseek
from intelhex import IntelHex

# +-> X
# |
# v
# Y

I = ['####',
     '0000']

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

x_coord = []
y_coord = []

for i in [I, J, L, O, S, T, Z, I]:
    for x in range(4):
        for y in range(2):
            if i[y][x] == '#':
                # Инвертируем, т.к. выход ПЗУ = инверсный
                x_coord.append(x ^ 3)
                y_coord.append(y ^ 3)

# print(coord)

'''
MSB             LSB
0000 0000 0000 0000
                 \--x0
               \----y0
            \-------x1
          \---------y1
       \------------x2
     \--------------y2
  \-----------------x3
\-------------------y3
'''

offset = 32

hex = IntelHex()
for i in range(int(len(x_coord) / 4)):
     x = x_coord[i*4:i*4+4]
     y = y_coord[i*4:i*4+4]
     hex.puts(i*2  , bytes([x[0] + (y[0] << 2) + (x[1] << 4) + (y[1] << 6)]))
     hex.puts(i*2+1, bytes([x[2] + (y[2] << 2) + (x[3] << 4) + (y[3] << 6)]))
hex.write_hex_file('../proteus/figure.hex', byte_count = 16)

print(f'Writed {hex.maxaddr() + 1} bytes to figure.hex.')

for i in range(8):
    tmp = hex[i*2] + (hex[i*2+1] << 8)
    print(f'{tmp:0>16b}')

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

     hexsz = 512
     strsz = 16
     rectangle = ''.join([''.join(i.split()).ljust(strsz, '0') for i in v]).rjust(hexsz, '0')
     dots = bytearray([])
     for y in range(int(hexsz / strsz)):
          for x in range(strsz):
              ch = rectangle[hexsz - ((y + 1)  * strsz) + x]
              d = 0 if ch == '0' else 0xFF
              dots.append(d)

     hex = IntelHex()
     hex.frombytes(dots)
     hex.write_hex_file(f'../proteus/k155ru5_{k}.hex', byte_count = strsz)

     print(f'Writed {hex.maxaddr() + 1} bytes to k155ru5_{k}.hex.')
