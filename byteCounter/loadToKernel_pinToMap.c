#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//gcc loadToKernel_pinToMap.c -o loadToKernel_pinToMap -I./../p4c/backends/ebpf/runtime/ -I/usr/include/ -I/usr/src/linux-headers-$(uname -r)/tools/bpf/resolve_btfids/libbpf/include/ -I/usr/include/x86_64-linux-gnu/ -lbpf

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <bpf_program_obj> <map_name>\n", argv[0]);
        return 1;
    }
    
    const char *bpf_object_file = argv[1];
    const char *map_name = argv[2];
    char pin_path[256];
    struct bpf_object *obj;
    int map_fd;

    obj = bpf_object__open_file(bpf_object_file, NULL);
    if (!obj) {
        fprintf(stderr, "Failed to open BPF object\n");
        return 1;
    }

    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF object\n");
        bpf_object__close(obj);
        return 1;
    }

    map_fd = bpf_object__find_map_fd_by_name(obj, map_name);
    if (map_fd < 0) {
        fprintf(stderr, "Failed to find map\n");
        bpf_object__close(obj);
        return 1;
    }

    // 构造 pin 路径
    snprintf(pin_path, sizeof(pin_path), "/sys/fs/bpf/tc/globals/%s", map_name);

    // Pin map
    if (bpf_obj_pin(map_fd, pin_path) < 0) {
        fprintf(stderr, "Failed to pin map to: %s\n", pin_path);
        bpf_object__close(obj);
        return 1;
    }

    printf("Successfully pinned map '%s' to '%s'\n", map_name, pin_path);
    
    bpf_object__close(obj);

    return 0;
}