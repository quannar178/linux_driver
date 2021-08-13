#include <stdio.h>
 
int main(void)
{

   FILE *fp;
   char buff[255];

   fp = fopen("/dev/gpio_drv", "r");
   fscanf(fp, "%s", buff);
   printf("The value of GPIO is : %s\n", buff );

   fclose(fp);

   return 0;
}