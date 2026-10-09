#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 baseColor = texture(texSampler, fragTexCoord) * vec4(fragColor, 1.0);
    vec3 lightDirection = normalize(vec3(-1.0, -1.0, -1.0));
    float diffuse = max(dot(normalize(fragNormal), -lightDirection), 0.95);
    outColor = vec4(baseColor.rgb * diffuse, baseColor.a);
}