/**
 @file    RtoS_cortex_m7.S
 @author  AVI-crak
 @version V-45%
 @date    28-декабря-2016
 @brief   Аxis sRtoS, Cortex-M7 ARM GCC EmBitz

 license
 Million helicopters and one cent.

 форум для обсуждения
 http://forum.ixbt.com/topic.cgi?id=48:11735

 репозиторий
 https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc
*/


//#define _bitFlag volatile uint32_t __attribute__ ((section(".flag"))) // так лучше

#include "RtoS.h"


/// Запрос ресурса, бесконечный цикл - пока не освободится
/// Resource request, endless loop while resource not released
void sTask_resource_ask (uint32_t *name_resource)
{
    uint32_t tmp;
    tmp = *(sSystem_task.activ -> task_names);
    if ( *name_resource == tmp ) return;
    do
    {
        if(__LDREXW(name_resource) !=0)
            asm volatile   ("clrex              \n\t"
                            "mov   r3, #0x10    \n\t"
                            "svc    0x0         \n\t"
                            ::: "r3");
    }while ( __STREXW(tmp, name_resource));
}


/// Функция фиксированного времени сна по циклическому таймеру
/// пример
/// static uint32_t alarm_mc1; sTask_wake_mc(&alarm_mc1,1000);
/// активное состояние 0-999мс, сон 999-1мс, цикл 1с
void sTask_wake_mc(uint32_t * timer_name, uint32_t timer_mc)
{
    register    uint32_t    *__timer_name   asm  ("r0") = timer_name;
    register    uint32_t    __timer_mc      asm  ("r1") = timer_mc;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x6            \n\t" //__sTask_wake_mc 6;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__timer_name), "r" (__timer_mc) :"memory");
}

/// Выделить память (размер)
uint32_t* malloc (uint32_t malloc_zize)
{
    sSystem_task.activ->flag = 1;
    register uint32_t*   malloc_adres     asm  ("r0") = malloc_zize;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0xA            \n\t" //__nil_in 0xA;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    : "=r" (malloc_adres): "r" (malloc_adres):"memory");   // нить отложенных заданий
return malloc_adres;
}

/// Удалить память (адрес)
void free (void* malloc_adres)
{
    if  (malloc_adres ==0) return;
    if (sSystem_task.activ->mode_nomer.m_n.task_nomer != (( *(uint32_t*)(malloc_adres -4))>>23)) return;
    sSystem_task.activ->flag = 2;
    register    void   *__malloc_adres     asm  ("r0") = malloc_adres;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0xA            \n\t" //__nil_in 0xA;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                  :: "r" (__malloc_adres):);   // нить отложенных заданий
}

/// sTask_wake (& глобальный флаг) разбудить задачу
void sTask_wake(volatile uint32_t* task_global_flag)
{
    register volatile uint32_t   *__task_global_flag     asm  ("r0") = task_global_flag;
    asm volatile    ("push      {r3}        \n\t"
                    "ldr        r3, [r0]    \n\t"
                    "cmp        r3, #0      \n\t"
                    "itt        ne          \n\t"
                    "movne      r3, 0x9     \n\t" //__sTask_wake //9;
                    "svcne      0x0         \n\t"
                    "pop        {r3}        \n\t"
                    :: "r" (__task_global_flag):);
}


/// sTask_wait (& глобальный флаг) остановить задачу в ожидание пинка
void sTask_wait(volatile uint32_t* task_global_flag)
{
    register volatile uint32_t   *__task_global_flag     asm  ("r0") = task_global_flag;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x8            \n\t" //__sTask_wait //8;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__task_global_flag):"memory");
}

/// sDelay_mc (в миллисекундах)
void sDelay_mc(uint32_t Delay_mc)
{
    register    uint32_t   __Delay_mc     asm  ("r0") = Delay_mc;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x7            \n\t" //__sDelay_new //7;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__Delay_mc):"memory");
}


