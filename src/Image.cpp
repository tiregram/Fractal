
#include <iostream>
#include "Image.hpp"

#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int *)NULL
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <boost/gil/gil_all.hpp>

using namespace boost::gil;

bool export_image(int sizeX, int sizeY, std::string nameOut, int* buffer) {

  rgb8_image_t img(sizeX, sizeY);

  auto imgv = view(img);

  for (int i = 0; i < sizeX * sizeY; i++) {
    imgv(i / sizeX, i % sizeY) = rgb8_pixel_t( (buffer[i] >> 0 ) & 0XFF ,
                                               (buffer[i] >> 8 ) & 0XFF ,
                                               (buffer[i] >> 16) & 0XFF);
  }

  png_write_view(nameOut, const_view(img));
}

