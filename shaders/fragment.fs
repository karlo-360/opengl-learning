
#version 330 core

in vec3 ourColor;

out vec4 FragColor;

uniform vec3 lightColor;

void main(){
   //FragColor = vec4(ourColor, 1.0f);
   //FragColor = position;
   //FragColor = texture(texture1, texCoord) * vec4(ourColor, 1.0f);
   //FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2((texCoord.x * sign), texCoord.y)), offset);
   //texCoord.x = texCoord.x * sign;
   FragColor = vec4(ourColor * lightColor, 1.0f);

}
