#include "Shader.h"
#include <cstdlib>
#include <cstdio>

CGcontext ShaderContext;

void InitializeShaderContext() {
    //Initialize context
    ShaderContext = cgCreateContext();
    CheckForCgError("context", "creating context");
}

/* Nice function for checkign and reporting Cg errors */
void CheckForCgError(const char *shader, const char *situation)
{
  CGerror error;
  const char *string = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR) {
    printf("%s: %s: %s\n",
      shader, situation, string);
    if (error == CG_COMPILER_ERROR) {
      printf("%s\n", cgGetLastListing(ShaderContext));
    }
    exit(1);
  }
}