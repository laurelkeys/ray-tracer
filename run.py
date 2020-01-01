import os
import argparse
from time import time
from subprocess import run

def get_parser():
    parser = argparse.ArgumentParser(description="Compile and run main.cc, then convert its PPM output to PNG.")
    parser.add_argument("fname", type=str, help="Output image file name (with path)")
    parser.add_argument("--keep_ppm", "-ppm", action="store_true", help="Keep the original PPM output image")
    parser.add_argument("--debug", "-g", action="store_true", help="Use -g on g++ call")
    parser.add_argument("--warnings", "-Wall", action="store_true", help="Use -Wall on g++ call")
    parser.add_argument("--min_size", "-o", action="store_true", help="Use -o on g++ call (minimize size)")
    parser.add_argument("--max_speed", "-o2", action="store_true", help="Use -o2 on g++ call (maximize speed)")
    return parser

def build_cmd(args):
    cmd = ["g++", "-o", "main", "main.cc"]
    if args.debug:
        cmd.append("-g")
    if args.warnings:
        cmd.append("-Wall")
    if args.max_speed:
        cmd.append("-o2")
        if args.min_size:
            print("\nWarning: -o is ignored if -o2 is used as well\n")
    elif args.min_size:
        cmd.append("-o") # same as -o1
    return cmd

if __name__ == "__main__":
    args = get_parser().parse_args()
    img_name, _ = os.path.splitext(args.fname)

    start_time = time()

    cmd = build_cmd(args)
    run(cmd, cwd=os.path.join(os.getcwd(), "src"))
    with open(f"{img_name}.ppm", "w+") as output:
        run(["./src/main"], stdout=output, cwd=os.path.join(os.getcwd(), "src"))
    run(["magick", "convert", f"{img_name}.ppm", f"{img_name}.png"])
    
    print(f"Δt = {(time() - start_time):.2f}s")

    if not args.keep_ppm:
        run(["rm", f"{img_name}.ppm"])