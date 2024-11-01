/* Automatically generated by p4c-ebpf from packetProcessor.p4 on Wed Oct  2 14:26:47 2024
 */
#include "PA.h"
#include "ebpf_kernel.h"

enum ebpf_errorCodes {
    NoError,
    PacketTooShort,
    NoMatch,
    StackOutOfBounds,
    HeaderTooShort,
    ParserTimeout,
    ParserInvalidArgument,
};

#define EBPF_MASK(t, w) ((((t)(1)) << (w)) - (t)1)
#define BYTES(w) ((w) / 8)
#define write_partial(a, s, v) do { u8 mask = EBPF_MASK(u8, s); *((u8*)a) = ((*((u8*)a)) & ~mask) | (((v) >> (8 - (s))) & mask); } while (0)
#define write_partial_ex(a, w, s, v) do { *((u8*)a) = ((*((u8*)a)) & ~(EBPF_MASK(u8, w) << s)) | (v << s) ; } while (0)
#define write_byte(base, offset, v) do { *(u8*)((base) + (offset)) = (v); } while (0)
#define PTR_DIFF_BYTES(b, o) (ssize_t)((u8*)(b) - (u8*)(o))

void* memcpy(void* dest, const void* src, size_t num);

#define bpf_trace_message(fmt, ...)

//REGISTER_START()
//REGISTER_TABLE(MyIngress_vxlan_process, BPF_MAP_TYPE_HASH, struct MyIngress_vxlan_process_key, struct MyIngress_vxlan_process_value, 1024)
//BPF_ANNOTATE_KV_PAIR(MyIngress_vxlan_process, struct MyIngress_vxlan_process_key, struct MyIngress_vxlan_process_value)
//REGISTER_TABLE(MyIngress_vxlan_process_defaultAction, BPF_MAP_TYPE_ARRAY, u32, struct MyIngress_vxlan_process_value, 1)
//BPF_ANNOTATE_KV_PAIR(MyIngress_vxlan_process_defaultAction, u32, struct MyIngress_vxlan_process_value)
//REGISTER_END()

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, struct MyIngress_vxlan_process_key);
    __type(value, struct MyIngress_vxlan_process_value);
    __uint(max_entries, 1024);
}MyIngress_vxlan_process SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, u32);
    __type(value, struct MyIngress_vxlan_process_value);
    __uint(max_entries, 1);
}MyIngress_vxlan_process_defaultAction SEC(".maps");

// new add: time stamp map
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, u32);
    __type(value, u64);
    __uint(max_entries, 1);
} timestamp_map SEC(".maps");


// 新增: CPU ID map
struct cpu_stats {
    __u32 last_cpu;
    __u32 cpu_switches;
    __u64 packets_processed[30];  // 假设最多 256 个 CPU
};



struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, __u32);
    __type(value, struct cpu_stats);
    __uint(max_entries, 1);
} cpu_stats_map SEC(".maps");




