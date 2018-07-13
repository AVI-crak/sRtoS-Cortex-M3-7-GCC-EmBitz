/**
 @file+   RtoS_cortex_m7.S
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
#ifdef _RtoS_
 extern "C" {
#endif /* _RtoS_ */

//#ifndef _RtoS_

//#define _bitFlag volatile uint32_t __attribute__ ((section(".flag"))) // так лучше
#define STM32F7xx
#define STM32F746xx
#include "stm32f7xx.h"


//#pragma pack(push, 4)
/// sSystem_task - формат банка
struct S_task
{
    struct task* activ;                     ///#00- Адрес активной задачи, Active task pointer
    struct task* delay;                     ///#04- Адрес спящих задачь, Sleeping tsaks list pointer
    struct task* wait;                      ///#08- Адрес задач ожидающих пинка, Waiting tasks list pointer
    struct task* hold;                      ///#12- Адрес задач на обработку памяти, Tasks on hold list pointer
    volatile uint16_t NVIC_size_max;        ///#16- Рабочий стек прерываний, Task maximum NVIC used stack
    volatile uint16_t NVIC_size;            ///#18- Размер стека прерываний, Task NVIC stack size
    volatile uint16_t task_list_zize_sys;   ///#20- Количество тасков в системе
    volatile uint16_t task_list_zize_use;   ///#22- Количество тасков в списке
    volatile uint32_t Booked_stack;         ///#24- Удачный стек, booked stack address
    volatile uint32_t sSYSHCLK;             ///#28- Системная частота, гц - задается при старте
    volatile uint32_t tick_real;            ///#32- Таймер активности задачи 100%, Task activity timer 100%
    volatile uint32_t malloc_start;         ///#36- Первый адрес malloc, First malloc address
    volatile uint32_t malloc_stop;          ///#40- Последний адрес malloc, Last malloc address
    const volatile uint32_t task_stop;      ///#44- Последний адрес стека, Stack last address
    const volatile uint32_t system_us;      ///#48- Системное время, System time counter
    volatile uint32_t spall_us;             ///#52- Дробный остаток
    volatile uint32_t norm_mc;              ///#56- Норма остатка (1mc)
    struct task** task_list;                ///#60- Список тасков
    union  _fla                             ///#64- Набор кривых флагов
    {
        struct fl
        {
            volatile uint32_t reliability_task_list:1;  /// обработка task_list, 1 - можно читать
            volatile uint32_t stop:5;
            volatile uint32_t a:1;
            volatile uint32_t b:1;
            volatile uint32_t c:1;
            volatile uint32_t d:1;
            volatile uint32_t e:1;
            volatile uint32_t f:1;
            volatile uint32_t g:1;
            volatile uint32_t h:1;
            volatile uint32_t i:1;
            volatile uint32_t j:1;
            volatile uint32_t k:1;
            volatile uint32_t l:1;
            volatile uint32_t m:1;
            volatile uint32_t n:1;
            volatile uint32_t o:1;
            volatile uint32_t p:1;
            volatile uint32_t q:1;
            volatile uint32_t r:1;
            volatile uint32_t s:1;
            volatile uint32_t t:1;
            volatile uint32_t u:1;
            volatile uint32_t v:1;
            volatile uint32_t w:1;
            volatile uint32_t x:1;
            volatile uint32_t y:1;
            volatile uint32_t z:1;
        } flag;
        uint32_t flag_all;
    }sustem_flag;
    volatile uint32_t Random_register0;     ///#68- Ускоритель рандома0
    volatile uint32_t Random_register1;     ///#72- Ускоритель рандома1
    volatile uint32_t Random_register2;     ///#76- Ускоритель рандома2
}sSystem_task;
//#pragma pack(pop)




