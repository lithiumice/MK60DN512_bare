#ifndef __GYRO__
#define __GYRO__
#include "headfile.h"
#include "main.h"
#include "icm_spi.h"
void LPF_1_db(float hz, float time, float in, float *out);

class gyro
{
public:
    float yaw_rate_200hz = 0;
    float gyro_z_filter_200hz = 0;
    float delta_z_200hz = 0;
    float gyro_y_filter_200hz = 0;
    float pitch_rate_200hz = 0;
    float delta_y_200hz = 0;

    float acc_x_filter = 0;
    float acc_y_filter = 0;
    float acc_z_filter = 0;

    float gyro_x_filter = 0;
    float gyro_z_filter = 0;
    float gyro_y_filter = 0;

    float gyro_z_offset = -6;
    float gyro_y_offset = -4;
    float gyro_x_offset = 0;

    float delta_z = 0;
    float delta_y = 0;

    float yaw_rate = 0;
    float true_yaw_rate = 0;
    float pitch_rate = 0;

    float yaw_angle = 0;
    float pitch_angle = 0;
    float roll_angle = 0;

    float gyro_y_omega = 0;
    float acc_angle = 0;
    float turn_angle = 0;

    float cross_z_angle = 0;
    float gyro_z_angle = 0;
    float gyro_y_angle = 0;
    float start_angle = 0;

    // kalman_filter kalman_f;
    quad_num quad_n;
    _imu_st imu_data = {1, 0, 0, 0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0};

    void update()
    {
        // get_icm20602_accdata_spi();
        // get_icm20602_gyro_spi();
        icm_acc_x = Get_X_Acc();
        icm_acc_y = Get_Y_Acc();
        icm_acc_z = Get_Z_Acc();
        icm_gyro_x = Get_X_Gyro();
        icm_gyro_y = Get_Y_Gyro();
        icm_gyro_z = Get_Z_Gyro();

        LPF_1_db(35, 1000, (float)(icm_gyro_z - gyro_z_offset), &gyro_z_filter);
        LPF_1_db(35, 1000, (float)(icm_gyro_y - gyro_y_offset), &gyro_y_filter);
        LPF_1_db(35, 1000, (float)(icm_gyro_x - gyro_x_offset), &gyro_x_filter);

        LPF_1_db(35, 1000, (float)(icm_acc_x), &acc_x_filter);
        LPF_1_db(35, 1000, (float)(icm_acc_y), &acc_y_filter);
        LPF_1_db(35, 1000, (float)(icm_acc_z), &acc_z_filter);

        quad_n.IMUupdate(IMUConverter * gyro_x_filter,
                         IMUConverter * gyro_y_filter,
                         IMUConverter * gyro_z_filter,
                         acc_x_filter, acc_y_filter, acc_z_filter, &imu_data);

        pitch_angle = -imu_data.pit;
        yaw_angle = imu_data.yaw;
        roll_angle = imu_data.rol;

        pitch_rate = 0.03051757f * gyro_y_filter;
        yaw_rate = 0.03051757f * gyro_z_filter;
        delta_y = pitch_rate * 0.001;

        true_yaw_rate = -yaw_rate / cos(pitch_angle * (PI / 180));
        delta_z = 2 * true_yaw_rate * 0.001;
        turn_angle += delta_z;
    }
};
#endif