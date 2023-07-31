  precision mediump float;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform bool useLighting;
uniform sampler2D ourTexture;
uniform vec4 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

void main() {
    if (useLighting) {
            vec3 ambient = lightColor * material.ambient;

            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = lightColor * (diff * material.diffuse);

            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            vec3 specular = lightColor * (spec * material.specular);

            vec3 result = ambient + diffuse + specular;
            FragColor = vec4(result, 1.0) * texture(ourTexture, TexCoord);
    } else {
        vec4 mainColor = TexCoord != vec2(0,0) ? texture(ourTexture, TexCoord) : objectColor;
        FragColor = mainColor;
    }

}
