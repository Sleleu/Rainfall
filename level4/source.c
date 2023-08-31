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