/// Cortex-M3 GCC EmBitz 0.40
/// имя файла
/// RtoS.h
/// процент готовности 34%

/// мыло для заинтересованных
/// videocrak@maol.ru
/// форум для обсуждения
/// http://forum.ixbt.com/topic.cgi?id=48:11735

/// репозиторий
/// https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc


#ifndef _RtoS_

//#define _bitFlag volatile uint32_t __attribute__ ((section(".flag"))) // так лучше




/// sSustem_task - формат банка
struct
{
  struct task* activ;               ///- Адрес активной задачи
  struct task* delay;               ///- Адрес спящих задачь
  struct task* wait;                ///- Адрес задач ожидающих пинка
  struct task* hold;                ///- Адрес задач на обработку памяти
  __IO uint32_t NVIC_size_max;      ///- использованный размер стека прерываний
  __IO uint32_t NVIC_size;          ///- размер стека прерываний, задаётся при старте
  __IO uint32_t Main_size_start;    ///- размер стека майна// удачный стек
  __IO uint32_t sSYSHCLK;           ///- системная частота, гц - задается при старте
  __IO uint32_t tik_real;           ///- 100% тиков на задачу // us на задачу при старте
  __IO uint32_t malloc_start;       ///- первый адрес malloc
  __IO uint32_t malloc_stop;        ///- последний адрес malloc
  __IO uint32_t task_stop;          ///- последний адрес стека
}sSustem_task ;



struct task
{
  struct task* task_new;            /// 0x00, #00  32b,- Адрес новой задачи
  struct task* task_lid;            /// 0x04, #04, 32b,- Адрес старой задачи
  __IO uint32_t last_stack;         /// 0x08, #08, 32b,- Cтек задачи (активный хвост)
  __IO uint16_t life_time;          /// 0x0C, #12, 16b,- Таймер активности в потоке
  __IO uint8_t percent;             /// 0x0E, #14, 08b,- Выделенный процент активности потоке
  __IO uint8_t flag;                /// 0x0F, #15, 08b,- Флаг запроса на обработку
  __IO uint32_t sleep_wake;         /// 0x10, #16, 32b,- Время сна (мс) // адрес глобального флага пинка
  __IO uint32_t data_address;       /// 0x14, #20, 32b,- Malloc_zize - malloc_adres // malloc_adres
  __IO uint16_t stack_zize;         /// 0x18, #24, 16b,- Размер стека (задаётся при запуске)
  __IO uint16_t stack_max_zize;     /// 0x1A, #26, 16b,- Мах размер стека (мах заюзанный размер стека)
  __IO char* task_names;            /// 0x1C, #28, 32b,- Адрес имени задачи (char* const text)
};



volatile uint32_t Random_register[3];

/**

флаг запроса на обработку
1 получить новую область памяти
2 удалить область памяти
3 удалить все области памяти задачи
4 корректно убить задачу

3 отдать область памяти по адресу имени с потверждением о принятии (память ничейная)
4 подтверждение о приняти области памяти (память принадлежит новой нити)
5 создать кольцевую очередь для записи
6 создать кольцевую очередь для чтения
5 бла бла бла
254

маллок
0x00, #00, 32b, смещение до нового блока
0x04, #04, 32b, имя задачи
0x04, #08, 32b, дата (минимум 4b)
**/



