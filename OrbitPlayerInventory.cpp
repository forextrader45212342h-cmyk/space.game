#include "OrbitPlayerInventory.h"

namespace Orbit
{
    bool OrbitPlayerInventory::AddItem(
        const InventoryItem& item)
    {
        Items.push_back(item);
        return true;
    }

    bool OrbitPlayerInventory::RemoveItem(
        std::uint64_t itemId,
        int quantity)
    {
        for (auto it = Items.begin();
             it != Items.end();
             ++it)
        {
            if (it->Id != itemId)
                continue;

            if (it->Quantity > quantity)
            {
                it->Quantity -= quantity;
                return true;
            }

            if (it->Quantity == quantity)
            {
                Items.erase(it);
                return true;
            }

            return false;
        }

        return false;
    }

    bool OrbitPlayerInventory::MoveItem(
        std::uint64_t itemId,
        ItemLocation location)
    {
        InventoryItem* item =
            FindItem(itemId);

        if (!item)
            return false;

        item->Location = location;
        return true;
    }

    InventoryItem*
    OrbitPlayerInventory::FindItem(
        std::uint64_t itemId)
    {
        for (auto& item : Items)
        {
            if (item.Id == itemId)
                return &item;
        }

        return nullptr;
    }

    double OrbitPlayerInventory::GetTotalWeight() const
    {
        double total = 0.0;

        for (const auto& item : Items)
        {
            total +=
                item.WeightKg *
                item.Quantity;
        }

        return total;
    }

    const std::vector<InventoryItem>&
    OrbitPlayerInventory::GetItems() const
    {
        return Items;
    }
}
