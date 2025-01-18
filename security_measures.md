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
6. ### Data execution prevention
    modern Linux distributions typically make the stack non-executable by default.   
    _Kernel Support_: The Linux kernel has supported non-executable stack protection for many years. This is often enabled by default in the kernel configuration of most 
    distributions.  
    _Executable and Linkable Format (ELF)_: When a program is compiled, the ELF headers can specify that the stack should be non-executable. This is often the default 
    behavior unless explicitly overridden.  The "-z noexecstack" linker option modifies the ELF headers to include a note that the stack should be non-executable.  
    _Hardware Support_: Modern CPUs include a feature known as the NX (No Execute) bit, which allows the operating system to mark certain memory regions as non-executable. The Linux kernel uses this feature to enforce non-executable stack policies.
7. ### Kernel Address Display Restriction
When attackers try to develop "run anywhere" exploits for kernel vulnerabilities, they frequently need to know the location of internal kernel structures. By treating kernel addresses as sensitive information, those locations are not visible to regular local users. Starting with Ubuntu 11.04, /proc/sys/kernel/kptr_restrict is set to "1" to block the reporting of known kernel address leaks. Additionally, various files and directories were made readable only by the root user: /boot/vmlinuz*, /boot/System.map*, /sys/kernel/debug/, /proc/slabinfo
## References:
https://wiki.ubuntu.com/Security/Features/Historical
