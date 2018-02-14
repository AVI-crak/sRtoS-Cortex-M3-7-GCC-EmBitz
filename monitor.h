/**
 @file    monitor.h
 @author  AVI-crak
 @version V-51%
 @date    25-������-2017
 @brief   �xis sRtoS, Cortex-M7 ARM GCC EmBitz

 license
 Million helicopters and one cent.

 ����� ��� ����������
 http://forum.ixbt.com/topic.cgi?id=48:11735

 �����������
 https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc
*/


#ifndef _MONITOR_
#define _MONITOR_

#define buf_zize    (127)
uint8_t     m_mk_buf[buf_zize + 1];
uint8_t     m_ms_buf[buf_zize + 1];

/// ������ ��������� � EB monitor
/// \n ����� ������, \t ���������, \f �������
void monitor_print (uint8_t* text);

/// ������� ��������� �� EB monitor \n enter
uint8_t *monitor_scan (void);

/// ������ ���������� ���������� � EB monitor �� os
void monitor (void);

#endif /* _MONITOR_ */


