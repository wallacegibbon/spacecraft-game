#pragma once

#include <cmath>

constexpr int interval_from_speed(int speed) { return 1000 / speed; }

const int BASE_SPEED = 20;
const int BASE_INTERVAL = interval_from_speed(BASE_SPEED);

const int STATIC_Y_STEP = 6;

inline double hypotenuse(double a, double b) {
  return std::sqrt(a * a + b * b);
}
