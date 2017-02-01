/**
 @file    RtoS_cortex_m7.S
 @author  AVI-crak
 @version V-44%
 @date    28-�������-2016
 @brief   �xis sRtoS, Cortex-M7 ARM GCC EmBitz

 license
 Million helicopters and one cent.

 ����� ��� ����������
 http://forum.ixbt.com/topic.cgi?id=48:11735

 �����������
 https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc
*/


#ifndef _RtoS_

//#define _bitFlag volatile uint32_t __attribute__ ((section(".flag"))) // ��� �����


#pragma pack(push, 4)
/// sSystem_task - ������ �����
struct S_task
{
    struct task* activ;                 ///#00- ����� �������� ������, Active task pointer
    struct task* delay;                 ///#04- ����� ������ ������, Sleeping tsaks list pointer
    struct task* wait;                  ///#08- ����� ����� ��������� �����, Waiting tasks list pointer
    struct task* hold;                  ///#12- ����� ����� �� ��������� ������, Tasks on hold list pointer
    __IO uint16_t NVIC_size_max;        ///#16- ������� ���� ����������, Task maximum NVIC used stack
    __IO uint16_t NVIC_size;            ///#18- ������ ����� ����������, Task NVIC stack size
    __IO uint16_t task_list_zize_sys;   ///#20- ���������� ������ � �������
    __IO uint16_t task_list_zize_use;   ///#22- ���������� ������ � ������
    __IO uint32_t Booked_stack;         ///#24- ������� ����, booked stack address
    __IO uint32_t sSYSHCLK;             ///#28- ��������� �������, �� - �������� ��� ������
    __IO uint32_t tick_real;            ///#32- ������ ���������� ������ 100%, Task activity timer 100%
    __IO uint32_t malloc_start;         ///#36- ������ ����� malloc, First malloc address
    __IO uint32_t malloc_stop;          ///#40- ��������� ����� malloc, Last malloc address
    __I uint32_t task_stop;             ///#44- ��������� ����� �����, Stack last address
    __I uint32_t system_us;             ///#48- ��������� �����, System time counter
    __IO uint32_t spall_us;             ///#52- ������� �������
    __IO uint32_t norm_mc;              ///#56- ����� ������� (1mc)
    struct task** task_list;            ///#60- ������ ������
    union  _fla                         ///#64- ���� ������ ������
    {
        struct fl
        {
            uint32_t reliability_task_list:1;  /// ��������� task_list, 1 - ����� ������
            uint32_t stop:5;
            uint32_t a:1;
            uint32_t b:1;
            uint32_t c:1;
            uint32_t d:1;
            uint32_t e:1;
            uint32_t f:1;
            uint32_t g:1;
            uint32_t h:1;
            uint32_t i:1;
            uint32_t j:1;
            uint32_t k:1;
            uint32_t l:1;
            uint32_t m:1;
            uint32_t n:1;
            uint32_t o:1;
            uint32_t p:1;
            uint32_t q:1;
            uint32_t r:1;
            uint32_t s:1;
            uint32_t t:1;
            uint32_t u:1;
            uint32_t v:1;
            uint32_t w:1;
            uint32_t x:1;
            uint32_t y:1;
            uint32_t z:1;
        } flag;
        uint32_t flag_all;
    }sustem_flag;
}sSystem_task ={25,};
#pragma pack(pop)




struct  task
{
    struct task*    task_new;           /// 0x00, #00  32b,- ����� ����� ������, New task pointer
    struct task*    task_lid;           /// 0x04, #04, 32b,- ����� ������ ������, Old task pointer
    volatile uint32_t   last_stack;         /// 0x08, #08, 32b,- C��� ������, Stack pointer
    union{
    struct _nomer
        {
    const volatile uint16_t    task_nomer:9;       /// 0x0C, #12, 9b,- ����� �����, Task unique ID
    const volatile    uint16_t    fri:5;
    const volatile uint16_t    mode:2;             /// 0x0C, #12, >>14 2b - ����� �����
        }m_n;
    volatile uint16_t    ntt;
    }mode_nomer;
    volatile uint8_t    percent;            /// 0x0E, #14, 08b,- ������� ���������� ������, Task percentage usage
    volatile uint8_t    flag;               /// 0x0F, #15, 08b,- ���� ������� �� ���������, Processing request flag
    union{
    struct _wake
        {
    __IO uint8_t    delay;          /// 0x10, #16, 8b,- �������� ����������
    __IO uint8_t    wake1;          /// 0x11, #17, 8b,- ������ � ����
    __IO uint8_t    wake2;          /// 0x12, #18, 8b,- ������ � ����
    __IO uint8_t    wake3;          /// 0x13, #19, 8b,- ������ � ����
        }t_wake;
    volatile uint32_t   delay_wake;
    }d_wake;
    volatile uint32_t   life_time;          /// 0x14, #20, 32b,- ������ ���������� ������, Task activity timer
    volatile uint16_t   stack_zize;         /// 0x18, #24, 16b,- ������ �����, Task stack size
    volatile uint16_t   stack_max_zize;     /// 0x1A, #26, 16b,- ������� ����, Task maximum used stack
    const volatile char*      task_names;         /// 0x1C, #28, 32b,- ��� ������, Task name
};



