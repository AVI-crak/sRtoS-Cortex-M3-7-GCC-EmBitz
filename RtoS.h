/// Cortex-M3 GCC EmBitz 0.40
/// ��� �����
/// RtoS.h
/// ������� ���������� 34%

/// ���� ��� ����������������
/// videocrak@maol.ru
/// ����� ��� ����������
/// http://forum.ixbt.com/topic.cgi?id=48:11735

/// �����������
/// https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc

//#define __HEAP_SIZE = 1000;

#ifndef _RtoS.h
#include "stm32f107xc.h"
//#define _bitFlag volatile uint32_t __attribute__ ((section(".flag"))) // ��� �����

volatile uint32_t sSustem_task [11];
volatile uint32_t Random_register[3];

/**

sSustem_task [10] - ������ �����
0x00, #0,  32b,  task_presently         - ����� �������� ������
0x04, #4,  32b,  task_sDelay            - ����� ������ ������
0x08, #8,  32b,  task_wait              - ����� ����� ��������� �����
0x0C, #12, 32b,  task_malloc            - ����� ����� �� ��������� ������
0x10, #16, 32b,  NVIC_size_max          - �������������� ������ ����� ����������
0x14, #20, 32b,  NVIC_size              - ������ ����� ����������, ������� ��� ������
0x20, #24, 32b,  Main_size/start        - ������ ����� �����// ������� ����
0x1C, #28, 32b,  sSYSHCLK               - ��������� �������, �� - �������� ��� ������
0x20, #32, 32b,  tik_real               - 100% ����� �� ������ // us �� ������ ��� ������
0x24, #36, 32b,  malloc_start           - ������ ����� malloc
0x28, #40, 32b,  malloc_stop            - ��������� ����� malloc

 ���� ������ ( ��������� 0x20)
 ������������ � ������� ����� ������ - � ������
0x00, #00  32b,  ����� ����� ������ (��������� �� ������)
0x04, #04, 32b,  ����� ���������� ������ ( ��������� �� ������)
0x08, #08, 32b,  ���� ������ (�������� �����)
0x0C, #12, 16b,  ������ ���������� � ������
0x0E, #14, 08b - ���������� ������� ���������� ������
0x0F, #15, 08b - ���� ������� �� ���������
0x10, #16, 32b,  ����� ��� (��) // ����� ����������� ����� �����
0x14, #20, 32b,  malloc_zize - malloc_adres // malloc_adres
0x18, #24, 16b,  ������ ����� (������� ��� �������)
0x1A, #26, 16b,  ��� ������ ����� (��� ��������� ������ �����)
0x1C, #28, 32b,  ����� ����� ������ (char* const text)


���� ������� �� ���������
1 �������� ����� ������� ������
2 ������� ������� ������
3 ������ ������� ������ �� ������ ����� � ������������� � �������� (������ ��������)
4 ������������� � ������� ������� ������ (������ ����������� ����� ����)
5 ������� ��������� ������� ��� ������
6 ������� ��������� ������� ��� ������
5 ��� ��� ���
254

������
0x00, #00, 32b, �������� �� ������ �����
0x02, #02, 32b, ��� ������
0x04, #04, 32b, ���� (������� 4b)
**/



/// �������� ������ (������)
static uint32_t malloc (uint32_t malloc_zize)
{
register volatile uint32_t malloc_adres = malloc_zize;
asm volatile ("mov      r1, %[malloczize__] \n\t"
              "cbnz     r1, _Za_%=            \n\t"
              "bkpt                         \n\t"   // ������� ������ ���������  ������
              "_Za_%=:" "push     {r3}        \n\t"
              "ldr     r3,  =sSustem_task   \n\t"
              "ldr     r3, [r3]             \n\t"   // �������� ������
              "str     r1, [r3, #20]        \n\t"   // malloc_zize - malloc_adres // malloc_adres
              "mov     r1, #1               \n\t"   // ���� ��������� ��� sTask_nil, 1 = malloc_in
              "strb    r1, [r3, #15]        \n\t"   // ���� ������� �� ���������
              "SVC      0xA                 \n\t"   // ���� ���������� �������
              "mov     r1, r3               \n\t"
              "pop      {r3}                \n\t"
              "ldr      %[malloc_ad__],  [r1, #20]   \n\t"

               : [malloc_ad__] "=&r" (malloc_adres)
               : [malloczize__] "r" (malloc_zize): "r1" );
return malloc_adres;
}

/// ������� ������ (�����)
static void free (void* malloc_adres)
{
asm volatile (//"push     {r1, r2, r3}              \n\t"
              "ldr      r1,  =sSustem_task        \n\t"
              "ldr      r3, [r1]                  \n\t"   // �������� ������
              "ldr      r2, [r3, #28]             \n\t"   // �������� ������
              "str      %[adres__], [r3, #20]     \n\t"   // malloc_adres
              "ldr      %[adres__], [%[adres__], #-4] \n\t"
              "subs     r2, %[adres__]            \n\t"
              "beq      _Zb_%=                    \n\t"
              "bkpt                               \n\t"   // ����� ������
              "nop                                \n\t"
    "_Zb_%=:" "mov      r1, #2                    \n\t"   // ���� ��������� ��� sTask_nil, 2 = free (malloc_adres)
              "strb     r1, [r3, #15]             \n\t"   // ���� ������� �� ���������
              //"pop      {r1, r2, r3}              \n\t"
              "SVC      0xA                       \n\t" // ���� ���������� �������
               :: [adres__] "r" (malloc_adres): "memory", "r1", "r2", "r3" );
}

