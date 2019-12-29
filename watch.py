import os
import time
import random
import argparse
import threading
import subprocess

from time import time
from subprocess import run

import numpy as np
import matplotlib.pyplot as plt

def get_parser():
    parser = argparse.ArgumentParser(description="Show render progress (note: image noise may only be due to plt).")
    parser.add_argument("fname", type=str, help="Output image file name (with path)")
    parser.add_argument("dy", type=int, nargs='?', default=2, help="Print after every dy rows (defaults to 2)")
    parser.add_argument("--keep_ppm", "-ppm", action="store_true", help="Keep the original PPM output image")
    return parser

if __name__ == "__main__":
    args = get_parser().parse_args()
    img_name, _ = os.path.splitext(args.fname)

    start_time = time()
    run(["g++", "-o", "main", "main.cc"], cwd=os.path.join(os.getcwd(), "src"))

    output = open(f"{img_name}.ppm", "w+")
    process = subprocess.Popen(["./src/main"], stdout=subprocess.PIPE, 
                               cwd=os.path.join(os.getcwd(), "src"), 
                               universal_newlines=True, bufsize=1)
    line_count = 0
    while True:
        line = process.stdout.readline()
        line_count += 1
        if not line:
            if process.poll() is not None:
                break
        else:
            output.write(line)
            if line_count == 2:
                x, y = 0, 0
                nx, ny = map(int, line.split(' '))
                # img = np.full(shape=(ny, nx, 3), fill_value=255, dtype='uint8')
                # plt.figure(img_name)
                # plt.axis('off')
                print(f">>> {nx} x {ny} = {nx * ny} pixels")
            elif line_count >= 4:
                # r, g, b = map(int, line.split(' '))
                # img[y, x, :] = [r, g, b]
                x += 1
                if x == nx:
                    x = 0
                    y += 1
                    # if y % args.dy == 0:
                    #     if plt.fignum_exists(img_name):
                    #         plt.imshow(img, interpolation='none', vmin=0, vmax=255)
                    #         plt.pause(0.001)
                    print(f">>> {100*(line_count - 3)/(nx * ny):.1f}% of pixels processed ({line_count - 3} / {nx * ny})", end='\r')
    output.close()
    print(f">>> 100.0% of pixels processed ({nx * ny} / {nx * ny})")
    
    run(["magick", "convert", f"{img_name}.ppm", f"{img_name}.png"])
    print(f"Δt = {(time() - start_time):.2f}s")

    if not args.keep_ppm:
        run(["rm", f"{img_name}.ppm"])