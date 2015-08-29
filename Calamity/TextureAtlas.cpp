#include "TextureAtlas.h"
#include <climits>
#include <assert.h>
#include <cstdio>
//#include <GL/glew.h>
#include <cstring>
#include <cstdlib>

namespace clm {

void writePngFile(unsigned char* data, int width, int height, int depth, const char* fileName);

TextureAtlas::TextureAtlas(const size_t width, const size_t height, const size_t depth) :
	width(width),
	height(height),
	depth(depth),

	skyline(),
	used(0),

	id(0),

	data()
{
	assert((depth == 1) || (depth == 3) || (depth == 4));

	SkylineSegment s = {1, 1, (int)width - 2};
	skyline.push_back(s);

	data = (unsigned char*)calloc(width * height * depth, sizeof(unsigned char));
	if (data == NULL)
		fprintf(stderr, "line %d: No more memory for allocating data\n", __LINE__);
}

TextureAtlas::~TextureAtlas() {
	if (data)
		free(data);
	if (id)
		glDeleteTextures(1, &id);
}

void TextureAtlas::setRegion(const size_t x, const size_t y, const size_t regionWidth, const size_t regionHeight, const unsigned char* regionData, const size_t stride) {
	assert(x > 0);
	assert(y > 0);
	assert(x < width - 1);
	assert(x + regionWidth <= width - 1);
	assert(y < height - 1);
	assert(y + regionHeight <= height - 1);

	//printf("%d\n", regionData[0]);
	// top left
	/*memcpy(data + ((y - 1) * width + x - 1) * sizeof(unsigned char) * depth,
		   regionData,
		   sizeof(unsigned char) * depth);*/
	// top border
	memcpy(data + ((y - 1) * width + x) * sizeof(unsigned char) * depth,
		   regionData,
		   regionWidth * sizeof(unsigned char) * depth);
	// top right
	/*memcpy(data + ((y - 1) * width + x + regionWidth) * sizeof(unsigned char) * depth,
		   regionData + (regionWidth - 1) * sizeof(unsigned char) * depth,
		   sizeof(unsigned char) * depth);
	// bottom left
	memcpy(data + ((y + regionHeight) * width + x - 1) * sizeof(unsigned char) * depth,
		   regionData + ((regionHeight - 1) * stride) * sizeof(unsigned char),
		   sizeof(unsigned char) * depth);*/
	// bottom border
	memcpy(data + ((y + regionHeight) * width + x) * sizeof(unsigned char) * depth,
		   regionData + ((regionHeight - 1) * stride) * sizeof(unsigned char),
		   regionWidth * sizeof(unsigned char) * depth);
	// bottom right
	/*memcpy(data + ((y + regionHeight) * width + x + regionWidth) * sizeof(unsigned char) * depth,
		   regionData + ((regionHeight - 1) * stride) * sizeof(unsigned char) + (regionWidth - 1) * sizeof(unsigned char) * depth,
		   sizeof(unsigned char) * depth);*/

	for (size_t i = 0; i < regionHeight; i++) {
		// left border
		memcpy(data + ((y + i) * width + x - 1) * sizeof(unsigned char) * depth,
			   regionData + (i * stride) * sizeof(unsigned char),
			   sizeof(unsigned char) * depth);
		// texture
		memcpy(data + ((y + i) * width + x) * sizeof(unsigned char) * depth,
			   regionData + (i * stride) * sizeof(unsigned char),
			   regionWidth * sizeof(unsigned char) * depth);
		// right border
		memcpy(data + ((y + i) * width + x + regionWidth) * sizeof(unsigned char) * depth,
			   regionData + (i * stride) * sizeof(unsigned char) + (regionWidth - 1) * sizeof(unsigned char) * depth,
			   sizeof(unsigned char) * depth);
	}
}

int TextureAtlas::fit(list<SkylineSegment>::iterator position, const size_t width, const size_t height) {
	int y = (*position).y;
	int widthLeft = width;

	if ((*position).x + width > this->width - 1)
		return -1;

	while (widthLeft > 0) {
		if ((*position).y > y)
			y = (*position).y;
		if (y + height > this->height - 1)
			return -1;
		widthLeft -= (*position).width;
		++position;
	}
	return y;
}

void TextureAtlas::merge() {
	list<SkylineSegment>::iterator i;
	list<SkylineSegment>::iterator last;
	for (i = ++skyline.begin(); i != skyline.end(); ++i) {
		last = i;
		--last;
		while ((*i).y == (*last).y) {
			(*last).width += (*i).width;
			i = skyline.erase(i);
		}
	}
}

AtlasRegion TextureAtlas::getRegion(const size_t width, const size_t height) {
    AtlasRegion region = {0, 0, (int)width, (int)height};
	size_t width2 = width + 2;
	size_t height2 = height + 2;

	int y;
	int bestHeight = INT_MAX;
	int bestWidth = INT_MAX;
	list<SkylineSegment>::iterator bestPosition = skyline.end();
	list<SkylineSegment>::iterator i;
	for (i = skyline.begin(); i != skyline.end(); ++i) {
		y = fit(i, width2, height2);
		if (y >= 0) {
			if ((y + (int)height2 < bestHeight) || ((y + (int)height2 == bestHeight) && ((*i).width < bestWidth))) {
				bestPosition = i;
				bestHeight = y + height2;
				bestWidth = (*i).width;
				region.x = (*i).x;
				region.y = y;
			}
		}
	}

	if (bestPosition == skyline.end()) {
		region.x = -1;
		region.y = -1;
		region.width = 0;
		region.height = 0;
		return region;
	}

	++region.x;
	++region.y;

	SkylineSegment s = {region.x, region.y + (int)height2, (int)width2};
	list<SkylineSegment>::iterator last = skyline.insert(bestPosition, s);
	i = last;
	++i;
	while ((*i).x < (*last).x + (*last).width) {
		int shrink = (*last).x + (*last).width - (*i).x;
		(*i).x += shrink;
		(*i).width -= shrink;
		if ((*i). width <= 0)
			i = skyline.erase(i);
		else
			break;
	}

	merge();
	used += width2 * height2;
return region;
}

void TextureAtlas::clear() {
	assert(data);

	skyline.clear();
	SkylineSegment s = {1, 1, (int)width - 2};
	skyline.push_back(s);
	used = 0;
	memset(data, 0, width * height * depth);
}

void TextureAtlas::upload() {
	assert(data);

    if (id == 0)
        glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (depth == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else if (depth == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
}

void TextureAtlas::writeToPng(const char* fileName) {
	writePngFile(data, width, height, depth, fileName);
}

void TextureAtlas::bind(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void TextureAtlas::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
