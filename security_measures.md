## Linux Kernel Hardening
1. ### Kernel Module loading
       kernel module loding must be disabled at runtime or must be authenticated.  
The CONFIG_MODULE_SIG and CONFIG_MODULE_SIG_FORCE are kernel configuration options related to module signing in the Linux kernel. They serve different purposes in the context of module signing:  
CONFIG_MODULE_SIG  
Purpose: This option enables the support for module signature verification in the kernel.  
Functionality: When CONFIG_MODULE_SIG is enabled, the kernel is capable of verifying the signatures of kernel modules before loading them. This means that the kernel can check if a module has been signed with a valid key.  
Flexibility: With this option enabled, unsigned modules can still be loaded if the signature verification fails, unless CONFIG_MODULE_SIG_FORCE is also enabled. This provides flexibility during development or in environments where not all modules are signed.  
CONFIG_MODULE_SIG_FORCE  
Purpose: This option enforces strict signature verification for all kernel modules.  
Functionality: When CONFIG_MODULE_SIG_FORCE is enabled, the kernel will refuse to load any module that is not properly signed. This means that only modules with a valid signature will be accepted, and any unsigned or improperly signed modules will be rejected.  
Security: This option is used to enhance security by ensuring that only trusted modules are loaded into the kernel. It is particularly useful in production environments where security is a priority.  
Summary  
CONFIG_MODULE_SIG: Enables the capability to verify module signatures. It allows for both signed and unsigned modules to be loaded, depending on the verification result.  
CONFIG_MODULE_SIG_FORCE: Enforces that only signed modules can be loaded, providing a higher level of security by rejecting any unsigned or improperly signed modules.  
In practice, if you want to ensure that only trusted modules are loaded into your kernel, you would enable both CONFIG_MODULE_SIG and CONFIG_MODULE_SIG_FORCE. This setup is particularly important in secure or production environments where the integrity and authenticity of kernel modules are critical.  

3. ### Kernel debug trace should be disabled
4. ### Kernel Module Versioning
     Rationale? Kernel Module Versioning helps prevent the loading of modules that were compiled for a different kernel version or configuration. This is important because kernel modules interact closely with the kernel, and mismatches can lead to system instability or crashes.  
   
5. ### Kernel stack smash protection
    is not enabled by default
6. ### Disable Application Binary Interface compatibility modes
7. ### Disable /dev/mem and /dev/kmem
    Rationale?
8. ### debugfs and configfs Must be disabled
    Rationale?
9. ### The static and dynamic object size bounds checking
10. ### Kernel must never access user space memory
11. ### Enable control flow integrity
12. ### enable KASLR
13. ### The kernel executable code and read-only data must not be writable.
     enable Kconfig options: CONFIG_STRICT_KERNEL_RWX and CONFIG_STRICT_MODULE_RWX
14. ### Should Not use NFS  
15. ### Kernel Address Display Restriction
When attackers try to develop "run anywhere" exploits for kernel vulnerabilities, they frequently need to know the location of internal kernel structures. By treating kernel addresses as sensitive information, those locations are not visible to regular local users. Starting with Ubuntu 11.04, /proc/sys/kernel/kptr_restrict is set to "1" to block the reporting of known kernel address leaks. Additionally, various files and directories were made readable only by the root user: /boot/vmlinuz*, /boot/System.map*, /sys/kernel/debug/, /proc/slabinfo
 
15. ### Dmesg restrictions  
When attackers try to develop "run anywhere" exploits for vulnerabilties, they frequently will use dmesg output. By treating dmesg output as sensitive information, this output is not available to the attacker. Starting with Ubuntu 12.04 LTS, /proc/sys/kernel/dmesg_restrict can be set to "1" to treat dmesg output as sensitive. Starting with 20.10, this is enabled by default.

16. ### Linux Lockdown  
    it was introduced in Linux kernel version 5.4.  
_Modes_: The kernel lockdown feature operates in different modes, which determine the level of restriction applied:
Integrity Mode: This mode focuses on preventing unauthorized modifications to the kernel. It restricts actions that could alter the kernel's code or data.
Confidentiality Mode: In addition to the restrictions of integrity mode, this mode also prevents access to confidential information that could be used to compromise the system.  
_Restrictions_: When the kernel lockdown feature is enabled, it imposes several restrictions, including:
Disabling the ability to load unsigned kernel modules.
Preventing access to kernel memory through interfaces like /dev/mem and /dev/kmem.
Restricting access to certain kernel parameters and interfaces that could be used to modify kernel behavior.
Blocking the use of certain debugging features that could be exploited by attackers.

## User Space Hardening
1. ### The stack smash protection
   a. -fstack-protector-strong  
   b. -fstack-protector-all  
2. ### dynamic lib link options:
   add linker option -Wl, -z relro -Wl, -z now  
   -Wl, -z relro:  
This option enables "Read-Only Relocations" (RELRO). It is a security feature that makes certain sections of the program's memory read-only after they have been initialized. This helps prevent certain types of attacks, such as overwriting the Global Offset Table (GOT).  
   -Wl, -z now:  
This option forces the program to resolve all dynamic symbols at startup, rather than lazily resolving them as needed. This is known as "full RELRO" and enhances security by ensuring that the GOT is fully populated and then made read-only before the program starts executing. This prevents attackers from being able to overwrite GOT entries to redirect function calls.  

4. ### The Address Space Layout Randomization (ASLR)
   kernel configuraton /proc/sys/kernel/randomize_va_space to 2  
   program should be built as Position Independent Code (PIC) with "-fPIE -pie".  
   _-fPIE_ is used during the compilation phase to generate position-independent code in object files.  
   _-pie_ is used during the linking phase to create a position-independent executable from those object files.  
   _-fPIC_ is used to compile shared libraries (dynamic libraries) so that they can be loaded at any memory address.
5. ### No debug flag (-g for GCC) when building software components
6. ### static buffer checking
     use -D_FORTIFY_SOURCE=2 for GCC.
7. ### Data execution prevention
    modern Linux distributions typically make the stack non-executable by default.   
    _Kernel Support_: The Linux kernel has supported non-executable stack protection for many years. This is often enabled by default in the kernel configuration of most 
    distributions.  
    _Executable and Linkable Format (ELF)_: When a program is compiled, the ELF headers can specify that the stack should be non-executable. This is often the default 
    behavior unless explicitly overridden.  The "-z noexecstack" linker option modifies the ELF headers to include a note that the stack should be non-executable.  
    _Hardware Support_: Modern CPUs include a feature known as the NX (No Execute) bit, which allows the operating system to mark certain memory regions as non-executable. The Linux kernel uses this feature to enforce non-executable stack policies.

## Isolation
   ### MAC (Mandatory Access Control), Selinux and Apparmor, doesn't directly manage user ID and groups.
   
## References:
https://wiki.ubuntu.com/Security/Features/Historical
