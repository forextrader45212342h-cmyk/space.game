#include "OrbitGPUCommandList.h"

namespace OrbitGPU
{
    void CommandList::Begin()
    {
        commands.clear();
        recording = true;

        commands.push_back(
        {
            CommandType::Begin
        });
    }

    void CommandList::End()
    {
        if (!recording)
            return;

        commands.push_back(
        {
            CommandType::End
        });

        recording = false;
    }

    void CommandList::SetViewport(
        const Viewport& viewport)
    {
        if (!recording)
            return;

        Command command{};
        command.type =
            CommandType::SetViewport;

        command.x = viewport.x;
        command.y = viewport.y;
        command.z = viewport.width;
        command.w = viewport.height;

        commands.push_back(command);
    }

    void CommandList::SetScissor(
        const Scissor& scissor)
    {
        if (!recording)
            return;

        Command command{};
        command.type =
            CommandType::SetScissor;

        command.a =
            static_cast<uint32_t>(scissor.x);

        command.b =
            static_cast<uint32_t>(scissor.y);

        command.c = scissor.width;

        command.w =
            static_cast<float>(scissor.height);

        commands.push_back(command);
    }

    void CommandList::ClearColor(
        float r,
        float g,
        float b,
        float a)
    {
        if (!recording)
            return;

        Command command{};

        command.type =
            CommandType::ClearColor;

        command.x = r;
        command.y = g;
        command.z = b;
        command.w = a;

        commands.push_back(command);
    }

    void CommandList::ClearDepth(float depth)
    {
        if (!recording)
            return;

        Command command{};

        command.type =
            CommandType::ClearDepth;

        command.x = depth;

        commands.push_back(command);
    }

    void CommandList::Draw(
        uint32_t vertexCount,
        uint32_t firstVertex)
    {
        if (!recording)
            return;

        Command command{};

        command.type =
            CommandType::Draw;

        command.a = vertexCount;
        command.b = firstVertex;

        commands.push_back(command);
    }

    void CommandList::DrawIndexed(
        uint32_t indexCount,
        uint32_t firstIndex,
        int32_t vertexOffset)
    {
        if (!recording)
            return;

        Command command{};

        command.type =
            CommandType::DrawIndexed;

        command.a = indexCount;
        command.b = firstIndex;
        command.c =
            static_cast<uint32_t>(vertexOffset);

        commands.push_back(command);
    }

    bool CommandList::IsRecording() const
    {
        return recording;
    }

    const std::vector<Command>&
    CommandList::GetCommands() const
    {
        return commands;
    }

    void CommandList::Clear()
    {
        commands.clear();
        recording = false;
    }
}
