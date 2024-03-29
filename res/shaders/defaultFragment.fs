  precision mediump float;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 color;
    vec3 position;

    vec3 spotDir;
    float spotCutOff;
};

uniform bool useLighting;
uniform vec4 objectColor;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

void main() {
    if (useLighting) {
        vec3 lightDir = normalize(light.position - FragPos);

        vec3 ld = normalize(lightDir);
        vec3 sd = normalize(-light.spotDir);

        // inside the cone?
        if (dot(sd,ld) > light.spotCutOff) {
            vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

            vec3 norm = normalize(Normal);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            vec3 specular = light.color * (spec * material.specular);

            vec3 result = ambient + diffuse + specular;
            FragColor = vec4(result, 1.0);
        } else {
            FragColor = texture(material.diffuse, TexCoord);
        }
    } else {
        vec4 mainColor = TexCoord != vec2(0,0) ? texture(material.diffuse, TexCoord) : objectColor;
        FragColor = mainColor;
    }

}
