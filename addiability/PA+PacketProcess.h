/* Automatically generated by p4c-ebpf from PA+PacketProcess.p4 on Tue Oct 22 21:14:01 2024
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

struct tcp_t {
    u16 srcPort; /* bit<16> */
    u16 dstPort; /* bit<16> */
    u32 seqNo; /* bit<32> */
    u32 ackNo; /* bit<32> */
    u8 dataOffset; /* bit<4> */
    u8 res; /* bit<4> */
    u8 flags; /* bit<8> */
    u16 window; /* bit<16> */
    u16 checksum; /* bit<16> */
    u16 urgentPtr; /* bit<16> */
    u8 ebpf_valid;
};

struct headers {
    struct ethernet_t ethernet; /* ethernet_t */
    struct ipv4_t ipv4; /* ipv4_t */
    struct tcp_t tcp; /* tcp_t */
};

typedef u32 MyFilter_http_counter_key;
typedef u32 MyFilter_http_counter_value;
typedef u32 MyFilter_tcp_counter_key;
typedef u32 MyFilter_tcp_counter_value;
#if CONTROL_PLANE
static void init_tables() 
{
    u32 ebpf_zero = 0;
}
#endif
#endif
