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
    @name("byte_counter")
    @type("percpu_array")
    @key_size(32)
    @value_size(32)
    @max_entries(256)
    CounterArray(32w256, false) byte_counter;

    action drop() {
        accept = false;
    }

    action allow() {
        accept = true;
    }

    action count_bytes(bit<32> index) {
        bit<32> byte_count = (bit<32>)hdr.ipv4.totalLen;
        byte_counter.add(index, byte_count);
        accept = true;
    }

    table ipv4_filter {
        key = {
            hdr.ipv4.srcAddr: lpm;
            hdr.ipv4.dstAddr: exact;
            hdr.ipv4.protocol: ternary;
        }
        actions = {
            allow;
            drop;
            count_bytes;
        }
        default_action = allow();
    }

    apply {
        if (hdr.ipv4.isValid()) {
            ipv4_filter.apply();
            if (accept) {
                if (hdr.ipv4.ttl > 1) {
                    hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
                } else {
                    drop();
                }
            }
        } else {
            accept = true;
        }
    }
}

ebpfFilter(MyParser(), MyFilter()) main;