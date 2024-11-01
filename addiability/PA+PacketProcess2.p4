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
    @name("tcp_counter")
    @type("percpu_array")
    @key_size(4)
    @value_size(4)
    @max_entries(1)
    CounterArray(32w1, false) tcp_counter;

    @name("http_counter")
    @type("percpu_array")
    @key_size(4)
    @value_size(4)
    @max_entries(1)
    CounterArray(32w1, false) http_counter;

    apply {
        accept = false;
        
        // 基本验证保持不变
        if (!hdr.ethernet.isValid() || !hdr.ipv4.isValid()) {
            return;
        }

        // 增加更复杂的计算
        bit<32> complex_value = (bit<32>)hdr.ipv4.ttl;
        complex_value = complex_value + (bit<32>)hdr.ipv4.protocol;
        complex_value = complex_value * (bit<32>)hdr.ipv4.ihl;
        complex_value = complex_value + (bit<32>)hdr.ipv4.version;
        
        // 展开的TTL操作（保持但增加次数）
        if (complex_value < 1000) {
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl + 1;
            hdr.ipv4.ttl = hdr.ipv4.ttl - 1;
            // ... (重复20次)
        }

        if (hdr.ipv4.ihl < 5) {
            return;
        }

        if (hdr.ipv4.fragOffset != 0 || (hdr.ipv4.flags & 0x1) != 0) {
            return;
        }

        // HTTP处理部分
        if (hdr.tcp.isValid() && (hdr.tcp.dstPort == 80 || hdr.tcp.dstPort == 9999)) {
            bit<8> syn_flag = (hdr.tcp.flags & 0x2);
            bit<8> rst_flag = (hdr.tcp.flags & 0x4);
            bit<8> ack_flag = (hdr.tcp.flags & 0x10);

            // 增加复杂的条件判断和计算
            bit<32> http_calc = (bit<32>)hdr.tcp.window;
            http_calc = http_calc + (bit<32>)hdr.tcp.urgentPtr;
            http_calc = http_calc * (bit<32>)hdr.tcp.dataOffset;
            http_calc = http_calc + (bit<32>)hdr.tcp.res;
            
            if (http_calc < 65000) {
                // 增加计算密度
                bit<16> temp_id = hdr.ipv4.identification;
                bit<16> temp_checksum = hdr.ipv4.hdrChecksum;
                
                // 执行多次计算
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                temp_id = temp_id + temp_checksum;
                temp_checksum = temp_checksum + temp_id;
                // ... (重复多次)
                
                hdr.ipv4.identification = temp_id;
                hdr.ipv4.hdrChecksum = temp_checksum;
            }

            http_counter.increment(0);

            if ((syn_flag != 0 && rst_flag == 0) || ack_flag != 0) {
                accept = true;
                return;
            }
        }

        // TCP处理部分
        if (hdr.tcp.isValid()) {
            // 增加TCP相关的复杂计算
            bit<32> tcp_calc = (bit<32>)hdr.tcp.seqNo;
            tcp_calc = tcp_calc + (bit<32>)hdr.tcp.ackNo;
            tcp_calc = tcp_calc * (bit<32>)hdr.tcp.window;
            
            if (tcp_calc < 1000000) {
                bit<16> temp_window = hdr.tcp.window;
                bit<16> temp_checksum = hdr.tcp.checksum;
                
                // 多次计算操作
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                temp_window = temp_window + temp_checksum;
                temp_checksum = temp_checksum + temp_window;
                // ... (重复多次)
                
                hdr.tcp.window = temp_window;
                hdr.tcp.checksum = temp_checksum;
            }

            tcp_counter.increment(0);
           
            bit<8> syn_flag = (hdr.tcp.flags & 0x2);
            bit<8> rst_flag = (hdr.tcp.flags & 0x4);
            bit<8> ack_flag = (hdr.tcp.flags & 0x10);

            if ((syn_flag != 0 && rst_flag == 0) || ack_flag != 0) {
                accept = true;
                return;
            }
        }

        if (hdr.ipv4.ttl > 0) {
            accept = true;
        }
    }
}

ebpfFilter(MyParser(), MyFilter()) main;