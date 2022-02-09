#!/usr/bin/python

'''
Each ROM contains 4 waves, 8bit unsigned format, with the following data:
2048 samples octave 0
2048 samples octave 1
1024 samples octave 2
1024 samples octave 3
512 samples octave 4
512 samples octave 5
512 samples octave 6
512 samples octave 7

Standard waves:
HN613256P-T70	1-4
HN613256P-T71	5-8
HN613256P-CB4	9-12
HN613256P-CB5	13-16

Expansion waves ("Version E"):
EXP-1		1-4
EXP-3		5-8
EXP-2		9-12
EXP-4		13-16
'''

import sys, struct, tarfile
import matplotlib.pyplot as plt

STANDARD     = ["HN613256P-T70", "HN613256P-T71", "HN613256P-CB4", "HN613256P-CB5"]
EXPANSION    = ["EXP-1", "EXP-3", "EXP-2", "EXP-4"]
OCTAVE_SIZES = [2048, 2048, 1024, 1024, 512, 512, 512, 512]

def read_rom(fo, tables):
    tables.extend([[bytearray(fo.read(n)) for n in OCTAVE_SIZES] for i in range(4)])
     
def avg(lst):
    return sum(lst) / len(lst)

with tarfile.open("dw8000-waves.tar.gz", "r:gz") as f:
    wavetables = { "std": [], "exp": [] }
    for fname in STANDARD:
        read_rom(f.extractfile(f"dw8000-waves/{fname}.bin"), wavetables["std"])
    for fname in EXPANSION:
        read_rom(f.extractfile(f"dw8000-waves/{fname}.bin"), wavetables["exp"])

if __name__ == "__main__":
    if len(sys.argv) < 2 or sys.argv[1] not in ("std", "exp"):
        print("Usage: ./extract_waves.py std|exp [octave]")
        sys.exit(-1)
    octave = 4 if len(sys.argv) < 3 else int(sys.argv[2])
    table = wavetables[sys.argv[1]]

    fig, axs = plt.subplots(4, 4)
    print("const uint8_t wavetable[16][32] = {")
    for i in range(4):
        for j in range(4):
            wave = [table[4 * i + j][octave][x*16 + 8] for x in range(32)]
            print("    {" + ", ".join(["{:3d}".format(x) for x in wave]) + "},")
            axs[i, j].axes.xaxis.set_visible(False)
            axs[i, j].axes.yaxis.set_visible(False)
            axs[i, j].plot(wave)
    print("};")
    plt.show()

