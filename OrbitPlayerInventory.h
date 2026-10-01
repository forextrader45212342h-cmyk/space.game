#pragma once

#include "OrbitTypes.h"
#include <string>
#include <vector>
#include <cstdint>

namespace Orbit
{
    enum class ItemLocation
    {
        Hand,
        Pocket,
        Backpack,
        Vehicle,
        Home,
        Storage
    };

    struct InventoryItem
    {
        std::uint64_t Id = 0;
        std::string Name;
        int Quantity = 1;
        double WeightKg = 0.0;

        ItemLocation Location =
            ItemLocation::Pocket;

        bool Usable = false;
    };

    class OrbitPlayerInventory
    {
    public:
        bool AddItem(
            const InventoryItem& item);

        bool RemoveItem(
            std::uint64_t itemId,
            int quantity);

        bool MoveItem(
            std::uint64_t itemId,
            ItemLocation location);

        InventoryItem* FindItem(
            std::uint64_t itemId);

        double GetTotalWeight() const;

        const std::vector<InventoryItem>&
            GetItems() const;

    private:
        std::vector<InventoryItem> Items;
    };
}
