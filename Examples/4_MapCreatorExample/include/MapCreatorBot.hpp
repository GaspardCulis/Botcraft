#pragma once

#include <botcraft/Game/Vector3.hpp>
#include <botcraft/Game/InterfaceClient.hpp>
#include <botcraft/Game/World/Blockstate.hpp>

#include <random>
#include <set>
#include <map>

class MapCreatorBot : public Botcraft::InterfaceClient
{
public:
    MapCreatorBot(const bool use_renderer_);
    ~MapCreatorBot();

    void LoadNBTFile(const std::string& path, const Botcraft::Position& offset_, const std::string& temp_block, const bool display_neeed);
protected:
    virtual void Handle(ProtocolCraft::ClientboundChatPacket &msg) override;

private:

    // Get the positions of all loaded chests
    // returns: a vector with all the found positions
    const std::vector<Botcraft::Position> GetAllChestsAround() const;

    const bool GetSomeFood(const std::string& item_name);
    const bool SwapChestsInventory(const std::string& food_name, const bool take);
    const std::set<std::string> GetBlocksAvailableInInventory() const;
    const bool FindNextPositionToPlaceBlock(const std::set<std::string>& available, Botcraft::Position& out_pos, std::string& out_item, Botcraft::PlayerDiggingFace& out_face);
    const bool CheckCompletion(const bool full, const bool print_errors, const bool print_details) const;

    void CreateMap();

private:
    std::vector<std::vector<std::vector<short> > > target;
    std::map<short, std::string> palette;
    Botcraft::Position start;
    Botcraft::Position end;

    std::mt19937 random_engine;

};