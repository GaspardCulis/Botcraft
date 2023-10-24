#pragma once

#include <any>
#include <chrono>
#include <map>
#include <memory>
#include <optional>
#include <shared_mutex>

#if PROTOCOL_VERSION > 340 /* > 1.12.2 */
#include "protocolCraft/Types/Chat/Chat.hpp"
#endif
#include "protocolCraft/Types/Slot.hpp"
#include "protocolCraft/Utilities/Json.hpp"

#include "botcraft/Game/AABB.hpp"
#include "botcraft/Game/Enums.hpp"
#include "botcraft/Game/Vector3.hpp"

#if USE_GUI
#include "botcraft/Game/Model.hpp"
#endif

namespace Botcraft
{
    enum class EntityType;
#if PROTOCOL_VERSION < 458 /* < 1.14 */
    enum class ObjectEntityType;
#endif

    struct EntityEffect
    {
        EntityEffectType type;
        unsigned char amplifier;
        std::chrono::steady_clock::time_point end;
    };

    class Entity
    {
    protected:
#if PROTOCOL_VERSION > 754 /* > 1.16.4/5 */
        static constexpr int metadata_count = 8;
#elif PROTOCOL_VERSION > 404 /* > 1.13.2 */
        static constexpr int metadata_count = 7;
#else
        static constexpr int metadata_count = 6;
#endif
        static const std::array<std::string, metadata_count> metadata_names;
        static constexpr int hierarchy_metadata_count = 0;

    public:
        Entity();
        virtual ~Entity();

        // Object related stuff
        virtual std::string GetName() const = 0;
        virtual EntityType GetType() const = 0;
        AABB GetCollider() const;
        virtual double GetWidth() const;
        virtual double GetHeight() const;

        // Metadata stuff
        void LoadMetadataFromRawArray(const std::vector<unsigned char>& data);
        virtual void SetMetadataValue(const int index, const std::any& value);

        char GetDataSharedFlagsId() const;
        int GetDataAirSupplyId() const;
#if PROTOCOL_VERSION > 340 /* > 1.12.2 */
        std::optional<ProtocolCraft::Chat> GetDataCustomName() const;
#else
        std::string GetDataCustomName() const;
#endif
        bool GetDataCustomNameVisible() const;
        bool GetDataSilent() const;
        bool GetDataNoGravity() const;
#if PROTOCOL_VERSION > 404 /* > 1.13.2 */
        Pose GetDataPose() const;
#endif
#if PROTOCOL_VERSION > 754 /* > 1.16.4/5 */
        int GetDataTicksFrozen() const;
#endif

        void SetDataSharedFlagsId(const char data_shared_flags_id);
        void SetDataAirSupplyId(const int data_air_supply_id);
#if PROTOCOL_VERSION > 340 /* > 1.12.2 */
        void SetDataCustomName(const std::optional<ProtocolCraft::Chat>& data_custom_name);
#else
        void SetDataCustomName(const std::string& data_custom_name);
#endif
        void SetDataCustomNameVisible(const bool data_custom_name_visible);
        void SetDataSilent(const bool data_silent);
        void SetDataNoGravity(const bool data_no_gravity);
#if PROTOCOL_VERSION > 404 /* > 1.13.2 */
        void SetDataPose(const Pose data_pose);
#endif
#if PROTOCOL_VERSION > 754 /* > 1.16.4/5 */
        void SetDataTicksFrozen(const int data_ticks_frozen);
#endif

        // Generic properties getter
        int GetEntityID() const;
        Vector3<double> GetPosition() const;
        double GetX() const;
        double GetY() const;
        double GetZ() const;
        float GetYaw() const;
        float GetPitch() const;
        Vector3<double> GetSpeed() const;
        double GetSpeedX() const;
        double GetSpeedY() const;
        double GetSpeedZ() const;
        bool GetOnGround() const;
        std::map<EquipmentSlot, ProtocolCraft::Slot> GetEquipments() const;
        ProtocolCraft::Slot GetEquipment(const EquipmentSlot slot) const;
        std::vector<EntityEffect> GetEffects() const;
#if USE_GUI
        std::vector<Renderer::Face> GetFaces(const bool reset_uptodate_status);
        bool GetAreRenderedFacesUpToDate() const;
#endif

        // Generic properties setter
        void SetEntityID(const int entity_id_);
        virtual void SetPosition(const Vector3<double>& position_);
        virtual void SetX(const double x_);
        virtual void SetY(const double y_);
        virtual void SetZ(const double z_);
        virtual void SetYaw(const float yaw_);
        virtual void SetPitch(const float pitch_);
        void SetSpeed(const Vector3<double>& speed_);
        void SetSpeedX(const double speed_x_);
        void SetSpeedY(const double speed_y_);
        void SetSpeedZ(const double speed_z_);
        void SetOnGround(const bool on_ground_);
        void SetEquipment(const EquipmentSlot slot, const ProtocolCraft::Slot& item);
        void SetEffects(const std::vector<EntityEffect>& effects_);
        void AddEffect(const EntityEffect& effect);
        void RemoveEffect(const EntityEffectType type);
#if USE_GUI
        void SetAreRenderedFacesUpToDate(const bool are_rendered_faces_up_to_date_);
#endif

