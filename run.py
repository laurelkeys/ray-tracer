import os
import argparse
from subprocess import run

def get_parser():
    parser = argparse.ArgumentParser(description="Compile and run main.cc, then convert its PPM output to PNG.")
    parser.add_argument("fname", type=str, help="Output image file name (with path)")
    parser.add_argument("--keep_ppm", "-ppm", action="store_true", help="Keep the original PPM output image")
    return parser

if __name__ == "__main__":
    args = get_parser().parse_args()
    img_name, _ = os.path.splitext(args.fname)

    run(["g++", "-o", "main", "main.cc"], shell=True, cwd=os.path.join(os.getcwd(), "src"))    
    with open(f"{img_name}.ppm", "w+") as output:
        run(["./src/main"], stdout=output)    
    run(["magick", "convert", f"{img_name}.ppm", f"{img_name}.png"])

    if not args.keep_ppm:
        run(["rm", f"{img_name}.ppm"])