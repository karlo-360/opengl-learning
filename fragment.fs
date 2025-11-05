
#version 330 core

//in vec4 position;
in vec2 texCoord;
in vec2 texCoord2;
in vec3 ourColor;

out vec4 FragColor;

uniform float offset;
uniform int sign;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
   //FragColor = vec4(ourColor, 1.0f);
   //FragColor = position;
   //FragColor = texture(texture1, texCoord) * vec4(ourColor, 1.0f);
   //FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2((texCoord.x * sign), texCoord.y)), offset);
   //texCoord.x = texCoord.x * sign;
   FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(texCoord.x * sign, texCoord.y)), offset) * vec4(ourColor, 1.0f);

}
