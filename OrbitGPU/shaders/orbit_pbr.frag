#version 450

layout(location = 0) in vec3 worldPosition;
layout(location = 1) in vec3 worldNormal;
layout(location = 2) in vec2 uv;

layout(set = 0, binding = 1) uniform Material
{
    vec4 baseColor;
    vec4 parameters;
} material;

layout(location = 0) out vec4 outColor;

const float PI = 3.14159265359;

void main()
{
    vec3 N = normalize(worldNormal);

    vec3 L =
        normalize(
            vec3(0.35, 0.75, 0.55)
        );

    vec3 V =
        normalize(
            -worldPosition
        );

    vec3 H =
        normalize(L + V);

    float metallic =
        clamp(material.parameters.x, 0.0, 1.0);

    float roughness =
        clamp(material.parameters.y, 0.04, 1.0);

    float NdotL =
        max(dot(N, L), 0.0);

    float NdotV =
        max(dot(N, V), 0.0);

    float NdotH =
        max(dot(N, H), 0.0);

    float VdotH =
        max(dot(V, H), 0.0);

    vec3 albedo =
        material.baseColor.rgb;

    vec3 F0 =
        mix(
            vec3(0.04),
            albedo,
            metallic
        );

    float alpha =
        roughness * roughness;

    float alpha2 =
        alpha * alpha;

    float denom =
        NdotH * NdotH *
        (alpha2 - 1.0) +
        1.0;

    float D =
        alpha2 /
        max(
            PI * denom * denom,
            0.0001
        );

    float k =
        (roughness + 1.0);

    k = (k * k) / 8.0;

    float Gv =
        NdotV /
        max(
            NdotV * (1.0 - k) + k,
            0.0001
        );

    float Gl =
        NdotL /
        max(
            NdotL * (1.0 - k) + k,
            0.0001
        );

    float G =
        Gv * Gl;

    vec3 F =
        F0 +
        (1.0 - F0) *
        pow(
            1.0 - VdotH,
            5.0
        );

    vec3 specular =
        (D * G * F) /
        max(
            4.0 * NdotV * NdotL,
            0.0001
        );

    vec3 kS = F;
    vec3 kD =
        (1.0 - kS) *
        (1.0 - metallic);

    vec3 diffuse =
        kD * albedo / PI;

    vec3 lighting =
        (diffuse + specular) *
        NdotL;

    vec3 ambient =
        albedo * 0.025;

    vec3 color =
        ambient + lighting;

    color =
        color /
        (color + vec3(1.0));

    color =
        pow(
            color,
            vec3(1.0 / 2.2)
        );

    outColor =
        vec4(color, 1.0);
}
