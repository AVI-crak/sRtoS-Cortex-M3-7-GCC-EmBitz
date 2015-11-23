/// Cortex-M3 GCC EmBitz 0.40
/* RtoS.h */
/* videocrak@mail.ru */


#ifndef _RtoS.h
#include "stm32f107xc.h"
//#define _bitFlag volatile uint32_t __attribute__ ((section(".flag"))) // ��� �����

volatile uint32_t sSustem_task [8];
volatile uint32_t Random_register[3];

/*
//
sMore_task [80] - ������ �����
0x00, #0,  32b,  task_presently - ����� �������� ������
0x04, #4,  32b,  task_sDelay    - ����� ������ ������
0x08, #8,  32b,  task_wait      - ����� ������ ��������� �����
0x0C, #12, 32b,  tik_real       - 100% ����� �� ������ // us �� ������ ��� ������
0x10, #16, 32b,  Delay/flag/but - ���������� ����� �� ���, ����� ����� ��� �����,
0x14, #20, 32b,  NVIC_size      - ������ ����� ����������, �������� ��� ������
0x20, #24, 32b,  Main_size/start- ������ ����� �����,// ������� ����
0x1C, #28, 32b,  sSYSHCLK       -  ��������� �������, �� - �������� ��� ������

// ���� ������ ( ��������� 0x20)
// ������������ � ������� ����� ������ - � ������
0x00, #00  32b,  ����� ����� ������ (��������� �� ������)
0x04, #04, 32b,  ����� ���������� ������ ( ��������� �� ������)
0x08, #08, 32b,  ���� ������ (�������� �����)
0x0C, #12, 32b,  ����� ���������� ������ � ������
0x10, #16, 32b,  ����� ����� ��� ����� ������ ��� ����� ������
0x14, #20, 32b,  ����� ������� ���������� ������
0x18, #24, 16b,  ������ ����� (������� ��� �������)
0x1A, #26, 16b,  ��� ������ ����� (��� ��������� ������ �����)
0x1C, #28, 32b,  ����� ����� ������ (char* const text)

*/


