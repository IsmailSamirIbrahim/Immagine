# Immagine
Immagine is a lightweight image processing library, written in C/C++.

The main goals of this project are ease of use and performance. Image decoding/encoding is done by using [stb](https://github.com/nothings/stb). Currently supported image formats are: PMB, PNG and jpeg.

## Build Guide
### Windows
- make sure you have a C++17 compiler (recommended vs2017)
- make sure you have CMake 3.9+ (recommended 3.14)
- at this point you may have 3 options
	1. configure cmake (`cmake . -G "Visual Studio 15 Win64" -B build`) then open the solution
	2. open the cmake file using Visual Studio Directly
	3. open the cmake file using QtCreator which will give you all the Qt Goodies right out of the box
	
## Usage
```cpp
#include <immagine/Image.h>

int main()
{
    immagine::Image image = immagine::image_load("path/to/image.png");

    immagine::Image image_grayscale = immagine::image_grayscale(image);

    immagine::image_write("path/to/image.bmp", image_grayscale, IMAGE_FORMAT::BMP);

    immagine::image_free(image);
    immagine::image_free(image_grayscale);
	
    return 0;
}
```
### Result
<img src="/images/lena.png" width="300" height="300"> 		 <img src="/images/lena_grayscale.png" width="300" height="300">

```cpp
#include <immagine/Image.h>
#include <immagine/Convolution.h>

int main()
{
    immagine::Image image = immagine::image_load("path/to/image.png");

    immagine::Image blured_image = immagine::image_gaussian_filter(image, 7);

    immagine::image_write("path/to/image.bmp", blured_image, IMAGE_FORMAT::BMP);

    immagine::image_free(image);
    immagine::image_free(blured_image);
	
    return 0;
}
```
### Result
<img src="/images/image.png" width="300" height="300"> 		 <img src="/images/blured_image.png" width="300" height="300">
