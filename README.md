# QEMU for the DR-05 [![(build status)](https://travis-ci.org/dr05-homebrew/qemu.png?branch=bfin)](https://travis-ci.org/dr05-homebrew/qemu)

This repository is an attempt at emulating the DR-05 with [QEMU]. It would not be
possible without [Mike Frysinger's Blackfin port of QEMU][bfin-qemu].

[QEMU]: http://wiki.qemu.org/Main_Page
[bfin-qemu]: https://github.com/vapier/qemu


## Booting

1. Get ADSP-BF514-ROM-V02.dxe from the [VisualDSP++] software package.
2. Run `./patch-bootrom.rb ADSP-BF514-ROM-V02.dxe`
3. Run `./boot-dr05.sh -kernel ADSP-BF514-ROM-V02.dxe.patched`

Currently, the boot ROM will hang in `_bootrom.bootmenu`, because it detects BMODE=0.

[VisualDSP++]: http://www.analog.com/en/design-center/processors-and-dsp/evaluation-and-development-software/vdsp-bf-sh-ts.html
