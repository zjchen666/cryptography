## Linux Kernel Hardening
## User Space Hardening
1. ### The stack smash protection
   a. -fstack-protector-strong  
   b. -fstack-protector-all  
2. ### dynamic lib link options:
   a.  add linker option -Wl, -z relro -Wl, -z now
3. ### The Address Space Layout Randomization (ASLR)
   kernel configuraton /proc/sys/kernel/randomize_va_space to 2  
   program should be built as Position Independent Code (PIC) with "-fPIE -pie".  
   _-fPIE_ is used during the compilation phase to generate position-independent code in object files.  
   _-pie_ is used during the linking phase to create a position-independent executable from those object files.  
   _-fPIC_ is used to compile shared libraries (dynamic libraries) so that they can be loaded at any memory address.
4. ### No debug flag (-g for GCC) when building software components
5. ### static buffer checking
     use -D_FORTIFY_SOURCE=2 for GCC.
