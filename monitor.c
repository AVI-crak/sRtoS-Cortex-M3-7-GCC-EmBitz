/**
 @file    monitor.c
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


#include <stdint.h>
#include "monitor.h"
#include "RtoS.h"
#include "sPrint.h"


uint8_t     _std_out_buffer[buf_zize];
uint8_t     _std_in_buffer[buf_zize];
uint8_t     m_mk_buf[64];

#pragma pack(push, 4)
static struct _stdout
{
    const uint16_t              length;
    const volatile  uint16_t    tail;
    volatile uint16_t           head;
    uint16_t                    mode;
    volatile uint8_t            *ptr;
}_eb_monitor_stdout ={buf_zize,0,0,1,&_std_out_buffer[0]};

static struct _stdin
{
    const uint16_t          length;
    volatile uint16_t       tail;
    const volatile uint16_t head;
    uint16_t                mode;
    const volatile uint8_t  *ptr;
}_eb_monitor_stdin ={buf_zize,0,0,1,&_std_in_buffer[0]};
#pragma pack(pop)

void monitor_fining (void);
void monitor_fining (void)
{
    uint16_t tmp = 0;
    do
    {
        m_mk_buf[tmp++] = 0x20;
    }while (tmp != 64);
};

uint32_t monitor_timtemp = 0;

 /// печать строки
void monitor_print (uint8_t* text)
{

    if (monitor_timtemp == ((uint32_t) 0-1)) return;
    uint16_t temp_t = 0;
    if (text[temp_t] == '\0') return;
    uint16_t temp_h = _eb_monitor_stdout.head;
    uint16_t temp_l;

    do
    {
        temp_l = temp_h; temp_h++;
        if (temp_h == buf_zize) temp_h = 0;
        if (_eb_monitor_stdout.tail != temp_h ) _eb_monitor_stdout.ptr[temp_h] = text[temp_t++];
        else
        {
            _eb_monitor_stdout.head = temp_l;
            while (_eb_monitor_stdout.tail != temp_l )
            {
                if (monitor_timtemp == 0) monitor_timtemp = sSystem_task.system_us + 5000;
                if (monitor_timtemp < sSystem_task.system_us)
                {
                   monitor_timtemp = (uint32_t) 0-1;
                   return;
                }else sTask_skip();// Delay(1000);// там занято
            };
            _eb_monitor_stdout.ptr[temp_h] = text[temp_t++];
            _eb_monitor_stdout.head = temp_h;
        }
    }while (text[temp_t] != '\0') ;
    _eb_monitor_stdout.head = temp_h;
    monitor_timtemp = 0;
    __DSB();
};

uint8_t *monitor_scan (void)
{
    uint16_t temp_h = _eb_monitor_stdin.head;
    uint16_t temp_l = _eb_monitor_stdin.tail;
    uint16_t temp_x = 0;
    if (temp_l != temp_h)
        {
            if (_eb_monitor_stdin.ptr[temp_h] == '\n')
                {
                    while (temp_l != temp_h)
                    {
                        temp_l++; if (temp_l == buf_zize) temp_l = 0;
                        m_ms_buf[temp_x++] = _eb_monitor_stdin.ptr[temp_l];
                    }
                    m_ms_buf[--temp_x] = 0; _eb_monitor_stdin.tail = temp_l;
                } else m_ms_buf[0] = 0;
        } else m_ms_buf[0] = 0;
    __memory();
    return m_ms_buf;
}



void monitor (void)
{
uint32_t    Ltask_list_zize_sys;
uint32_t    temp_list;
uint32_t    temp_use;
uint16_t    temp_buf;
uint32_t    temp_n;
                        //  5    10     16
char     txt_mode[]= "hold\0wait\0delay\0activ\0";
uint32_t time_tax;
uint32_t time_tax2;

while(1)
{
time_tax = sSystem_task.system_us;
//                   15           5        10        10        10
monitor_print("Task_name     ;  ID;     zize; max_zize;     time;   mode\n");
while ( ! sSystem_task.sustem_flag.flag.reliability_task_list ) sTask_skip();
Ltask_list_zize_sys = sSystem_task.task_list_zize_sys;    //Количество тасков в системе
temp_list = 0;
while(Ltask_list_zize_sys != 0 )
{
   if ( sSystem_task.task_list[temp_list] != 0 )
   {
        monitor_fining();__DSB();
        temp_use = 0; temp_buf = 0;
        do
        {
          m_mk_buf[temp_buf++] = sSystem_task.task_list[temp_list]->task_names[temp_use++];
        }
        while (sSystem_task.task_list[temp_list]->task_names[temp_use] != '\0');
        temp_use = sSystem_task.task_list[temp_list]->mode_nomer.m_n.task_nomer; temp_buf = 19;
        while (temp_use != 0){m_mk_buf[--temp_buf] = temp_use %10 + '0'; temp_use /= 10; };
        temp_use = sSystem_task.task_list[temp_list]->stack_zize;temp_buf = 29;
        while (temp_use != 0){m_mk_buf[--temp_buf] = temp_use %10 + '0'; temp_use /= 10; };
        temp_use = sSystem_task.task_list[temp_list]->stack_max_zize; temp_buf = 39;
        while (temp_use != 0){m_mk_buf[--temp_buf] = temp_use %10 + '0'; temp_use /= 10; };
        temp_use = sSystem_task.task_list[temp_list]->life_time; temp_buf = 49;
        while (temp_use != 0){m_mk_buf[--temp_buf] = temp_use %10 + '0'; temp_use /= 10; };
        temp_use = sSystem_task.task_list[temp_list]->mode_nomer.m_n.mode;
        if (temp_use == 0)temp_n = 0; //hold = 0  >0
        if (temp_use == 1)temp_n = 5; //wait = 1  >5
        if (temp_use == 2)temp_n = 10;  //delay = 2 >10
        if (temp_use == 3)temp_n = 16;  //activ = 3 >16
        temp_buf = 52;
        do
        {
          m_mk_buf[temp_buf++] = txt_mode[temp_n++];
        }
        while ( txt_mode[temp_n] != '\0');
        temp_buf = 59;
        m_mk_buf[temp_buf++] = '\n';
        m_mk_buf[temp_buf++] = 0;
        monitor_print (&m_mk_buf[0]);
    }
    temp_list++; Ltask_list_zize_sys--;
}
uint8_t shaize[12];
printo("\nNVIC_size_max: ",sSystem_task.NVIC_size_max);
printo("\nNVIC_size: ", sSystem_task.NVIC_size);
printo("\nTime_ptint: ", time_tax2);
monitor_print ("ms\n");
while (_eb_monitor_stdout.tail != _eb_monitor_stdout.head ) sTask_skip();
time_tax2 = sSystem_task.system_us - time_tax;
sDelay_mc(3000);
monitor_print("\f");

}


}

