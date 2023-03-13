# Atom_image_processor
A simple image editor made based on the [Image Processing in C++](https://www.youtube.com/playlist?list=PLG5M8QIx5lkzdGkdYQeeCK__As6sI2tOY) tutorial by Code Break
([github repository](https://github.com/Code-Break0/Image-Processing))

Changes include:
* change in the function recognising file extensions (used c++ 17 library)
* chromatic aberration filters
* distortion filter 
* cutting images into separate pieces (4 options)
* fusing separate images into one big piece
* rotating image
* resizing image
* changing hue of the image
* changing saturation of the image
* implemented multithreading

----------------------------

The goal of this project is to better understand image processing by 
implementing basic functions operating on an array of image data. To 
understand the code good knowledge of matrix operations is needed. 

----

### Features:
| function                    | description                                                                          |
|-----------------------------|--------------------------------------------------------------------------------------|
| constructor                 | open the given filename or create empty image                                        |
| grayscale_avg               | turn image into grayscale by using average of colors                                 |
| grayscale_lum               | turn image into grayscale using weighted average of colors                           |
| color_mask                  | apply color mask over the image                                                      |
| flip_x                      | flip the image along x axis                                                          |
| flip_y                      | flip the image along y axis                                                          |
| neon_chromatic_aberration   | variation of chromatic aberration                                                    |
| purple_chromatic_aberration | variation of chromatic aberration                                                    |
| crop                        | crop the image to given size                                                         |
| chromatic_aberration        | apply chromatic aberration                                                           |
| distortion_filter           | apply corruption-like filter to image                                                |
| overlay                     | overlay another image over the opened one                                            |
| fuse                        | fuse a series of images together                                                     |
| rotate_right                | rotate the image 90 degrees right                                                    |
| resize                      | resize the image                                                                     |
| change_hue                  | change the hue of the image                                                          |
| overlay_text                | insert text into image                                                               |
| change_saturation           | change the saturation of the image                                                   |
| cut                         | cut the image into number or parts or parts of given size vertically or horizontally |