/// Выделить память (размер)
static uint32_t malloc (uint32_t malloc_zize)
{
register volatile uint32_t malloc_adres asm  ("r0") = malloc_zize;
asm volatile ("push     {r1, r2, r3}        \n\t"
              "mov      r1, %[malloczize__] \n\t"
              "ldr      r3,  =sSustem_task  \n\t"
              "ldr      r3, [r3]            \n\t"   // активная задача
              "cbnz     r1, _Za_%=          \n\t"
              "bkpt                         \n\t"   // нулевой размер требуемой  памяти
   "_Zaa_%=:" "mov      r2, #10             \n\t"   // пауза 10мс
              "svc      0x7                 \n\t"
   "_Za_%=:"  "str      r1, [r3, #20]       \n\t"   // malloc_zize - malloc_adres // malloc_adres
              "mov      r2, #1              \n\t"   // флаг обработки для sTask_nil, 1 = malloc_in
              "strb     r2, [r3, #15]       \n\t"   // флаг запроса на обработку
              "svc      0xA                 \n\t"   // нить отложенных заданий
              "ldr      r2,  [r3, #20]      \n\t"
              "cmp      r2, #0              \n\t"
              "beq      _Zaa_%=             \n\t"   // возвращён нулевой адрес
              "mov      %[malloc_ad__], r2  \n\t"
              "pop      {r1, r2, r3}        \n\t"
               : [malloc_ad__] "=r" (malloc_adres)
               : [malloczize__] "r" (malloc_zize):  );
return malloc_adres;
}

/// Удалить память (адрес)
static void free (void* malloc_adres)
{
asm volatile ("push     {r0, r1}                  \n\t"
              "ldr      r1,  =sSustem_task        \n\t"
              "ldr      r1, [r1]                  \n\t"   // активная задача
              "str      %[adres__], [r1, #20]     \n\t"   // malloc_adres
              "mov      %[adres__], #2            \n\t"   // флаг обработки для sTask_nil, 2 = free (malloc_adres)
              "strb     %[adres__], [r1, #15]     \n\t"   // флаг запроса на обработку
              "svc      0xA                       \n\t" // нить отложенных заданий
              "pop      {r0, r1}                  \n\t"
               :: [adres__] "r" (malloc_adres): "memory" );
}

/// sTask_wake (глобальный флаг) разбудить задачу
__attribute__( ( always_inline ) ) static inline sTask_wake(uint32_t* task_wake_flag)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__wake_flag]  \n\t"
                "svc    0x9                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wake_flag] "r" (task_wake_flag):);
}


/// sTask_wait (глобальный флаг) остановить задачу в ожидание пинка
__attribute__( ( always_inline ) ) static inline sTask_wait(uint32_t* task_wait_flag)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__wait_flag]  \n\t"
                "svc    0x8                 \n\t"
                "pop    {r2}                \n\t"
                :: [__wait_flag] "r" (task_wait_flag):);
}



/// sDelay_mc (в миллисекундах)
__attribute__( ( always_inline ) ) static inline sDelay_mc(uint32_t Delay_mc)
{
asm volatile  ("push   {r2}                \n\t"
                "mov    r2, %[__Delay_mc]   \n\t"
                "svc    0x7                 \n\t"
                "pop    {r2}                \n\t"
                :: [__Delay_mc] "r" (Delay_mc):);
}

/// Старт ОS
/// частота ядра в гц, размер стека майна,
///       > размер стека прерываний, стартовое время задачи в микросекундах
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

/// Барьер оптимизатора GCC
__attribute__( ( always_inline ) ) static inline __memory(void){asm volatile ("nop" : : : "memory");}


/// Включить прерывание, преоритет (от 14 до 1)
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



/// Отключить прерывание - после запуска ос
__attribute__( ( always_inline ) ) static inline sNVIC_DisableIRQ(IRQn_Type IRQn)
{
asm volatile ("push    {r1}        \n\t"
               "mov     r1, %[IRQ]  \n\t"
               "svc     0x1         \n\t"
               "pop     {r1}        \n\t"
               :
               : [IRQ] "r" (IRQn):"memory");
}




/// Новая задача - после запуска ос
///  функция , размер стека , процент времени 1-100 ,
///         указатель на массив параметров новой задачи [4]
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
                    "nop                    \n\t"   // уступить время
                    "nop                    \n\t"   // недостаток места
                    "b      _Zd_%=          \n\t"
     "_Zc_%=:"      "pop     {r4, r5, r6}   \n\t"
                  :
                  :  "r" (taskS_func), "r" (task_size), "r" (task_time_rate), "r" (task_func_name),
                     "r" (task_func_massif4_data)
                  : "memory" );
}



/// Уступить время - только внутри работающей задачи
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

/// Убить задачу - только внутри работающей задачи
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
