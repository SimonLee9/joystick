#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <QString>

typedef struct{
    QString cond_auto_state;
    float cond_inlier_error;
    float cond_inlier_ratio;
    float cond_mapping_error;
    float cond_mapping_ratio;
    QString cond_obs_state;

    float imu_acc_x;
    float imu_acc_y;
    float imu_acc_z;
    float imu_gyr_x;
    float imu_gyr_y;
    float imu_gyr_z;
    float imu_imu_rx;
    float imu_imu_ry;
    float imu_imu_rz;

    QString lidar_connection;
    QString lidar_port;
    QString lidar_serial;

    QString motor_connection[2];
    float motor_current[2];
    int motor_status[2];
    float motor_temp[2];

    float pose_x;
    float pose_y;
    float pose_rz;

    float power_bat_current;
    float power_bat_in;
    float power_bat_out;
    float power_bat_power;
    float power_bat_tot_power;

    QString state_charge;
    QString state_emo;
    QString state_localization;
    QString state_map;
    QString state_power;

    float vel_vx;
    float vel_vy;
    float vel_wz;

}ST_STATUS;



typedef enum{
    MOVE_CMD_CHECK_NON = 0,
    MOVE_CMD_CHECK_SEND_CMD,
    MOVE_CMD_CHECK_RECV_RET,
    MOVE_CMD_CHECK_MOVE_STATE_CONFIRM,
    MOVE_CMD_CHECK_STOP_STATE_CONFIRM
}ENUM_MOVE_CMD_CHECK;


#endif // COMMON_DATA_H
