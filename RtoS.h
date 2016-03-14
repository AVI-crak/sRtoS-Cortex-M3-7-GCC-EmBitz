/// Cortex-M3 GCC EmBitz 0.40
/// ��� �����
/// RtoS.h
/// ������� ���������� 41,5%
/// ������ rom 2446b� ram 128b*n + 64b

/// ���� ��� ����������������
/// videocrak@maol.ru
/// ����� ��� ����������
/// http://forum.ixbt.com/topic.cgi?id=48:11735

/// �����������
/// https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc


#ifndef _RtoS_

//#define _bitFlag volatile uint32_t __attribute__ ((section(".flag"))) // ��� �����


/// sSystem_task - ������ �����
struct
{
  struct task* activ;               ///#00- ����� �������� ������, Active task pointer
  struct task* delay;               ///#04- ����� ������ ������, Sleeping tsaks list pointer
  struct task* wait;                ///#08- ����� ����� ��������� �����, Waiting tasks list pointer
  struct task* hold;                ///#12- ����� ����� �� ��������� ������, Tasks on hold list pointer
  __IO uint32_t NVIC_size_max;      ///#16- ������� ���� ����������, Task maximum NVIC used stack
  __IO uint32_t NVIC_size;          ///#20- ������ ����� ����������, Task NVIC stack size
  __IO uint32_t Main_size_start;    ///#24-
  /// ����� - ������ ����� �����, Task MAIN stack size
  /// ������ - ������� ����, booked stack address
  __IO uint32_t sSYSHCLK;           ///#28- ��������� �������, �� - �������� ��� ������
  __IO uint32_t tick_real;          ///#32- ������ - ������ ���������� ������ 100%, Task activity timer 100%
  __IO uint32_t malloc_start;       ///#36- ������ ����� malloc, First malloc address
  __IO uint32_t malloc_stop;        ///#40- ��������� ����� malloc, Last malloc address
  __IO uint32_t task_stop;          ///#44- ��������� ����� �����, Stack last address
  __IO uint32_t system_us;          ///#48- ��������� �����, System time counter
  __IO uint32_t spall_us;           ///#52- ������� �������
  __IO uint32_t norm_mc;            ///#56- ����� ������� (1mc)
  __IO uint32_t task_amt;           ///#60- ���������� ������ - ��������

}sSystem_task ;



