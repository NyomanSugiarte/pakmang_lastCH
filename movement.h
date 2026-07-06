#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "Gmini_Tr.h"   
#include "IK2.h"
#include <iostream>
#include <cmath>
void Forward(double speed, double body_height, double step_length, double step_height);
void Backward(double speed, double body_height, double step_length, double step_height);
void StandUp (double target_body_height, double duration_fase1, double duration_fase2);
#endif // MOVEMENT_H