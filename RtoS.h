/// Cortex-M3 GCC EmBitz 0.40
/// ��� �����
/// RtoS.h
/// ������� ���������� 42%
/// ������ rom 2446b� ram 128b*n + 64b

/// ���� ��� ����������������
/// videocrak@maol.ru
/// ����� ��� ����������
/// http://forum.ixbt.com/topic.cgi?id=48:11735

/// �����������
/// https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc


#ifndef _RtoS_

//#define _bitFlag volatile uint32_t __attribute__ ((section(".flag"))) // ��� �����

#define __STACK_Mx3  32

/// sSystem_task - ������ �����
struct
{
    struct task* activ;                 ///#00- ����� �������� ������, Active task pointer
    struct task* delay;                 ///#04- ����� ������ ������, Sleeping tsaks list pointer
    struct task* wait;                  ///#08- ����� ����� ��������� �����, Waiting tasks list pointer
    struct task* hold;                  ///#12- ����� ����� �� ��������� ������, Tasks on hold list pointer
  //  __IO uint32_t NVIC_size_max;        ///#16- ������� ���� ����������, Task maximum NVIC used stack
  //  __IO uint32_t NVIC_size;            ///#20- ������ ����� ����������, Task NVIC stack size
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
    __I uint32_t *task_list;            ///#60- ������ ������
}sSystem_task ;

/// ����� ��������� ����������, State mode hold
struct t_wake {
    __IO uint8_t    delay;              /// 0x10, #16, 8b,- �������� ����������
    __IO uint8_t    wake1;              /// 0x11, #17, 8b,- ������ � ����
    __IO uint8_t    wake2;              /// 0x12, #18, 8b,- ������ � ����
    __IO uint8_t    wake3;              /// 0x13, #19, 8b,- ������ � ����
    };

struct  task
{
    struct task* task_new;          /// 0x00, #00  32b,- ����� ����� ������, New task pointer
    struct task* task_lid;          /// 0x04, #04, 32b,- ����� ������ ������, Old task pointer
    __IO uint32_t last_stack;       /// 0x08, #08, 32b,- C��� ������, Stack pointer
    __IO uint16_t task_nomer;       /// 0x0C, #12, 16b,- ����� �����, Task unique ID
    __IO uint8_t percent;           /// 0x0E, #14, 08b,- ������� ���������� ������, Task percentage usage
    __IO uint8_t flag;              /// 0x0F, #15, 08b,- ���� ������� �� ���������, Processing request flag
    struct t_wake task_wake;        /// 0x10, #16, 32b,
  /// struct task* delay - ����� ��� (��), Sleeping time
  /// struct task* wait - ����� ����������� ����� �����, Address global flag kick
  /// struct task* hold - ����� ��������� ����������, State mode hold
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
6 ����� ������


254


������
0x00, #00, 32b, 9b-H (����� ������) - 23b-L (����� ��������)
0x04, #04, 32b, ���� (������� 32b)


����� ��������� ���������� task_wake - hold
0x10, #16   16b-L (�������, ������� � ���������)
0x12, #18   8b-HL (����� ����������)
0x13, #19   8b-HH (����� ���������)



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
static uint32_t sTask_memory_get (char* const task_func_name); //X �� ������
uint32_t sTask_memory_get (char* const task_func_name)
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
        if(__LDREXW(name_resource) !=0)
            asm volatile   ("mov   r3, #0x10   \n\t"
                            "svc    0x0         \n\t"
                            ::: "r3");else;
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
    register volatile   *malloc_adres     asm  ("r0") = malloc_zize;
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
    if (sSystem_task.activ->task_nomer != (( *(uint32_t*)(malloc_adres -4))>>23))return; else;
    sSystem_task.activ->flag = 2;
    register volatile   __malloc_adres     asm  ("r0") = malloc_adres;
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
    register volatile   *__task_global_flag     asm  ("r0") = task_global_flag;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x9            \n\t" //__sTask_wake //9;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__task_global_flag):);
}


