# About this project

This is a cybersecurity project of 42 school, following in the footsteps of the [snow-crash](https://github.com/Sleleu/snow_crash) project, aims to familiarize oneself with binary exploitation.

Several notions are covered in this project, including:

- Basics of reverse engineering
- Stack / heap buffer overflow
- Buffer overflow via environment variables
- GOT overwrite
- Format string attack vulnerabilities
- Use of gdb
- Understanding assembly (a bit 🙃)


```
	  _____       _       ______    _ _ 
	 |  __ \     (_)     |  ____|  | | |
	 | |__) |__ _ _ _ __ | |__ __ _| | |
	 |  _  /  _` | | '_ \|  __/ _` | | |
	 | | \ \ (_| | | | | | | | (_| | | |
	 |_|  \_\__,_|_|_| |_|_|  \__,_|_|_|

                 Good luck & Have fun

  To start, ssh with level0/level0 on 10.0.2.15:4242
```

This project is presented in the form of a virtual machine with 14 levels available. Each level contains a binary in the user's /home directory. The objective is to exploit this binary in order to progress to the next level.

Each level should have a folder containing:

- The flag that allows access to the user of the next level
- Source code obtained from the decompiled executable, providing a better understanding of the binary's structure to be exploited
- A walkthrough describing the steps to successfully exploit it


Many challenges, after identifying how to exploit the vulnerability through the debugger, assembly instructions, and decompiled code, will look like this:

```
(python -c "print('A' * 76 + '\x44\x84\x04\x08')"; cat) | ./binary <== poof 💣
Good... Wait what? <== 💀
whoami
levelup
cat /home/user/levelup/.pass
{flag} <== 🚩
```


# Ressources

Here are some resources that can be helpful:

### Find offset
-  https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/

### Online decompiler
- https://dogbolt.org/

### Shellcode used
- https://shell-storm.org/shellcode/files/shellcode-811.html
- https://shell-storm.org/shellcode/files/shellcode-752.html

### Buffer overflow basics
-  https://beta.hackndo.com/buffer-overflow/
-  https://www.root-me.org/fr/Documentation/Applicatif/Debordement-de-tampon-dans-la-pile
-  https://repository.root-me.org/Exploitation%20-%20Syst%C3%A8me/Unix/FR%20-%20Stack%20Bug%20-%20Exploitation%20avancee%20de%20buffer%20overflow.pdf
-  https://www.youtube.com/watch?v=u-OZQkv2ebw (king)

### Assembly basics
- http://www2.ift.ulaval.ca/~marchand/ift17583/Supplement2.pdf

### Format string attack
- https://www.unilim.fr/pages_perso/patrick.poulingeas/Enseignements/2004_2005/Master1/FormatString.pdf
- https://repo.zenk-security.com/Techniques%20d.attaques%20%20.%20%20Failles/Les%20failles%20Format%20String.pdf
- https://www.re-xe.com/format-string-bugs/

### Got overwrite
- https://ctf101.org/binary-exploitation/what-is-the-got/
- https://axcheron.github.io/exploit-101-format-strings/#code-execution-redirect
