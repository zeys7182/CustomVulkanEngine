#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;

layout(std140, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec4 lightDirection;
    vec4 lightColor;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 objectModel;
} pushConstants;

void main() {
    mat4 modelMatrix = pushConstants.objectModel * ubo.model;
    gl_Position = ubo.projection * ubo.view * modelMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;
}