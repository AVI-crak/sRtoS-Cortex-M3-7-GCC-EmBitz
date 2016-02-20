/// Cortex-M3 GCC EmBitz 0.40
/// ��� �����
/// RtoS.h
/// ������� ���������� 40%
/// ������ rom 2446b� ram 128b*n + 64b

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
  struct task* activ;               ///#00- ����� �������� ������
  struct task* delay;               ///#04- ����� ������ ������
  struct task* wait;                ///#08- ����� ����� ��������� �����
  struct task* hold;                ///#12- ����� ����� �� ��������� ������
  __IO uint32_t NVIC_size_max;      ///#16- �������������� ������ ����� ����������
  __IO uint32_t NVIC_size;          ///#20- ������ ����� ����������, ������� ��� ������
  __IO uint32_t Main_size_start;    ///#24- ������ ����� �����// ������� ����
  __IO uint32_t sSYSHCLK;           ///#28- ��������� �������, �� - �������� ��� ������
  __IO uint32_t tik_real;           ///#32- 100% ����� �� ������ // us �� ������ ��� ������
  __IO uint32_t malloc_start;       ///#36- ������ ����� malloc
  __IO uint32_t malloc_stop;        ///#40- ��������� ����� malloc
  __IO uint32_t task_stop;          ///#44- ��������� ����� �����
  __IO uint32_t alarm_mc;           ///#48- ����� ��� �������� ���������� �������

}sSustem_task ;


/*
struct
{
    uint16_t task_nomer_f[10];
    char* task_names_f[10];

}sSustem_tas;
//sSustem_task.activ -> task_names
*/



volatile struct  task
{
  struct task* task_new;            /// 0x00, #00  32b,- ����� ����� ������
  struct task* task_lid;            /// 0x04, #04, 32b,- ����� ������ ������
  __IO uint32_t last_stack;         /// 0x08, #08, 32b,- C��� ������ (�������� �����)
  __IO uint16_t life_time;          /// 0x0C, #12, 16b,- ������ ���������� � ������
  __IO uint8_t percent;             /// 0x0E, #14, 08b,- ���������� ������� ���������� ������
  __IO uint8_t flag;                /// 0x0F, #15, 08b,- ���� ������� �� ���������
  __IO uint32_t sleep_wake;         /// 0x10, #16, 32b,- ����� ��� (��) // ����� ����������� ����� �����
  //__IO uint32_t data_address;       /// 0x14, #20, 32b,- Malloc_zize - malloc_adres // malloc_adres
  __IO uint16_t task_nomer;         /// 0x14, #20, 16b,- ���������� ����� ����� - �������� ����������
  __IO uint16_t task_xxxxx;         /// 0x16, #22, 16b,- �����
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
5 ������ ������
6 ������� ������

254


������
0x00, #00, 32b, 9b (����� ������) - 23b (����� ��������)
0x04, #04, 32b, ���� (������� 32b)
*/
/*
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

void fputc(uint32_t ch) {
 // if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
*/
/*
void sustem_data (void)
{
    uint32_t *tmp1;
    uint32_t *tmp2 = 0;
    uint32_t tmp3 = 0;
    uint32_t tmp4;
    tmp1 = sSustem_task.activ;
    tmp2 = *tmp1;
    while (!(tmp1 == tmp2))
        {
            tmp2 = *tmp2;
            sSustem_tas.task_nomer_f [tmp3] = *(tmp2 +5);
            sSustem_tas.task_names_f [tmp3] = *(tmp2 +7);
            tmp3++;

        }

}
*/

/// ������ ������ ������ ����� ( & link_memory, "task_func_name") (������� �������������)
static void sTask_memory_donate (uint32_t *link_memory, char* const task_func_name)
{
asm volatile  ( "svc    0xD                     \n\t"
                "nop                            \n\t"
                ::"r" (link_memory),"r" (task_func_name):"memory");
}

/// �������� ������ �� ������ ����� ("task_func_name") (������� �������������)
static uint32_t sTask_memory_have (char* const task_func_name)
{
register volatile uint32_t malloc_adres asm  ("r0") = task_func_name;
asm volatile  ( "svc    0xE                     \n\t"
                "nop                            \n\t"
                :"=r" (malloc_adres)
                :"r" (task_func_name):"memory");
return malloc_adres;
}


/// ������ �������, ����������� ���� - ���� �� �����������
//__attribute__( ( always_inline ) )
static inline sTask_resource_ask (uint32_t *name_resource)
{
    while ( *name_resource ) {TIM6 -> CNT = TIM6 -> ARR;};
        *name_resource = sSustem_task.activ -> task_names;
}

/// ���������� ������
static void sTask_resource_free (uint32_t *name_resource);
void sTask_resource_free (uint32_t *name_resource)
{
    if ( *name_resource == sSustem_task.activ -> task_names ) *name_resource = 0; else;
}

/// ������� ����������� ������� �� ������������ �������
/// ������
/// static uint32_t alarm_mc2;
/// if (sTask_alarm_mc(&alarm_mc1,1000)) { �������� ������ ����� ������� }
static uint32_t sTask_alarm_mc(uint32_t * timer_name, uint32_t timer_mc)
{
    uint32_t alarm;

    if ( *timer_name ==0)
    {
        if  ((sSustem_task.alarm_mc + timer_mc) < sSustem_task.alarm_mc)
            alarm = 0; else *timer_name = sSustem_task.alarm_mc + timer_mc;
    }else
    {
        if  (sSustem_task.alarm_mc > *timer_name)
           {alarm = 1; *timer_name = 0;}
            else alarm =0;
    }

return alarm;
}


/// �������� ������ (������)
static uint32_t malloc (uint32_t malloc_zize);
uint32_t malloc (uint32_t malloc_zize)
{
sSustem_task.activ->flag = 1;
register *malloc_adres  asm ("r0") ;
asm volatile ("mov      r0, %[malloc_zize_] \n\t"
              "svc      0xA                 \n\t"
              : "=r" (malloc_adres): [malloc_zize_] "r" (malloc_zize):"memory");   // ���� ���������� �������
return malloc_adres;
}

/// ������� ������ (�����)
static void free (void* malloc_adres);
void free (void* malloc_adres)
{
    sSustem_task.activ->flag = 2;
    asm volatile ("mov      r0, %[malloc_ad_]   \n\t"
                  "svc      0xA                 \n\t"
                  :: [malloc_ad_] "r" (malloc_adres): "r0","memory");   // ���� ���������� �������
}

/// sTask_wake (���������� ����) ��������� ������
static inline sTask_wake(uint32_t* task_global_flag)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__wake_flag]  \n\t"
                "svc    0x9                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wake_flag] "r" (task_global_flag):);
}


