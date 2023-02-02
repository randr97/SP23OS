#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    char *a = argv[1];
    int year = atoi(a);

    if ((year % 4 == 0 && year % 100 != 0) || (year % 4 == 0 && year % 400 == 0)) printf("%d was a leap year", year);
    else printf("%d was not a leap year", year);
}
