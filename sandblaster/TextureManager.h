#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

class TextureManager {
public:
    TextureManager() { }

    void Initialize();
    void Shutdown();

    GLuint LoadFromFile(const char *filename);
    GLuint LoadFromFile2(ILstring filename);

private:
    bool isInitialized;
};

#endif // TEXTURE_MANAGER_H