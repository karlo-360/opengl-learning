
#version 330 core

struct Material {
    //vec3 diffuse;
    //vec3 ambient;
    //vec3 specular;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 texCoords;
in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main(){

    //ambient
    //float ambientStrenght = 0.1f;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    //deffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    //specular
    float specularStrenght = 0.5f;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}