volatile uint32_t Random_register[3];

/**

������� ������
7b - ����������
6b-0b - ����� ������� �� ���������
1 �������� ����� ������� ������
2 ������� ������� ������
3 ������� ��� ������� ������ ������
4 ��������� ����� ������
5 ������ ������
6 ����� ������


254


������
0x00, #00, 32b, 9b-H (����� ������) - 23b-L (����� ��������)
0x04, #04, 32b, ���� (������� 32b)


����� ��������� ���������� task_wake - hold
0x10, #16   16b-L (�������, ������� � ���������)
0x12, #18   8b-HL (����� ����������)
0x13, #19   8b-HH (����� ���������)

����� ����� mode
hold = 0
wait = 1
delay = 2
activ = 3

*/


/// ������ ������ ������ ����� ( & link_memory, "task_func_name") (������� �������������)
/// Release memory for another thread
static void sTask_memory_donate (uint32_t *link_memory, char* const task_func_name)//X �� ������
{
asm volatile  ( "svc    0xD                     \n\t"
                "nop                            \n\t"
                ::"r" (link_memory),"r" (task_func_name):"memory");
}

/// �������� ������ �� ������ ����� ("task_func_name") (������� �������������)
///Get memory from another threa
static uint32_t sTask_memory_get (char* task_func_name); //X �� ������
uint32_t sTask_memory_get (char* task_func_name)
{
register  uint32_t malloc_adres asm  ("r0") = task_func_name;
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
    tmp = *(sSystem_task.activ -> task_names);
    if ( *name_resource == tmp ) return; else;
    do
    {
        if(__LDREXW(name_resource) !=0)
            asm volatile   ("clrex              \n\t"
                            "mov   r3, #0x10    \n\t"
                            "svc    0x0         \n\t"
                            ::: "r3");else;
    }while ( __STREXW(tmp, name_resource));
}

/// ���������� ������
/// Release resourse
static void sTask_resource_free (uint32_t *name_resource);
void sTask_resource_free (uint32_t *name_resource)
{
    if ( *name_resource == *(sSystem_task.activ -> task_names) ) *name_resource = 0; else;
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
    register uint32_t   malloc_adres     asm  ("r0") = malloc_zize;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0xA            \n\t" //__nil_in 0xA;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    : "=r" (malloc_adres): "r" (malloc_adres):"memory");   // ���� ���������� �������
return malloc_adres;
}

/// ������� ������ (�����)
static void free (void* malloc_adres);
void free (void* malloc_adres)
{
    if  (malloc_adres ==0) return; else;
    if (sSystem_task.activ->mode_nomer.m_n.task_nomer != (( *(uint32_t*)(malloc_adres -4))>>23))return; else;
    sSystem_task.activ->flag = 2;
    register    void   *__malloc_adres     asm  ("r0") = malloc_adres;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0xA            \n\t" //__nil_in 0xA;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                  :: "r" (__malloc_adres):);   // ���� ���������� �������
}

/// sTask_wake (& ���������� ����) ��������� ������
static void sTask_wake(uint32_t* task_global_flag);
void sTask_wake(uint32_t* task_global_flag)
{
    register    uint32_t   *__task_global_flag     asm  ("r0") = task_global_flag;
    asm volatile    ("push      {r3}        \n\t"
                    "ldr        r3, [r0]    \n\t"
                    "cmp        r3, #0      \n\t"
                    "itt        ne          \n\t"
                    "movne      r3, 0x9     \n\t" //__sTask_wake //9;
                    "svcne      0x0         \n\t"
                    "pop        {r3}        \n\t"
                    :: "r" (__task_global_flag):);
}


