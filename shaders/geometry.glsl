#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

in vec3 fragNormal[];
in vec2 fragTexCoord[];

out vec3 geoNormal;
out vec2 geoTexCoord;

uniform float furLength = 0.1; // Adjust the fur length as necessary

void main() {
    for(int i = 0; i < 3; ++i) {
        for(int layer = 0; layer < 6; ++layer) {
            float layerFraction = float(layer) / 6.0;
            vec3 offset = fragNormal[i] * furLength * layerFraction;

            geoNormal = fragNormal[i];
            geoTexCoord = fragTexCoord[i];
            gl_Position = gl_in[i].gl_Position + vec4(offset, 0.0);
            EmitVertex();
        }
        EndPrimitive();
    }
}