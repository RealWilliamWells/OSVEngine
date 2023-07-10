#version 300 es
  precision mediump float;

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = vec4(250, 250, 0, 0);
     FragColor = texture(ourTexture, TexCoord);
}