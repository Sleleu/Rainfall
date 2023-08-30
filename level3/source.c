unsigned int v()
{
    void var_20c; // buffer
    fgets(&var_20c, 0x200, stdin);
    printf(&var_20c);
    unsigned int eax = m;
    if (eax == 0x40) // 64
    {
        fwrite("Wait what?!\n", 1, 0xc, stdout);
        eax = system("/bin/sh");
    }
    return eax;
}

int main(int argc, char** argv, char** envp)
{
    return v();
}