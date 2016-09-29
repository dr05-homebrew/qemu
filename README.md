# QEMU for the DR-05 [![(build status)](https://travis-ci.org/dr05-homebrew/qemu.png?branch=bfin)](https://travis-ci.org/dr05-homebrew/qemu)

This repository is an attempt at emulating the DR-05 with [QEMU]. It would not be
possible without [Mike Frysinger's Blackfin port of QEMU][bfin-qemu].

[QEMU]: http://wiki.qemu.org/Main_Page
[bfin-qemu]: https://github.com/vapier/qemu


## Booting

1. Get ADSP-BF514-ROM-V02.dxe from the [VisualDSP++] software package.
2. Run `./patch-bootrom.rb ADSP-BF514-ROM-V02.dxe`
3. Get a raw flash image of the firmware (with [fwunpack.py]). Convert it into
   a QCOW2 image with the following commands:
    - `qemu-img convert -f raw -O qcow2 DR-05_44.211.flash firmware.qcow2`
    - `qemu-img resize firmware.qcow2 2M`
4. Run `./boot-dr05.sh -kernel ADSP-BF514-ROM-V02.dxe.patched`; Additional
   arguments:
    - Use `-d in_asm,exec,nochain,trace:bfin_reg_memory_* -singlestep` to trace
      the executed instructions

Currently, the boot ROM will hang in `_bootrom.bootkernel.dmawait.idle`,
waiting for the first DMA transfer to finish.

[VisualDSP++]: http://www.analog.com/en/design-center/processors-and-dsp/evaluation-and-development-software/vdsp-bf-sh-ts.html
[fwunpack.py]: https://github.com/dr05-homebrew/dr05-homebrew/blob/master/fwupdate/fwunpack.py
