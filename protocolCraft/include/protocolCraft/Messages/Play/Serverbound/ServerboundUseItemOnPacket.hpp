#pragma once

#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Types/NetworkPosition.hpp"

namespace ProtocolCraft
{
    class ServerboundUseItemOnPacket : public BaseMessage<ServerboundUseItemOnPacket>
    {
    public:
#if   PROTOCOL_VERSION == 340
        static constexpr int packet_id = 0x1F;
#elif PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 ||  \
      PROTOCOL_VERSION == 404
        static constexpr int packet_id = 0x29;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 ||  \
      PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 ||  \
      PROTOCOL_VERSION == 498 || PROTOCOL_VERSION == 573 ||  \
      PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578
        static constexpr int packet_id = 0x2C;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736
        static constexpr int packet_id = 0x2D;
#elif PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 ||  \
      PROTOCOL_VERSION == 754 || PROTOCOL_VERSION == 755 ||  \
      PROTOCOL_VERSION == 756 || PROTOCOL_VERSION == 757 ||  \
      PROTOCOL_VERSION == 758
        static constexpr int packet_id = 0x2E;
#elif PROTOCOL_VERSION == 759
        static constexpr int packet_id = 0x30;
#elif PROTOCOL_VERSION == 760 || PROTOCOL_VERSION == 761 ||  \
      PROTOCOL_VERSION == 762 || PROTOCOL_VERSION == 763
        static constexpr int packet_id = 0x31;
#else
#error "Protocol version not implemented"
#endif

        static constexpr std::string_view packet_name = "Use Item On";

        virtual ~ServerboundUseItemOnPacket() override
        {

        }

        void SetHand(const int hand_)
        {
            hand = hand_;
        }

        void SetLocation(const NetworkPosition& location_)
        {
            location = location_;
        }

        void SetDirection(const int direction_)
        {
            direction = direction_;
        }

        void SetCursorPositionX(const float cursor_position_x_)
        {
            cursor_position_x = cursor_position_x_;
        }

        void SetCursorPositionY(const float cursor_position_y_)
        {
            cursor_position_y = cursor_position_y_;
        }

        void SetCursorPositionZ(const float cursor_position_z_)
        {
            cursor_position_z = cursor_position_z_;
        }

#if PROTOCOL_VERSION > 452
        void SetInside(const bool inside_)
        {
            inside = inside_;
        }
#endif

#if PROTOCOL_VERSION > 758
        void SetSequence(const int sequence_)
        {
            sequence = sequence_;
        }
#endif


        int GetHand() const
        {
            return hand;
        }

        const NetworkPosition& GetLocation() const
        {
            return location;
        }

        int GetDirection() const
        {
            return direction;
        }

        float GetCursorPositionX() const
        {
            return cursor_position_x;
        }

        float GetCursorPositionY() const
        {
            return cursor_position_y;
        }

        float GetCursorPositionZ() const
        {
            return cursor_position_z;
        }

#if PROTOCOL_VERSION > 452
        bool GetInside() const
        {
            return inside;
        }
#endif

#if PROTOCOL_VERSION > 758
        int GetSequence() const
        {
            return sequence;
        }
#endif


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
#if PROTOCOL_VERSION > 452
            hand = ReadData<VarInt>(iter, length);
#endif
            location = ReadData<NetworkPosition>(iter, length);
            direction = ReadData<VarInt>(iter, length);
#if PROTOCOL_VERSION < 453
            hand = ReadData<VarInt>(iter, length);
#endif
            cursor_position_x = ReadData<float>(iter, length);
            cursor_position_y = ReadData<float>(iter, length);
            cursor_position_z = ReadData<float>(iter, length);
#if PROTOCOL_VERSION > 452
            inside = ReadData<bool>(iter, length);
#endif
#if PROTOCOL_VERSION > 758
            sequence = ReadData<VarInt>(iter, length);
#endif
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
#if PROTOCOL_VERSION > 452
            WriteData<VarInt>(hand, container);
#endif
            WriteData<NetworkPosition>(location, container);
            WriteData<VarInt>(direction, container);
#if PROTOCOL_VERSION < 453
            WriteData<VarInt>(hand, container);
#endif
            WriteData<float>(cursor_position_x, container);
            WriteData<float>(cursor_position_y, container);
            WriteData<float>(cursor_position_z, container);
#if PROTOCOL_VERSION > 452
            WriteData<bool>(inside, container);
#endif
#if PROTOCOL_VERSION > 758
            WriteData<VarInt>(sequence, container);
#endif
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output["location"] = location;
            output["direction"] = direction;
            output["cursor_position_x"] = cursor_position_x;
            output["cursor_position_y"] = cursor_position_y;
            output["cursor_position_z"] = cursor_position_z;
#if PROTOCOL_VERSION > 452
            output["inside"] = inside;
#endif
            output["hand"] = hand;
#if PROTOCOL_VERSION > 758
            output["sequence"] = sequence;
#endif


            return output;
        }

    private:
        NetworkPosition location;
        int direction = 0;
        float cursor_position_x = 0.0f;
        float cursor_position_y = 0.0f;
        float cursor_position_z = 0.0f;
#if PROTOCOL_VERSION > 452
        bool inside = false;
#endif
        int hand = 0;
#if PROTOCOL_VERSION > 758
        int sequence = 0;
#endif

    };
} //ProtocolCraft
