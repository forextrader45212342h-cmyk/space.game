#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{

struct CharacterColor
{
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
};

struct CharacterAppearance
{
    int skinTone = 0;
    int hairStyle = 0;
    int hairColor = 0;
    int eyeColor = 0;
    int faceShape = 0;
    int bodyType = 0;
    int heightClass = 0;
    int ageClass = 0;

    CharacterColor skin;
    CharacterColor hair;
    CharacterColor eyes;
    CharacterColor clothingPrimary;
    CharacterColor clothingSecondary;
};

struct CharacterIdentity
{
    std::uint64_t id = 0;
    std::string firstName;
    std::string lastName;
    std::string profession;
    std::string faction;
    std::string homePlanet;

    float personality = 0.5f;
    float bravery = 0.5f;
    float intelligence = 0.5f;
    float friendliness = 0.5f;
};

struct CharacterTransform
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;

    float scale = 1.0f;
};

struct CharacterMeshVertex
{
    float px, py, pz;
    float nx, ny, nz;
    float u, v;
};

struct OrbitCharacter
{
    CharacterIdentity identity;
    CharacterAppearance appearance;
    CharacterTransform transform;

    std::vector<CharacterMeshVertex> vertices;
    std::vector<std::uint32_t> indices;

    bool alive = true;
    bool active = true;
};

}
