#!/bin/sh

$(dirname "$0")/bfin-softmmu/qemu-system-bfin \
	-nographic -monitor none \
	-M bf514-dr05 \
	"$@"
