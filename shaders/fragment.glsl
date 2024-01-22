/*

#version 450 core

out vec4 FragColor;

uniform vec3 objectColor;

void main() {
    FragColor = vec4(objectColor, 1.0);
}
*/

#version 450 core

in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 outColor;

uniform vec3 furColor = vec3(1.0, 0.5, 0.0); // Example: Orange fur

void main() {
    float alpha = 1.0; // Adjust as necessary
    outColor = vec4(furColor, alpha);
}