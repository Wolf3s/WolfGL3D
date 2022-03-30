#ifndef NVIDIA_PNG_CODE
#define NVIDIA_PNG_CODE

#include <glh_linear.h>
#include <glh_array.h>
//#include <data_path.h>
//#include "png\data_path.h"
#include "data_path.h"


data_path get_png_path();
void set_png_path(const data_path & newpath);

void read_png_rgba(const char * filename, glh::array2<glh::vec4ub> & image);
void read_png_rgb(const char * filename, glh::array2<glh::vec3ub> & image);
void read_png_grey(const char * filename, glh::array2<unsigned char> & image);

#endif