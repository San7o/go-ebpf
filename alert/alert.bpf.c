//go:build ignore

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

SEC("kprobe/do_unlinkat")
int kprobe_do_unlinkat(struct pt_regs *ctx)
{
    pid_t pid;
    const char *filename;

    int dfd = (int) ctx->di;
    struct filename* name = (struct filename *) ctx->si;

    pid = bpf_get_current_pid_tgid() >> 32;
    filename = BPF_CORE_READ(name, name);
    bpf_printk("KPROBE ENTRY pid = %d, filename = %s\n", pid, filename);

    return 0;
}

char LICENSE[] SEC("license") = "Dual MIT/GPL";
