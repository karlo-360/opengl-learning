
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 ourColor;

//out vec4 position;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
   //gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
   //gl_Position = vec4(aPos.x, aPos.y, aPos.z + offset, 1.0);
   //gl_Position =  vec4(aPos.x, aPos.y, aPos.z, 1.0f);
     gl_Position = projection * view * model * vec4(aPos, 1.0f);

   //position = gl_Position;

   ourColor = aColor;
   texCoord = aTexCoord;
}
