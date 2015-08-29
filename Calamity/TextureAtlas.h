#pragma once

#include <list>
#include <cstddef>
#include <GL/glew.h>

//using std::vector;
using std::list;

namespace clm {

struct SkylineSegment {
    int x;
    int y;
    int width;
};

struct AtlasRegion {
    int x;
    int y;
    int width;
    int height;
};

class TextureAtlas {
private:
    size_t width;
    size_t height;
    size_t depth;

    //vector_t * nodes;
    list<SkylineSegment> skyline;
    size_t used;

    unsigned int id;

    unsigned char* data;
public:
	explicit TextureAtlas(const size_t width, const size_t height, const size_t depth);
    ~TextureAtlas();

	void setRegion(const size_t x, const size_t y, const size_t regionWidth, const size_t regionHeight, const unsigned char* regionData, const size_t stride);
    int fit(list<SkylineSegment>::iterator position, const size_t width, const size_t height);
    void merge();
    AtlasRegion getRegion(const size_t width, const size_t height);
    void clear();
    void upload();
	void writeToPng(const char* fileName);
	void bind(GLenum textureUnit);
	void unbind();
};

}
