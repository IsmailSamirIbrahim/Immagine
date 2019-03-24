#include <iostream>

#include "Immagine/Image.h"

using namespace immagine;

int
main(int argc, char** argv)
{

	Image img = image_load("D:/rgb.jpg");

	image_save("D:/red.bmp", image_red_channel(img), BMP);
	image_save("D:/green.bmp", image_green_channel(img), BMP);
	image_save("D:/blue.bmp", image_blue_channel(img), BMP);

	float hist[256] = {};
	image_histogram(img, hist);
	image_histogram_red_channel(img, hist);
	image_histogram_green_channel(img, hist);
	image_histogram_blue_channel(img, hist);
	image_save("D:/equalize.bmp", image_histogram_equalization(img), BMP);

	image_save("D:/bright.bmp", image_brightness(img, 100), BMP);
	image_save("D:/dark.bmp", image_brightness(img, -100), BMP);

	Image gray = image_gray_scale(img);
	image_save("D:/gray.bmp", gray, BMP);

	image_save("D:/binarized.bmp", image_binarize(gray), BMP);

	image_save("D:/padded.bmp", image_padding(img, 10, 0), BMP);


	return 0;
}