        // In case it's needed one day, could be useful
        virtual ProtocolCraft::Json::Value Serialize() const;

        virtual bool IsLocalPlayer() const;
        virtual bool IsRemotePlayer() const;
        // Can be used to know if an entity has a certain virtual type as ancestor
        virtual bool IsLivingEntity() const;
        virtual bool IsAbstractArrow() const;
        virtual bool IsAnimal() const;
        virtual bool IsAmbientCreature() const;
        virtual bool IsMonster() const;
#if PROTOCOL_VERSION > 761 /* > 1.19.3 */
        virtual bool IsDisplay() const;
#endif
        virtual bool IsTamableAnimal() const;
        virtual bool IsAbstractSchoolingFish() const;
        virtual bool IsWaterAnimal() const;
        virtual bool IsAbstractChestedHorse() const;
        virtual bool IsAbstractHurtingProjectile() const;
        virtual bool IsMob() const;
        virtual bool IsSpellcasterIllager() const;
#if PROTOCOL_VERSION > 578 /* > 1.15.2 */
        virtual bool IsProjectile() const;
#endif
        virtual bool IsFlyingMob() const;
        virtual bool IsAbstractHorse() const;
        virtual bool IsAbstractGolem() const;
        virtual bool IsHangingEntity() const;
        virtual bool IsFireball() const;
        virtual bool IsAbstractMinecart() const;
        virtual bool IsAbstractMinecartContainer() const;
        virtual bool IsShoulderRidingEntity() const;
#if PROTOCOL_VERSION > 736 /* > 1.16.1 */
        virtual bool IsAbstractPiglin() const;
#endif
        virtual bool IsAbstractIllager() const;
        virtual bool IsAbstractFish() const;
#if PROTOCOL_VERSION > 404 /* > 1.13.2 */
        virtual bool IsRaider() const;
#endif
        virtual bool IsAbstractSkeleton() const;
        virtual bool IsThrowableItemProjectile() const;
#if PROTOCOL_VERSION > 477 /* > 1.14 */
        virtual bool IsAbstractVillager() const;
#endif
        virtual bool IsAgeableMob() const;
        virtual bool IsPathfinderMob() const;
#if PROTOCOL_VERSION > 404 /* > 1.13.2 */
        virtual bool IsPatrollingMonster() const;
#endif
        virtual bool IsThrowableProjectile() const;

        // Factory stuff
        static std::shared_ptr<Entity> CreateEntity(const EntityType type);
#if PROTOCOL_VERSION < 458 /* < 1.14 */
        static std::shared_ptr<Entity> CreateObjectEntity(const ObjectEntityType type);
#endif

    protected:
#if USE_GUI
        virtual void InitializeFaces();
#endif

    protected:
        mutable std::shared_mutex entity_mutex;

        int entity_id;
        Vector3<double> position;
        float yaw;
        float pitch;
        Vector3<double> speed;
        bool on_ground;
        /// @brief Items on this entity. Note that for the local player
        /// this will **NOT** be populated. Check corresponding
        /// player inventory slots instead.
        std::map<EquipmentSlot, ProtocolCraft::Slot> equipments;
        std::vector<EntityEffect> effects;

        std::map<std::string, std::any> metadata;

#if USE_GUI
        //All the faces of this model
        std::vector<FaceDescriptor> face_descriptors;
        std::vector<Renderer::Face> faces;

        bool are_rendered_faces_up_to_date;
#endif
    };

