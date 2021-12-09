#include <stdio.h>
#include <time.h>

int main() {
  time_t epoch = 0;
  int ret = time(&epoch);
  if (ret < 0) {
    printf("time(2) call failed.\n");
    return 1;
  }
  return 0;
}
