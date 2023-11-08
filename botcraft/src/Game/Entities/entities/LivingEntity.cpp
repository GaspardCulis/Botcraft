#include "botcraft/Game/Entities/entities/LivingEntity.hpp"

#include <mutex>

namespace Botcraft
{
    const std::array<std::string, LivingEntity::metadata_count> LivingEntity::metadata_names{ {
        "data_living_entity_flags",
        "data_health_id",
        "data_effect_color_id",
        "data_effect_ambience_id",
        "data_arrow_count_id",
#if PROTOCOL_VERSION > 498 /* > 1.14.4 */
        "data_stinger_count_id",
#endif
#if PROTOCOL_VERSION > 404 /* > 1.13.2 */
        "sleeping_pos_id",
#endif
    } };

    LivingEntity::LivingEntity()
    {
        // Initialize all metadata with default values
        SetDataLivingEntityFlags(0);
        SetDataHealthId(1.0f);
        SetDataEffectColorId(0);
        SetDataEffectAmbienceId(false);
        SetDataArrowCountId(0);
#if PROTOCOL_VERSION > 498 /* > 1.14.4 */
        SetDataStingerCountId(0);
#endif
#if PROTOCOL_VERSION > 404 /* > 1.13.2 */
        SetSleepingPosId(std::optional<Position>());
#endif
    }

    LivingEntity::~LivingEntity()
    {

    }

    bool LivingEntity::IsLivingEntity() const
    {
        return true;
    }


    ProtocolCraft::Json::Value LivingEntity::Serialize() const
    {
        ProtocolCraft::Json::Value output = Entity::Serialize();

        output["metadata"]["data_living_entity_flags"] = GetDataLivingEntityFlags();
        output["metadata"]["data_health_id"] = GetDataHealthId();
        output["metadata"]["data_effect_color_id"] = GetDataEffectColorId();
        output["metadata"]["data_effect_ambience_id"] = GetDataEffectAmbienceId();
        output["metadata"]["data_arrow_count_id"] = GetDataArrowCountId();
#if PROTOCOL_VERSION > 498 /* > 1.14.4 */
        output["metadata"]["data_stinger_count_id"] = GetDataStingerCountId();
#endif
#if PROTOCOL_VERSION > 404 /* > 1.13.2 */
        output["metadata"]["sleeping_pos_id"] = GetSleepingPosId() ? GetSleepingPosId().value().Serialize() : ProtocolCraft::Json::Value();
#endif

        return output;
    }


    void LivingEntity::SetMetadataValue(const int index, const std::any& value)
    {
        if (index < hierarchy_metadata_count)
        {
            Entity::SetMetadataValue(index, value);
        }
        else if (index - hierarchy_metadata_count < metadata_count)
        {
            std::scoped_lock<std::shared_mutex> lock(entity_mutex);
            metadata[metadata_names[index - hierarchy_metadata_count]] = value;
        }
    }

    char LivingEntity::GetDataLivingEntityFlags() const
    {
        std::shared_lock<std::shared_mutex> lock(entity_mutex);
        return std::any_cast<char>(metadata.at("data_living_entity_flags"));
    }

    float LivingEntity::GetDataHealthId() const
    {
        std::shared_lock<std::shared_mutex> lock(entity_mutex);
        return std::any_cast<float>(metadata.at("data_health_id"));
    }

    int LivingEntity::GetDataEffectColorId() const
    {
        std::shared_lock<std::shared_mutex> lock(entity_mutex);
        return std::any_cast<int>(metadata.at("data_effect_color_id"));
    }

    bool LivingEntity::GetDataEffectAmbienceId() const
    {
        std::shared_lock<std::shared_mutex> lock(entity_mutex);
        return std::any_cast<bool>(metadata.at("data_effect_ambience_id"));
    }

    int LivingEntity::GetDataArrowCountId() const
    {
        std::shared_lock<std::shared_mutex> lock(entity_mutex);
        return std::any_cast<int>(metadata.at("data_arrow_count_id"));
    }

#if PROTOCOL_VERSION > 498 /* > 1.14.4 */
    int LivingEntity::GetDataStingerCountId() const
    {
        std::shared_lock<std::shared_mutex> lock(entity_mutex);
        return std::any_cast<int>(metadata.at("data_stinger_count_id"));
    }
#endif

#if PROTOCOL_VERSION > 404 /* > 1.13.2 */
    std::optional<Position> LivingEntity::GetSleepingPosId() const
    {
        std::shared_lock<std::shared_mutex> lock(entity_mutex);
        return std::any_cast<std::optional<Position>>(metadata.at("sleeping_pos_id"));
    }
#endif


    void LivingEntity::SetDataLivingEntityFlags(const char data_living_entity_flags)
    {
        std::scoped_lock<std::shared_mutex> lock(entity_mutex);
        metadata["data_living_entity_flags"] = data_living_entity_flags;
    }

    void LivingEntity::SetDataHealthId(const float data_health_id)
    {
        std::scoped_lock<std::shared_mutex> lock(entity_mutex);
        metadata["data_health_id"] = data_health_id;
    }

    void LivingEntity::SetDataEffectColorId(const int data_effect_color_id)
    {
        std::scoped_lock<std::shared_mutex> lock(entity_mutex);
        metadata["data_effect_color_id"] = data_effect_color_id;
    }

    void LivingEntity::SetDataEffectAmbienceId(const bool data_effect_ambience_id)
    {
        std::scoped_lock<std::shared_mutex> lock(entity_mutex);
        metadata["data_effect_ambience_id"] = data_effect_ambience_id;
    }

    void LivingEntity::SetDataArrowCountId(const int data_arrow_count_id)
    {
        std::scoped_lock<std::shared_mutex> lock(entity_mutex);
        metadata["data_arrow_count_id"] = data_arrow_count_id;
    }

#if PROTOCOL_VERSION > 498 /* > 1.14.4 */
    void LivingEntity::SetDataStingerCountId(const int data_stinger_count_id)
    {
        std::scoped_lock<std::shared_mutex> lock(entity_mutex);
        metadata["data_stinger_count_id"] = data_stinger_count_id;
    }
#endif

#if PROTOCOL_VERSION > 404 /* > 1.13.2 */
    void LivingEntity::SetSleepingPosId(const std::optional<Position>& sleeping_pos_id)
    {
        std::scoped_lock<std::shared_mutex> lock(entity_mutex);
        metadata["sleeping_pos_id"] = sleeping_pos_id;
    }
#endif

}
