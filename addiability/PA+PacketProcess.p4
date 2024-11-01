#include "ebpf_model.p4"

// Header definitions
header ethernet_t {
    bit<48> dstAddr;
    bit<48> srcAddr;
    bit<16> etherType;
}

header ipv4_t {
    bit<4>  version;
    bit<4>  ihl;
    bit<8>  diffserv;
    bit<16> totalLen;
    bit<16> identification;
    bit<3>  flags;
    bit<13> fragOffset;
    bit<8>  ttl;
    bit<8>  protocol;
    bit<16> hdrChecksum;
    bit<32> srcAddr;
    bit<32> dstAddr;
}

header tcp_t {
    bit<16> srcPort;
    bit<16> dstPort;
    bit<32> seqNo;
    bit<32> ackNo;
    bit<4>  dataOffset;
    bit<4>  res;
    bit<8>  flags;
    bit<16> window;
    bit<16> checksum;
    bit<16> urgentPtr;
}

struct headers {
    ethernet_t ethernet;
    ipv4_t ipv4;
    tcp_t tcp;
}

// Parser definition
parser MyParser(packet_in packet, out headers hdr) {
    state start {
        packet.extract(hdr.ethernet);
        transition select(hdr.ethernet.etherType) {
            0x0800: parse_ipv4;    // IPv4 packets
            default: accept;
        }
    }

    state parse_ipv4 {
        packet.extract(hdr.ipv4);
        transition select(hdr.ipv4.protocol) {
            6: parse_tcp;          // TCP protocol
            default: accept;
        }
    }

    state parse_tcp {
        packet.extract(hdr.tcp);
        transition accept;
    }
}

control MyFilter(inout headers hdr, out bool accept) {
    // Counter for TCP packets
    @name("tcp_counter")
    @type("percpu_array")
    @key_size(4)
    @value_size(4)
    @max_entries(1)
    CounterArray(32w1, false) tcp_counter;

    // Counter for HTTP packets
    @name("http_counter")
    @type("percpu_array")
    @key_size(4)
    @value_size(4)
    @max_entries(1)
    CounterArray(32w1, false) http_counter;

    apply {
        accept = false;

        // Basic packet validation
        if (!hdr.ethernet.isValid() || !hdr.ipv4.isValid()) {
            return;
        }

        // Check minimum IPv4 header length
        if (hdr.ipv4.ihl < 5) {
            return;
        }

        // Drop fragmented packets
        if (hdr.ipv4.fragOffset != 0 || (hdr.ipv4.flags & 0x1) != 0) {
            return;
        }

        // Fast path for HTTP traffic
        if (hdr.tcp.isValid() && (hdr.tcp.dstPort == 80 || hdr.tcp.dstPort == 9999)) {
            bit<8> syn_flag = (hdr.tcp.flags & 0x2);
            bit<8> rst_flag = (hdr.tcp.flags & 0x4);
            bit<8> ack_flag = (hdr.tcp.flags & 0x10);

            // Count HTTP packets
            http_counter.increment(0);

            // Accept valid HTTP connections
            if ((syn_flag != 0 && rst_flag == 0) || ack_flag != 0) {
                accept = true;
                return;
            }
        }

        // Handle TCP traffic
        if (hdr.tcp.isValid()) {
            // Count TCP packets
            tcp_counter.increment(0);
            
            // Basic TCP validation
            bit<8> syn_flag = (hdr.tcp.flags & 0x2);
            bit<8> rst_flag = (hdr.tcp.flags & 0x4);
            bit<8> ack_flag = (hdr.tcp.flags & 0x10);

            // Accept packets with valid TCP flags
            if ((syn_flag != 0 && rst_flag == 0) || ack_flag != 0) {
                accept = true;
                return;
            }
        }

        // Default accept for non-TCP traffic if TTL is valid
        if (hdr.ipv4.ttl > 0) {
            accept = true;
        }
    }
}

ebpfFilter(MyParser(), MyFilter()) main;