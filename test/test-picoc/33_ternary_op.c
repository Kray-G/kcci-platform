#include <stdio.h>

void main() {
    int Count;

    for (Count = 0; Count < 10; Count++)
    {
        printf("%d\n", (Count < 5) ? (Count*Count) : (Count * 3));
    }
}
