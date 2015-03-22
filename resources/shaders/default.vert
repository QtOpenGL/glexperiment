attribute vec4 aVertex;
attribute vec2 aTexCoords;
varying vec2 vTexCoords;

void main(void)
{
  gl_Position = aVertex;
  vTexCoords = aTexCoords;
}