struct  task
{
    struct task*    task_new;               /// 0x00, #00  32b,- Адрес новой задачи, New task pointer
    struct task*    task_lid;               /// 0x04, #04, 32b,- Адрес старой задачи, Old task pointer
    volatile uint32_t   last_stack;         /// 0x08, #08, 32b,- Cтек задачи, Stack pointer
    union{
    struct _nomer
        {
    const volatile uint16_t    task_nomer:9;    /// 0x0C, #12, 9b,- Номер таска, Task unique ID
    const volatile    uint16_t    fri:5;
    const volatile uint16_t    mode:2;      /// 0x0C, #12, >>14 2b - Режим таска
        }m_n;
    volatile uint16_t    ntt;
    }mode_nomer;
    volatile uint8_t    percent;            /// 0x0E, #14, 08b,- Процент активности задачи, Task percentage usage
    volatile uint8_t    flag;               /// 0x0F, #15, 08b,- Флаг запроса на обработку, Processing request flag
    union{
    struct _wake
        {
    volatile uint8_t    delay;              /// 0x10, #16, 8b,- Задержка выполнения
    volatile uint8_t    wake1;              /// 0x11, #17, 8b,- Первая в цепи
    volatile uint8_t    wake2;              /// 0x12, #18, 8b,- Вторая в цепи
    volatile uint8_t    wake3;              /// 0x13, #19, 8b,- Третья в цепи
        }t_wake;
    volatile uint32_t   delay_wake;
    }d_wake;
    volatile uint32_t   life_time;          /// 0x14, #20, 32b,- Таймер активности задачи, Task activity timer
    volatile uint16_t   stack_zize;         /// 0x18, #24, 16b,- Размер стека, Task stack size
    volatile uint16_t   stack_max_zize;     /// 0x1A, #26, 16b,- Рабочий стек, Task maximum used stack
    const volatile char*    task_names;     /// 0x1C, #28, 32b,- Имя задачи, Task name
};





/**

Область флагов
7b - математика
6b-0b - флаги запроса на обработку
1 получить новую область памяти
2 удалить область памяти
3 удалить все области памяти задачи
4 корректно убить задачу
5 отдать память
6 новая задача


254


маллок
0x00, #00, 32b, 9b-H (номер задачи) - 23b-L (адрес смещения)
0x04, #04, 32b, дата (минимум 32b)


Режим состояния отложенных task_wake - hold
0x10, #16   16b-L (пропуск, неудача в обработке)
0x12, #18   8b-HL (адрес назначения)
0x13, #19   8b-HH (режим состояния)

Режим таска mode
hold = 0
wait = 1
delay = 2
activ = 3

*/



/// Запрос ресурса, бесконечный цикл - пока не освободится
/// Resource request, endless loop while resource not released
void sTask_resource_ask (uint32_t *name_resource);


/// Освободить ресурс
/// Release resourse
void sTask_resource_free (uint32_t *name_resource);


/// Функция отложенного события по циклическому таймеру
/// пример
/// static uint32_t alarm_mc2;
/// if (sTask_alarm_mc(&alarm_mc1,1000)) { действие каждую новую секунду }
__attribute__( ( always_inline ) ) static inline uint32_t sTask_alarm_mc(volatile uint32_t * timer_name, uint32_t timer_mc)
{
    if (*timer_name == 0)
    {
        if (sSystem_task.system_us == 0) *timer_name = (uint32_t) 0-1; else *timer_name = sSystem_task.system_us;
    }else if (( sSystem_task.system_us - *timer_name ) > timer_mc)
    {
        *timer_name = 0; return 1;
    }else return 0;
}

/// Функция фиксированного времени сна по циклическому таймеру
/// пример
/// static uint32_t alarm_mc1; sTask_wake_mc(&alarm_mc1,1000);
/// активное состояние 0-999мс, сон 999-1мс, цикл 1с
void sTask_wake_mc(uint32_t * timer_name, uint32_t timer_mc);



/// Выделить память (размер)
uint32_t* malloc (uint32_t malloc_zize);


/// Удалить память (адрес)
void free (void* malloc_adres);


/// sTask_wake (& глобальный флаг) разбудить задачу
void sTask_wake(volatile uint32_t* task_global_flag);



/// sTask_wait (& глобальный флаг) остановить задачу в ожидание пинка
void sTask_wait(volatile uint32_t* task_global_flag);




/// sDelay_mc (в миллисекундах)
void sDelay_mc(uint32_t Delay_mc);


void __attribute__ ((weak))Start_task(void);
void __attribute__ ((weak))SVC_Handler(void);

