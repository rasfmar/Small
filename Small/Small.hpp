#ifndef SMALL
#define SMALL

#include <iostream>
#include "Small_Terminal.hpp"

#define SMALL_RAM 0x1000

typedef uint16_t u16;
typedef uint8_t u8;

namespace small {
    namespace {
        bool running = false;
    };

    enum _reg16 { ax, bx, cx, dx };
    enum _reg8 { al, bl, cl, dl, ah, bh, ch, dh };
    enum _op { 
        movax = 0x10, movbx, movcx, movdx,
            moval, movbl, movcl, movdl,
            movah, movbh, movch, movdh,
        intr = 0x20, 
        quit = 0xFF
    };

    struct _cpu {
        union {
            u16 ax;
            struct {
                u8 al;
                u8 ah;
            };
        };
        union {
            u16 bx;
            struct {
                u8 bl;
                u8 bh;
            };
        };
        union {
            u16 cx;
            struct {
                u8 cl;
                u8 ch;
            };
        };
        union {
            u16 dx;
            struct {
                u8 dl;
                u8 dh;
            };
        };
        u16 ip;
        _cpu() :
            ax(0), bx(0), cx(0), dx(0), 
            ip(0) {};
    } *cpu;

    u8 *memory;
    u16 ap;

    void init() {
        cpu = new _cpu();
        memory = new u8[SMALL_RAM]();
        ap = 0;
        running = true;
    }

    void die() {
        delete cpu;
        delete[] memory;
    }

    void dump() {
        if(running) {
            running = false;
            terminal::reset();

            std::cout << "Bytecode: " << std::endl;

            for(u16 i = 0x0; i < ap; i++)
                std::cout << "\\x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)*(memory + i);
            std::cout << std::endl;
            
            std::cout << "AX: 0x" << std::setfill('0') << std::setw(4) << std::hex << cpu->ax << std::setw(2) << std::endl \
                      << "\tAL=0x" << (u16)cpu->al << std::endl \
                      << "\tAH=0x" << (u16)cpu->ah << std::endl;

            std::cout << "BX: 0x" << std::setfill('0') << std::setw(4) << std::hex << cpu->bx << std::setw(2) << std::endl \
                      << "\tBL=0x" << (u16)cpu->bl << std::endl \
                      << "\tBH=0x" << (u16)cpu->bh << std::endl;

            std::cout << "CX: 0x" << std::setfill('0') << std::setw(4) << std::hex << cpu->cx << std::setw(2) << std::endl \
                      << "\tCL=0x" << (u16)cpu->cl << std::endl \
                      << "\tCH=0x" << (u16)cpu->ch << std::endl;
                      
            std::cout << "DX: 0x" << std::setfill('0') << std::setw(4) << std::hex << cpu->dx << std::setw(2) << std::endl \
                      << "\tDL=0x" << (u16)cpu->dl << std::endl \
                      << "\tDH=0x" << (u16)cpu->dh << std::endl;
                      
            std::cout << "IP: 0x" << std::hex << cpu->ip << std::endl;

            die();
        }
    }

    template<typename T>
    void alloc(T v) {
        *(T*)(memory + ap) = v;
        ap += sizeof(T);
    }

    void alloc(_op op) {
        alloc<u8>((u8)op);
    }

    void alloc(_op op, u8 v) {
        alloc(op);
        alloc<u8>(v);
    }

    void alloc(_op op, u16 v) {
        alloc(op);
        // flip 0xabcd to 0xcdab
        u16 vl = v & 0xFF, vh = v >> 8;
        alloc<u16>(vl + vh*0x100);
    }

    void alloc(_op op, u8* v, u8 sz) {
        alloc(op);
        memcpy((void*)(memory + ap), (void*)v, sz);
        ap += sz;
    }

    void alloc(_op op, const char* c) {
        alloc(op, (u8*)c, strlen(c));
    }
    
    void _mov(_reg16 r, u16 v) {
        switch(r) {
        case ax: cpu->ax = v; break;
        case bx: cpu->bx = v; break;
        case cx: cpu->cx = v; break;
        case dx: cpu->dx = v; break;
        default: break;
        }
    }

    void _mov(_reg8 r, u8 v) {
        switch(r) {
        case al: cpu->al = v; break;
        case ah: cpu->ah = v; break;
        case bl: cpu->bl = v; break;
        case bh: cpu->bh = v; break;
        case cl: cpu->cl = v; break;
        case ch: cpu->ch = v; break;
        case dl: cpu->dl = v; break;
        case dh: cpu->dh = v; break;
        default: break;
        }
    }

    void _int(u8 v) {
        switch(v) {
        case 0x00:
            terminal::move(cpu->dh,cpu->dl);
            break;
        case 0x01:
            terminal::fcolor(cpu->cl,cpu->dh,cpu->dl);
            break;
        case 0x02:
            terminal::bcolor(cpu->cl,cpu->dh,cpu->dl);
            break;
        case 0x03: {
            std::cout << *(char*)(memory + cpu->dx);
            break;
        }
        default:
            break;
        }
    }

    void exec() {
        // interpret bytecode
        if(running) {
            for(; cpu->ip < ap && cpu->ip < SMALL_RAM; ++cpu->ip) {
                u8 c = *(memory + cpu->ip);

                switch(c >> 4) {
                case 0x1: { // mov
                    if((c & 0xF) < 0x4) {
                        // 16 bit, 2 bytes
                        if(cpu->ip < SMALL_RAM - 2) {
                            u16 v = *(memory + ++cpu->ip) + 0x100 * (u16)(*(memory + ++cpu->ip));
                            _mov((_reg16)(c & 0xF), v);
                        } else {
                            dump();
                            return;
                        }
                    } else {
                        // 8 bit
                        if(cpu->ip < SMALL_RAM - 0x1) {
                            u8 v = *(memory + ++cpu->ip);
                            _mov((_reg8)((c & 0xF) - 0x4),v);
                        } else {
                            dump();
                            return;
                        }
                    }
                    break;
                }
                case 0x2: { // int
                    if(cpu->ip < SMALL_RAM - 0x1) {
                        u8 v = *(memory + ++cpu->ip);
                        _int(v);
                    } else {
                        dump();
                        return;
                    }
                    break;
                }
                case 0xf: default:
                    goto die;
                }
            }
        }
    die:
        terminal::reset();
        std::cout << "\nProcess completed\n";

        return;
    }
}

#endif