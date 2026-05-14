#!/usr/bin/env python3
"""Extract references/biosync.png into a packed C bitmap header.

Mirrors the format used by hourly_static.h / power_static.h: a 176x136
monochrome bitmap packed MSB-first, 22 bytes per row.
"""

from PIL import Image

SRC = "/home/q/1Projects/Watchy/watchy-screens/references/biosync.png"
DST = "/home/q/1Projects/Watchy/watchy-screens/src/faces/biosync_static.h"

im = Image.open(SRC).convert("RGBA")
W, H = im.size
assert (W, H) == (176, 136), f"Unexpected size {W}x{H}"

px = im.load()
stride = W // 8  # 22

# Alpha-aware extraction: source has hidden black pixels under alpha=0 in
# the chamfered corners. Only treat a pixel as ink if it's both visible
# (alpha >= 128) AND dark (mean RGB < 128).
rows = []
for y in range(H):
    row = bytearray(stride)
    for x in range(W):
        r, g, b, a = px[x, y]
        if a >= 128 and (r + g + b) / 3 < 128:
            row[x // 8] |= 0x80 >> (x % 8)
    rows.append(bytes(row))

with open(DST, "w") as f:
    f.write("// Auto-extracted from references/biosync.png by tools/extract_biosync.py\n")
    f.write("// Full 176x136 face content as a single packed bitmap.\n")
    f.write("#ifndef WATCHY_SCREENS_BIOSYNC_STATIC_H\n")
    f.write("#define WATCHY_SCREENS_BIOSYNC_STATIC_H\n\n")
    f.write("#include <stdint.h>\n\n")
    f.write(f"static const int GLYPH_BIO_FACE_W = {W};\n")
    f.write(f"static const int GLYPH_BIO_FACE_H = {H};\n")
    f.write(f"static const int GLYPH_BIO_FACE_STRIDE = {stride};\n")
    f.write(f"static const uint8_t GLYPH_BIO_FACE_ROWS[{H}][{stride}] = {{\n")
    for row in rows:
        bytes_str = ", ".join(f"0x{b:02X}" for b in row)
        f.write(f"    {{ {bytes_str} }},\n")
    f.write("};\n\n")
    f.write("#endif // WATCHY_SCREENS_BIOSYNC_STATIC_H\n")

print(f"wrote {DST} ({H} rows x {stride} bytes)")
