# ray-tracer
An implementation of Peter Shirley's [Ray Tracing in One Weekend](http://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf).

Check out many 200x100 rendered images [here](https://github.com/laurelkeys/ray-tracer/blob/master/i/README.md) (inside the [i/](https://github.com/laurelkeys/ray-tracer/blob/master/i/) folder).
These images with few objects take less than a second to render (even with 100 samples per pixel).

Below are some 1200x800 images with ~500 spheres and 10 samples per pixel, which took around 3 minutes to render:

![](https://github.com/laurelkeys/ray-tracer/blob/master/i/large/random_scene_10xAA_fixed_cam_large.png)

![](https://github.com/laurelkeys/ray-tracer/blob/master/i/large/random_scene_10xAA_y10_large.png)

![](https://github.com/laurelkeys/ray-tracer/blob/master/i/large/random_scene_10xAA_y18_x18_fixed_dof_large.png)

(more at [i/large/](https://github.com/laurelkeys/ray-tracer/blob/master/i/large/)).

*obs.:* If you have [ImageMagick](https://imagemagick.org/script/convert.php) installed, you can convert the generated **.ppm** file to **.png** with: 

> `$ g++ -o main main.cc && .\main > [file_name].ppm && magick convert [file_name].ppm [file_name].png`.
