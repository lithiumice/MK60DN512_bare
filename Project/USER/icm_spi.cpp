#include "headfile.h"
#include "math.h"
#include "icm_spi.h"

void Icm20602_init_spi(void)
{
    uint8 val = 0x0;
    spi_init(spi1, SPI_PCS0, MASTER, 10 * 1000 * 1000); //硬件SPI初始化
    systick_delay_ms(20);
    //检测
    icm_spi_w_reg_byte(MPU_RA_PWR_MGMT_1, 0x80);
    systick_delay_ms(20);
    icm_spi_w_reg_byte(MPU_RA_PWR_MGMT_1, 0x01);
    systick_delay_ms(20);
    do
    {
        icm_spi_r_reg_byte(0x75, &val);
    } while (0x12 != val);
    /*复位reg*/
    icm_spi_w_reg_byte(MPU_RA_SIGNAL_PATH_RESET, 0x03);
    systick_delay_ms(10);
    /*复位reg*/
    icm_spi_w_reg_byte(MPU_RA_USER_CTRL, 0x01);
    systick_delay_ms(10);
    icm_spi_w_reg_byte(0x70, 0x40); //dmp
    systick_delay_ms(10);
    icm_spi_w_reg_byte(MPU_RA_PWR_MGMT_2, 0x00);
    systick_delay_ms(10);
    icm_spi_w_reg_byte(MPU_RA_SMPLRT_DIV, 0);
    systick_delay_ms(10);
    icm_spi_w_reg_byte(MPU_RA_CONFIG, ICM20602_LPF_20HZ);
    systick_delay_ms(10);
    icm_spi_w_reg_byte(MPU_RA_GYRO_CONFIG, (2 << 3));
    systick_delay_ms(10);
    icm_spi_w_reg_byte(MPU_RA_ACCEL_CONFIG, (0 << 3));
    systick_delay_ms(10);
    /*加速度计LPF 20HZ*/
    icm_spi_w_reg_byte(0X1D, 0x04);
    systick_delay_ms(10);
    /*关闭低功耗*/
    icm_spi_w_reg_byte(0X1E, 0x00);
    systick_delay_ms(10);
    /*关闭FIFO*/
    icm_spi_w_reg_byte(0X23, 0x00);
    systick_delay_ms(10);
}

/******************获得X轴加速度******************/
int16 Get_X_Acc()
{
    int16 temp;
    uint8 dat[2];
    icm_spi_r_reg_byte(ICM20602_ACCEL_XOUT_H, dat + 0);
    icm_spi_r_reg_byte(ICM20602_ACCEL_XOUT_L, dat + 1);
    temp = (int16)(((uint16)dat[0] << 8 | dat[1]));
    return temp;
}
/******************获得Y轴加速度******************/
int16 Get_Y_Acc()
{
    int16 temp;
    uint8 dat[2];
    icm_spi_r_reg_byte(ICM20602_ACCEL_YOUT_H, dat + 0);
    icm_spi_r_reg_byte(ICM20602_ACCEL_YOUT_L, dat + 1);
    temp = (int16)(((uint16)dat[0] << 8 | dat[1]));
    return temp;
}
/******************获得Z轴加速度******************/
int16 Get_Z_Acc()
{
    int16 temp;
    uint8 dat[2];
    icm_spi_r_reg_byte(ICM20602_ACCEL_ZOUT_H, dat + 0);
    icm_spi_r_reg_byte(ICM20602_ACCEL_ZOUT_L, dat + 1);
    temp = (int16)(((uint16)dat[0] << 8 | dat[1]));
    return temp;
}
/*****************获得x轴角速度*****************/
int16 Get_X_Gyro()
{
    uint8 dat[2];
    int16 temp;
    icm_spi_r_reg_byte(ICM20602_GYRO_XOUT_H, dat + 0);
    icm_spi_r_reg_byte(ICM20602_GYRO_XOUT_L, dat + 1);
    temp = (int16)(((uint16)dat[0] << 8 | dat[1]));
    return temp;
}
/*****************获得y轴角速度*****************/
int16 Get_Y_Gyro()
{
    uint8 dat[2];
    int16 temp;
    icm_spi_r_reg_byte(ICM20602_GYRO_YOUT_H, dat + 0);
    icm_spi_r_reg_byte(ICM20602_GYRO_YOUT_L, dat + 1);
    temp = (int16)(((uint16)dat[0] << 8 | dat[1]));
    return temp;
}
/*****************获得z轴角速度*****************/
int16 Get_Z_Gyro()
{
    uint8 dat[2];
    int16 temp;
    icm_spi_r_reg_byte(ICM20602_GYRO_ZOUT_H, dat + 0);
    icm_spi_r_reg_byte(ICM20602_GYRO_ZOUT_L, dat + 1);
    temp = (int16)(((uint16)dat[0] << 8 | dat[1]));
    return temp;
}
