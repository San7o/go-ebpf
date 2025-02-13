.PHONY: alert run
all: alert
alert: alert.bpf.c main.go
	go generate
	go build

run: alert
	sudo ./alert
