#pragma once

#include "Immagine/Exports.h"
#include "Immagine/Image.h"
#include "Immagine/Kernel.h"

namespace immagine
{
	IMMAGINE_EXPORT Image
	image_erode(const Image& image, const Kernel& SE);
}