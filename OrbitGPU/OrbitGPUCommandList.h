#pragma once

#include "OrbitGPUTypes.h"

#include <cstdint>
#include <vector>

namespace OrbitGPU
{
    enum class CommandType
    {
        Begin,
        End,
        SetViewport,
        SetScissor,
        ClearColor,
        ClearDepth,
        Draw,
        DrawIndexed
    };

    struct Command
    {
        CommandType type{};

        uint32_t a = 0;
        uint32_t b = 0;
        uint32_t c = 0;

        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;
    };

    class CommandList
    {
    private:
        std::vector<Command> commands;
        bool recording = false;

    public:
        void Begin();
        void End();

        void SetViewport(const Viewport& viewport);
        void SetScissor(const Scissor& scissor);

        void ClearColor(
            float r,
            float g,
            float b,
            float a);

        void ClearDepth(float depth);

        void Draw(
            uint32_t vertexCount,
            uint32_t firstVertex = 0);

        void DrawIndexed(
            uint32_t indexCount,
            uint32_t firstIndex = 0,
            int32_t vertexOffset = 0);

        bool IsRecording() const;

        const std::vector<Command>& GetCommands() const;

        void Clear();
    };
}
