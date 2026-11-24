#include "Step_Motor.h"
#include <stdio.h>
#include "Emm_V5.h"

/* 位置命令脉冲换算：16 细分 → 一圈 3200 脉冲（与驱动器细分设置一致） */
#define MOTOR_PULSES_PER_REV 3200.0f

//Motor结构体初始化
void Motor_Init(Motor *self, uint8_t addr) {
    self->address = addr;
    self->currentAngle = 0.0f;
    self->targetAngle = 0.0f;
    self->enable = true;
}
void setCurrentAngle(Motor *self, float currentAngle) {
    self->currentAngle = currentAngle;
}
void setTargetAngle(Motor *self, float targetAngle) {
    self->targetAngle = targetAngle;
}
//使能电机
void EnableMotor(Motor *self) {
    Emm_V5_En_Control(self->address, true, false);
    self->enable = true;
}
//失能电机
void DisableMotor(Motor *self) {
    Emm_V5_En_Control(self->address, false, false);
    self->enable = false;
}
//读电机当前角度
float readCurrentAngle(Motor *self) {
    int32_t  raw;
    /* 1. 发读实时位置命令：01 36 6B */
    can.rxFrameFlag = false;
    Emm_V5_Read_Sys_Params(self->address, S_CPOS);
    /* 2. 等回帧（带超时，不卡死） */
    {
        uint32_t t0 = HAL_GetTick();
        while (!can.rxFrameFlag && (HAL_GetTick() - t0) < 100U) { }
    }
    /* 3. 解析 */
    if (can.rxFrameFlag){
        can.rxFrameFlag = false;
        if (can.rxData[0] == 0x36U){
            /* 回帧：[0]=0x36  [1]=符号  [2..5]=位置  [6]=0x6B */
            raw = ((int32_t)can.rxData[2] << 24) |
                  ((int32_t)can.rxData[3] << 16) |
                  ((int32_t)can.rxData[4] << 8)  |
                  ((int32_t)can.rxData[5]);
            if (can.rxData[1] != 0U) { raw = -raw; }

            /* Emm 固件换算：0~65536 表示一圈 360° */
            self->currentAngle = (float)raw * 360.0f / 65536.0f;
            printf("angle = %ld.%ld deg  (DLC=%d raw=%ld)\r\n",(long)(self->currentAngle),(long)((self->currentAngle >= 0 ? self->currentAngle : -self->currentAngle) * 10) % 10,can.CAN_RxMsg.DLC,(long)raw);
        }else{
            printf("unexpected code = 0x%02X, DLC=%d\r\n",
                   can.rxData[0], can.CAN_RxMsg.DLC);
        }
        return self->currentAngle;
    }
    return self->currentAngle;
}
//电机转动角度：angle 单位度，按 dir 方向相对运动
void moveAngle(Motor *self, uint8_t dir, float angle) {
    self->targetAngle = angle;
    float absAngle = (angle < 0.0f) ? -angle : angle;
    uint32_t clk = (uint32_t)(absAngle * MOTOR_PULSES_PER_REV / 360.0f + 0.5f);
    Emm_V5_Pos_Control(self->address, dir, 200, 10, clk, false, false);
}