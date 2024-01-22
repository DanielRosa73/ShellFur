/*

#version 450 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view *  vec4(aPos, 1.0);
}

*/

#version 450 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec3 fragNormal;
out vec2 fragTexCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {
    fragNormal = inNormal;
    fragTexCoord = inTexCoord;
    gl_Position = projection * view * vec4(inPosition, 1.0);
}