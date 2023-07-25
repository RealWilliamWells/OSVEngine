  precision mediump float;

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 Color;

void main()
{
    FragColor = TexCoord != vec2(0,0) ? texture(ourTexture, TexCoord) : vec4(Color.xyzw);
}
