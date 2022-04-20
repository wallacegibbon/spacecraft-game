#ifndef __COMMON_H
#define __COMMON_H

constexpr int interval_from_speed(int speed)
{
    return 1000 / speed;
}

const int BASE_SPEED = 20;
const int BASE_INTERVAL = interval_from_speed(20);

#endif
