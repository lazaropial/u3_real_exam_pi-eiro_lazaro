/*
 * File: u3_real_exam_Pieiro_Lazaro.c
 * Date: 18 de julio del 2019
 * Description:
 * Author: Lazaro Piñeiro Alcocer
 * Modifications:
 */
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/position_sensor.h>
#include <webots/keyboard.h>

#include <stdio.h>

#include <distance.h>
#include <distance2.h>
#include <ask.h>

#define TIME_STEP 64
#define PI 3.1416

enum {
 ENEMY_DETECTED,
 LOOKING_ENEMIES
};
/*variables globales*/
double left_distance_sensor,right_distance_sensor,enemy_detector,shoot;
short int robot_state;
int max_distance=2;
int time_rotating;
double a;

void stopRobot(WbDeviceTag wheel1,WbDeviceTag wheel2,WbDeviceTag wheel3) {
 wb_motor_set_velocity(wheel1, 0);
 wb_motor_set_velocity(wheel2, 0);
 wb_motor_set_velocity(wheel3, 0);
}

void forwardRobot(WbDeviceTag wheel1,WbDeviceTag wheel2,WbDeviceTag wheel3) {
 int vel=2;
 wb_motor_set_position(wheel1, INFINITY);
 wb_motor_set_velocity(wheel1, 0);
 wb_motor_set_position(wheel2, INFINITY);
 wb_motor_set_velocity(wheel2, -vel);
 wb_motor_set_position(wheel3, INFINITY);
 wb_motor_set_velocity(wheel3, vel);
}

void turnRightRobot(WbDeviceTag wheel1,WbDeviceTag wheel2,WbDeviceTag wheel3) {
 int vel=1;
 wb_motor_set_velocity(wheel1, vel);
 wb_motor_set_velocity(wheel2, vel);
 wb_motor_set_velocity(wheel3, vel);
}

void lookingForEnemies(WbDeviceTag enemy_sen) {
 float vel=0.6;
 wb_motor_set_position(enemy_sen, INFINITY);
 wb_motor_set_velocity(enemy_sen, vel);
}

void enemyDetected(WbDeviceTag enemy_sen) {
 wb_motor_set_position(enemy_sen, INFINITY);
 wb_motor_set_velocity(enemy_sen, 0);
}

void gunPosition(WbDeviceTag gun,float a) {
 int vel=1;
 wb_motor_set_position(gun, a);
 wb_motor_set_velocity(gun, vel);
}

void shootGun(WbDeviceTag a) {
 if (a<2 && a>1.3) {
   printf("THA\n");
 }
 if (a<1.2 && a>0.5) {
   printf("THAAAAA\n");
 }
 if (a<=0.4) {
   printf("THAAAAAAAAAAAAAAAAAAAAAAAA\n");
 }
}

int main(int argc, char **argv) {
  wb_robot_init();
  /////motors///////////
  WbDeviceTag wheel1 = wb_robot_get_device("motor1");
  WbDeviceTag wheel2 = wb_robot_get_device("motor2");
  WbDeviceTag wheel3 = wb_robot_get_device("motor3");
  WbDeviceTag enemy_sen = wb_robot_get_device("rot_sen");
  WbDeviceTag gun = wb_robot_get_device("gun");

  /////////position sensor//////////
  WbDeviceTag ps1 = wb_robot_get_device("pos1");
  WbDeviceTag ps2 = wb_robot_get_device("pos2");
  WbDeviceTag ps3 = wb_robot_get_device("pos3");
  WbDeviceTag enem_pos = wb_robot_get_device("pos_sen");
  WbDeviceTag gun_pos = wb_robot_get_device("gun_position");
  wb_position_sensor_enable(ps1, TIME_STEP);
  wb_position_sensor_enable(ps2, TIME_STEP);
  wb_position_sensor_enable(ps3, TIME_STEP);
  wb_position_sensor_enable(enem_pos, TIME_STEP);
  wb_position_sensor_enable(gun_pos, TIME_STEP);

  ///////////distance sensor/////////
  WbDeviceTag ds_l = wb_robot_get_device("distance_sensor");
  WbDeviceTag ds_r = wb_robot_get_device("distance_sensor2");
  WbDeviceTag gun_sen = wb_robot_get_device("gun_sensor");
  WbDeviceTag shoot_gun = wb_robot_get_device("gunn");
  wb_distance_sensor_enable(ds_l, TIME_STEP);
  wb_distance_sensor_enable(ds_r, TIME_STEP);
  wb_distance_sensor_enable(gun_sen, TIME_STEP);
  wb_distance_sensor_enable(shoot_gun, TIME_STEP);

  while (wb_robot_step(TIME_STEP) != -1) {
    forwardRobot(wheel1,wheel2,wheel3);
    lookingForEnemies(enemy_sen);
    robot_state=LOOKING_ENEMIES;

    enemy_detector=checkTheEnemyDistance(enemy_detector,gun_sen);
    if (enemy_detector < max_distance || robot_state==ENEMY_DETECTED) {
      robot_state=ENEMY_DETECTED;
      stopRobot(wheel1, wheel2, wheel3);
      a=askForPosition(a,enem_pos);
      enemyDetected(enemy_sen);
    }

    shoot=checkTheEnemyDistance(shoot,shoot_gun);
    if (robot_state==ENEMY_DETECTED) {
      gunPosition(gun, a);
      shootGun(shoot);
    }

    left_distance_sensor=chekingForObstacle(left_distance_sensor,ds_l);
    right_distance_sensor=chekingForObstacle(right_distance_sensor,ds_r);
    if (left_distance_sensor<=0.17 || right_distance_sensor<=0.17) {
        time_rotating++;
    }
    if (time_rotating>=1 && time_rotating<=65) {
      turnRightRobot(wheel1, wheel2, wheel3);
      time_rotating++;
    } else {
      time_rotating=0;
    }

  };

  wb_robot_cleanup();

  return 0;
}
