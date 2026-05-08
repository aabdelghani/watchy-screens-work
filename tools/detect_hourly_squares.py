#!/usr/bin/env python3
"""Compute 60 minute-diamond positions from references/hourly.png.

The HOURLY dial has visible square markers along its straight edges,
spaced 8 px apart between perpendicular tick strokes. The four chamfer
corners use a decorative checker pattern with no individual square
markers — diamonds in those regions are placed evenly along the inset
chamfer line.

Geometry (matches existing kMinuteDot conventions):
  - Top straight edge:    14 cells, diamonds at y = 11
  - Right straight edge:   8 cells, diamonds at x = 164
  - Bottom straight edge: 14 cells, diamonds at y = 124
  - Left straight edge:    8 cells, diamonds at x = 12
  - 4 chamfers:            4 cells each (16 total), evenly along the
                           inset diagonal (1 px inside the dial line)

Total: 44 + 16 = 60.

Clockwise minute assignment (validated against the existing override
for minute 47 = (12, 48)):
  - Minutes 53..6  → top edge cells 0..13   (left → right)
  - Minutes 7..10  → TR chamfer cells 0..3
  - Minutes 11..18 → right edge cells 0..7  (top → bottom)
  - Minutes 19..22 → BR chamfer cells 0..3
  - Minutes 23..36 → bottom edge cells 0..13 (right → left)
  - Minutes 37..40 → BL chamfer cells 0..3
  - Minutes 41..48 → left edge cells 0..7   (bottom → top)
  - Minutes 49..52 → TL chamfer cells 0..3

Outputs:
  - prints `SQUARE_CENTERS = [...]` to stdout for paste-in.
  - writes /tmp/hourly_squares_debug.png with red 6×6 diamond bboxes
    at each computed center, scaled 4× for inspection.
"""

from PIL import Image, ImageDraw

SRC = "/home/q/1Projects/Watchy/watchy-screens/references/hourly.png"
DEBUG_OUT = "/tmp/hourly_squares_debug.png"

INSET_TOP = 11
INSET_BOT = 124
INSET_LEFT = 12
INSET_RIGHT = 164

# Inset chamfer endpoints (where the diamond *sits*, 1 px inside the
# dial chamfer line). These match the inner-octagon vertices used in
# the original gen_hourly_minute_map.py walk.
TL_INSET = ((29, 11), (11, 29))   # top edge end → left edge start
TR_INSET = ((146, 11), (164, 29))
BR_INSET = ((164, 106), (146, 124))
BL_INSET = ((29, 124), (11, 106))


def load_mask():
    im = Image.open(SRC).convert("L")
    return [[1 if im.getpixel((x, y)) < 128 else 0
             for x in range(im.size[0])] for y in range(im.size[1])], im.size


def cluster(seq):
    if not seq:
        return []
    runs = [[seq[0]]]
    for v in seq[1:]:
        if v - runs[-1][-1] <= 1:
            runs[-1].append(v)
        else:
            runs.append([v])
    return [sum(r) / len(r) for r in runs]


def detect_ticks(mask, axis, band, scan_range):
    """Find tick positions on a straight edge.

    axis='v' → vertical strokes (top/bottom edges): scan x in scan_range,
              count black pixels in y in `band`. Returns x-coords.
    axis='h' → horizontal strokes (left/right edges): scan y in scan_range,
              count black pixels in x in `band`. Returns y-coords.
    """
    raw = []
    for s in range(*scan_range):
        if axis == 'v':
            c = sum(mask[y][s] for y in range(*band))
        else:
            c = sum(mask[s][x] for x in range(*band))
        if c >= 5:
            raw.append(s)
    return cluster(raw)


def edge_cell_centers(ticks):
    """Return midpoint between each consecutive pair of ticks."""
    return [(a + b) / 2 for a, b in zip(ticks[:-1], ticks[1:])]


def chamfer_cells(p_start, p_end, n=4):
    """n evenly-spaced cell centers along the inset chamfer line."""
    out = []
    for i in range(n):
        t = (i + 0.5) / n  # cell midpoints: 1/(2n), 3/(2n), ... (2n-1)/(2n)
        x = p_start[0] + (p_end[0] - p_start[0]) * t
        y = p_start[1] + (p_end[1] - p_start[1]) * t
        out.append((x, y))
    return out


