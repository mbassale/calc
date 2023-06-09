#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void calc_write(int v) {
  printf("The result is: %d\n", v);
}

int calc_read(char *s) {
  char buf[64];
  int val;
  printf("Enter a value for %s: ", s);
  fgets(buf, sizeof(buf), stdin);
  if (EOF == sscanf(buf, "%d", &val)) {
    printf("Error: invalid input: %s\n", buf);
    exit(1);
  }
  return val;
}

int calc_pow(int a, int b) {
  return (int)pow(a, b);
}