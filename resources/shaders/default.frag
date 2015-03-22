varying vec2 vTexCoords;
uniform sampler2D sTex;
void main (void)
{
  gl_FragColor = texture2D(sTex, vTexCoords);
}