def main():
    mask, _ = load_mask()

    # Detect ticks on each straight edge.
    top_ticks = detect_ticks(mask, 'v', (0, 7), (29, 148))
    bot_ticks = detect_ticks(mask, 'v', (129, 136), (29, 148))
    # Left edge: filter out the spurious y=34 read that bleeds in from
    # the chamfer area (the regular 8-px grid starts at y=36).
    left_ticks_raw = detect_ticks(mask, 'h', (0, 7), (29, 108))
    left_ticks = [v for v in left_ticks_raw if v >= 36 - 0.5]
    right_ticks = detect_ticks(mask, 'h', (169, 176), (29, 108))

    assert len(top_ticks) == 15, f"top: {top_ticks}"
    assert len(bot_ticks) == 15, f"bot: {bot_ticks}"
    assert len(left_ticks) == 9, f"left: {left_ticks}"
    assert len(right_ticks) == 9, f"right: {right_ticks}"

    # Cell centers along straight edges.
    top_cells = [(x, INSET_TOP) for x in edge_cell_centers(top_ticks)]
    bot_cells = [(x, INSET_BOT) for x in edge_cell_centers(bot_ticks)]
    left_cells = [(INSET_LEFT, y) for y in edge_cell_centers(left_ticks)]
    right_cells = [(INSET_RIGHT, y) for y in edge_cell_centers(right_ticks)]

    assert len(top_cells) == 14
    assert len(bot_cells) == 14
    assert len(left_cells) == 8
    assert len(right_cells) == 8

    # Chamfer cells (4 each, evenly along inset diagonal).
    tl_cells = chamfer_cells(*TL_INSET)
    tr_cells = chamfer_cells(*TR_INSET)
    br_cells = chamfer_cells(*BR_INSET)
    bl_cells = chamfer_cells(*BL_INSET)

    # Map minutes to cells.
    centers = [None] * 60

    # Minutes 53..6 → top cells 0..13 (left → right).
    # Minutes 53,54,55,56,57,58,59,0,1,2,3,4,5,6 = 14 minutes.
    top_minute_order = [53, 54, 55, 56, 57, 58, 59, 0, 1, 2, 3, 4, 5, 6]
    for m, cell in zip(top_minute_order, top_cells):
        centers[m] = cell

    # Minutes 7..10 → TR chamfer cells 0..3.
    for i, m in enumerate(range(7, 11)):
        centers[m] = tr_cells[i]

    # Minutes 11..18 → right cells 0..7 (top → bottom).
    for i, m in enumerate(range(11, 19)):
        centers[m] = right_cells[i]

    # Minutes 19..22 → BR chamfer cells 0..3.
    for i, m in enumerate(range(19, 23)):
        centers[m] = br_cells[i]

    # Minutes 23..36 → bottom cells reversed (right → left).
    bot_cells_rev = list(reversed(bot_cells))
    for i, m in enumerate(range(23, 37)):
        centers[m] = bot_cells_rev[i]

    # Minutes 37..40 → BL chamfer cells 0..3.
    for i, m in enumerate(range(37, 41)):
        centers[m] = bl_cells[i]

    # Minutes 41..48 → left cells reversed (bottom → top).
    left_cells_rev = list(reversed(left_cells))
    for i, m in enumerate(range(41, 49)):
        centers[m] = left_cells_rev[i]

    # Minutes 49..52 → TL chamfer cells 0..3.
    for i, m in enumerate(range(49, 53)):
        centers[m] = tl_cells[i]

    assert all(c is not None for c in centers), "missing minute"

    out = [(int(round(x)), int(round(y))) for x, y in centers]

    print("SQUARE_CENTERS = [")
    for m, (x, y) in enumerate(out):
        print(f"    ({x:3d}, {y:3d}),  # minute {m}")
    print("]")

    # Debug overlay (4× zoom).
    im = Image.open(SRC).convert("RGB").resize((176 * 4, 136 * 4),
                                               Image.NEAREST)
    d = ImageDraw.Draw(im)
    for m, (x, y) in enumerate(out):
        d.rectangle([(x - 3) * 4, (y - 3) * 4,
                     (x + 3) * 4 - 1, (y + 3) * 4 - 1],
                    outline=(255, 0, 0))
        d.line([(x * 4 - 2, y * 4), (x * 4 + 2, y * 4)], fill=(0, 200, 0))
        d.line([(x * 4, y * 4 - 2), (x * 4, y * 4 + 2)], fill=(0, 200, 0))
    im.save(DEBUG_OUT)
    print(f"\n# debug overlay → {DEBUG_OUT}")


if __name__ == "__main__":
    main()
