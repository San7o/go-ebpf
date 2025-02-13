.PHONY: counter run
counter: counter.bpf.c main.go
	go generate
	go build

run: counter
	sudo ./counter
