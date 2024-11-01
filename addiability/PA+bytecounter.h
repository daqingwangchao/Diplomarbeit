/* Automatically generated by p4c-ebpf from PA+bytecounter.p4 on Thu Oct 10 11:08:48 2024
 */
#ifndef _P4_GEN_HEADER_
#define _P4_GEN_HEADER_
#include "ebpf_kernel.h"

#define MAP_PATH "/sys/fs/bpf/tc/globals"
struct ethernet_t {
    u64 dstAddr; /* bit<48> */
    u64 srcAddr; /* bit<48> */
    u16 etherType; /* bit<16> */
    u8 ebpf_valid;
};

struct ipv4_t {
    u8 version; /* bit<4> */
    u8 ihl; /* bit<4> */
    u8 diffserv; /* bit<8> */
    u16 totalLen; /* bit<16> */
    u16 identification; /* bit<16> */
    u8 flags; /* bit<3> */
    u16 fragOffset; /* bit<13> */
    u8 ttl; /* bit<8> */
    u8 protocol; /* bit<8> */
    u16 hdrChecksum; /* bit<16> */
    u32 srcAddr; /* bit<32> */
    u32 dstAddr; /* bit<32> */
    u8 ebpf_valid;
};

struct headers {
    struct ethernet_t ethernet; /* ethernet_t */
    struct ipv4_t ipv4; /* ipv4_t */
};

typedef u32 MyFilter_byte_counter_key;
typedef u32 MyFilter_byte_counter_value;
#if CONTROL_PLANE
static void init_tables() 
{
    u32 ebpf_zero = 0;
}
#endif
#endif
