#include "botcraft/Game/Entities/entities/projectile/SpectralArrowEntity.hpp"

namespace Botcraft
{
    SpectralArrowEntity::SpectralArrowEntity()
    {

    }

    SpectralArrowEntity::~SpectralArrowEntity()
    {

    }


    std::string SpectralArrowEntity::GetName() const
    {
        return "SpectralArrow";
    }

    EntityType SpectralArrowEntity::GetType() const
    {
        return EntityType::SpectralArrow;
    }

    double SpectralArrowEntity::GetWidth() const
    {
        return 0.5;
    }

    double SpectralArrowEntity::GetHeight() const
    {
        return 0.5;
    }


    std::string SpectralArrowEntity::GetClassName()
    {
        return "SpectralArrow";
    }

    EntityType SpectralArrowEntity::GetClassType()
    {
        return EntityType::SpectralArrow;
    }

}
