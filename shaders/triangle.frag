#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;

layout(std140, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec4 lightDirection;
    vec4 lightColor;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 baseColor = texture(texSampler, fragTexCoord) * vec4(fragColor, 1.0);

    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(ubo.lightDirection.xyz);
    vec3 lightColor = ubo.lightColor.rgb;

    vec3 ambient = ubo.lightColor.w * lightColor;
    float diffuseStrength = max(dot(normal, -lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    outColor = vec4(baseColor.rgb * (ambient + diffuse), baseColor.a);
}