/// Старт ОS
/// частота ядра в гц, размер стека майна,
///       > размер стека прерываний, стартовое время задачи в микросекундах
void setup_run(uint32_t __SYSHCLK, uint32_t _main_size, uint32_t NVIC_size)
{
	sSystem_task.sSYSHCLK = __SYSHCLK;
	sSystem_task.Booked_stack = _main_size;
	sSystem_task.NVIC_size = (((NVIC_size + 31) >> 5) << 5);
	sSystem_task.tick_real = (__SYSHCLK / 1000);
	sSystem_task.norm_mc = (__SYSHCLK / 1000) - 6;
	sSystem_task.task_list_zize_sys = 2;

#if __CM3_REV
	sSystem_task.Random_register0 = BKP->DR1;
	sSystem_task.Random_register0 |= BKP->DR2 << 16;
	sSystem_task.Random_register1 = BKP->DR3;
	sSystem_task.Random_register1 |= BKP->DR4 << 16;
	sSystem_task.Random_register2 = BKP->DR5;
	sSystem_task.Random_register2 |= BKP->DR6 << 16;
#elif __CM4_REV
	sSystem_task.Random_register0 = RTC->BKP0R;
	sSystem_task.Random_register1 = RTC->BKP1R;
	sSystem_task.Random_register2 = RTC->BKP2R;
#elif __CM7_REV
	DWT->LAR = 0xC5ACCE55; // разблокировать таймер DWT->CYCCNT для Cortex-M7
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	SCB->CPACR = 0x0000008F << 20; /// FPU settings
	sSystem_task.Random_register0 = RTC->BKP0R;
	sSystem_task.Random_register1 = RTC->BKP1R;
	sSystem_task.Random_register2 = RTC->BKP2R;
#endif

	if((sSystem_task.Random_register0 == 0) || (sSystem_task.Random_register1 == 0) || (sSystem_task.Random_register2 == 0))
 {
	sSystem_task.Random_register0 = 0xADF48355;
	sSystem_task.Random_register1 = 0x24353452;
	sSystem_task.Random_register2 = 0x8667548A;
 };

	DWT->CYCCNT = 0;
	DWT->CTRL |= 1; // enable the counter
	SCB->CCR |= SCB_CCR_USERSETMPEND_Msk;
	Start_task();
}






/// Включить прерывание, преоритет (от 14 до 0)
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



/// Отключить прерывание - после запуска ос
void sNVIC_DisableIRQ(IRQn_Type IRQn)
{
    register uint8_t    __IRQn          asm  ("r0") = IRQn;
    asm volatile    ("push   {r3}               \n\t"
                    "mov     r3, 0x0            \n\t" //__DisableIRQS //1;
                    "svc    0x0                 \n\t"
                    "pop    {r3}                \n\t"
                    :: "r" (__IRQn):"memory");
}




/// Новая задача - после запуска ос
///  функция , размер стека , процент времени 1-100 ,
///         указатель на массив параметров новой задачи [4]
void sTask_new (void (*taskS_func),
                        uint32_t task_size,
                        uint8_t task_time_rate,
                        char* const task_func_name,
                        void* task_func_massif4_data  )
{
sSystem_task.sustem_flag.flag.reliability_task_list = 0;
register void       *taskS_func__       asm     ("r0") = taskS_func;
register uint32_t   task_size__         asm     ("r1") = task_size;
if (task_time_rate >100) task_time_rate = 100;
if (task_time_rate < 1) task_time_rate = 1;
register uint32_t   task_time_rate__    asm     ("r2") = task_time_rate;
register uint32_t   task_nil_in         asm     ("r3") = 0xA; //A
register char       *task_func_name__   asm     ("r4") = task_func_name;
register void       *func_massif4__     asm     ("r5") = task_func_massif4_data;
sSystem_task.activ->flag = 6;
// да чтоб икалось в бесконечности всем рацинализаторам GCC
    asm volatile  ("svc    0x0             \n\t"  //
                  :: "r" (taskS_func__), "r" (task_size__), "r" (task_time_rate__),"r" (task_nil_in),
                     "r" (task_func_name__), "r" (func_massif4__) : "memory" );
sSystem_task.sustem_flag.flag.reliability_task_list = 1;
asm volatile ("nop" : : : "memory");
}


 uint32_t sRandom(uint32_t Random_max, uint32_t Random_min)
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


/// Быстрая сортировка 8_t, первыми мин значения
void qSort_8t(int8_t *arr8_t, int32_t left, int32_t right)
{
    int32_t iee_8, jee_8;
    int32_t tmp_8, pivot_8;
    pivot_8 = arr8_t[(left + right) / 2]; // центральный элемент
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



/// Быстрая сортировка 16_t, первыми мин значения
void qSort_16t(int16_t *arr16_t, int32_t left, int32_t right)
{
    int32_t iee, jee;
    int32_t tmp, pivot;
    pivot = arr16_t[(left + right) / 2]; // центральный элемент
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

/// Быстрая сортировка 32_t, первыми мин значения
void qSort_32t(int32_t *arr32_t, int32_t left, int32_t right)
{
    int32_t iee, jee;
    int32_t tmp, pivot;
    pivot = arr32_t[(left + right) / 2]; // центральный элемент
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
        };
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










