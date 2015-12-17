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


#ifndef _RtoS_

//#define _bitFlag volatile uint32_t __attribute__ ((section(".flag"))) // ��� �����




/// sSustem_task - ������ �����
struct
{
  struct task* activ;               ///- ����� �������� ������
  struct task* delay;               ///- ����� ������ ������
  struct task* wait;                ///- ����� ����� ��������� �����
  struct task* hold;                ///- ����� ����� �� ��������� ������
  __IO uint32_t NVIC_size_max;      ///- �������������� ������ ����� ����������
  __IO uint32_t NVIC_size;          ///- ������ ����� ����������, ������� ��� ������
  __IO uint32_t Main_size_start;    ///- ������ ����� �����// ������� ����
  __IO uint32_t sSYSHCLK;           ///- ��������� �������, �� - �������� ��� ������
  __IO uint32_t tik_real;           ///- 100% ����� �� ������ // us �� ������ ��� ������
  __IO uint32_t malloc_start;       ///- ������ ����� malloc
  __IO uint32_t malloc_stop;        ///- ��������� ����� malloc
  __IO uint32_t task_stop;          ///- ��������� ����� �����
}sSustem_task ;



struct task
{
  struct task* task_new;            /// 0x00, #00  32b,- ����� ����� ������
  struct task* task_lid;            /// 0x04, #04, 32b,- ����� ������ ������
  __IO uint32_t last_stack;         /// 0x08, #08, 32b,- C��� ������ (�������� �����)
  __IO uint16_t life_time;          /// 0x0C, #12, 16b,- ������ ���������� � ������
  __IO uint8_t percent;             /// 0x0E, #14, 08b,- ���������� ������� ���������� ������
  __IO uint8_t flag;                /// 0x0F, #15, 08b,- ���� ������� �� ���������
  __IO uint32_t sleep_wake;         /// 0x10, #16, 32b,- ����� ��� (��) // ����� ����������� ����� �����
  __IO uint32_t data_address;       /// 0x14, #20, 32b,- Malloc_zize - malloc_adres // malloc_adres
  __IO uint16_t stack_zize;         /// 0x18, #24, 16b,- ������ ����� (������� ��� �������)
  __IO uint16_t stack_max_zize;     /// 0x1A, #26, 16b,- ��� ������ ����� (��� ��������� ������ �����)
  __IO char* task_names;            /// 0x1C, #28, 32b,- ����� ����� ������ (char* const text)
};



volatile uint32_t Random_register[3];

/**

���� ������� �� ���������
1 �������� ����� ������� ������
2 ������� ������� ������
3 ������� ��� ������� ������ ������
4 ��������� ����� ������

3 ������ ������� ������ �� ������ ����� � ������������� � �������� (������ ��������)
4 ������������� � ������� ������� ������ (������ ����������� ����� ����)
5 ������� ��������� ������� ��� ������
6 ������� ��������� ������� ��� ������
5 ��� ��� ���
254

������
0x00, #00, 32b, �������� �� ������ �����
0x04, #04, 32b, ��� ������
0x04, #08, 32b, ���� (������� 4b)
**/



/// �������� ������ (������)
static uint32_t malloc (uint32_t malloc_zize)
{
register volatile uint32_t malloc_adres asm  ("r0") = malloc_zize;
asm volatile ("push     {r1, r2, r3}        \n\t"
              "mov      r1, %[malloczize__] \n\t"
              "ldr      r3,  =sSustem_task  \n\t"
              "ldr      r3, [r3]            \n\t"   // �������� ������
              "cbnz     r1, _Za_%=          \n\t"
              "bkpt                         \n\t"   // ������� ������ ���������  ������
   "_Zaa_%=:" "mov      r2, #10             \n\t"   // ����� 10��
              "svc      0x7                 \n\t"
   "_Za_%=:"  "str      r1, [r3, #20]       \n\t"   // malloc_zize - malloc_adres // malloc_adres
              "mov      r2, #1              \n\t"   // ���� ��������� ��� sTask_nil, 1 = malloc_in
              "strb     r2, [r3, #15]       \n\t"   // ���� ������� �� ���������
              "svc      0xA                 \n\t"   // ���� ���������� �������
              "ldr      r2,  [r3, #20]      \n\t"
              "cmp      r2, #0              \n\t"
              "beq      _Zaa_%=             \n\t"   // ��������� ������� �����
              "mov      %[malloc_ad__], r2  \n\t"
              "pop      {r1, r2, r3}        \n\t"
               : [malloc_ad__] "=r" (malloc_adres)
               : [malloczize__] "r" (malloc_zize):  );
return malloc_adres;
}

