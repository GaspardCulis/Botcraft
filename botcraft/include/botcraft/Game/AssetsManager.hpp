#pragma once

#include "botcraft/Game/World/Biome.hpp"
#include "botcraft/Game/World/Blockstate.hpp"
#include "botcraft/Game/Inventory/Item.hpp"

#include <vector>
#include <unordered_map>

namespace Botcraft
{
#if USE_GUI
    namespace Renderer
    {
        class Atlas;
    }
#endif

    class AssetsManager
    {
    public:
        static AssetsManager& getInstance();

        AssetsManager(AssetsManager const&) = delete;
        void operator=(AssetsManager const&) = delete;

#if PROTOCOL_VERSION < 347 /* < 1.13 */
        const std::unordered_map<int, std::unordered_map<unsigned char, std::unique_ptr<Blockstate> > >& Blockstates() const;
#else
        const std::unordered_map<int, std::unique_ptr<Blockstate> >& Blockstates() const;
#endif
        const Blockstate* GetBlockstate(const BlockstateId id) const;
        
#if PROTOCOL_VERSION < 358 /* < 1.13 */
        const std::unordered_map<unsigned char, std::unique_ptr<Biome> >& Biomes() const;
        const Biome* GetBiome(const unsigned char id) const;
#else
        const std::unordered_map<int, std::unique_ptr<Biome> >& Biomes() const;
        const Biome* GetBiome(const int id) const;
#endif

        const std::unordered_map<ItemId, std::unique_ptr<Item> >& Items() const;
        const Item* GetItem(const ItemId id) const;

#if PROTOCOL_VERSION < 347 /* < 1.13 */
        const std::pair<int, unsigned char> GetItemID(const std::string& item_name) const;
#else
        const int GetItemID(const std::string& item_name) const;
#endif

#if USE_GUI
        const Renderer::Atlas* GetAtlas() const;
#endif

    private:
        AssetsManager();

        void LoadBlocksFile();
#if PROTOCOL_VERSION > 340 /* > 1.12.2 */
        void FlattenBlocks();
#endif
        void LoadBiomesFile();
        void LoadItemsFile();
#if USE_GUI
        void LoadTextures();
#endif
        void ClearCaches();

#if USE_GUI
        void UpdateModelsWithAtlasData();
#endif

    private:
#if PROTOCOL_VERSION < 347 /* < 1.13 */
        std::unordered_map<int, std::unordered_map<unsigned char, std::unique_ptr<Blockstate> > > blockstates;
#else
        std::unordered_map<int, std::unique_ptr<Blockstate> > blockstates;
        std::vector<const Blockstate*> flattened_blockstates;
        size_t flattened_blockstates_size;
#endif
#if PROTOCOL_VERSION < 358 /* < 1.13 */
        std::unordered_map<unsigned char, std::unique_ptr<Biome> > biomes;
#else
        std::unordered_map<int, std::unique_ptr<Biome> > biomes;
#endif
        std::unordered_map<ItemId, std::unique_ptr<Item>> items;
#if USE_GUI
        std::unique_ptr<Renderer::Atlas> atlas;
#endif
    };
} // Botcraft
