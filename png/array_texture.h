#ifndef ARRAY_TEXTURE
#define ARRAY_TEXTURE

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <glh_linear.h>
#include <glh_array.h>

void make_rgba_texture(const glh::array2<glh::vec3ub> & rgb,
                       const glh::array2<unsigned char> & a,
					   bool mipmapped = false);
void make_rgba_texture(const glh::array2<glh::vec4ub> & rgb,
                       bool mipmapped = false);
void make_rgb_texture(const glh::array2<glh::vec3ub> & rgb,
					  bool mipmapped = false);
void make_scalar_texture(const glh::array2<unsigned char> & s,
                         GLenum internal_format, GLenum format,
						 bool mipmapped = false);

extern GLenum array_texture_target;

#endif