/// sTask_wake (����) ��������� ������
__attribute__( ( always_inline ) ) static inline sTask_wake(char* const task_wake_flag)
{
__ASM volatile ("push   {r2}                \n\t"
                "mov    r2, %[__wake_flag]  \n\t"
                "svc    0x9                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wake_flag] "r" (task_wake_flag):);
}




/// sTask_wait (����) ���������� ������ � �������� �����
__attribute__( ( always_inline ) ) static inline sTask_wait(uint32_t* task_wait_flag)
{
__ASM volatile ("push   {r2}                \n\t"
                "mov    r2, %[__wait_flag]  \n\t"
                "svc    0x8                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wait_flag] "r" (task_wait_flag):);
}



/// sDelay_mc (� �������������)
__attribute__( ( always_inline ) ) static inline sDelay_mc(uint32_t Delay_mc)
{
__ASM volatile ("push   {r2}                \n\t"
                "mov    r2, %[__Delay_mc]   \n\t"
                "svc    0x7                 \n\t"
                "pop    {r2}                \n\t"
                :: [__Delay_mc] "r" (Delay_mc):);
}

/// ����� �S
/// ������� ���� � ��, ������ ����� �����,
///       > ������ ����� ����������, ��������� ����� ������ � �������������
__attribute__( ( always_inline ) ) static inline setup_run(uint32_t __SYSHCLK,
                                                           uint32_t _main_size,
                                                           uint32_t NVIC_size,
                                                           uint32_t task_time_us )
{
  __ASM volatile ("push {r4,lr} \n\t"
                  "ldr  r4, =sSustem_task               \n\t"
                  "str  %[___SYSHCLK], [r4, #28]        \n\t"
                  "str  %[__main_size], [r4, #24]       \n\t"
                  "str  %[__NVIC_size], [r4, #20]       \n\t"
                  "str  %[__task_time_us], [r4, #12]    \n\t"
                  "ldr  r4, =Start_task                 \n\t"
                  "blx  r4                              \n\t"
                  "pop {r4,lr}                          \n\t"
                  ::[___SYSHCLK] "r" (__SYSHCLK),[__main_size] "r" (_main_size),
                    [__NVIC_size] "r" (NVIC_size),[__task_time_us] "r" (task_time_us)
                  : "memory");
}

/// ������ ������������ GCC
__attribute__( ( always_inline ) ) static inline __memory(void)
{
  __ASM volatile ("nop" : : : "memory");
}

/// �������� ���������� - ����� ������� �� � ��������� ���������� !!!!!
__attribute__( ( always_inline ) ) static inline sNVIC_EnableIRQ(IRQn_Type IRQn)
{
__asm volatile("mov r1, %[IRQ] \n\t"
               "SVC  0x0       \n\t"
               :
               : [IRQ] "r" (IRQn):"memory");
}

/// ��������� ���������� - ����� ������� ��
__attribute__( ( always_inline ) ) static inline sNVIC_DisableIRQ(IRQn_Type IRQn)
{
__asm volatile("mov r1, %[IRQ] \n\t"
               "SVC  0x1       \n\t"
               :
               : [IRQ] "r" (IRQn):"memory");
}

/// ���������� ��������� ���������� (�� 14 �� 1)- ����� ������� ��
__attribute__( ( always_inline ) ) static inline sNVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
    __asm volatile("push {r4, r5}       \n\t"
                   "mov r4, %[IRQ]      \n\t"
                   "mov r5, %[PRIORIT]  \n\t"
                   "SVC  0x2            \n\t"
                   "pop {r4, r5}        \n\t"
                  :
                  : [IRQ] "r" (IRQn),[PRIORIT] "r" (priority):"memory");
}

/// ����� ������ - ����� ������� ��
///  ������� , ������ ����� , ������� ������� 1-100
void sTask_new (void (*taskS_func()),
                uint32_t task_size,
                uint32_t task_time_rate,
                char* const task_func_name)
{
    __asm volatile("push {r4, r5, r6, r7, r8, r9}       \n\t"
                   "mov r4, %[_sTask]       \n\t"
                   "mov r5, %[_sTime_task]  \n\t"
                   "mov r6, %[_sTask_size]  \n\t"
                   "mov r9, %[_func_name]   \n\t"
                   "SVC  0x4                \n\t"
                   "pop {r4, r5, r6, r7, r8, r9}        \n\t"
                  :
                  : [_sTask] "r" (taskS_func),[_sTask_size] "r" (task_size),[_sTime_task] "r" (task_time_rate),
                   [_func_name] "r" (task_func_name)
                  : "memory" );
}

/// �������� ����� - ������ ������ ���������� ������
__attribute__( ( always_inline ) ) static inline sTask_skip (void)
{
    __asm volatile( "push   {r0, r1}            \n\t"
                    "movw   r0, #0x1024         \n\t"
                    "movt   r0, #0x4000         \n\t"
                    "ldr    r1, [r0, #8]        \n\t"   // TIM6_ARR
                    "str    r1, [r0]            \n\t"   // TIM6_CNT
                    "nop                        \n\t"
                    "nop                        \n\t"
                    "pop    {r0, r1}            \n\t"
                    );
}

/// ����� ������ - ������ ������ ���������� ������
__attribute__( ( always_inline ) ) static inline sTask_kill(void){__asm volatile("SVC 0x5":::"memory");}


__attribute__( ( always_inline ) ) static inline uint32_t sRandom(uint32_t Random_max,uint32_t Random_min)
{
register volatile uint32_t value;
__asm volatile(
            "push   {r1, r2}        \n\t"
            "mov    r1, %[dom_max]  \n\t"
            "mov    r2, %[dom_min]  \n\t"
            "SVC    0x3             \n\t"
            "pop    {r1, r2}        \n\t"
            "mov    %[val], r0      \n\t"
            : [val] "=r" (value): [dom_max] "r" (Random_max),[dom_min]"r" (Random_min):"r0");
return value;
}

#endif _RtoS.h
#define _RtoS.h
