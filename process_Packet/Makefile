ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
CLANG ?= clang

INCLUDES += -I$(ROOT_DIR)../../p4c/backends/ebpf/runtime/ \
            -I/usr/include/ \
            -I/usr/src/linux-headers-$(shell uname -r)/tools/bpf/resolve_btfids/libbpf/include/ \
            -I/usr/include/x86_64-linux-gnu/


CFLAGS += -O2 -g -D__KERNEL__ -D__ASM_SYSREG_H \
          -Wno-unused-value -Wno-pointer-sign \
          -Wno-compare-distinct-pointer-types \
          -Wno-gnu-variable-sized-type-not-at-end \
          -Wno-address-of-packed-member -Wno-tautological-compare \
          -Wno-unknown-warning-option -Wnoparentheses-equality

# 指定输入的 C 文件和输出的 .o 文件
CFILE ?= 
BPFOBJ ?= 

all: verify_clang $(BPFOBJ)

verify_clang:
	@if ! which $(CLANG) > /dev/null 2>&1; then \
		echo "*** ERROR: Cannot find LLVM tool $(CLANG)"; \
		exit 1; \
	else \
		echo "CLANG compiler found: $(CLANG)"; \
	fi
	@if ! ($(CLANG) -target bpf -v > /dev/null 2>&1); then \
		echo "*** ERROR: LLVM ($(CLANG)) does not support 'bpf' target"; \
		echo "   NOTICE: LLVM version >= 3.7.1 required"; \
		exit 2; \
	else \
		echo "BPF target supported by CLANG"; \
	fi

$(BPFOBJ): $(CFILE)
	$(CLANG) $(CFLAGS) $(INCLUDES) -target bpf -c $< -o $@

clean:
	rm -f $(BPFOBJ)

.PHONY: all verify_clang clean
