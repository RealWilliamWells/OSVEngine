  precision mediump float;

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 objectColor;

void main() {
//     vec4 mainColor = TexCoord != vec2(0,0) ? texture(ourTexture, TexCoord) * objectColor : objectColor;
    vec4 mainColor = TexCoord != vec2(0,0) ? texture(ourTexture, TexCoord) : objectColor;
//     FragColor = mainColor * vec4(lightColor, 1.0);
    FragColor = mainColor;
}