    enum class EntityType
    {
        None = -1,
#if PROTOCOL_VERSION > 761 /* > 1.19.3 */
        Allay = 0,
        AreaEffectCloud = 1,
        ArmorStand = 2,
        Arrow = 3,
        Axolotl = 4,
        Bat = 5,
        Bee = 6,
        Blaze = 7,
        DisplayBlockDisplay = 8,
        Boat = 9,
        Camel = 10,
        Cat = 11,
        CaveSpider = 12,
        ChestBoat = 13,
        MinecartChest = 14,
        Chicken = 15,
        Cod = 16,
        MinecartCommandBlock = 17,
        Cow = 18,
        Creeper = 19,
        Dolphin = 20,
        Donkey = 21,
        DragonFireball = 22,
        Drowned = 23,
        ThrownEgg = 24,
        ElderGuardian = 25,
        EndCrystal = 26,
        EnderDragon = 27,
        ThrownEnderpearl = 28,
        EnderMan = 29,
        Endermite = 30,
        Evoker = 31,
        EvokerFangs = 32,
        ThrownExperienceBottle = 33,
        ExperienceOrb = 34,
        EyeOfEnder = 35,
        FallingBlockEntity = 36,
        FireworkRocketEntity = 37,
        Fox = 38,
        Frog = 39,
        MinecartFurnace = 40,
        Ghast = 41,
        Giant = 42,
        GlowItemFrame = 43,
        GlowSquid = 44,
        Goat = 45,
        Guardian = 46,
        Hoglin = 47,
        MinecartHopper = 48,
        Horse = 49,
        Husk = 50,
        Illusioner = 51,
        Interaction = 52,
        IronGolem = 53,
        ItemEntity = 54,
        DisplayItemDisplay = 55,
        ItemFrame = 56,
        LargeFireball = 57,
        LeashFenceKnotEntity = 58,
        LightningBolt = 59,
        Llama = 60,
        LlamaSpit = 61,
        MagmaCube = 62,
        Marker = 63,
        Minecart = 64,
        MushroomCow = 65,
        Mule = 66,
        Ocelot = 67,
        Painting = 68,
        Panda = 69,
        Parrot = 70,
        Phantom = 71,
        Pig = 72,
        Piglin = 73,
        PiglinBrute = 74,
        Pillager = 75,
        PolarBear = 76,
        ThrownPotion = 77,
        Pufferfish = 78,
        Rabbit = 79,
        Ravager = 80,
        Salmon = 81,
        Sheep = 82,
        Shulker = 83,
        ShulkerBullet = 84,
        Silverfish = 85,
        Skeleton = 86,
        SkeletonHorse = 87,
        Slime = 88,
        SmallFireball = 89,
        Sniffer = 90,
        SnowGolem = 91,
        Snowball = 92,
        MinecartSpawner = 93,
        SpectralArrow = 94,
        Spider = 95,
        Squid = 96,
        Stray = 97,
        Strider = 98,
        Tadpole = 99,
        DisplayTextDisplay = 100,
        PrimedTnt = 101,
        MinecartTNT = 102,
        TraderLlama = 103,
        ThrownTrident = 104,
        TropicalFish = 105,
        Turtle = 106,
        Vex = 107,
        Villager = 108,
        Vindicator = 109,
        WanderingTrader = 110,
        Warden = 111,
        Witch = 112,
        WitherBoss = 113,
        WitherSkeleton = 114,
        WitherSkull = 115,
        Wolf = 116,
        Zoglin = 117,
        Zombie = 118,
        ZombieHorse = 119,
        ZombieVillager = 120,
        ZombifiedPiglin = 121,
        Player = 122,
        FishingHook = 123,
#elif PROTOCOL_VERSION > 760 /* > 1.19.1/2 */
        Allay = 0,
        AreaEffectCloud = 1,
        ArmorStand = 2,
        Arrow = 3,
        Axolotl = 4,
        Bat = 5,
        Bee = 6,
        Blaze = 7,
        Boat = 8,
        ChestBoat = 9,
        Cat = 10,
        Camel = 11,
        CaveSpider = 12,
        Chicken = 13,
        Cod = 14,
        Cow = 15,
        Creeper = 16,
        Dolphin = 17,
        Donkey = 18,
        DragonFireball = 19,
        Drowned = 20,
        ElderGuardian = 21,
        EndCrystal = 22,
        EnderDragon = 23,
        EnderMan = 24,
        Endermite = 25,
        Evoker = 26,
        EvokerFangs = 27,
        ExperienceOrb = 28,
        EyeOfEnder = 29,
        FallingBlockEntity = 30,
        FireworkRocketEntity = 31,
        Fox = 32,
        Frog = 33,
        Ghast = 34,
        Giant = 35,
        GlowItemFrame = 36,
        GlowSquid = 37,
        Goat = 38,
        Guardian = 39,
        Hoglin = 40,
        Horse = 41,
        Husk = 42,
        Illusioner = 43,
        IronGolem = 44,
        ItemEntity = 45,
        ItemFrame = 46,
        LargeFireball = 47,
        LeashFenceKnotEntity = 48,
        LightningBolt = 49,
        Llama = 50,
        LlamaSpit = 51,
        MagmaCube = 52,
        Marker = 53,
        Minecart = 54,
        MinecartChest = 55,
        MinecartCommandBlock = 56,
        MinecartFurnace = 57,
        MinecartHopper = 58,
        MinecartSpawner = 59,
        MinecartTNT = 60,
        Mule = 61,
        MushroomCow = 62,
        Ocelot = 63,
        Painting = 64,
        Panda = 65,
        Parrot = 66,
        Phantom = 67,
        Pig = 68,
        Piglin = 69,
        PiglinBrute = 70,
        Pillager = 71,
        PolarBear = 72,
        PrimedTnt = 73,
        Pufferfish = 74,
        Rabbit = 75,
        Ravager = 76,
        Salmon = 77,
        Sheep = 78,
        Shulker = 79,
        ShulkerBullet = 80,
        Silverfish = 81,
        Skeleton = 82,
        SkeletonHorse = 83,
        Slime = 84,
        SmallFireball = 85,
        SnowGolem = 86,
        Snowball = 87,
        SpectralArrow = 88,
        Spider = 89,
        Squid = 90,
        Stray = 91,
        Strider = 92,
        Tadpole = 93,
        ThrownEgg = 94,
        ThrownEnderpearl = 95,
        ThrownExperienceBottle = 96,
        ThrownPotion = 97,
        ThrownTrident = 98,
        TraderLlama = 99,
        TropicalFish = 100,
        Turtle = 101,
        Vex = 102,
        Villager = 103,
        Vindicator = 104,
        WanderingTrader = 105,
        Warden = 106,
        Witch = 107,
        WitherBoss = 108,
        WitherSkeleton = 109,
        WitherSkull = 110,
        Wolf = 111,
        Zoglin = 112,
        Zombie = 113,
        ZombieHorse = 114,
        ZombieVillager = 115,
        ZombifiedPiglin = 116,
        Player = 117,
        FishingHook = 118,
#elif PROTOCOL_VERSION > 758 /* > 1.18.2 */
        Allay = 0,
        AreaEffectCloud = 1,
        ArmorStand = 2,
        Arrow = 3,
        Axolotl = 4,
        Bat = 5,
        Bee = 6,
        Blaze = 7,
        Boat = 8,
        ChestBoat = 9,
        Cat = 10,
        CaveSpider = 11,
        Chicken = 12,
        Cod = 13,
        Cow = 14,
        Creeper = 15,
        Dolphin = 16,
        Donkey = 17,
        DragonFireball = 18,
        Drowned = 19,
        ElderGuardian = 20,
        EndCrystal = 21,
        EnderDragon = 22,
        EnderMan = 23,
        Endermite = 24,
        Evoker = 25,
        EvokerFangs = 26,
        ExperienceOrb = 27,
        EyeOfEnder = 28,
        FallingBlockEntity = 29,
        FireworkRocketEntity = 30,
        Fox = 31,
        Frog = 32,
        Ghast = 33,
        Giant = 34,
        GlowItemFrame = 35,
        GlowSquid = 36,
        Goat = 37,
        Guardian = 38,
        Hoglin = 39,
        Horse = 40,
        Husk = 41,
        Illusioner = 42,
        IronGolem = 43,
        ItemEntity = 44,
        ItemFrame = 45,
        LargeFireball = 46,
        LeashFenceKnotEntity = 47,
        LightningBolt = 48,
        Llama = 49,
        LlamaSpit = 50,
        MagmaCube = 51,
        Marker = 52,
        Minecart = 53,
        MinecartChest = 54,
        MinecartCommandBlock = 55,
        MinecartFurnace = 56,
        MinecartHopper = 57,
        MinecartSpawner = 58,
        MinecartTNT = 59,
        Mule = 60,
        MushroomCow = 61,
        Ocelot = 62,
        Painting = 63,
        Panda = 64,
        Parrot = 65,
        Phantom = 66,
        Pig = 67,
        Piglin = 68,
        PiglinBrute = 69,
        Pillager = 70,
        PolarBear = 71,
        PrimedTnt = 72,
        Pufferfish = 73,
        Rabbit = 74,
        Ravager = 75,
        Salmon = 76,
        Sheep = 77,
        Shulker = 78,
        ShulkerBullet = 79,
        Silverfish = 80,
        Skeleton = 81,
        SkeletonHorse = 82,
        Slime = 83,
        SmallFireball = 84,
        SnowGolem = 85,
        Snowball = 86,
        SpectralArrow = 87,
        Spider = 88,
        Squid = 89,
        Stray = 90,
        Strider = 91,
        Tadpole = 92,
        ThrownEgg = 93,
        ThrownEnderpearl = 94,
        ThrownExperienceBottle = 95,
        ThrownPotion = 96,
        ThrownTrident = 97,
        TraderLlama = 98,
        TropicalFish = 99,
        Turtle = 100,
        Vex = 101,
        Villager = 102,
        Vindicator = 103,
        WanderingTrader = 104,
        Warden = 105,
        Witch = 106,
        WitherBoss = 107,
        WitherSkeleton = 108,
        WitherSkull = 109,
        Wolf = 110,
        Zoglin = 111,
        Zombie = 112,
        ZombieHorse = 113,
        ZombieVillager = 114,
        ZombifiedPiglin = 115,
        Player = 116,
        FishingHook = 117,
#elif PROTOCOL_VERSION > 754 /* > 1.16.4/5 */
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Axolotl = 3,
        Bat = 4,
        Bee = 5,
        Blaze = 6,
        Boat = 7,
        Cat = 8,
        CaveSpider = 9,
        Chicken = 10,
        Cod = 11,
        Cow = 12,
        Creeper = 13,
        Dolphin = 14,
        Donkey = 15,
        DragonFireball = 16,
        Drowned = 17,
        ElderGuardian = 18,
        EndCrystal = 19,
        EnderDragon = 20,
        EnderMan = 21,
        Endermite = 22,
        Evoker = 23,
        EvokerFangs = 24,
        ExperienceOrb = 25,
        EyeOfEnder = 26,
        FallingBlockEntity = 27,
        FireworkRocketEntity = 28,
        Fox = 29,
        Ghast = 30,
        Giant = 31,
        GlowItemFrame = 32,
        GlowSquid = 33,
        Goat = 34,
        Guardian = 35,
        Hoglin = 36,
        Horse = 37,
        Husk = 38,
        Illusioner = 39,
        IronGolem = 40,
        ItemEntity = 41,
        ItemFrame = 42,
        LargeFireball = 43,
        LeashFenceKnotEntity = 44,
        LightningBolt = 45,
        Llama = 46,
        LlamaSpit = 47,
        MagmaCube = 48,
        Marker = 49,
        Minecart = 50,
        MinecartChest = 51,
        MinecartCommandBlock = 52,
        MinecartFurnace = 53,
        MinecartHopper = 54,
        MinecartSpawner = 55,
        MinecartTNT = 56,
        Mule = 57,
        MushroomCow = 58,
        Ocelot = 59,
        Painting = 60,
        Panda = 61,
        Parrot = 62,
        Phantom = 63,
        Pig = 64,
        Piglin = 65,
        PiglinBrute = 66,
        Pillager = 67,
        PolarBear = 68,
        PrimedTnt = 69,
        Pufferfish = 70,
        Rabbit = 71,
        Ravager = 72,
        Salmon = 73,
        Sheep = 74,
        Shulker = 75,
        ShulkerBullet = 76,
        Silverfish = 77,
        Skeleton = 78,
        SkeletonHorse = 79,
        Slime = 80,
        SmallFireball = 81,
        SnowGolem = 82,
        Snowball = 83,
        SpectralArrow = 84,
        Spider = 85,
        Squid = 86,
        Stray = 87,
        Strider = 88,
        ThrownEgg = 89,
        ThrownEnderpearl = 90,
        ThrownExperienceBottle = 91,
        ThrownPotion = 92,
        ThrownTrident = 93,
        TraderLlama = 94,
        TropicalFish = 95,
        Turtle = 96,
        Vex = 97,
        Villager = 98,
        Vindicator = 99,
        WanderingTrader = 100,
        Witch = 101,
        WitherBoss = 102,
        WitherSkeleton = 103,
        WitherSkull = 104,
        Wolf = 105,
        Zoglin = 106,
        Zombie = 107,
        ZombieHorse = 108,
        ZombieVillager = 109,
        ZombifiedPiglin = 110,
        Player = 111,
        FishingHook = 112,
#elif PROTOCOL_VERSION > 736 /* > 1.16.1 */
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Bee = 4,
        Blaze = 5,
        Boat = 6,
        Cat = 7,
        CaveSpider = 8,
        Chicken = 9,
        Cod = 10,
        Cow = 11,
        Creeper = 12,
        Dolphin = 13,
        Donkey = 14,
        DragonFireball = 15,
        Drowned = 16,
        ElderGuardian = 17,
        EndCrystal = 18,
        EnderDragon = 19,
        EnderMan = 20,
        Endermite = 21,
        Evoker = 22,
        EvokerFangs = 23,
        ExperienceOrb = 24,
        EyeOfEnder = 25,
        FallingBlockEntity = 26,
        FireworkRocketEntity = 27,
        Fox = 28,
        Ghast = 29,
        Giant = 30,
        Guardian = 31,
        Hoglin = 32,
        Horse = 33,
        Husk = 34,
        Illusioner = 35,
        IronGolem = 36,
        ItemEntity = 37,
        ItemFrame = 38,
        LargeFireball = 39,
        LeashFenceKnotEntity = 40,
        LightningBolt = 41,
        Llama = 42,
        LlamaSpit = 43,
        MagmaCube = 44,
        Minecart = 45,
        MinecartChest = 46,
        MinecartCommandBlock = 47,
        MinecartFurnace = 48,
        MinecartHopper = 49,
        MinecartSpawner = 50,
        MinecartTNT = 51,
        Mule = 52,
        MushroomCow = 53,
        Ocelot = 54,
        Painting = 55,
        Panda = 56,
        Parrot = 57,
        Phantom = 58,
        Pig = 59,
        Piglin = 60,
        PiglinBrute = 61,
        Pillager = 62,
        PolarBear = 63,
        PrimedTnt = 64,
        Pufferfish = 65,
        Rabbit = 66,
        Ravager = 67,
        Salmon = 68,
        Sheep = 69,
        Shulker = 70,
        ShulkerBullet = 71,
        Silverfish = 72,
        Skeleton = 73,
        SkeletonHorse = 74,
        Slime = 75,
        SmallFireball = 76,
        SnowGolem = 77,
        Snowball = 78,
        SpectralArrow = 79,
        Spider = 80,
        Squid = 81,
        Stray = 82,
        Strider = 83,
        ThrownEgg = 84,
        ThrownEnderpearl = 85,
        ThrownExperienceBottle = 86,
        ThrownPotion = 87,
        ThrownTrident = 88,
        TraderLlama = 89,
        TropicalFish = 90,
        Turtle = 91,
        Vex = 92,
        Villager = 93,
        Vindicator = 94,
        WanderingTrader = 95,
        Witch = 96,
        WitherBoss = 97,
        WitherSkeleton = 98,
        WitherSkull = 99,
        Wolf = 100,
        Zoglin = 101,
        Zombie = 102,
        ZombieHorse = 103,
        ZombieVillager = 104,
        ZombifiedPiglin = 105,
        Player = 106,
        FishingHook = 107,
#elif PROTOCOL_VERSION > 578 /* > 1.15.2 */
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Bee = 4,
        Blaze = 5,
        Boat = 6,
        Cat = 7,
        CaveSpider = 8,
        Chicken = 9,
        Cod = 10,
        Cow = 11,
        Creeper = 12,
        Dolphin = 13,
        Donkey = 14,
        DragonFireball = 15,
        Drowned = 16,
        ElderGuardian = 17,
        EndCrystal = 18,
        EnderDragon = 19,
        EnderMan = 20,
        Endermite = 21,
        Evoker = 22,
        EvokerFangs = 23,
        ExperienceOrb = 24,
        EyeOfEnder = 25,
        FallingBlockEntity = 26,
        FireworkRocketEntity = 27,
        Fox = 28,
        Ghast = 29,
        Giant = 30,
        Guardian = 31,
        Hoglin = 32,
        Horse = 33,
        Husk = 34,
        Illusioner = 35,
        IronGolem = 36,
        ItemEntity = 37,
        ItemFrame = 38,
        LargeFireball = 39,
        LeashFenceKnotEntity = 40,
        LightningBolt = 41,
        Llama = 42,
        LlamaSpit = 43,
        MagmaCube = 44,
        Minecart = 45,
        MinecartChest = 46,
        MinecartCommandBlock = 47,
        MinecartFurnace = 48,
        MinecartHopper = 49,
        MinecartSpawner = 50,
        MinecartTNT = 51,
        Mule = 52,
        MushroomCow = 53,
        Ocelot = 54,
        Painting = 55,
        Panda = 56,
        Parrot = 57,
        Phantom = 58,
        Pig = 59,
        Piglin = 60,
        Pillager = 61,
        PolarBear = 62,
        PrimedTnt = 63,
        Pufferfish = 64,
        Rabbit = 65,
        Ravager = 66,
        Salmon = 67,
        Sheep = 68,
        Shulker = 69,
        ShulkerBullet = 70,
        Silverfish = 71,
        Skeleton = 72,
        SkeletonHorse = 73,
        Slime = 74,
        SmallFireball = 75,
        SnowGolem = 76,
        Snowball = 77,
        SpectralArrow = 78,
        Spider = 79,
        Squid = 80,
        Stray = 81,
        Strider = 82,
        ThrownEgg = 83,
        ThrownEnderpearl = 84,
        ThrownExperienceBottle = 85,
        ThrownPotion = 86,
        ThrownTrident = 87,
        TraderLlama = 88,
        TropicalFish = 89,
        Turtle = 90,
        Vex = 91,
        Villager = 92,
        Vindicator = 93,
        WanderingTrader = 94,
        Witch = 95,
        WitherBoss = 96,
        WitherSkeleton = 97,
        WitherSkull = 98,
        Wolf = 99,
        Zoglin = 100,
        Zombie = 101,
        ZombieHorse = 102,
        ZombieVillager = 103,
        ZombifiedPiglin = 104,
        Player = 105,
        FishingHook = 106,
#elif PROTOCOL_VERSION > 498 /* > 1.14.4 */
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Bee = 4,
        Blaze = 5,
        Boat = 6,
        Cat = 7,
        CaveSpider = 8,
        Chicken = 9,
        Cod = 10,
        Cow = 11,
        Creeper = 12,
        Donkey = 13,
        Dolphin = 14,
        DragonFireball = 15,
        Drowned = 16,
        ElderGuardian = 17,
        EndCrystal = 18,
        EnderDragon = 19,
        EnderMan = 20,
        Endermite = 21,
        EvokerFangs = 22,
        Evoker = 23,
        ExperienceOrb = 24,
        EyeOfEnder = 25,
        FallingBlockEntity = 26,
        FireworkRocketEntity = 27,
        Fox = 28,
        Ghast = 29,
        Giant = 30,
        Guardian = 31,
        Horse = 32,
        Husk = 33,
        Illusioner = 34,
        ItemEntity = 35,
        ItemFrame = 36,
        LargeFireball = 37,
        LeashFenceKnotEntity = 38,
        Llama = 39,
        LlamaSpit = 40,
        MagmaCube = 41,
        Minecart = 42,
        MinecartChest = 43,
        MinecartCommandBlock = 44,
        MinecartFurnace = 45,
        MinecartHopper = 46,
        MinecartSpawner = 47,
        MinecartTNT = 48,
        Mule = 49,
        MushroomCow = 50,
        Ocelot = 51,
        Painting = 52,
        Panda = 53,
        Parrot = 54,
        Pig = 55,
        Pufferfish = 56,
        PigZombie = 57,
        PolarBear = 58,
        PrimedTnt = 59,
        Rabbit = 60,
        Salmon = 61,
        Sheep = 62,
        Shulker = 63,
        ShulkerBullet = 64,
        Silverfish = 65,
        Skeleton = 66,
        SkeletonHorse = 67,
        Slime = 68,
        SmallFireball = 69,
        SnowGolem = 70,
        Snowball = 71,
        SpectralArrow = 72,
        Spider = 73,
        Squid = 74,
        Stray = 75,
        TraderLlama = 76,
        TropicalFish = 77,
        Turtle = 78,
        ThrownEgg = 79,
        ThrownEnderpearl = 80,
        ThrownExperienceBottle = 81,
        ThrownPotion = 82,
        ThrownTrident = 83,
        Vex = 84,
        Villager = 85,
        IronGolem = 86,
        Vindicator = 87,
        Pillager = 88,
        WanderingTrader = 89,
        Witch = 90,
        WitherBoss = 91,
        WitherSkeleton = 92,
        WitherSkull = 93,
        Wolf = 94,
        Zombie = 95,
        ZombieHorse = 96,
        ZombieVillager = 97,
        Phantom = 98,
        Ravager = 99,
        LightningBolt = 100,
        Player = 101,
        FishingHook = 102,
#elif PROTOCOL_VERSION > 404 /* > 1.13.2 */
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Blaze = 4,
        Boat = 5,
        Cat = 6,
        CaveSpider = 7,
        Chicken = 8,
        Cod = 9,
        Cow = 10,
        Creeper = 11,
        Donkey = 12,
        Dolphin = 13,
        DragonFireball = 14,
        Drowned = 15,
        ElderGuardian = 16,
        EndCrystal = 17,
        EnderDragon = 18,
        EnderMan = 19,
        Endermite = 20,
        EvokerFangs = 21,
        Evoker = 22,
        ExperienceOrb = 23,
        EyeOfEnder = 24,
        FallingBlockEntity = 25,
        FireworkRocketEntity = 26,
        Fox = 27,
        Ghast = 28,
        Giant = 29,
        Guardian = 30,
        Horse = 31,
        Husk = 32,
        Illusioner = 33,
        ItemEntity = 34,
        ItemFrame = 35,
        LargeFireball = 36,
        LeashFenceKnotEntity = 37,
        Llama = 38,
        LlamaSpit = 39,
        MagmaCube = 40,
        Minecart = 41,
        MinecartChest = 42,
        MinecartCommandBlock = 43,
        MinecartFurnace = 44,
        MinecartHopper = 45,
        MinecartSpawner = 46,
        MinecartTNT = 47,
        Mule = 48,
        MushroomCow = 49,
        Ocelot = 50,
        Painting = 51,
        Panda = 52,
        Parrot = 53,
        Pig = 54,
        Pufferfish = 55,
        PigZombie = 56,
        PolarBear = 57,
        PrimedTnt = 58,
        Rabbit = 59,
        Salmon = 60,
        Sheep = 61,
        Shulker = 62,
        ShulkerBullet = 63,
        Silverfish = 64,
        Skeleton = 65,
        SkeletonHorse = 66,
        Slime = 67,
        SmallFireball = 68,
        SnowGolem = 69,
        Snowball = 70,
        SpectralArrow = 71,
        Spider = 72,
        Squid = 73,
        Stray = 74,
        TraderLlama = 75,
        TropicalFish = 76,
        Turtle = 77,
        ThrownEgg = 78,
        ThrownEnderpearl = 79,
        ThrownExperienceBottle = 80,
        ThrownPotion = 81,
        ThrownTrident = 82,
        Vex = 83,
        Villager = 84,
        IronGolem = 85,
        Vindicator = 86,
        Pillager = 87,
        WanderingTrader = 88,
        Witch = 89,
        WitherBoss = 90,
        WitherSkeleton = 91,
        WitherSkull = 92,
        Wolf = 93,
        Zombie = 94,
        ZombieHorse = 95,
        ZombieVillager = 96,
        Phantom = 97,
        Ravager = 98,
        LightningBolt = 99,
        Player = 100,
        FishingHook = 101,
#elif PROTOCOL_VERSION > 340 /* > 1.12.2 */
        AreaEffectCloud = 0,
        ArmorStand = 1,
        Arrow = 2,
        Bat = 3,
        Blaze = 4,
        Boat = 5,
        CaveSpider = 6,
        Chicken = 7,
        Cod = 8,
        Cow = 9,
        Creeper = 10,
        Donkey = 11,
        Dolphin = 12,
        DragonFireball = 13,
        Drowned = 14,
        ElderGuardian = 15,
        EndCrystal = 16,
        EnderDragon = 17,
        EnderMan = 18,
        Endermite = 19,
        EvokerFangs = 20,
        Evoker = 21,
        ExperienceOrb = 22,
        EyeOfEnder = 23,
        FallingBlockEntity = 24,
        FireworkRocketEntity = 25,
        Ghast = 26,
        Giant = 27,
        Guardian = 28,
        Horse = 29,
        Husk = 30,
        Illusioner = 31,
        ItemEntity = 32,
        ItemFrame = 33,
        LargeFireball = 34,
        LeashFenceKnotEntity = 35,
        Llama = 36,
        LlamaSpit = 37,
        MagmaCube = 38,
        Minecart = 39,
        MinecartChest = 40,
        MinecartCommandBlock = 41,
        MinecartFurnace = 42,
        MinecartHopper = 43,
        MinecartSpawner = 44,
        MinecartTNT = 45,
        Mule = 46,
        MushroomCow = 47,
        Ocelot = 48,
        Painting = 49,
        Parrot = 50,
        Pig = 51,
        Pufferfish = 52,
        PigZombie = 53,
        PolarBear = 54,
        PrimedTnt = 55,
        Rabbit = 56,
        Salmon = 57,
        Sheep = 58,
        Shulker = 59,
        ShulkerBullet = 60,
        Silverfish = 61,
        Skeleton = 62,
        SkeletonHorse = 63,
        Slime = 64,
        SmallFireball = 65,
        SnowGolem = 66,
        Snowball = 67,
        SpectralArrow = 68,
        Spider = 69,
        Squid = 70,
        Stray = 71,
        TropicalFish = 72,
        Turtle = 73,
        ThrownEgg = 74,
        ThrownEnderpearl = 75,
        ThrownExperienceBottle = 76,
        ThrownPotion = 77,
        Vex = 78,
        Villager = 79,
        IronGolem = 80,
        Vindicator = 81,
        Witch = 82,
        WitherBoss = 83,
        WitherSkeleton = 84,
        WitherSkull = 85,
        Wolf = 86,
        Zombie = 87,
        ZombieHorse = 88,
        ZombieVillager = 89,
        Phantom = 90,
        LightningBolt = 91,
        Player = 92,
        FishingHook = 93,
        ThrownTrident = 94,
#else // 1.12.2
        FishingHook = -3,
        Player = -2,
        ItemEntity = 1,
        ExperienceOrb = 2,
        AreaEffectCloud = 3,
        ElderGuardian = 4,
        WitherSkeleton = 5,
        Stray = 6,
        ThrownEgg = 7,
        LeashFenceKnotEntity = 8,
        Painting = 9,
        Arrow = 10,
        Snowball = 11,
        LargeFireball = 12,
        SmallFireball = 13,
        ThrownEnderpearl = 14,
        EyeOfEnder = 15,
        ThrownPotion = 16,
        ThrownExperienceBottle = 17,
        ItemFrame = 18,
        WitherSkull = 19,
        PrimedTnt = 20,
        FallingBlockEntity = 21,
        FireworkRocketEntity = 22,
        Husk = 23,
        SpectralArrow = 24,
        ShulkerBullet = 25,
        DragonFireball = 26,
        ZombieVillager = 27,
        SkeletonHorse = 28,
        ZombieHorse = 29,
        ArmorStand = 30,
        Donkey = 31,
        Mule = 32,
        EvokerFangs = 33,
        Evoker = 34,
        Vex = 35,
        Vindicator = 36,
        Illusioner = 37,
        MinecartCommandBlock = 40,
        Boat = 41,
        Minecart = 42,
        MinecartChest = 43,
        MinecartFurnace = 44,
        MinecartTNT = 45,
        MinecartHopper = 46,
        MinecartSpawner = 47,
        Creeper = 50,
        Skeleton = 51,
        Spider = 52,
        Giant = 53,
        Zombie = 54,
        Slime = 55,
        Ghast = 56,
        PigZombie = 57,
        EnderMan = 58,
        CaveSpider = 59,
        Silverfish = 60,
        Blaze = 61,
        MagmaCube = 62,
        EnderDragon = 63,
        WitherBoss = 64,
        Bat = 65,
        Witch = 66,
        Endermite = 67,
        Guardian = 68,
        Shulker = 69,
        Pig = 90,
        Sheep = 91,
        Cow = 92,
        Chicken = 93,
        Squid = 94,
        Wolf = 95,
        MushroomCow = 96,
        SnowGolem = 97,
        Ocelot = 98,
        IronGolem = 99,
        Horse = 100,
        Rabbit = 101,
        PolarBear = 102,
        Llama = 103,
        LlamaSpit = 104,
        Parrot = 105,
        Villager = 120,
        EndCrystal = 200,
#endif
        MaxEntityIndex
    };

#if PROTOCOL_VERSION < 458 /* < 1.14 */
    enum class ObjectEntityType
    {
        None = -1,
        Boat = 1,
        ItemEntity = 2,
        AreaEffectCloud = 3,
        PrimedTnt = 50,
        EndCrystal = 51,
        Arrow = 60,
        Snowball = 61,
        ThrownEgg = 62,
        LargeFireball = 63,
        SmallFireball = 64,
        ThrownEnderpearl = 65,
        WitherSkull = 66,
        ShulkerBullet = 67,
        LlamaSpit = 68,
        FallingBlockEntity = 70,
        ItemFrame = 71,
        EyeOfEnder = 72,
        ThrownPotion = 73,
        ThrownExperienceBottle = 75,
        FireworkRocketEntity = 76,
        LeashFenceKnotEntity = 77,
        ArmorStand = 78,
        EvokerFangs = 79,
        FishingHook = 90,
        SpectralArrow = 91,
        DragonFireball = 93,
#if PROTOCOL_VERSION > 340 /* > 1.12.2 */
        ThrownTrident = 94,
#endif
        MaxEntityIndex
    };
#endif
}