/// Старт ОS
/// частота ядра в гц, размер стека майна,
///       > размер стека прерываний, стартовое время задачи в микросекундах
void setup_run(uint32_t __SYSHCLK,uint32_t _main_size,uint32_t NVIC_size);



/// Барьер оптимизатора GCC
static inline void __memory(void){asm volatile ("nop" : : : "memory");}

/// Измерение рабочего размера тека прерываний
void __attribute__ ((weak)) sHandler_zize(void);


/// Включить прерывание, преоритет (от 14 до 0)
void  sNVIC_EnableIRQ(IRQn_Type IRQn, uint8_t priority);




/// Отключить прерывание - после запуска ос
void sNVIC_DisableIRQ(IRQn_Type IRQn);





/// Новая задача - после запуска ос
///  функция , размер стека , процент времени 1-100 ,
///         указатель на массив параметров новой задачи [4]
void sTask_new (void (*taskS_func),uint32_t task_size,uint8_t task_time_rate,char* const task_func_name,void* task_func_massif4_data  );




/// Уступить время - только внутри работающей задачи
__attribute__( ( always_inline ) ) static inline void sTask_skip (void)
{
    asm volatile    ("mov   r3, #0x10   \n\t"
                    "svc    0x0         \n\t"
                    ::: "r3");
}

/// Убить задачу - только внутри работающей задачи
void __attribute__ ((weak)) sTask_kill(void);



uint32_t sRandom( uint32_t Random_max, uint32_t Random_min) __attribute__((used)); //++


//
#ifdef   __CM7_REV
uint32_t sRandom_m4(uint32_t Random_max,uint32_t Random_min); //++
#endif /* __CM7_REV */




/// Быстрая сортировка 8_t, первыми мин значения
void qSort_8t(int8_t *arr8_t, int32_t left, int32_t right);


/// Быстрая сортировка 16_t, первыми мин значения
void qSort_16t(int16_t *arr16_t, int32_t left, int32_t right);


/// Быстрая сортировка 32_t, первыми мин значения
void qSort_32t(int32_t *arr32_t, int32_t left, int32_t right);



static inline uint8_t unit_step (uint32_t in)
{
    uint8_t out;
    asm volatile    ("rbit  %[_out], %[_in]     \n\t"
                    "clz    %[_out], %[_out]    \n\t"
                    :[_out] "=r" (out) :[_in] "r" (in):);
    return (out);
}





/*
Обратите внимание, что для ARM вы можете указать тип прерывания, которое нужно обработать, добавив необязательный параметр к атрибуту прерывания, например:

          __attribute__( ( interrupt_handler ) )void I2C2_EV_IRQHandler (void);

Допустимыми значениями для этого параметра являются: IRQ, FIQ, SWI, ABORT и UNDEF.
 */

//#endif /* _RtoS_ */
//#define _RtoS_


#ifdef _RtoS_
}
#endif /* _RtoS_ */

/// Установить в main.c/h #define PVD_IRQHandler_s
#ifdef PVD_IRQHandler_s
void PVD_IRQHandler (void)
{
#ifdef   __CM7_REV
RTC->BKP0R = sSystem_task.Random_register0;
RTC->BKP1R = sSystem_task.Random_register1;
RTC->BKP2R = sSystem_task.Random_register2;
QUADSPI->DLR = 1;
QUADSPI->CCR = 0x01000101;
QUADSPI->FCR = 0x1B;
while(!(QUADSPI->SR & 0x04)); /// FTF flag  FIFO пуст -  =1
QUADSPI->DR = 0xFFFF; /// младший байт первым
while(!(QUADSPI->SR & 0x02)); /// Wait for TCF flag to be set - передача данных завершена =1
PWR->CR1 &= ~PWR_CR1_DBP;     ///  защита записи  защищённого домена вкл
#else
BKP->DR1 = sSystem_task.Random_register0;
BKP->DR2 = sSystem_task.Random_register0 >> 16;
BKP->DR3 = sSystem_task.Random_register1;
BKP->DR4 = sSystem_task.Random_register1 >> 16;
BKP->DR5 = sSystem_task.Random_register2;
BKP->DR6 = sSystem_task.Random_register2 >> 16;
#endif
while(1);
};
#endif




#define _RtoS_