/// ������� ������ (�����)
static void free (void* malloc_adres)
{
asm volatile ("push     {r0, r1}                  \n\t"
              "ldr      r1,  =sSustem_task        \n\t"
              "ldr      r1, [r1]                  \n\t"   // �������� ������
              "str      %[adres__], [r1, #20]     \n\t"   // malloc_adres
              "mov      %[adres__], #2            \n\t"   // ���� ��������� ��� sTask_nil, 2 = free (malloc_adres)
              "strb     %[adres__], [r1, #15]     \n\t"   // ���� ������� �� ���������
              "svc      0xA                       \n\t" // ���� ���������� �������
              "pop      {r0, r1}                  \n\t"
               :: [adres__] "r" (malloc_adres): "memory" );
}

/// sTask_wake (���������� ����) ��������� ������
__attribute__( ( always_inline ) ) static inline sTask_wake(uint32_t* task_wake_flag)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__wake_flag]  \n\t"
                "svc    0x9                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wake_flag] "r" (task_wake_flag):);
}


/// sTask_wait (���������� ����) ���������� ������ � �������� �����
__attribute__( ( always_inline ) ) static inline sTask_wait(uint32_t* task_wait_flag)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__wait_flag]  \n\t"
                "svc    0x8                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wait_flag] "r" (task_wait_flag):);
}



/// sDelay_mc (� �������������)
__attribute__( ( always_inline ) ) static inline sDelay_mc(uint32_t Delay_mc)
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
__attribute__( ( always_inline ) ) static inline setup_run(uint32_t __SYSHCLK,
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
__attribute__( ( always_inline ) ) static inline __memory(void){asm volatile ("nop" : : : "memory");}


/// �������� ����������, ��������� (�� 14 �� 1)
static inline  sNVIC_EnableIRQ(IRQn_Type IRQn, uint32_t priority)
{
asm volatile ("push     {r1, r2}       \n\t"
              "mov      r1, %[IRQ]      \n\t"
              "mov      r2, %[PRIORIT]  \n\t"
              "svc      0x0             \n\t"
              "pop      {r1, r2}        \n\t"
               :
               : [IRQ] "r" (IRQn),[PRIORIT] "r" (priority):"memory");
}



/// ��������� ���������� - ����� ������� ��
__attribute__( ( always_inline ) ) static inline sNVIC_DisableIRQ(IRQn_Type IRQn)
{
asm volatile ("push    {r1}        \n\t"
               "mov     r1, %[IRQ]  \n\t"
               "svc     0x1         \n\t"
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
     "_Zd_%=:"      "svc     0x4            \n\t"  //
                    "cbz    r5, _Zc_%=      \n\t"
                    "movw   r5, #0x1024     \n\t"
                    "movt   r5, #0x4000     \n\t"
                    "ldr    r6, [r5, #8]    \n\t"   // TIM6_ARR
                    "str    r6, [r5]        \n\t"   // TIM6_CNT
                    "nop                    \n\t"   // �������� �����
                    "nop                    \n\t"   // ���������� �����
                    "b      _Zd_%=          \n\t"
     "_Zc_%=:"      "pop     {r4, r5, r6}   \n\t"
                  :
                  :  "r" (taskS_func), "r" (task_size), "r" (task_time_rate), "r" (task_func_name),
                     "r" (task_func_massif4_data)
                  : "memory" );
}



/// �������� ����� - ������ ������ ���������� ������
__attribute__( ( always_inline ) ) static inline sTask_skip (void)
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
// static void sTask_kill(void)


static uint32_t sRandom(uint32_t Random_max,uint32_t Random_min)
{
register volatile uint32_t Random_max__     asm     ("r0") = Random_max;
register volatile uint32_t Random_min__     asm     ("r1") = Random_min;
__ASM volatile("svc    0x3             \n\t"
            : "=r" (Random_max__): "r" (Random_max), "r" (Random_min):);
return Random_max__;
}

#endif _RtoS_
#define _RtoS_
