#include "botcraft/Game/Entities/entities/monster/SlimeEntity.hpp"

#include <mutex>

namespace Botcraft
{
    const std::array<std::string, SlimeEntity::metadata_count> SlimeEntity::metadata_names{ {
        "id_size",
    } };

    SlimeEntity::SlimeEntity()
    {
        // Initialize all metadata with default values
        SetIdSize(1);
    }

    SlimeEntity::~SlimeEntity()
    {

    }


    std::string SlimeEntity::GetName() const
    {
        return "slime";
    }

    EntityType SlimeEntity::GetType() const
    {
        return EntityType::Slime;
    }

    double SlimeEntity::GetWidth() const
    {
        return 0.255 * 2.04 * GetIdSize();
    }

    double SlimeEntity::GetHeight() const
    {
        return 0.255 * 2.04 * GetIdSize();
    }


    std::string SlimeEntity::GetClassName()
    {
        return "slime";
    }

    EntityType SlimeEntity::GetClassType()
    {
        return EntityType::Slime;
    }


    ProtocolCraft::Json::Value SlimeEntity::Serialize() const
    {
        ProtocolCraft::Json::Value output = MobEntity::Serialize();

        output["metadata"]["id_size"] = GetIdSize();

        return output;
    }


    void SlimeEntity::SetMetadataValue(const int index, const std::any& value)
    {
        if (index < hierarchy_metadata_count)
        {
            MobEntity::SetMetadataValue(index, value);
        }
        else if (index - hierarchy_metadata_count < metadata_count)
        {
            std::scoped_lock<std::shared_mutex> lock(entity_mutex);
            metadata[metadata_names[index - hierarchy_metadata_count]] = value;
        }
    }

    int SlimeEntity::GetIdSize() const
    {
        std::shared_lock<std::shared_mutex> lock(entity_mutex);
        return std::any_cast<int>(metadata.at("id_size"));
    }


    void SlimeEntity::SetIdSize(const int id_size)
    {
        std::scoped_lock<std::shared_mutex> lock(entity_mutex);
        metadata["id_size"] = id_size;
#if USE_GUI
        are_rendered_faces_up_to_date = false;
        for (size_t i = 0; i < faces.size(); ++i)
        {
            std::static_pointer_cast<Renderer::Scale>(face_descriptors[i].transformations.scales.back())->axis_x = static_cast<float>(GetWidth() / 2.0);
            std::static_pointer_cast<Renderer::Scale>(face_descriptors[i].transformations.scales.back())->axis_y = static_cast<float>(GetHeight() / 2.0);
            std::static_pointer_cast<Renderer::Scale>(face_descriptors[i].transformations.scales.back())->axis_z = static_cast<float>(GetWidth() / 2.0);
        }
#endif
    }

}
