#ifndef SHADER_H
#define SHADER_H

#include <windows.h>
#include <gl\Glee.h>
#include <cg\cg.h>		
#include <cg\cggl.h>

extern CGcontext ShaderContext;

void InitializeShaderContext();
void CheckForCgError(const char *shader, const char *situation);

#endif