#ifndef __COMMON_H
#define __COMMON_H

constexpr int interval_from_speed(int speed)
{
    return 1000 / speed;
}

const int BASE_SPEED = 20;
const int BASE_INTERVAL = interval_from_speed(BASE_SPEED);

const int STATIC_Y_STEP = 6;

template <typename T>
inline T max(T a, T b)
{
    return a > b ? a : b;
}

#endif
