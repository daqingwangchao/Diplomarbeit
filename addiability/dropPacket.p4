#include <core.p4>
#include <ebpf_model.p4>

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
    ethernet_t   ethernet;
    ipv4_t       ipv4;
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

control MyIngress(inout headers hdr, out bool pass) {
    action drop() {
        pass = false;
    }

    action allow() {
        pass = true;
    }

    apply {
        pass = true;  // Default to allowing packets
        if (hdr.ipv4.isValid()) {
            bit<8> drop_bit = (bit<8>)hdr.ipv4.identification & 0x0F;
            if (drop_bit == 0) {
                drop();
            }
        }
    }
}

ebpfFilter(MyParser(), MyIngress()) main;