/// sTask_wait (& ���������� ����) ���������� ������ � �������� �����
static void sTask_wait(uint32_t* task_global_flag);
void sTask_wait(uint32_t* task_global_flag)
{
    register volatile   *__task_global_flag     asm  ("r0") = task_global_flag;
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
    register volatile   *__Delay_mc     asm  ("r0") = Delay_mc;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x7            \n\t" //__sDelay_new //7;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__Delay_mc):"memory");
}

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
    sSystem_task.norm_mc = (__SYSHCLK / 1000) - 3;
    sSystem_task.task_list_zize_sys = 2;
    CoreDebug-> DEMCR |= 0x01000000;
    DWT->CYCCNT =0;
    DWT->CTRL |=1; // enable the counter
    SCB->CCR |= SCB_CCR_USERSETMPEND;
    Start_task();
}


/// ������ ������������ GCC
static inline __memory(void){asm volatile ("nop" : : : "memory");}


/// �������� ����������, ��������� (�� 14 �� 1)
static void  sNVIC_EnableIRQ(IRQn_Type IRQn, uint32_t priority);
void  sNVIC_EnableIRQ(IRQn_Type IRQn, uint32_t priority)
{
    register volatile   __IRQn          asm  ("r0") = IRQn;
    register volatile   *__priority     asm ("r1") = priority;
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
    register volatile   __IRQn     asm  ("r0") = IRQn;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x0            \n\t" //__DisableIRQS //1;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__IRQn):"memory");
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
register volatile   *task_nil_in      asm     ("r3") = 0xA; //A
register volatile   *task_func_name__ asm     ("r4") = task_func_name;
register volatile   *func_massif4__   asm     ("r5") = task_func_massif4_data;
sSystem_task.activ->flag = 6;
// �� ���� ������� � ������������� ���� ��������������� GCC
    asm volatile  ("svc    0x0             \n\t"  //
                  :: "r" (taskS_func__), "r" (task_size__), "r" (task_time_rate__),"r" (task_nil_in),
                     "r" (task_func_name__), "r" (func_massif4__) : "memory" );
}



/// �������� ����� - ������ ������ ���������� ������
__attribute__( ( always_inline ) ) static inline sTask_skip (void)
{
    asm volatile    ("mov   r3, #0x10   \n\t"
                    "svc    0x0         \n\t"
                    ::: "r3");
}

/// ����� ������ - ������ ������ ���������� ������
//static void sTask_kill(void)

static uint32_t sRandom(uint32_t Random_max,uint32_t Random_min); //++
uint32_t sRandom(uint32_t Random_max,uint32_t Random_min)
{
    register volatile uint32_t *Random_max__     asm     ("r0") = Random_max;
    register volatile uint32_t *Random_min__     asm     ("r1") = Random_min;
    asm volatile    ("push  {r3}                \n\t"
                     "mov   r3, #3              \n\t"
                     "svc   0x0                 \n\t" //3
                     "pop   {r3}                \n\t"
                    : "=r" (Random_max__): "r" (Random_max__), "r" (Random_min__):"memory");
return Random_max__;
}

