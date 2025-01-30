//go:build ignore

// libc
#include <sys/types.h>  // pid_t, mode_t

// bpf imports
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>    // BTF types
#include <bpf/bpf_tracing.h>    // BPF_PROG

// Defining maps like this utilized BTF type information
// enabling co-re. Defined in bpf/bpf_helpers.h
// - NOTE: Requires a recent enough kernel version
// types are deifned in linux/bpf.h or vmlinux.h
//
// A map has:
// - type
// - key type
// - value type
// - max entries
struct {
  __uint(type, BPF_MAP_TYPE_ARRAY);
  __type(key, __u32);
  __type(value, __u64);
  __uint(max_entries, 1);  // "array" with one __u64 element
} pkt_count SEC(".maps"); 

// Tracepoint eBPF
// tracepoint/<category>/<name>
// SEC macro just adds the function to the specified section
SEC("tracepoint/syscalls/sys_enter_openat2")
// BPF_PROG allows user to the clas a list of named and typed input aruments without
// writing manual casts. Defined in bpf/bpf_tracing.h
int BPF_PROG(sys_enter_open, char* filename, int flags, mode_t mode)
{
        __u32 key     = 0;
	__u64 initval = 1, *valp;

	// bpf_map_ functions defined in kernel/bpf.h
	// int bpf_map_lookup_elem(int fd, const void *key, void *value);
	valp = bpf_map_lookup_elem(&pkt_count, &key);
	if (!valp) {
                //int bpf_map_update_elem(int fd, const void *key, const void *value,
	        //			   __u64 flags);
	        // BPF_ANY: Create new elementes or update existing
		bpf_map_update_elem(&pkt_count, &key, &initval, BPF_ANY);
		return 0;
	}
	__sync_fetch_and_add(valp, 1);

	return 0;
}

char LICENSE[] SEC("license") = "Dual MIT/GPL"; 
