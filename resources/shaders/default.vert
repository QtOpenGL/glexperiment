#version 430 core
#pragma debug(on)
in vec4 aVertex;
in vec2 aTexCoords;
out vec2 vTexCoords;

void main(void)
{
  gl_Position = aVertex;
  vTexCoords = aTexCoords;
}
