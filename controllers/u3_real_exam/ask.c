#include <webots/robot.h>
#include <webots/position_sensor.h>
#include "ask.h"

float askForPosition(float a, WbDeviceTag b) {
  a=wb_position_sensor_get_value(b);
  return a;
}