/*
static void sTask_nil_zz(void);
void sTask_nil_zz(void)
{
    uint32_t *hold_a;
    uint32_t *hold_a_delay = 0;
    uint8_t   step;
    uint32_t malloc_zize;
    uint32_t malloc_adres;
    uint32_t malloc_start;
    uint16_t malloc_nomer;
    uint32_t malloc_zize_tmp;
    uint32_t malloc_name_tmp;
    uint16_t malloc_nomer_tmp;
    uint32_t tmp;

for(;;)
{
    nil_re:
    if (sSystem_task.hold == 0)
    {
        if(sSystem_task.activ == sSystem_task.activ->task_new)
        {
            asm volatile("mov   r3, #0x10   \n\t"   // __switch 0x10
                         "svc    0x0         \n\t" :::"r3");
            goto nil_re;
        }else
        {
            asm volatile ("wfi"); goto nil_re;
        };
    }else;
    if (sSystem_task.hold->task_wake.delay !=0)
    {
        sSystem_task.hold->task_wake.delay--;
        asm volatile("mov   r3, #0xF     \n\t" //__nil_step 0xF
                    "svc    0x0         \n\t" :::"r3");
        goto nil_re;
    }else;
    if (sSystem_task.hold->task_wake.wake1 !=0)
    {
        step = sSystem_task.hold->task_wake.wake1;
        sSystem_task.hold->task_wake.wake1 = sSystem_task.hold->task_wake.wake2;
        sSystem_task.hold->task_wake.wake2 = sSystem_task.hold->task_wake.wake3;
        sSystem_task.hold->task_wake.wake3 = 0;
    }else step = sSystem_task.hold->flag;

    switch (step)
     {
        case 0:  // ����0 - ������
            asm volatile("mov   r3, #0xB     \n\t" //__nil_exit 0xB
                        "svc    0x0         \n\t" :::"r3");
            goto nil_re;

        case 1: // ���� 1, _maloc_in
            if (sSystem_task.hold->task_wake.wake1 != 0) malloc_zize = sSystem_task.task_list_zize_use;
            else malloc_zize = ( *(uint32_t*)(sSystem_task.hold->last_stack + __STACK_Mx3));
            if ((malloc_zize ==0)&& ((malloc_zize>>23) >0))
                {
                    ( *(uint32_t*)(sSystem_task.hold->last_stack + __STACK_Mx3)) = (uint32_t) 0 - 1;
                    asm volatile("mov   r3, #0xB     \n\t" //__nil_exit 0xB
                        "svc    0x0         \n\t" :::"r3");
                    goto nil_re;
                }else;
            malloc_zize = (malloc_zize + 7) >>2;
            malloc_start = sSystem_task.malloc_start;
        maloc_in_for:
                malloc_name_tmp = ( *(uint32_t*)(malloc_start));
                malloc_zize_tmp = (malloc_name_tmp <<9)>>9;
                if (malloc_name_tmp ==0)
                    {
                        malloc_zize_tmp = malloc_zize;
                        if (sSystem_task.task_stop < (malloc_start + (malloc_zize_tmp << 2)))
                        {
                            sSystem_task.hold->task_wake.delay = 20;
                            sSystem_task.hold->task_wake.wake3 = sSystem_task.hold->task_wake.wake2;
                            sSystem_task.hold->task_wake.wake2 = sSystem_task.hold->task_wake.wake1;
                            sSystem_task.hold->task_wake.wake1 = 1;
                            asm volatile("mov   r3, #0xF     \n\t" //__nil_step 0xF
                                         "svc    0x0         \n\t" :::"r3");
                            goto nil_re;
                        }else;
                        if (sSystem_task.malloc_stop < (malloc_start + (malloc_zize_tmp << 2)))
                            sSystem_task.malloc_stop = malloc_start + (malloc_zize_tmp << 2); else;
                        *(uint32_t*)(malloc_start + (malloc_zize_tmp << 2)) = 0;
                        if (sSystem_task.hold->task_wake.wake1 != 0) malloc_name_tmp = 1;
                        else malloc_name_tmp = sSystem_task.hold->task_nomer;
                        *(uint32_t*) malloc_start = malloc_zize_tmp | (malloc_name_tmp <<23);
                        tmp = malloc_start + 4; // ������� �����
                        if (sSystem_task.hold->task_wake.wake1 != 0) sSystem_task.task_list = tmp;
                        else *(uint32_t*)(sSystem_task.hold->last_stack + __STACK_Mx3) = tmp;
                        if (sSystem_task.hold->task_wake.wake1 == 0)
                                asm volatile("mov   r3, #0xB     \n\t" //__nil_exit 0xB
                                            "svc    0x0         \n\t" :::"r3");
                            else; goto nil_re;
                    }else;

                malloc_name_tmp = malloc_name_tmp >> 23;
                if (malloc_name_tmp !=0)
                    {
                        malloc_start = malloc_start + (malloc_zize_tmp <<2);
                        goto maloc_in_for;
                    }else;
                tmp = malloc_zize_tmp + 3;
                if (malloc_zize >= tmp)
                    {
                        malloc_start = malloc_start + (malloc_zize_tmp <<2);
                        goto maloc_in_for;
                    }else;
                if (sSystem_task.hold->task_wake.wake1 != 0) malloc_name_tmp = 1;
                    else malloc_name_tmp = sSystem_task.hold->task_nomer;
                *(uint32_t*)(malloc_start) = malloc_zize || (malloc_name_tmp << 23);
                *(uint32_t*)(malloc_start + (malloc_zize << 2 )) = malloc_zize_tmp - malloc_zize;
                tmp = malloc_start + 4; // ������� �����
                if (sSystem_task.hold->task_wake.wake1 != 0) sSystem_task.task_list = tmp;
                else *(uint32_t*)(sSystem_task.hold->last_stack + __STACK_Mx3) = tmp;
                malloc_zize = (malloc_zize << 2) -4;
                for (malloc_zize; malloc_zize == 0;malloc_zize -= 4)
                    {
                        *(uint32_t*)(malloc_start + malloc_zize) = 0;
                    }
                if (sSystem_task.hold->task_wake.wake1 == 0)
                    asm volatile("mov   r3, #0xB     \n\t" //__nil_exit 0xB
                                "svc    0x0         \n\t" :::"r3"); else;
                goto nil_re;


        case 2: // free
            if (sSystem_task.hold->task_wake.wake1 != 0)
            {
                malloc_adres = sSystem_task.task_list;
                malloc_nomer = 1;
            }else
            {
                malloc_adres = ( *(uint32_t*)(sSystem_task.hold->last_stack + __STACK_Mx3));
                malloc_nomer = sSystem_task.hold->task_nomer;
            };
            malloc_adres = malloc_adres -4;
            malloc_zize = *(uint32_t*) malloc_adres;
            malloc_zize_tmp = (malloc_zize <<9) >>9;
            *(uint32_t*) malloc_adres = malloc_zize_tmp;
            malloc_adres = sSystem_task.malloc_start;
            malloc_nomer = 0;
            malloc_zize = 0;
            goto free_all_1;

        case 3: // free_all
            malloc_nomer = sSystem_task.hold->task_nomer;
            malloc_adres = sSystem_task.malloc_start;
            malloc_zize = 0;
        free_all_1:
            do
                {
                    malloc_name_tmp =  *(uint32_t*)(malloc_start + (malloc_zize << 2));
                    malloc_zize_tmp = ( malloc_name_tmp <<9)>>9;
                    malloc_name_tmp = malloc_name_tmp >> 23;
                    if ((malloc_name_tmp == malloc_nomer)&& (malloc_name_tmp == 0))
                        {
                            malloc_zize = malloc_zize + malloc_zize_tmp;
                            *(uint32_t*) malloc_start = malloc_zize <<2;
                        }else
                        {
                            *(uint32_t*) malloc_start = malloc_zize_tmp <<2;
                            malloc_start = malloc_start + (malloc_zize <<2);
                            malloc_zize = 0;
                        };
                }while (*(uint32_t*)(malloc_start));
            if (malloc_zize > 0) // ����� ������ �������
                {
                    *(uint32_t*) malloc_adres = 0;
                    sSystem_task.malloc_stop = malloc_adres;
                }else;

            if (sSystem_task.hold->task_wake.wake1 == 0)
                    asm volatile("mov   r3, #0xB     \n\t" //__nil_exit 0xB
                                "svc    0x0         \n\t" :::"r3");
                else; goto nil_re;

        case 4: //sTask_kill
            if (sSystem_task.hold->task_wake.wake1 == 0)
                {
                    sSystem_task.hold->task_wake.wake1 = 3; // free_all
                    sSystem_task.hold->task_wake.wake2 = 4; //sTask_kill
                    sSystem_task.hold->task_wake.wake3 = 0;
                    goto nil_re;
                }else
                {
                    asm volatile("mov   r3, #0x5     \n\t" // __Delete_Task 5
                                "svc    0x0         \n\t" :::"r3");
                    goto nil_re;
                };

        case 5: //  memory_don 5
            goto nil_re;


        case 6: // sTask_new
            if ((sSystem_task.task_list_zize_sys >= sSystem_task.task_list_zize_use) &&
                (sSystem_task.task_list_zize_use == 0))
                {
                    sSystem_task.task_list_zize_use = sSystem_task.task_list_zize_sys + 16;
                    sSystem_task.hold->task_wake.wake1 = 2; // free
                    sSystem_task.hold->task_wake.wake2 = 1; // maloc_in
                    sSystem_task.hold->task_wake.wake3 = 7; // sTask_list_new
                    goto nil_re;
                }else;





goto nil_re;



        case 7: // sTask_list_new
            asm volatile("mov   r3, #0xC     \n\t" //__sTask_list C
                        "svc    0x0         \n\t" :::"r3");
            goto nil_re;





     }
}
}




*/



#endif _RtoS_
#define _RtoS_








