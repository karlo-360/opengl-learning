
#version 330 core

in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 viewPos;

void main(){

    //ambient
    float ambientStrenght = 0.1f;
    vec3 ambient = ambientStrenght * lightColor;

    //deffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    //specular
    float specularStrenght = 0.5f;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrenght * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * ourColor;
    FragColor = vec4(result, 1.0f);
}