SEC("prog")
int ebpf_filter(SK_BUFF *skb){
    struct headers hdr = {
        .outer_ethernet = {
            .ebpf_valid = 0
        },
        .outer_ipv4 = {
            .ebpf_valid = 0
        },
        .outer_udp = {
            .ebpf_valid = 0
        },
        .vxlan = {
            .ebpf_valid = 0
        },
        .inner_ethernet = {
            .ebpf_valid = 0
        },
        .inner_ipv4 = {
            .ebpf_valid = 0
        },
    };
/************************************************************************** */
    // New add: 使用bpf_ktime_get_ns()获取当前时间戳
    u64 current_time = bpf_ktime_get_ns();
    u32 key = 0;
    bpf_map_update_elem(&timestamp_map, &key, &current_time, BPF_ANY);
/************************************************************************ */

/******************************************************************************* */
    // Update CPU ID
    struct cpu_stats *stats = bpf_map_lookup_elem(&cpu_stats_map, &key);
    if (!stats) {
        return TC_ACT_OK;  // 如果找不到 stats，直接返回
    }

    __u32 current_cpu = bpf_get_smp_processor_id();
    
    // 更新 CPU 统计
    if (current_cpu < 30) {  // 确保不会越界
        __sync_fetch_and_add(&stats->packets_processed[current_cpu], 1);
    }

    if (stats->last_cpu != current_cpu) {
        __sync_fetch_and_add(&stats->cpu_switches, 1);
        stats->last_cpu = current_cpu;
    }
/******************************************************************************* */

    enum ebpf_errorCodes ebpf_errorCode = NoError;
    void* ebpf_packetStart = ((void*)(long)skb->data);
    u8* ebpf_headerStart = ebpf_packetStart;
    void* ebpf_packetEnd = ((void*)(long)skb->data_end);
    u8 accept = 0;
    u32 ebpf_zero = 0;
    unsigned char ebpf_byte;
    u32 ebpf_pkt_len = skb->len;

    goto start;
    parse_inner_ipv4: {
/* extract(hdr.inner_ipv4) */
        if ((u8*)ebpf_packetEnd < ebpf_headerStart + BYTES(160 + 0)) {
            ebpf_errorCode = PacketTooShort;
            goto reject;
        }

        hdr.inner_ipv4.version = (u8)((load_byte(ebpf_headerStart, BYTES(0)) >> 4) & EBPF_MASK(u8, 4));
        hdr.inner_ipv4.ihl = (u8)((load_byte(ebpf_headerStart, BYTES(4))) & EBPF_MASK(u8, 4));
        hdr.inner_ipv4.diffserv = (u8)((load_byte(ebpf_headerStart, BYTES(8))));
        hdr.inner_ipv4.totalLen = (u16)((load_half(ebpf_headerStart, BYTES(16))));
        hdr.inner_ipv4.identification = (u16)((load_half(ebpf_headerStart, BYTES(32))));
        hdr.inner_ipv4.flags = (u8)((load_byte(ebpf_headerStart, BYTES(48)) >> 5) & EBPF_MASK(u8, 3));
        hdr.inner_ipv4.fragOffset = (u16)((load_half(ebpf_headerStart, BYTES(51))) & EBPF_MASK(u16, 13));
        hdr.inner_ipv4.ttl = (u8)((load_byte(ebpf_headerStart, BYTES(64))));
        hdr.inner_ipv4.protocol = (u8)((load_byte(ebpf_headerStart, BYTES(72))));
        hdr.inner_ipv4.hdrChecksum = (u16)((load_half(ebpf_headerStart, BYTES(80))));
        hdr.inner_ipv4.srcAddr = (u32)((load_word(ebpf_headerStart, BYTES(96))));
        hdr.inner_ipv4.dstAddr = (u32)((load_word(ebpf_headerStart, BYTES(128))));

        hdr.inner_ipv4.ebpf_valid = 1;
        ebpf_headerStart += BYTES(160);

;
         goto accept;
    }
    parse_outer_ipv4: {
/* extract(hdr.outer_ipv4) */
        if ((u8*)ebpf_packetEnd < ebpf_headerStart + BYTES(160 + 0)) {
            ebpf_errorCode = PacketTooShort;
            goto reject;
        }

        hdr.outer_ipv4.version = (u8)((load_byte(ebpf_headerStart, BYTES(0)) >> 4) & EBPF_MASK(u8, 4));
        hdr.outer_ipv4.ihl = (u8)((load_byte(ebpf_headerStart, BYTES(4))) & EBPF_MASK(u8, 4));
        hdr.outer_ipv4.diffserv = (u8)((load_byte(ebpf_headerStart, BYTES(8))));
        hdr.outer_ipv4.totalLen = (u16)((load_half(ebpf_headerStart, BYTES(16))));
        hdr.outer_ipv4.identification = (u16)((load_half(ebpf_headerStart, BYTES(32))));
        hdr.outer_ipv4.flags = (u8)((load_byte(ebpf_headerStart, BYTES(48)) >> 5) & EBPF_MASK(u8, 3));
        hdr.outer_ipv4.fragOffset = (u16)((load_half(ebpf_headerStart, BYTES(51))) & EBPF_MASK(u16, 13));
        hdr.outer_ipv4.ttl = (u8)((load_byte(ebpf_headerStart, BYTES(64))));
        hdr.outer_ipv4.protocol = (u8)((load_byte(ebpf_headerStart, BYTES(72))));
        hdr.outer_ipv4.hdrChecksum = (u16)((load_half(ebpf_headerStart, BYTES(80))));
        hdr.outer_ipv4.srcAddr = (u32)((load_word(ebpf_headerStart, BYTES(96))));
        hdr.outer_ipv4.dstAddr = (u32)((load_word(ebpf_headerStart, BYTES(128))));

        hdr.outer_ipv4.ebpf_valid = 1;
        ebpf_headerStart += BYTES(160);

;
        u8 select_0;
        select_0 = hdr.outer_ipv4.protocol;
        if (select_0 == 17)goto parse_outer_udp;
        if ((select_0 & 0x0) == (0x0 & 0x0))goto accept;
        else goto reject;
    }
    parse_outer_udp: {
/* extract(hdr.outer_udp) */
        if ((u8*)ebpf_packetEnd < ebpf_headerStart + BYTES(64 + 0)) {
            ebpf_errorCode = PacketTooShort;
            goto reject;
        }

        hdr.outer_udp.srcPort = (u16)((load_half(ebpf_headerStart, BYTES(0))));
        hdr.outer_udp.dstPort = (u16)((load_half(ebpf_headerStart, BYTES(16))));
        hdr.outer_udp.length = (u16)((load_half(ebpf_headerStart, BYTES(32))));
        hdr.outer_udp.checksum = (u16)((load_half(ebpf_headerStart, BYTES(48))));

        hdr.outer_udp.ebpf_valid = 1;
        ebpf_headerStart += BYTES(64);

;
        u16 select_1;
        select_1 = hdr.outer_udp.dstPort;
        if (select_1 == 4789)goto parse_vxlan;
        if ((select_1 & 0x0) == (0x0 & 0x0))goto accept;
        else goto reject;
    }
    parse_vxlan: {
/* extract(hdr.vxlan) */
        if ((u8*)ebpf_packetEnd < ebpf_headerStart + BYTES(64 + 0)) {
            ebpf_errorCode = PacketTooShort;
            goto reject;
        }

        hdr.vxlan.flags = (u8)((load_byte(ebpf_headerStart, BYTES(0))));
        hdr.vxlan.reserved = (u32)((load_word(ebpf_headerStart, BYTES(8)) >> 8) & EBPF_MASK(u32, 24));
        hdr.vxlan.vni = (u32)((load_word(ebpf_headerStart, BYTES(32)) >> 8) & EBPF_MASK(u32, 24));
        hdr.vxlan.reserved2 = (u8)((load_byte(ebpf_headerStart, BYTES(56))));

        hdr.vxlan.ebpf_valid = 1;
        ebpf_headerStart += BYTES(64);

;
/* extract(hdr.inner_ethernet) */
        if ((u8*)ebpf_packetEnd < ebpf_headerStart + BYTES(112 + 0)) {
            ebpf_errorCode = PacketTooShort;
            goto reject;
        }

        hdr.inner_ethernet.dstAddr = (u64)((load_dword(ebpf_headerStart, BYTES(0)) >> 16) & EBPF_MASK(u64, 48));
        hdr.inner_ethernet.srcAddr = (u64)((load_dword(ebpf_headerStart, BYTES(48)) >> 16) & EBPF_MASK(u64, 48));
        hdr.inner_ethernet.etherType = (u16)((load_half(ebpf_headerStart, BYTES(96))));

        hdr.inner_ethernet.ebpf_valid = 1;
        ebpf_headerStart += BYTES(112);

;
        u16 select_2;
        select_2 = hdr.inner_ethernet.etherType;
        if (select_2 == 0x800)goto parse_inner_ipv4;
        if ((select_2 & 0x0) == (0x0 & 0x0))goto accept;
        else goto reject;
    }
    start: {
/* extract(hdr.outer_ethernet) */
        if ((u8*)ebpf_packetEnd < ebpf_headerStart + BYTES(112 + 0)) {
            ebpf_errorCode = PacketTooShort;
            goto reject;
        }

        hdr.outer_ethernet.dstAddr = (u64)((load_dword(ebpf_headerStart, BYTES(0)) >> 16) & EBPF_MASK(u64, 48));
        hdr.outer_ethernet.srcAddr = (u64)((load_dword(ebpf_headerStart, BYTES(48)) >> 16) & EBPF_MASK(u64, 48));
        hdr.outer_ethernet.etherType = (u16)((load_half(ebpf_headerStart, BYTES(96))));

        hdr.outer_ethernet.ebpf_valid = 1;
        ebpf_headerStart += BYTES(112);

;
        u16 select_3;
        select_3 = hdr.outer_ethernet.etherType;
        if (select_3 == 0x800)goto parse_outer_ipv4;
        if ((select_3 & 0x0) == (0x0 & 0x0))goto accept;
        else goto reject;
    }

    reject: {
        return TC_ACT_SHOT;
    }

    accept:
    {
        u8 hit;
        {
if (/* hdr.vxlan.isValid() */
            hdr.vxlan.ebpf_valid && /* hdr.inner_ipv4.isValid() */
            hdr.inner_ipv4.ebpf_valid) {
/* vxlan_process_0.apply() */
                {
                    /* construct key */
                    struct MyIngress_vxlan_process_key key = {};
                    key.field0 = hdr.vxlan.vni;
                    key.field1 = hdr.inner_ipv4.dstAddr;
                    /* value */
                    struct MyIngress_vxlan_process_value *value = NULL;
                    /* perform lookup */
                    value = BPF_MAP_LOOKUP_ELEM(MyIngress_vxlan_process, &key);
                    if (value == NULL) {
                        /* miss; find default action */
                        hit = 0;
                        value = BPF_MAP_LOOKUP_ELEM(MyIngress_vxlan_process_defaultAction, &ebpf_zero);
                    } else {
                        hit = 1;
                    }
                    if (value != NULL) {
                        /* run action */
                        switch (value->action) {
                            case MYINGRESS_VXLAN_PROCESS_ACT_MYINGRESS_ALLOW: 
                                {
                                    accept = true;
                                }
                                break;
                            case MYINGRESS_VXLAN_PROCESS_ACT_MYINGRESS_DROP: 
                                {
                                    accept = false;
                                }
                                break;
                            case MYINGRESS_VXLAN_PROCESS_ACT_MYINGRESS_REWRITE_VNI: 
                                {
                                    hdr.vxlan.vni = value->u.MyIngress_rewrite_vni.new_vni;
                                                                        accept = true;
                                }
                                break;
                            default:
                                return TC_ACT_SHOT;
                        }
                    } else {
                        return TC_ACT_SHOT;
;
                    }
                }
;            }

            else {
                accept = true;            }

        }
    }
    ebpf_end:
    if (accept)
        return TC_ACT_OK;
    else
        return TC_ACT_SHOT;
}
char _license[] SEC("license") = "GPL";
