#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(set = 0, binding = 0) uniform Camera
{
    mat4 model;
    mat4 view;
    mat4 projection;
} camera;

layout(location = 0) out vec3 worldPosition;
layout(location = 1) out vec3 worldNormal;
layout(location = 2) out vec2 uv;

void main()
{
    vec4 world =
        camera.model *
        vec4(inPosition, 1.0);

    worldPosition = world.xyz;

    worldNormal =
        mat3(camera.model) *
        inNormal;

    uv = inUV;

    gl_Position =
        camera.projection *
        camera.view *
        world;
}
