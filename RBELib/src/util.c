#include "RBELib/util.h"

float clipf(float n, float min, float max) {
  return (n < min) ? min : ((n > max) ? max : n);
}

int clipi(int n, int min, int max) {
  return (n < min) ? min : ((n > max) ? max : n);
}
