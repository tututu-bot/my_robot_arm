#ifndef ZERO_ARM_STEP_MOTOR_H
#define ZERO_ARM_STEP_MOTOR_H
#include <stdbool.h>

#include "main.h"

typedef struct {
    uint8_t address;
    float currentAngle;
    float targetAngle;
    bool enable;
}Motor;
void Motor_Init(Motor *self, uint8_t addr);
void setCurrentAngle(Motor *self, float currentAngle);
void setTargetAngle(Motor *self, float targetAngle);
void EnableMotor(Motor *self);
void DisableMotor(Motor *self);
float readCurrentAngle(Motor *self);
void moveAngle(Motor *self, uint8_t dir, float angle);

#endif //ZERO_ARM_STEP_MOTOR_H
