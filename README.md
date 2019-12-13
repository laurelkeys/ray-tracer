# ray-tracer
An implementation of Peter Shirley's [*Ray Tracing in One Weekend* book series](https://raytracing.github.io/):
- [x] [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [ ] [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [ ] [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)

Check out many 200 x 100 rendered images [here](https://github.com/laurelkeys/ray-tracer/blob/master/i/README.md) (inside the [i/](https://github.com/laurelkeys/ray-tracer/blob/master/i/) folder).
These images with few objects take less than a second to render (even with 100 samples per pixel).

Below are some 1200 x 800 images with 10 samples per pixel, which take over a minute to render:

<img align="left" src="https://raw.githubusercontent.com/laurelkeys/ray-tracer/master/i/large/wiki.png" width="31%"/><img align="left" src="https://raw.githubusercontent.com/laurelkeys/ray-tracer/master/i/large/wiki_metal_ground.png" width="31%"/><img align="left" src="https://raw.githubusercontent.com/laurelkeys/ray-tracer/master/i/large/wiki_glass_ground.png" width="31%"/>
<br><br><br><br><br><br><br><br>

<img align="left" src="https://raw.githubusercontent.com/laurelkeys/ray-tracer/master/i/large/random_scene_10xAA_fixed_cam_large.png" width="31%"/><img align="left" src="https://raw.githubusercontent.com/laurelkeys/ray-tracer/master/i/large/random_scene_10xAA_y10_large.png" width="31%"/><img align="left" src="https://raw.githubusercontent.com/laurelkeys/ray-tracer/master/i/large/random_scene_10xAA_y18_x18_fixed_dof_large.png" width="31%"/>
<br><br><br><br><br><br><br>

(more at [i/large/](https://github.com/laurelkeys/ray-tracer/blob/master/i/large/)).

*obs.:* If you have [ImageMagick](https://imagemagick.org/script/convert.php) installed, you can convert the generated **.ppm** file to **.png** with: 

> `$ g++ -o main main.cc && .\main > [file_name].ppm && magick convert [file_name].ppm [file_name].png`.
