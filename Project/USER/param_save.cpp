#include "headfile.h"

void Params_ReadFlash()
{
    u8 size, len;
    len = sizeof(PIDflash);
    size = len / (4 + (len % 4) ? 1 : 0);
    // STMFLASH_Read(FLASH_SAVE_ADDR, (uint32_t *)(&PIDflash), size);
    TableToParams(); //将整数转换成浮点数
    if (PIDflash.PIT_Angle_P == 0xFFFF && PIDflash.ROL_Angle_P == 0xFFFF && PIDflash.YAW_Angle_P == 0xFFFF)
    {
        DefaultParams_WriteFlash();
        printf("Flash Read Error!!!\r\n");
    }
}

void Params_WriteFlash()
{
    u8 size, len;
    // DefaultParams();//初始化默认参数
    len = sizeof(PIDflash);
    size = len / (4 + (len % 4) ? 1 : 0); //保存的数据长度
                                          // STMFLASH_Write(FLASH_SAVE_ADDR,(uint32_t*)(&PIDflash),size);
}