/// sTask_wait (& ���������� ����) ���������� ������ � �������� �����
static void sTask_wait(uint32_t* task_global_flag);
void sTask_wait(uint32_t* task_global_flag)
{
    register    uint32_t   *__task_global_flag     asm  ("r0") = task_global_flag;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x8            \n\t" //__sTask_wait //8;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__task_global_flag):"memory");
}



/// sDelay_mc (� �������������)
static void sDelay_mc(uint32_t Delay_mc);
void sDelay_mc(uint32_t Delay_mc)
{
    register    uint32_t   __Delay_mc     asm  ("r0") = Delay_mc;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x7            \n\t" //__sDelay_new //7;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__Delay_mc):"memory");
}

void __attribute__ ((weak))Start_task(void);

/// ����� �S
/// ������� ���� � ��, ������ ����� �����,
///       > ������ ����� ����������, ��������� ����� ������ � �������������
static void setup_run(uint32_t __SYSHCLK,uint32_t _main_size,uint32_t NVIC_size);
void setup_run(uint32_t __SYSHCLK, uint32_t _main_size, uint32_t NVIC_size)
{
    sSystem_task.sSYSHCLK = __SYSHCLK;
    sSystem_task.Booked_stack = _main_size;
    sSystem_task.NVIC_size = (((NVIC_size +31) >> 5) << 5);
    sSystem_task.tick_real = (__SYSHCLK / 1000);
    sSystem_task.norm_mc = (__SYSHCLK / 1000) - 6;
    sSystem_task.task_list_zize_sys = 2;
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    #ifdef   __CM7_REV
    DWT->LAR = 0xC5ACCE55; //  �������������� ������ DWT->CYCCNT ��� Cortex-M7
    #endif
    DWT->CYCCNT =0;
    DWT->CTRL |=1; // enable the counter
    SCB->CCR |= SCB_CCR_USERSETMPEND_Msk;
    Start_task();
}


/// ������ ������������ GCC
static inline void __memory(void){asm volatile ("nop" : : : "memory");}

/// ��������� �������� ������� ���� ����������
void __attribute__ ((weak)) sHandler_zize(void);


/// �������� ����������, ��������� (�� 14 �� 0)
static void  sNVIC_EnableIRQ(IRQn_Type IRQn, uint8_t priority);
void  sNVIC_EnableIRQ(IRQn_Type IRQn, uint8_t priority)
{
    register uint8_t    __IRQn          asm  ("r0") = IRQn;
    register uint8_t    __priority      asm  ("r1") = priority;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x0            \n\t" //__EnableIRQS //0;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__IRQn), "r" (__priority):"memory");
}



/// ��������� ���������� - ����� ������� ��
static void sNVIC_DisableIRQ(IRQn_Type IRQn);
void sNVIC_DisableIRQ(IRQn_Type IRQn)
{
    register uint8_t    __IRQn          asm  ("r0") = IRQn;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x0            \n\t" //__DisableIRQS //1;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__IRQn):"memory");
}




/// ����� ������ - ����� ������� ��
///  ������� , ������ ����� , ������� ������� 1-100 ,
///         ��������� �� ������ ���������� ����� ������ [4]
static void sTask_new (void (*taskS_func),uint32_t task_size,uint8_t task_time_rate,char* const task_func_name,void* task_func_massif4_data  );
void sTask_new (void (*taskS_func),
                        uint32_t task_size,
                        uint8_t task_time_rate,
                        char* const task_func_name,
                        void* task_func_massif4_data  )
{
register void       *taskS_func__       asm     ("r0") = taskS_func;
register uint32_t   task_size__         asm     ("r1") = task_size;
if (task_time_rate >100) task_time_rate = 100;else;
if (task_time_rate < 1) task_time_rate = 1;else;
register uint32_t   task_time_rate__    asm     ("r2") = task_time_rate;
register uint32_t   task_nil_in         asm     ("r3") = 0xA; //A
register uint8_t    *task_func_name__   asm     ("r4") = task_func_name;
register uint32_t   *func_massif4__     asm     ("r5") = task_func_massif4_data;
sSystem_task.activ->flag = 6;
// �� ���� ������� � ������������� ���� ��������������� GCC
    asm volatile  ("svc    0x0             \n\t"  //
                  :: "r" (taskS_func__), "r" (task_size__), "r" (task_time_rate__),"r" (task_nil_in),
                     "r" (task_func_name__), "r" (func_massif4__) : "memory" );
}



