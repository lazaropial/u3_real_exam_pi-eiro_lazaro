#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include "distance2.h"

float checkTheEnemyDistance(float a, WbDeviceTag b) {
    a=((wb_distance_sensor_get_value(b)*2)/1023);
    return a;
}
