/**
 @file    monitor.h
 @author  AVI-crak
 @version V-51%
 @date    25-января-2017
 @brief   Аxis sRtoS, Cortex-M7 ARM GCC EmBitz

 license
 Million helicopters and one cent.

 форум для обсуждения
 http://forum.ixbt.com/topic.cgi?id=48:11735

 репозиторий
 https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc
*/


#ifndef _MONITOR_
#define _MONITOR_

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

//#define buf_zize    (127)
#define buf_zize    (256)

uint8_t     m_ms_buf[buf_zize + 1];

/// печать сообщение в EB monitor
/// \n новая строка, \t табуляция, \f очистка
/// \b позиция минус 1 символ, \r позиция строка с нуля.
void monitor_print (uint8_t* text);

/// Принять сообщение из EB monitor \n enter
uint8_t *monitor_scan (void);

/// Печать отладочной информации в EB monitor из os
void monitor (void);


#ifdef __cplusplus
}
#endif /* __cplusplus  */
#endif /* _MONITOR_ */