/// �������� ����� - ������ ������ ���������� ������
__attribute__( ( always_inline ) ) static inline void sTask_skip (void)
{
    asm volatile    ("mov   r3, #0x10   \n\t"
                    "svc    0x0         \n\t"
                    ::: "r3");
}

/// ����� ������ - ������ ������ ���������� ������
void __attribute__ ((weak)) sTask_kill(void);

static uint32_t sRandom(uint32_t volatile Random_max,uint32_t volatile Random_min); //++
uint32_t sRandom(uint32_t Random_max,uint32_t Random_min)
{
    register  uint32_t Random_max__     asm     ("r0") = Random_max;
    register  uint32_t Random_min__     asm     ("r1") = Random_min;
    asm volatile    ("push  {r3}                \n\t"
                     "mov   r3, #3              \n\t"
                     "svc   0x0                 \n\t" //3
                     "pop   {r3}                \n\t"
                    : "=r" (Random_max__): "r" (Random_max__), "r" (Random_min__):"memory");
return Random_max__;
}

//
#ifdef   __CM7_REV
static uint32_t sRandom_m4(uint32_t Random_max,uint32_t Random_min); //++
uint32_t sRandom_m4(uint32_t Random_max,uint32_t Random_min)
{
    while(!(RNG->SR));
    uint32_t tmp1;
    uint32_t tmp2;
    uint32_t Random;
    if (Random_max < Random_min) return Random = 0;
    tmp1 =0;
     tmp1--;
      Random = Random_max - Random_min;
       tmp1 = tmp1 / Random;
        tmp2 = (RNG->DR);
         tmp2 = tmp2 / tmp1;
          Random = tmp2 + Random_min;
return Random;
}
#endif /* __CM7_REV */

 /*
/// ������� ���������� 8_t, ������� ��� ��������
static void qSort_8t(uint8_t *arr8_t, int32_t left, int32_t right);
void qSort_8t(uint8_t *arr8_t, int32_t left, int32_t right)
{
    int32_t iee_8, jee_8;
    int32_t tmp_8, pivot_8;
    pivot_8 = arr8_t[(left + right) / 2]; // ����������� �������
    iee_8 = left;
    jee_8 = right;
    while (iee_8 <= jee_8)
    {
        while (arr8_t[iee_8] < pivot_8) iee_8++;
        while (arr8_t[jee_8] > pivot_8) jee_8--;
        if (iee_8 <= jee_8)
        {
            tmp_8 = arr8_t[iee_8];
            arr8_t[iee_8] = arr8_t[jee_8];
            arr8_t[jee_8] = tmp_8;
            iee_8++;
            jee_8--;
        }
    };
    if (left < jee_8)
    {
        tmp_8 = jee_8 - left;
        if (tmp_8 > 128) qSort_8t(arr8_t, left, jee_8);
        else
        {
            jee_8++;
            for(; left < jee_8; left++)
            {
                pivot_8 = arr8_t[left];
                for (tmp_8 = left - 1; tmp_8 >= 0 && arr8_t[tmp_8] > pivot_8; tmp_8--)
                    {
                        arr8_t[tmp_8 + 1] = arr8_t[tmp_8];
                    };
            arr8_t[tmp_8 + 1] = pivot_8;
            };
        };
    };

    if (iee_8 < right)
    {
        tmp_8 = right - iee_8;
        if (tmp_8 > 128) qSort_8t(arr8_t, iee_8, right);
        else
        {
            right++;
            for(; iee_8 < right; iee_8++)
            {
                pivot_8 = arr8_t[iee_8];
                for (tmp_8 = iee_8 - 1; tmp_8 >= 0 && arr8_t[tmp_8] > pivot_8; tmp_8--)
                    {
                        arr8_t[tmp_8 + 1] = arr8_t[tmp_8];
                    };
            arr8_t[tmp_8 + 1] = pivot_8;
            };
        };
    };

};
*/


/// ������� ���������� 8_t, ������� ��� ��������
static void qSort_8t(uint8_t *arr8_t, int32_t left, int32_t right);
void qSort_8t(uint8_t *arr8_t, int32_t left, int32_t right)
{
    int32_t iee_8, jee_8;
    int32_t tmp_8, pivot_8;
    pivot_8 = arr8_t[(left + right) / 2]; // ����������� �������
    iee_8 = left;
    jee_8 = right;
    while (iee_8 <= jee_8)
    {
        while (arr8_t[iee_8] < pivot_8) iee_8++;
        while (arr8_t[jee_8] > pivot_8) jee_8--;
        if (iee_8 <= jee_8)
        {
            tmp_8 = arr8_t[iee_8];
            arr8_t[iee_8] = arr8_t[jee_8];
            arr8_t[jee_8] = tmp_8;
            iee_8++;
            jee_8--;
        }
    };
    if (left < jee_8) qSort_8t(arr8_t, left, jee_8);
    if (iee_8 < right) qSort_8t(arr8_t, iee_8, right);

};





