# eBPF in go

This repository contains a simple example of using eBPF programs
to log the access of certain files and create honeypots for
malicious actors.

The code is intentionally highly-commented to provide an explanation
of the examples and their usage.

## Dependencies

To run any eBPF program you will need:
- Linux kernel version 5.7 or later, for bpf_link support
- LLVM 11 or later 1 (clang and llvm-strip)
- libbpf headers
- Linux kernel headers
- Go compiler version supported by ebpf-go's Go module

## Commands

This repository contains two sample programs: counter and alert.
```bash
make counter
make alert
```

To test the application, you can compile a test program located
in `test` that will call the traced funcion:

```
gcc test/open.c -o test
./test
```

Bpf printk output can be viewed in `/sys/kernel/debug/tracing/trace_pipe`.

---

# Hooks

To know where to hook your eBPF program, you can use the following
commands

## List perf events

```bash
perf list
```

## Tracepoints

To see what tracepoints (tp) you can use, first you need to have
tracefs mounted, then you can see what directories are enabled:
```
sudo ls /sys/kernel/tracing/events/<system>/<event>/
```

You can also list `format/` to know the function arguments.
