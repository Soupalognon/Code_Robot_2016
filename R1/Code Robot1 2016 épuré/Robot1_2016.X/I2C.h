/* 
 * File:   I2C.h
 * Author: user
 *
 * Created on 12 novembre 2015, 22:32
 */

#ifndef I2C_H
#define	I2C_H

#include "Config_robots.h"

#ifdef	__cplusplus
extern "C" {
#endif

void InitI2C(u32 Fscl, u8 delay);
s8 envoiInformationI2C(u8 ID, u8 data); //En tant que MAITRE
s8 lectureInformationI2C(u8 ID, u8 data); //En tant que MAITRE  
u8 get_flagCompteurDelayI2C();
void add_compteurDelayI2C();



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

