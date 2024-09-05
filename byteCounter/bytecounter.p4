#include "ebpf_model.p4"

// Define headers
header ethernet_t {
    bit<48> dstAddr;
    bit<48> srcAddr;
    bit<16> etherType;
}

header ipv4_t {
    bit<4> version;
    bit<4> ihl;
    bit<8> diffserv;
    bit<16> totalLen;
    bit<16> identification;
    bit<3> flags;
    bit<13> fragOffset;
    bit<8> ttl;
    bit<8> protocol;
    bit<16> hdrChecksum;
    bit<32> srcAddr;
    bit<32> dstAddr;
}

struct headers {
    ethernet_t ethernet;
    ipv4_t ipv4;
}

// Define the parser
parser MyParser(packet_in pkt, out headers hdr) {
    state start {
        pkt.extract(hdr.ethernet);
        transition select(hdr.ethernet.etherType) {
            0x0800: parse_ipv4;
            default: accept;
        }
    }

    state parse_ipv4 {
        pkt.extract(hdr.ipv4);
        transition accept;
    }
}

// Define the filter control block
control MyFilter(inout headers hdr, out bool accept) {
    @name("byte_counter") 
    @type("percpu_array") 
    @key_size(4) 
    @value_size(4) 
    @max_entries(1)
    // Define the counter array as per the model
    CounterArray(32w1, false) byte_counter;
    //bit<32> byteCount;

    apply {
        //byteCount = 0;
        if (hdr.ipv4.isValid()) {
            // Cast totalLen to bit<32> and add to counter
            byte_counter.add(32w0, (bit<32>)hdr.ipv4.totalLen);
            // Update byteCount
            //byteCount = byteCount + (bit<32>)hdr.ipv4.totalLen;
        }
        // Set accept to true
        accept = true;
    }
}

// Define the package
//package ebpfFilter(MyParser(), MyFilter()) main;
// Corrected package declaration
ebpfFilter(
    MyParser(),
    MyFilter()
) main;
