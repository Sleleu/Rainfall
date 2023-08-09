#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int value;
    char* arg;
    gid_t egroup_id;
    uid_t euser_id;

    value = atoi(argv[1]);
    if (value == 0x1a7) // (423 in hex) = 0x08048ed9 <+25>:  cmp    $0x1a7,%eax
    {
        arg = strdup("/bin/sh");
        egroup_id = getegid(); // returns the effective group ID of the calling process.
        euser_id = geteuid(); // returns the effective user ID of the calling process.
        // id = 2020
        #ifdef __linux__
            setresgid(egroup_id, egroup_id, egroup_id);
            setresuid(euser_id, euser_id, euser_id);
        #elif __APPLE__
            setregid(egroup_id, egroup_id);
            setreuid(euser_id, euser_id);
        #endif
        execve("/bin/sh", &arg, NULL);
        //  here : jmp    0x8048f80 <main+192>
    }
    else // = 0x08048ede <+30>:	jne    0x8048f58 <main+152>
        fwrite("No !", 1, 5, stderr);
    return (0);
}