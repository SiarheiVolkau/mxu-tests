This repo contains tests for Ingenic MXU version 1 release 2 - MIPS SIMD extension.
It consists of 114 instructions aimed to speedup multimedia applications.
These tests aimed to verify emulation implementation of the MXU ASE in the QEMU project.

-------------------------------

### Building tests

#### Building toolchain
In order to build these tests you have to use GCC toolchain modified to support MXU
ASE as well. The patches for GCC 12.2.0 and binutils 2.38 are provided in this repo,
see `patches/` subfolder.

As of me, I use OpenDingux buildroot to build full toolchain as a part of build process.
Full toolchain provides sysroot, which is used to run tests in QEMU.
In order to build OpenDingux you have to [clone it from there](https:://github.com/OpenDingux/buildroot)
and copy patches to respective folders in `buildroot/board/opendingux/patches/gcc` and
`buildroot/board/opendingux/patches/binutils`.
**IMPORTANT NOTE: remove existing patches in these folders as they inflict problems.**

Then build RS90 variant by running ``CONFIG=rs90 ./rebuild.sh``
RS90 CPU - jz4725b supports MIPS32R1 + MXU1 release 2 a.k.a. `XBurstR1` alias in QEMU.
The toolchain can be found in `buildroot/output/rs90/images/rs90-toolchain-<date>.tar.xz` on successful build. Unpack it somewhere and launch `./relocate-sdk.sh` script.
Now the toolchain is ready to use.

Additionally you can build GCW0 toolchain variant by running ``CONFIG=gcw0 ./rebuild.sh``
GCW0 CPU - jz4770 supports MIPS32R2 + MXU1 release 2 a.k.a. `XBurstR2` alias in QEMU.

#### Building tests

In order to build `testapp` application you have to provide `CROSS_COMPILE` option to `make` utility, e.g. ``make CROSS_COMPILE=/path/to/rs90-toolchain/bin/mipsel-linux-``
as a result of build `testapp` binary will appear.

-------------------------------

### Launching tests

#### Launching tests on a hardware device

Only OpenDingux device is reviewed here as it was the base for running tests.
However, it might be run on any Ingenic MXU capable hardware with respect to
the toolchain compatibility.
To launch `testapp` on hardware device, copy it by any way to the device,
it can be done easily by using SSH e.g. ``scp -O testapp od@10.1.1.3:~``
when the device configured it's USB interface in the network mode and password
for user is set (empty password also ok).
Then you can simply run copied binary ``./testapp``

#### Launching tests in QEMU
  
For launching `testapp` in QEMU you have to use OpenDingux toolchain as it have
suitable linux sysroot. The simple command is like:
```qemu-mipsel -cpu XBurstR1 -L /path/to/rs90-toolchain/mipsel-rs90-linux-musl/sysroot/ ./testapp```  

for GCW0 the command is like:
```qemu-mipsel -cpu XBurstR2 -L /path/to/gcw0-toolchain/mipsel-gcw0-linux-uclibc/sysroot/ ./testapp```
