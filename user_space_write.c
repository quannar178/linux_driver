#include <stdio.h>
// #include <fcntl.h>
 
int main(int argc, char *argv[])
{

    if( argc == 2 ) {
        printf("The argument supplied is %s\n", argv[1]);
        FILE *fp;

        fp = fopen("/dev/gpio_drv", "w+");
        fprintf(fp, "%s" , argv[1]);
        fclose(fp);
    }
    else if( argc > 2 ) {
        printf("Too many arguments supplied.\n");
    }
    else {
        printf("One argument expected.\n");
    }
    return 0;
}