/// ������� ���������� 16_t, ������� ��� ��������
static void qSort_16t(uint16_t *arr16_t, int32_t left, int32_t right);
void qSort_16t(uint16_t *arr16_t, int32_t left, int32_t right)
{
    int32_t iee, jee;
    int32_t tmp, pivot;
    pivot = arr16_t[(left + right) / 2]; // ����������� �������
    iee = left;
    jee = right;
    while (iee <= jee)
    {
        while (arr16_t[iee] < pivot) iee++;
        while (arr16_t[jee] > pivot) jee--;
        if (iee <= jee)
        {
            tmp = arr16_t[iee];
            arr16_t[iee] = arr16_t[jee];
            arr16_t[jee] = tmp;
            iee++;
            jee--;
        }
    };
    if (left < jee)
    {
        tmp = jee - left;
        if (tmp > 128) qSort_16t(arr16_t, left, jee);
        else
        {
            jee++;
            for(; left < jee; left++)
            {
                pivot = arr16_t[left];
                for (tmp = left - 1; tmp >= 0 && arr16_t[tmp] > pivot; tmp--)
                    {
                        arr16_t[tmp + 1] = arr16_t[tmp];
                    };
            arr16_t[tmp + 1] = pivot;
            };
        };
    };

    if (iee < right)
    {
        tmp = right - iee;
        if (tmp > 128) qSort_16t(arr16_t, iee, right);
        else
        {
            right++;
            for(; iee < right; iee++)
            {
                pivot = arr16_t[iee];
                for (tmp = iee - 1; tmp >= 0 && arr16_t[tmp] > pivot; tmp--)
                    {
                        arr16_t[tmp + 1] = arr16_t[tmp];
                    };
            arr16_t[tmp + 1] = pivot;
            };
        };
    };

};

/// ������� ���������� 32_t, ������� ��� ��������
static void qSort_32t(uint32_t *arr32_t, int32_t left, int32_t right);
void qSort_32t(uint32_t *arr32_t, int32_t left, int32_t right)
{
    int32_t iee, jee;
    int32_t tmp, pivot;
    pivot = arr32_t[(left + right) / 2]; // ����������� �������
    iee = left;
    jee = right;
    while (iee <= jee)
    {
        while (arr32_t[iee] < pivot) iee++;
        while (arr32_t[jee] > pivot) jee--;
        if (iee <= jee)
        {
            tmp = arr32_t[iee];
            arr32_t[iee] = arr32_t[jee];
            arr32_t[jee] = tmp;
            iee++;
            jee--;
        }
    };
    if (left < jee)
    {
        tmp = jee - left;
        if (tmp > 128) qSort_32t(arr32_t, left, jee);
        else
        {
            jee++;
            for(; left < jee; left++)
            {
                pivot = arr32_t[left];
                for (tmp = left - 1; tmp >= 0 && arr32_t[tmp] > pivot; tmp--)
                    {
                        arr32_t[tmp + 1] = arr32_t[tmp];
                    };
            arr32_t[tmp + 1] = pivot;
            };
        };
    };

    if (iee < right)
    {
        tmp = right - iee;
        if (tmp > 128) qSort_32t(arr32_t, iee, right);
        else
        {
            right++;
            for(; iee < right; iee++)
            {
                pivot = arr32_t[iee];
                for (tmp = iee - 1; tmp >= 0 && arr32_t[tmp] > pivot; tmp--)
                    {
                        arr32_t[tmp + 1] = arr32_t[tmp];
                    };
            arr32_t[tmp + 1] = pivot;
            };
        };
    };

};


static inline uint8_t unit_step (uint32_t in)
{
    uint8_t out;
    asm volatile    ("rbit  %[_out], %[_in]     \n\t"
                    "clz    %[_out], %[_out]    \n\t"
                    :[_out] "=r" (out) :[_in] "r" (in):);
    return (out);
}




#endif /* _RtoS_ */
#define _RtoS_