struct  task
{
    struct task* task_new;          /// 0x00, #00  32b,- ����� ����� ������, New task pointer
    struct task* task_lid;          /// 0x04, #04, 32b,- ����� ������ ������, Old task pointer
    __IO uint32_t last_stack;       /// 0x08, #08, 32b,- C��� ������, Stack pointer
    __IO uint16_t task_nomer;       /// 0x0C, #12, 16b,- ����� �����, Task unique ID
    __IO uint8_t percent;           /// 0x0E, #14, 08b,- ������� ���������� ������, Task percentage usage
    __IO uint8_t flag;              /// 0x0F, #15, 08b,- ���� ������� �� ���������, Processing request flag
    __IO uint32_t task_wake;        /// 0x10, #16, 32b,
  /// struct task* delay - ����� ��� (��), Sleeping time
  /// struct task* wait - ����� ����������� ����� �����, Address global flag kick
  /// struct task* hold - ���������� ��������� malloc, Malloc processing variable
    __IO uint32_t life_time;        /// 0x14, #20, 32b,- ������ ���������� ������, Task activity timer
    __IO uint16_t stack_zize;       /// 0x18, #24, 16b,- ������ �����, Task stack size
    __IO uint16_t stack_max_zize;   /// 0x1A, #26, 16b,- ������� ����, Task maximum used stack
    __IO char* task_names;          /// 0x1C, #28, 32b,- ��� ������, Task name
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
0x00, #00, 32b, 9b-H (����� ������) - 23b-L (����� ��������)
0x04, #04, 32b, ���� (������� 32b)
*/


/// ������ ������ ������ ����� ( & link_memory, "task_func_name") (������� �������������)
/// Release memory for another thread
static void sTask_memory_donate (uint32_t *link_memory, char* const task_func_name)
{
asm volatile  ( "svc    0xD                     \n\t"
                "nop                            \n\t"
                ::"r" (link_memory),"r" (task_func_name):"memory");
}

/// �������� ������ �� ������ ����� ("task_func_name") (������� �������������)
///Get memory from another threa
static uint32_t sTask_memory_get (char* const task_func_name)
{
register volatile uint32_t malloc_adres asm  ("r0") = task_func_name;
asm volatile  ( "svc    0xE                     \n\t"
                "nop                            \n\t"
                :"=r" (malloc_adres)
                :"r" (task_func_name):"memory");
return malloc_adres;
}


/// ������ �������, ����������� ���� - ���� �� �����������
/// Resource request, endless loop while resource not released
static void sTask_resource_ask (uint32_t *name_resource);
void sTask_resource_ask (uint32_t *name_resource)
{
    uint32_t tmp;
    tmp = sSystem_task.activ -> task_names;
    if ( *name_resource == tmp ) return; else;
    do
    {
        if(__LDREXW(name_resource) !=0) asm volatile  ( "svc    0x10    ");else;
    }while ( __STREXW(tmp, name_resource));
    __CLREX();

}

/// ���������� ������
/// Release resourse
static void sTask_resource_free (uint32_t *name_resource);
void sTask_resource_free (uint32_t *name_resource)
{
    if ( *name_resource == sSystem_task.activ -> task_names ) *name_resource = 0; else;
}

/// ������� ����������� ������� �� ������������ �������
/// ������
/// static uint32_t alarm_mc2;
/// if (sTask_alarm_mc(&alarm_mc1,1000)) { �������� ������ ����� ������� }
static uint32_t sTask_alarm_mc(uint32_t * timer_name, uint32_t timer_mc);
uint32_t sTask_alarm_mc(uint32_t * timer_name, uint32_t timer_mc)
{
    uint32_t alarm;

    if ( *timer_name ==0)
    {
        if  ((sSystem_task.system_us + timer_mc) < sSystem_task.system_us)
            alarm = 0; else *timer_name = sSystem_task.system_us + timer_mc;
    }else
    {
        if  (sSystem_task.system_us > *timer_name)
           {alarm = 1; *timer_name = 0;}
            else alarm =0;
    }

return alarm;
}


/// �������� ������ (������)
static uint32_t malloc (uint32_t malloc_zize);
uint32_t malloc (uint32_t malloc_zize)
{
sSystem_task.activ->flag = 1;
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
    sSystem_task.activ->flag = 2;
    asm volatile ("mov      r0, %[malloc_ad_]   \n\t"
                  "svc      0xA                 \n\t"
                  :: [malloc_ad_] "r" (malloc_adres): "r0","memory");   // ���� ���������� �������
}

/// sTask_wake (& ���������� ����) ��������� ������
static inline sTask_wake(uint32_t* task_global_flag)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__wake_flag]  \n\t"
                "svc    0x9                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wake_flag] "r" (task_global_flag):);
}


/// sTask_wait (& ���������� ����) ���������� ������ � �������� �����
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
static void setup_run(uint32_t __SYSHCLK,uint32_t _main_size,uint32_t NVIC_size);
void setup_run(uint32_t __SYSHCLK, uint32_t _main_size, uint32_t NVIC_size)
{
    sSystem_task.sSYSHCLK = __SYSHCLK;
    sSystem_task.Main_size_start = (((_main_size+31) >> 5) << 5);
    sSystem_task.NVIC_size = (((NVIC_size +31) >> 5) << 5);
    sSystem_task.tick_real = (__SYSHCLK / 1000);
    sSystem_task.norm_mc = (__SYSHCLK / 1000) - 3;
    sSystem_task.task_amt = 2;
    CoreDebug-> DEMCR |= 0x01000000;
    DWT->CYCCNT =0;
    DWT->CTRL |=1; // enable the counter
    SCB->CCR |= SCB_CCR_USERSETMPEND;
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
if (task_time_rate >100) task_time_rate = 100;else;
if (task_time_rate < 1) task_time_rate = 1;else;
register volatile   *task_time_rate__ asm     ("r2") = task_time_rate;
register volatile   *task_func_name__ asm     ("r3") = task_func_name;
register volatile   *func_massif4__   asm     ("r4") = task_func_massif4_data;
// �� ���� ������� � ������������� ���� ��������������� GCC
    asm volatile  ( "push    {r4, r5, r6}   \n\t"
                    "svc     0x4            \n\t"  //
                    "pop     {r4, r5, r6}   \n\t"
                  :
                  :  "r" (taskS_func__), "r" (task_size__), "r" (task_time_rate__), "r" (task_func_name__),
                     "r" (func_massif4__)
                  : "memory" );
sSystem_task.task_amt++;
}



/// �������� ����� - ������ ������ ���������� ������
__attribute__( ( always_inline ) ) static inline sTask_skip (void)
{
asm volatile  ( "svc    0x10    ");
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