/// sTask_wait (���������� ����) ���������� ������ � �������� �����
static inline sTask_wait(uint32_t* task_global_flag)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__wait_flag]  \n\t"
                "svc    0x8                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wait_flag] "r" (task_global_flag):"memory");
}



/// sDelay_mc (� �������������)
static inline sDelay_mc(uint32_t Delay_mc)
{
asm volatile   ("push   {r2}                \n\t"
                "mov    r2, %[__Delay_mc]   \n\t"
                "svc    0x7                 \n\t"
                "pop    {r2}                \n\t"
                :: [__Delay_mc] "r" (Delay_mc):"memory");
}

/// ����� �S
/// ������� ���� � ��, ������ ����� �����,
///       > ������ ����� ����������, ��������� ����� ������ � �������������
static void setup_run(uint32_t __SYSHCLK,uint32_t _main_size,uint32_t NVIC_size,uint32_t task_time_us );
void setup_run(uint32_t __SYSHCLK,
                        uint32_t _main_size,
                        uint32_t NVIC_size,
                        uint32_t task_time_us )
{
    sSustem_task.sSYSHCLK = __SYSHCLK;
    sSustem_task.Main_size_start = _main_size;
    sSustem_task.NVIC_size = NVIC_size;
    sSustem_task.tik_real = task_time_us;
    Start_task();
}


/// ������ ������������ GCC
static inline __memory(void){asm volatile ("nop" : : : "memory");}


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
static inline sNVIC_DisableIRQ(IRQn_Type IRQn)
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
static void sTask_new (void (*taskS_func()),uint32_t task_size,uint32_t task_time_rate,char* const task_func_name,void* task_func_massif4_data  );
void sTask_new (void (*taskS_func()),
                        uint32_t task_size,
                        uint32_t task_time_rate,
                        char* const task_func_name,
                        void* task_func_massif4_data  )
{
register volatile   *taskS_func__     asm     ("r0") = taskS_func;
register volatile   *task_size__      asm     ("r1") = task_size;
register volatile   *task_time_rate__ asm     ("r2") = task_time_rate;
register volatile   *task_func_name__ asm     ("r3") = task_func_name;
register volatile   *func_massif4__   asm     ("r4") = task_func_massif4_data;

// �� ���� ������� � ������������� ���� ��������������� GCC
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
                  :  "r" (taskS_func__), "r" (task_size__), "r" (task_time_rate__), "r" (task_func_name__),
                     "r" (func_massif4__)
                  : "memory" );
}



/// �������� ����� - ������ ������ ���������� ������
__attribute__( ( always_inline ) ) static inline sTask_skip (void)
{
    TIM6 -> CNT = TIM6 -> ARR;
}

/// ����� ������ - ������ ������ ���������� ������
//static void sTask_kill(void)

static uint32_t sRandom(uint32_t Random_max,uint32_t Random_min);
uint32_t sRandom(uint32_t Random_max,uint32_t Random_min)
{
register volatile uint32_t *Random_max__     asm     ("r0") = Random_max;
register volatile uint32_t *Random_min__     asm     ("r1") = Random_min;
asm volatile ("svc    0x3             \n\t"
              : "=r" (Random_max__): "r" (Random_max__), "r" (Random_min__):"memory");
return Random_max__;
}



#endif _RtoS_
#define _RtoS_








