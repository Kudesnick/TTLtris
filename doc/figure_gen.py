# -*- coding: utf-8 -*-

from os import lseek
from intelhex import IntelHex

points = [
    #X0 Y0 X1 X2
    [0, 1, 0, 2], # 1
    [1, 1, 0, 1], # 2
    [2, 1, 0, 0], # 3
    [0, 0, 1, 2], # 4
    [1, 0, 1, 1], # 5
    [2, 0, 1, 0], # 6
    [3, 0, 1, 3], # 7
]

fig = [
    [1, 2, 4, 5], # O
    [4, 5, 6, 7], # I
    [1, 2, 5, 6], # Z
    [2, 3, 4, 5], # S
    [1, 4, 5, 6], # J
    [3, 4, 5, 6], # L
    [2, 4, 5, 6], # T
    [2, 4, 5, 6], # T
]

hex = IntelHex()
addr = 0
for f in fig:
    for i in range(4):
        w = bytearray(4)
        w[0] = points[f[i]-1][0] << 0 # X0
        w[1] = points[f[i]-1][1] << 2 # Y0
        w[2] = points[f[i]-1][2] << 4 # X1
        w[3] = points[f[i]-1][3] << 6 # X2
        wb = sum(w)
        hex.puts(addr + i, bytes([wb]))
    addr += 4
fname = 'figure.hex'
hex.write_hex_file('../proteus/' + fname, byte_count = 8)

print(f'Writed {hex.maxaddr() + 1} bytes to {fname}.')

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
