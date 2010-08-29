klee-nush is an experimental fork of klee, forking from 106431 (on June 23).
this readme file is the original klee file. for more info go to http://klee.llvm.org/.
this is a test commit.

//===----------------------------------------------------------------------===//
// Klee Symbolic Virtual Machine
//===----------------------------------------------------------------------===//

klee is a symbolic virtual machine built on top of the LLVM compiler
infrastructure. Currently, there are two primary components:

  1. The core symbolic virtual machine engine; this is responsible for
     executing LLVM bitcode modules with support for symbolic
     values. This is comprised of the code in lib/.

  2. A POSIX/Linux emulation layer oriented towards supporting uClibc,
     with additional support for making parts of the operating system
     environment symbolic.

Additionally, there is a simple library for replaying computed inputs
on native code (for closed programs). There is also a more complicated
infrastructure for replaying the inputs generated for the POSIX/Linux
emulation layer, which handles running native programs in an
environment that matches a computed test input, including setting up
files, pipes, environment variables, and passing command line
arguments.

For further information, see the webpage or docs in www/.
