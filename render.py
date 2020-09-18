import os
from time import time
from argparse import ArgumentParser
from subprocess import run, Popen, PIPE

import numpy as np
import matplotlib.pyplot as plt

def get_parser():
    parser = ArgumentParser(description="Compile and run main.cc, then convert its PPM output to PNG.")
    parser.add_argument("fname", type=str, help="Output image file name (with path)")
    parser.add_argument("--keep_ppm", "-ppm", action="store_true", help="Keep the original PPM output image")
    parser.add_argument("--debug", "-g", action="store_true", help="Use -g on g++ call")
    parser.add_argument("--warnings", "-Wall", action="store_true", help="Use -Wall on g++ call")
    parser.add_argument("--min_size", "-O", action="store_true", help="Use -O on g++ call (minimize size)")
    parser.add_argument("--max_speed", "-O2", action="store_true", help="Use -O2 on g++ call (maximize speed)")
    parser.add_argument("--watch", action="store_true", help="Show rendering")
    return parser

def build_cmd(args):
    cmd = ["g++", "-o", "main", "main.cc"]
    if args.debug:
        cmd.append("-g")
    if args.warnings:
        cmd.append("-Wall")
    if args.max_speed:
        cmd.append("-O2")
        if args.min_size:
            print("\nWarning: -O is ignored if -O2 is used as well\n")
    elif args.min_size:
        cmd.append("-O")
    return cmd

if __name__ == "__main__":
    args = get_parser().parse_args()
    img_name, _ = os.path.splitext(args.fname)

    # compile main.cc into main.exe
    cmd = build_cmd(args)
    run(cmd, cwd=os.path.join(os.getcwd(), "src"))

    # render image
    start_time = time()
    with open(f"{img_name}.ppm", "w+") as output:
        process = Popen(["./src/main"], stdout=PIPE,
                        cwd=os.path.join(os.getcwd(), "src"),
                        universal_newlines=True, bufsize=1)
        line_count = 0
        while True:
            line = process.stdout.readline()
            line_count += 1
            if not line:
                if process.poll() is not None:
                    break # process terminated
            else:
                output.write(line)
                if line_count == 2:
                    nx, ny = map(int, line.split(' '))
                    x, y, n_of_pixels = 0, 0, nx * ny
                    print(f">>> {nx} x {ny} = {n_of_pixels} pixels")
                    if args.watch:
                        img = np.full(shape=(ny, nx, 3), fill_value=255, dtype='uint8')
                        fig = plt.figure(img_name)
                        plt.axis('off')
                        plt.ion()
                        plt.show()
                elif line_count >= 4:
                    if args.watch:
                        r, g, b = map(int, line.split(' '))
                        img[y, x, :] = [r, g, b]
                    x += 1
                    if x == nx:
                        x = 0
                        y += 1
                        print(f">>> {100*(line_count - 3)/(n_of_pixels):.1f}% of pixels processed ({line_count - 3} / {n_of_pixels})", end='\r')
                        if args.watch and y % 2 == 0 and plt.fignum_exists(img_name):
                            plt.imshow(img, interpolation='none', vmin=0, vmax=255)
                            fig.canvas.start_event_loop(0.001)
    print(f">>> 100.0% of pixels processed ({n_of_pixels} / {n_of_pixels})")
    print(f"Δt = {(time() - start_time):.2f}s")

    # convert PPM rendered imageinto PNG
    run(["magick", "convert", f"{img_name}.ppm", f"{img_name}.png"])

    if not args.keep_ppm:
        run(["rm", f"{img_name}.ppm"])