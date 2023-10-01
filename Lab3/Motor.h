/*
 * Motor.h
 *
 *  Created on: Feb 19, 2023
 *      Author: farha
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void Motor_Stop(void);
void Motor_Forward(volatile uint16_t leftDuty, volatile uint16_t rightDuty);
void Motor_Right(uint16_t rightDuty);

#endif /* MOTOR_H_ */
