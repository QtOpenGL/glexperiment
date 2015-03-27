#version 430 core
#pragma debug(on)
in vec2 vTexCoords;
out vec4 frag;
uniform sampler2D sTex;
void main (void)
{
  frag = texture2D(sTex, vTexCoords);
}