/// sTask_wake (���������� ����) ��������� ������
static inline sTask_wake(uint32_t* task_wake_flag)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__wake_flag]  \n\t"
                "svc    0x9                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wake_flag] "r" (task_wake_flag):);
}


/// sTask_wait (���������� ����) ���������� ������ � �������� �����
static inline sTask_wait(uint32_t* task_wait_flag)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__wait_flag]  \n\t"
                "svc    0x8                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wait_flag] "r" (task_wait_flag):);
}



/// sDelay_mc (� �������������)
static inline sDelay_mc(uint32_t Delay_mc)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__Delay_mc]   \n\t"
                "svc    0x7                 \n\t"
                "pop    {r2}                \n\t"
                :: [__Delay_mc] "r" (Delay_mc):);
}

/// ����� �S
/// ������� ���� � ��, ������ ����� �����,
///       > ������ ����� ����������, ��������� ����� ������ � �������������
static inline setup_run(uint32_t __SYSHCLK,
                        uint32_t _main_size,
                        uint32_t NVIC_size,
                        uint32_t task_time_us )
{
asm volatile    ("push {r4,lr} \n\t"
                  "ldr  r4, =sSustem_task               \n\t"
                  "str  %[___SYSHCLK], [r4, #28]        \n\t"
                  "str  %[__main_size], [r4, #24]       \n\t"
                  "str  %[__NVIC_size], [r4, #20]       \n\t"
                  "str  %[__task_time_us], [r4, #32]    \n\t"
                  "ldr  r4, =Start_task                 \n\t"
                  "blx  r4                              \n\t"
                  "pop {r4,lr}                          \n\t"
                  ::[___SYSHCLK] "r" (__SYSHCLK),[__main_size] "r" (_main_size),
                    [__NVIC_size] "r" (NVIC_size),[__task_time_us] "r" (task_time_us)
                  : "memory");
}

/// ������ ������������ GCC
static inline __memory(void){asm volatile ("nop" : : : "memory");}


/// �������� ����������, ��������� (�� 14 �� 1)
static inline  sNVIC_EnableIRQ(IRQn_Type IRQn, uint32_t priority)
{
asm volatile ("push     {r1, r2}       \n\t"
              "mov      r1, %[IRQ]      \n\t"
              "mov      r2, %[PRIORIT]  \n\t"
              "SVC      0x0             \n\t"
              "pop      {r1, r2}        \n\t"
               :
               : [IRQ] "r" (IRQn),[PRIORIT] "r" (priority):"memory");
}



/// ��������� ���������� - ����� ������� ��
__attribute__( ( always_inline ) ) static inline sNVIC_DisableIRQ(IRQn_Type IRQn)
{
asm volatile ("push    {r1}        \n\t"
               "mov     r1, %[IRQ]  \n\t"
               "SVC     0x1         \n\t"
               "pop     {r1}        \n\t"
               :
               : [IRQ] "r" (IRQn):"memory");
}




/// ����� ������ - ����� ������� ��
///  ������� , ������ ����� , ������� ������� 1-100 ,
///         ��������� �� ������ ���������� ����� ������ [4]
void sTask_new (void (*taskS_func()),
                uint32_t task_size,
                uint32_t task_time_rate,
                char* const task_func_name,
                void* task_func_massif4_data  )
{
register volatile uint32_t taskS_func__     asm     ("r0") = taskS_func;
register volatile uint32_t task_size__      asm     ("r1") = task_size;
register volatile uint32_t task_time_rate__ asm     ("r2") = task_time_rate;
register volatile uint32_t task_func_name__ asm     ("r3") = task_func_name;
register volatile uint32_t func_massif4__   asm     ("r4") = task_func_massif4_data;


    asm volatile  ( "push    {r4, r5, r6}   \n\t"
                     "SVC     0x4            \n\t"  // "_Zd_%=:"
                //    "cbz    r5, _Zc_%=      \n\t"
              //      "movw   r5, #0x1024     \n\t"
               //     "movt   r5, #0x4000     \n\t"
               //     "ldr    r6, [r5, #8]    \n\t"   // TIM6_ARR
               //     "str    r6, [r5]        \n\t"   // TIM6_CNT
               //     "nop                    \n\t"   // �������� �����
               //     "nop                    \n\t"   // ���������� �����
               //     "b      _Zd_%=          \n\t"
                     "pop     {r4, r5, r6}   \n\t" // "_Zc_%=:"
                  :
                  :  "r" (taskS_func), "r" (task_size), "r" (task_time_rate), "r" (task_func_name),
                     "r" (task_func_massif4_data)
                  : "memory" );
}



/// �������� ����� - ������ ������ ���������� ������
static inline sTask_skip (void)
{
asm volatile     ( "push   {r0, r1}            \n\t"
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
static inline sTask_kill(void){__asm volatile("SVC 0x5":::"memory");}


static uint32_t sRandom(uint32_t Random_max,uint32_t Random_min)
{
register volatile uint32_t Random_max__     asm     ("r0") = Random_max;
register volatile uint32_t Random_min__     asm     ("r1") = Random_min;
__ASM volatile("SVC    0x3             \n\t"
            : "=r" (Random_max__): "r" (Random_max), "r" (Random_min):);
return Random_max__;
}

#endif _RtoS.h
#define _RtoS.h
