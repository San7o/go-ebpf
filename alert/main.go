package main

import (
    "log"

    "github.com/cilium/ebpf/link"
    "github.com/cilium/ebpf/rlimit"
)

// To interact with the BPF program, we can (and should) generate
// some go bindings using the following command:
//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -tags linux bpf alert.bpf.c

// userspace program to register the BPF program and read Its output
func main() {

    // Remove resource limits for kernels <5.11.
    if err := rlimit.RemoveMemlock(); err != nil {
        log.Fatal("Removing memlock:", err)
    }

    // Load the compiled eBPF ELF and load it into the kernel.
    objs := bpfObjects{}
    if err := loadBpfObjects(&objs, nil); err != nil {
        log.Fatal("Loading eBPF objects:", err)
    }
    defer objs.Close()

    // Tracepoint attaches the given eBPF program to the tracepoint with the given
    // group and name.
    kp, err := link.Kprobe("do_unlinkat", objs.KprobeDoUnlinkat, nil)
    if err != nil {
	log.Fatalf("opening tracepoint: %s", err)
    }
    defer kp.Close()

    for {}
}
