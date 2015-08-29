#include <cstdlib>
#include <png.h>

namespace clm {

void writePngFile(unsigned char* data, int width, int height, int depth, const char* fileName) {
	png_byte colorType;

	if (depth == 4)
		colorType = PNG_COLOR_TYPE_RGB_ALPHA;
	else if (depth == 3)
		colorType = PNG_COLOR_TYPE_RGB;
	else if (depth == 1)
		colorType = PNG_COLOR_TYPE_GRAY;
	else {
		printf("Invalid depth\n");
		return;
	}

	png_bytep* rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * height);

	for (int y = 0; y < height; y++) {
		rowPointers[y] = (png_byte*)(data + y * width * depth);
	}

	FILE* fp = fopen(fileName, "wb");
	if (!fp) {
		printf("File %s could not be opened for writing\n", fileName);

		free(rowPointers);

		return;
	}

	png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr) {
		printf("png_create_write_struct failed\n");

		free(rowPointers);
		fclose(fp);

		return;
	}

	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr) {
		printf("png_create_info_struct failed\n");

		png_destroy_write_struct(&pngPtr, (png_infopp)NULL);
		free(rowPointers);
		fclose(fp);

		return;
	}

	if (setjmp(png_jmpbuf(pngPtr))) {
		//printf("Error during init_io\n");
		printf("Error during writing bytes\n");

		png_destroy_write_struct(&pngPtr, &infoPtr);
		free(rowPointers);
		fclose(fp);

		return;
	}

	png_init_io(pngPtr, fp);

	png_set_IHDR(pngPtr, infoPtr, width, height,
				 8, colorType, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(pngPtr, infoPtr);

	png_write_image(pngPtr, rowPointers);

	png_write_end(pngPtr, NULL);

	png_destroy_write_struct(&pngPtr, &infoPtr);
	free(rowPointers);
	fclose(fp);

}

}
