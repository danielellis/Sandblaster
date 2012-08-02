#ifndef BMP_LOADER_H
#define BMP_LOADER_H

struct BMPImage {
   unsigned long sizeX;
   unsigned long sizeY;
   char *data;
};

class BMPLoader {
public:
    static BMPImage* LoadTexture(char* image_file, unsigned int texID);

protected:
    static int ImageLoad(char *filename, BMPImage *image);
};

#endif