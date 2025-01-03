/* Automatically generated by p4c-ebpf from PA+PacketProcess2.p4 on Fri Oct 25 11:34:34 2024
 */
#include "PA+PacketProcess2.h"
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
//REGISTER_TABLE(MyFilter_http_counter, BPF_MAP_TYPE_ARRAY, u32, MyFilter_http_counter_value, 1)
//BPF_ANNOTATE_KV_PAIR(MyFilter_http_counter, u32, MyFilter_http_counter_value)
//REGISTER_TABLE(MyFilter_tcp_counter, BPF_MAP_TYPE_ARRAY, u32, MyFilter_tcp_counter_value, 1)
//BPF_ANNOTATE_KV_PAIR(MyFilter_tcp_counter, u32, MyFilter_tcp_counter_value)
//REGISTER_END()

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, u32);
    __type(value, MyFilter_http_counter_value);
    __uint(max_entries, 1);
}MyFilter_http_counter SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, u32);
    __type(value, MyFilter_tcp_counter_value);
    __uint(max_entries, 1);
}MyFilter_tcp_counter SEC(".maps");

SEC("prog")
int ebpf_filter(SK_BUFF *skb){
    struct headers hdr = {
        .ethernet = {
            .ebpf_valid = 0
        },
        .ipv4 = {
            .ebpf_valid = 0
        },
        .tcp = {
            .ebpf_valid = 0
        },
    };
    enum ebpf_errorCodes ebpf_errorCode = NoError;
    void* ebpf_packetStart = ((void*)(long)skb->data);
    u8* ebpf_headerStart = ebpf_packetStart;
    void* ebpf_packetEnd = ((void*)(long)skb->data_end);
    u8 accept = 0;
    u32 ebpf_zero = 0;
    unsigned char ebpf_byte;
    u32 ebpf_pkt_len = skb->len;

    goto start;
    parse_ipv4: {
/* extract(hdr.ipv4) */
        if ((u8*)ebpf_packetEnd < ebpf_headerStart + BYTES(160 + 0)) {
            ebpf_errorCode = PacketTooShort;
            goto reject;
        }

        hdr.ipv4.version = (u8)((load_byte(ebpf_headerStart, BYTES(0)) >> 4) & EBPF_MASK(u8, 4));
        hdr.ipv4.ihl = (u8)((load_byte(ebpf_headerStart, BYTES(4))) & EBPF_MASK(u8, 4));
        hdr.ipv4.diffserv = (u8)((load_byte(ebpf_headerStart, BYTES(8))));
        hdr.ipv4.totalLen = (u16)((load_half(ebpf_headerStart, BYTES(16))));
        hdr.ipv4.identification = (u16)((load_half(ebpf_headerStart, BYTES(32))));
        hdr.ipv4.flags = (u8)((load_byte(ebpf_headerStart, BYTES(48)) >> 5) & EBPF_MASK(u8, 3));
        hdr.ipv4.fragOffset = (u16)((load_half(ebpf_headerStart, BYTES(51))) & EBPF_MASK(u16, 13));
        hdr.ipv4.ttl = (u8)((load_byte(ebpf_headerStart, BYTES(64))));
        hdr.ipv4.protocol = (u8)((load_byte(ebpf_headerStart, BYTES(72))));
        hdr.ipv4.hdrChecksum = (u16)((load_half(ebpf_headerStart, BYTES(80))));
        hdr.ipv4.srcAddr = (u32)((load_word(ebpf_headerStart, BYTES(96))));
        hdr.ipv4.dstAddr = (u32)((load_word(ebpf_headerStart, BYTES(128))));

        hdr.ipv4.ebpf_valid = 1;
        ebpf_headerStart += BYTES(160);

;
        u8 select_0;
        select_0 = hdr.ipv4.protocol;
        if (select_0 == 6)goto parse_tcp;
        if ((select_0 & 0x0) == (0x0 & 0x0))goto accept;
        else goto reject;
    }
    parse_tcp: {
/* extract(hdr.tcp) */
        if ((u8*)ebpf_packetEnd < ebpf_headerStart + BYTES(160 + 0)) {
            ebpf_errorCode = PacketTooShort;
            goto reject;
        }

        hdr.tcp.srcPort = (u16)((load_half(ebpf_headerStart, BYTES(0))));
        hdr.tcp.dstPort = (u16)((load_half(ebpf_headerStart, BYTES(16))));
        hdr.tcp.seqNo = (u32)((load_word(ebpf_headerStart, BYTES(32))));
        hdr.tcp.ackNo = (u32)((load_word(ebpf_headerStart, BYTES(64))));
        hdr.tcp.dataOffset = (u8)((load_byte(ebpf_headerStart, BYTES(96)) >> 4) & EBPF_MASK(u8, 4));
        hdr.tcp.res = (u8)((load_byte(ebpf_headerStart, BYTES(100))) & EBPF_MASK(u8, 4));
        hdr.tcp.flags = (u8)((load_byte(ebpf_headerStart, BYTES(104))));
        hdr.tcp.window = (u16)((load_half(ebpf_headerStart, BYTES(112))));
        hdr.tcp.checksum = (u16)((load_half(ebpf_headerStart, BYTES(128))));
        hdr.tcp.urgentPtr = (u16)((load_half(ebpf_headerStart, BYTES(144))));

        hdr.tcp.ebpf_valid = 1;
        ebpf_headerStart += BYTES(160);

;
         goto accept;
    }
    start: {
/* extract(hdr.ethernet) */
        if ((u8*)ebpf_packetEnd < ebpf_headerStart + BYTES(112 + 0)) {
            ebpf_errorCode = PacketTooShort;
            goto reject;
        }

        hdr.ethernet.dstAddr = (u64)((load_dword(ebpf_headerStart, BYTES(0)) >> 16) & EBPF_MASK(u64, 48));
        hdr.ethernet.srcAddr = (u64)((load_dword(ebpf_headerStart, BYTES(48)) >> 16) & EBPF_MASK(u64, 48));
        hdr.ethernet.etherType = (u16)((load_half(ebpf_headerStart, BYTES(96))));

        hdr.ethernet.ebpf_valid = 1;
        ebpf_headerStart += BYTES(112);

;
        u16 select_1;
        select_1 = hdr.ethernet.etherType;
        if (select_1 == 0x800)goto parse_ipv4;
        if ((select_1 & 0x0) == (0x0 & 0x0))goto accept;
        else goto reject;
    }

    reject: {
        return TC_ACT_SHOT;
    }

    accept:
    {
        u8 hit;
        u32 complex_value_0 = 0;
        u8 syn_flag_0 = 0;
        u8 rst_flag_0 = 0;
        u8 ack_flag_0 = 0;
        u32 http_calc_0 = 0;
        u16 temp_id_0 = 0;
        u16 temp_checksum_0 = 0;
        u32 tcp_calc_0 = 0;
        u16 temp_window_0 = 0;
        u16 temp_checksum_1 = 0;
        u8 syn_flag_1 = 0;
        u8 rst_flag_1 = 0;
        u8 ack_flag_1 = 0;
        u8 hasReturned;
        {
            hasReturned = false;
                        accept = false;
            if (!/* hdr.ethernet.isValid() */
            hdr.ethernet.ebpf_valid || !/* hdr.ipv4.isValid() */
            hdr.ipv4.ebpf_valid) {
                hasReturned = true;            }

            if (hasReturned) {
;            }

            else {
                complex_value_0 = (u32)hdr.ipv4.ttl;
                                complex_value_0 = (complex_value_0 + (u32)hdr.ipv4.protocol);
                                complex_value_0 = (complex_value_0 * (u32)hdr.ipv4.ihl);
                                complex_value_0 = (complex_value_0 + (u32)hdr.ipv4.version);
                if (complex_value_0 < 1000) {
                    hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 1);
                                        hdr.ipv4.ttl = (hdr.ipv4.ttl + 255);
                }
                if (hdr.ipv4.ihl < 5) {
                    hasReturned = true;                }

            }
            if (hasReturned) {
;            }

            else {
if (hdr.ipv4.fragOffset != 0 || ((hdr.ipv4.flags & 0x1) & ((1 << 3) - 1)) != 0) {
                    hasReturned = true;                }
            }

            if (hasReturned) {
;            }

            else {
if (/* hdr.tcp.isValid() */
                hdr.tcp.ebpf_valid && (hdr.tcp.dstPort == 80 || hdr.tcp.dstPort == 9999)) {
                    syn_flag_0 = (hdr.tcp.flags & 0x2);
                                        rst_flag_0 = (hdr.tcp.flags & 0x4);
                                        ack_flag_0 = (hdr.tcp.flags & 0x10);
                                        http_calc_0 = (u32)hdr.tcp.window;
                                        http_calc_0 = (http_calc_0 + (u32)hdr.tcp.urgentPtr);
                                        http_calc_0 = (http_calc_0 * (u32)hdr.tcp.dataOffset);
                                        http_calc_0 = (http_calc_0 + (u32)hdr.tcp.res);
                    if (http_calc_0 < 65000) {
                        temp_id_0 = hdr.ipv4.identification;
                                                temp_checksum_0 = hdr.ipv4.hdrChecksum;
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                temp_id_0 = (temp_id_0 + temp_checksum_0);
                                                temp_checksum_0 = (temp_checksum_0 + temp_id_0);
                                                hdr.ipv4.identification = temp_id_0;
                                                hdr.ipv4.hdrChecksum = temp_checksum_0;
                    }
                    /* http_counter_0.increment(0) */
{
                        MyFilter_http_counter_value *value_0;
                        MyFilter_http_counter_value init_val = 1;
                        MyFilter_http_counter_key key_0 = 0;
                        value_0 = BPF_MAP_LOOKUP_ELEM(MyFilter_http_counter, &key_0);
                        if (value_0 != NULL)
                            __sync_fetch_and_add(value_0, 1);
                        else
                            BPF_MAP_UPDATE_ELEM(MyFilter_http_counter, &key_0, &init_val, BPF_ANY);
                    }
;
                    if ((syn_flag_0 != 0 && rst_flag_0 == 0) || ack_flag_0 != 0) {
                        accept = true;
                                                hasReturned = true;
                    }
                }            }

            if (hasReturned) {
;            }

            else {
if (/* hdr.tcp.isValid() */
                hdr.tcp.ebpf_valid) {
                    tcp_calc_0 = hdr.tcp.seqNo;
                                        tcp_calc_0 = (tcp_calc_0 + hdr.tcp.ackNo);
                                        tcp_calc_0 = (tcp_calc_0 * (u32)hdr.tcp.window);
                    if (tcp_calc_0 < 1000000) {
                        temp_window_0 = hdr.tcp.window;
                                                temp_checksum_1 = hdr.tcp.checksum;
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                temp_window_0 = (temp_window_0 + temp_checksum_1);
                                                temp_checksum_1 = (temp_checksum_1 + temp_window_0);
                                                hdr.tcp.window = temp_window_0;
                                                hdr.tcp.checksum = temp_checksum_1;
                    }
                    /* tcp_counter_0.increment(0) */
{
                        MyFilter_tcp_counter_value *value_1;
                        MyFilter_tcp_counter_value init_val = 1;
                        MyFilter_tcp_counter_key key_1 = 0;
                        value_1 = BPF_MAP_LOOKUP_ELEM(MyFilter_tcp_counter, &key_1);
                        if (value_1 != NULL)
                            __sync_fetch_and_add(value_1, 1);
                        else
                            BPF_MAP_UPDATE_ELEM(MyFilter_tcp_counter, &key_1, &init_val, BPF_ANY);
                    }
;
                                        syn_flag_1 = (hdr.tcp.flags & 0x2);
                                        rst_flag_1 = (hdr.tcp.flags & 0x4);
                                        ack_flag_1 = (hdr.tcp.flags & 0x10);
                    if ((syn_flag_1 != 0 && rst_flag_1 == 0) || ack_flag_1 != 0) {
                        accept = true;
                                                hasReturned = true;
                    }
                }            }

            if (hasReturned) {
;            }

            else {
if (hdr.ipv4.ttl > 0) {
                    accept = true;                }
            }

        }
    }
    ebpf_end:
    if (accept)
        return TC_ACT_OK;
    else
        return TC_ACT_SHOT;
}
char _license[] SEC("license") = "GPL";
