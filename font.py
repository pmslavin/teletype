#!/usr/bin/env python
"""
            Bitmap glyph-to-include codec

  ..####..                                   ..####..
  .#.##.#.                                   .#.##.#.
  ##.##.##                               -d  ##.##.##
  ########  => 60,90,219,255,129,66,60,0 ==> ########
  #......#                                   #......#
  .#....#.                                   .#....#.
  ..####..                                   ..####..
  ........                                   ........
"""
import sys
from string import maketrans

def font(src):
    tr = maketrans(".#","01")
    char = []
    for i,r in enumerate(src.split(),1):
        char.append(str(int(r.translate(tr), 2)))
        if not i&7:
            print(",".join(char)+',')
            char = []

def defont(src):
    tr = maketrans("01",".#")
    for char in src.split():
        for uint in char.split(',')[:-1]:
            b = bin(int(uint)).replace("0b","")
            fline = '0'*(8-len(b)) + b
            print(fline.translate(tr))

        print('')


if __name__ == "__main__":
    src = sys.stdin.read()
    if sys.argv[-1:][0] == '-d':
        defont(src)
    else:
        font(src)
