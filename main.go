package main

import (
    "log"
    "time"

    "github.com/cilium/ebpf/link"
    "github.com/cilium/ebpf/rlimit"
)

// To interact with the BPF program, we can (and should) generate
// some go bindings using the following command:
//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -tags linux bpf ebpf/c/counter.bpf.c

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
    kp, err := link.Tracepoint("syscalls", "sys_enter_openat2", objs.SysEnterOpen, nil)
    if err != nil {
	log.Fatalf("opening tracepoint: %s", err)
    }
    defer kp.Close()

    // Read loop reporting the total amount of times the kernel
    // function was entered, once per second.
    ticker := time.NewTicker(1 * time.Second)
    defer ticker.Stop()
    const mapKey uint32 = 0
    var value uint64
    var old_value uint64 = 0
    log.Println("Waiting for events..")
    for range ticker.C {
	if err := objs.PktCount.Lookup(mapKey, &value); err != nil {
	    log.Fatalf("reading map: %v", err)
	}
	if old_value != value {
            log.Printf("%v times", value)
	    old_value = value;
	}
    }
}
