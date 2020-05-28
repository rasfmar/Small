#ifndef SMALL
#define SMALL

namespace small {
    namespace {
        bool running = false;
    };

    enum _reg16 { ax, bx };
    enum _reg8 { al, ah, bl, bh };

    struct _cpu {
        union {
            std::uint16_t ax;
            struct {
                uint8_t al;
                uint8_t ah;
            };
        };
        union {
            std::uint16_t bx;
            struct {
                uint8_t bl;
                uint8_t bh;
            };
        };
        _cpu() :
            ax(0), bx(0) {};
    } *cpu;

    uint8_t *memory;

    void init() {
        cpu = new _cpu();
        memory = new uint8_t[512]();
        running = true;
    }

    void dump() {
        if(running) {
            // ax
            std::cout << "AX: 0x" << std::hex << cpu->ax << std::endl \
                      << "\tAL=0x" << (std::uint16_t)cpu->al << std::endl \
                      << "\tAH=0x" << (std::uint16_t)cpu->ah << std::endl;

            // bx
            std::cout << "BX: 0x" << std::hex << cpu->bx << std::endl \
                      << "\tBL=0x" << (std::uint16_t)cpu->bl << std::endl \
                      << "\tBH=0x" << (std::uint16_t)cpu->bh << std::endl;
        }
    }
    
    void mov(_reg16 r, std::uint16_t v) {
        switch(r) {
        case ax: cpu->ax = v; break;
        case bx: cpu->bx = v; break;
        default: break;
        }
    }

    void mov(_reg8 r, std::uint8_t v) {
        switch(r) {
        case al: cpu->al = v; break;
        case ah: cpu->ah = v; break;
        case bl: cpu->bl = v; break;
        case bh: cpu->bh = v; break;
        default: break;
        }
    }

    void die() {
        delete cpu;
        delete[] memory;
    }
}

#endif