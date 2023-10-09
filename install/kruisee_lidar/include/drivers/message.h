#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>

enum class OPcode : uint8_t {
    kPrepare = 0x00,
    kSync,
    kFollowUp,
    kDelayRequest,
    kDelayResponse,
    kEnd,
};

enum class Flags : uint8_t {
    kNone = 0x00,
    kFinished,
    kFailed
};

#define MESSAGE_MAGIC  (0xfb)

template <size_t N>
class Message {
public:
    Message() {
        memset(bytes_, 0, sizeof(bytes_));
        bytes_[0] = MESSAGE_MAGIC;
        bytes_[1] = static_cast<uint8_t>(OPcode::kPrepare);
        bytes_[2] = static_cast<uint8_t>(Flags::kNone);
    }

    ~Message() = default;

    void Set(OPcode opcode, Flags flags, const char *data, size_t length) {
        assert(length == N);

        bytes_[1] = static_cast<uint8_t>(opcode);
        bytes_[2] = static_cast<uint8_t>(flags);

        if (length != 0)
            memcpy(&bytes_[3], data, length);
    }

    const char *bytes() const { return reinterpret_cast<const char *>(bytes_); }
    char *bytes() { return reinterpret_cast<char *>(bytes_); }

    size_t length() const { return sizeof(bytes_); }

    OPcode opcode() const { return static_cast<OPcode>(bytes_[1]); }
    Flags flags() const { return static_cast<Flags>(bytes_[2]); }

    const char *body() const { return reinterpret_cast<const char *>(&bytes_[3]); }

private:
    uint8_t bytes_[N + 3];
};

#pragma pack(1)

typedef struct {
    uint8_t magic;  // 魔数 (0xFC)
    uint8_t length; // 长度 (8)
    uint64_t stamp; // 时间戳，单位 (ns)
    uint16_t cksum; // 校验 (magic + length + stamp)
} timestamp_t;

#pragma pack()


