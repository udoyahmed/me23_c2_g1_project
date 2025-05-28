#include<stdio.h>

int main() {
    char ch[10];
    printf("Something");
    fgets(ch, sizeof(ch), stdin);

    printf("%s", ch);
    return 0;
}