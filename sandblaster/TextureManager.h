#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

class TextureManager {
public:
    TextureManager() { }

    void Initialize();
    void Shutdown();

    GLuint LoadFromFile(const char *filename);

private:
    bool isInitialized;
};

#endif // TEXTURE_MANAGER_H