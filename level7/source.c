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