#include "ebpf_model.p4"

typedef bit<48> macAddr_t;
typedef bit<32> ip4Addr_t;

header ethernet_t {
    macAddr_t dstAddr;
    macAddr_t srcAddr;
    bit<16>   etherType;
}

header ipv4_t {
    bit<4>    version;
    bit<4>    ihl;
    bit<8>    diffserv;
    bit<16>   totalLen;
    bit<16>   identification;
    bit<3>    flags;
    bit<13>   fragOffset;
    bit<8>    ttl;
    bit<8>    protocol;
    bit<16>   hdrChecksum;
    ip4Addr_t srcAddr;
    ip4Addr_t dstAddr;
}

struct headers {
    ethernet_t ethernet;
    ipv4_t     ipv4;
}

parser MyParser(packet_in packet, out headers hdr) {
    state start {
        packet.extract(hdr.ethernet);
        transition select(hdr.ethernet.etherType) {
            0x0800: parse_ipv4;
            default: accept;
        }
    }
    state parse_ipv4 {
        packet.extract(hdr.ipv4);
        transition accept;
    }
}

control MyFilter(inout headers hdr, out bool accept) {
    action count_packet(bit<32> index) {
        // 注意：这里我们不再直接增加计数器
        // 实际的计数逻辑将在生成的 C 代码中实现
        accept = true;
    }

    table ipv4_classifier {
        key = {
            hdr.ipv4.protocol: exact;
        }
        actions = {
            count_packet;
        }
        default_action = count_packet(255);
        implementation = hash_table(512);
    }

    apply {
        accept = true;
        if (hdr.ipv4.isValid()) {
            ipv4_classifier.apply();
        }
    }
}

ebpfFilter(MyParser(), MyFilter()) main;