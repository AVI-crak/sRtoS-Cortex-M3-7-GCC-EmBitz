// Cortex-M3 GCC EmBitz 0.40
/* имя файла */
/* RtoS_cortex_m3.S */
/* процент готовности 40% */
/* размер rom 2446bб ram 128b*n + 64b */

/* мыло для заинтересованных */
/* videocrak@maol.ru */
/* форум для обсуждения */
/* http://forum.ixbt.com/topic.cgi?id=48:11735 */

/* репозиторий */
/* https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc */


 .syntax unified
 .arch armv7-m

 .equ   __Test_psp,0               // проверка на ошибки
 .equ   __Vector_table,     0xE000ED08
 .equ   __DWT,              0xE0001000
 .equ   __SysTick_CTRL,     0xE000E010  // #0xE010 SysTick->CTRL, #0xE014 SysTick->LOAD, #0xE018 SysTick->VAL
 .equ   __TIM6_CR1,         0x40001000  // +12 DIER, +16 SR, +20 EGR, +36 CNT, +40 PSC, >+44 ARR
 .equ   __RCC_APB1ENR,      0x4002101C
 .equ   __tick_real_ms,      0x000F4240
 .equ   __TIM_Delay_IRQn,   54          //TIM6_IRQn
 .equ   __STACK_Mx,         32          //смещение до первого сохранёного элемента



 .align 4
 .globl    SVC_Handler
 .type    SVC_Handler, %function

SVC_Handler:
            tst     lr, #4
            ite     eq
            mrseq   r0, msp
            mrsne   r0, psp
            ldr     r0, [r0, #24]
            ldrsb   r0, [r0, #-2]
            tbh     [pc, r0, lsl #1]

SVC_Table:
    .hword   ((__EnableIRQS - SVC_Table)/2)      //0
    .hword   ((__DisableIRQS - SVC_Table)/2)     //1
    .hword   ((__malloc_in_0 - SVC_Table)/2)     //2
    .hword   ((___sRandom - SVC_Table)/2)        //3
    .hword   ((__sTask_new - SVC_Table)/2)       //4
    .hword   ((__Delete_Task - SVC_Table)/2)     //5
    .hword   ((__sDelay_work - SVC_Table)/2)     //6
    .hword   ((__sDelay_new - SVC_Table)/2)      //7
    .hword   ((__sTask_wait - SVC_Table)/2)      //8
    .hword   ((__sTask_wake - SVC_Table)/2)      //9
    .hword   ((__nil_ - SVC_Table)/2)            //A
    .hword   ((__nil_ww - SVC_Table)/2)          //B
    .hword   ((__sTask_ask - SVC_Table)/2)       //C
    .hword   ((__memory_d - SVC_Table)/2)        //D
    .hword   ((__memory_r - SVC_Table)/2)        //E
    .hword   ((__nil_nn - SVC_Table)/2)          //F



 .align 4
__EnableIRQS: // r1 IRQn, r2 Priority
            push    {r4}
            movw    r4, #0xE70C
            movt    r4, #0xE000
            and     r1, #0xFF       //IRQn безопасность
            and     r2, #0xF        //Priority безопасность
            cmp     r2, #0
            it      eq              //проверка на мах приоритет
            moveq   r2, #1
            cmp     r2, #15
            it      eq              //проверка на мин приоритет
            moveq   r2, #14
            cmp     r1, #0xFF       //проверка SysTick_IRQn
            it      eq
            moveq   r2, #15         //SysTick_IRQn должен иметь минимальный приоритет
            cmp     r1, #0xFB       //проверка SVCall_IRQn
            beq     EnableIrq_error //нельзя менять приоритет
            cmp     r1, #0xFE       //проверка PendSV_IRQn
            beq     EnableIrq_error //нельзя менять приоритет
            cmp     r1, __TIM_Delay_IRQn
            it      eq
            moveq   r2, #15         //минимальный приоритет
            lsl     r2, r2, #4      //сдвиг на границу
            mov     r3, r1
            and     r3, #0x0F
            sub     r3, r3, #4
            cmp     r1, #0xF1
            ittee   cs
            addwcs  r0, r4, #0x60C //SCB->SHPx 0xE000ED18
            strbcs  r2, [r0, r3]
            subwcc  r0, r4, #0x30c //NVIC->IPx 0xE000E400
            strbcc  r2, [r0, r1]
            and     r2, r1, #0x1F
            mov     r3, #0x1
            lsls	r2, r3, r2
            lsrs    r1, r1, #5
            lsls	r1, r1, #2
            subw    r0, r4, #0x60C //NVIC_ISERx 0xE000E100
            str     r2, [r0, r1]
EnableIrq_error:
            pop     {r4}
            bx      lr

 .align 4
__DisableIRQS:
//NVIC->ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* disable interrupt */
            cmp     r1, __TIM_Delay_IRQn
            beq     EnableIrq_error //нельзя удалять
            mov     r3, #0x1F
            and     r0, r1, r3
            mov     r3, #0x1
            lsls	r2, r3, r0
            lsrs    r0, r1, #5
            lsls	r0, r0, #2
            movw    r3, #0xE180
            movt    r3, #0xE000
            str     r2, [r3, r0]
            bx      lr
 .align 4
__malloc_in_0: //r7 - адрес возврата
            add     r7, r7, #12
            ldr     r2, [r12, #44]          // читем task_stop - последний адрес стека
            cmp     r2, r7
            itet    hi                      // есть место
            ldrhi   r2, [r12, #40]          // читаем malloc_stop - последний адрес malloc
            movls   r7, #0
            cmphi   r7, r2
            it      hi
            strhi   r7, [r12, #40]          // новый malloc_stop - последний адрес malloc
            bx      lr

 .align 4
___sRandom:
            ldr     r3, =Random_register
            ldmia   r3!, {r0-r2}           // читаем сохранённое
            cmp     r0, #0
            ittt    eq
            moveq   r3, #0x1000
            movteq.w r3, #0xE000
            ldreq   r0, [r3, #4]
            mov     r3, #48
___sRandom_1:
            sub     r3, r3, #1
            tst     r1, r1, lsr#1
            movs    r2, r0, rrx
            adc     r1, r1, r1
            eor     r2, r2, r0, lsl#15
            eor     r0, r2, r2, lsl#29
            cmp     r3, #0
            bne     ___sRandom_1
            ldr     r3, =Random_register
            stmia   r3!, {r0-r2}
            tst     lr, #4
            ite     eq
            mrseq   r3, msp
            mrsne   r3, psp
            ldr     r1, [r3]
            ldr     r2, [r3, #4]
            sub     r1, r1, r2
            mvn     r2, #0
            udiv    r1, r2, r1
            udiv    r0, r0, r1
            ldr     r2, [r3, #4]
            add     r0, r0, r2
            str     r0, [r3]
            bx      lr

 .align 4
__sTask_new: // [psp +0]) void (*taskS_func()), [psp +4] task_size ,
             // [psp +8] task_time_rate , [psp +12] char* const task_func_name
             // pop {r4} = *func_parametr;
            push    {r4, r7, r8}
 .ifdef  __Test_psp
            tst     lr, #4
            bne     __sTask_new_step_0
            bkpt                            // ошибка, попытка запуска задачи в прерывании
__sTask_new_step_0:
 .endif
            ldr     r8,= sSystem_task       // адрес банка задачь
            ldr     r0, [r8, #24]           // удачный стек
            mrs     r4, psp                 // r4 будет адресом параметров
            ldr     r6, [r4, #4]            // [psp +4]) task_size
            add     r6, r6, #31
            bfc     r6, #0, #5              // выровняли размер новой задачи
            mov     r1, #16                 // активные, спящие, оживающие пинка, обработка памяти
SVC_step4_1:
            sub     r1, r1, #4
            cmp     r1, #18
            bhi     SVC_step_stop           // проверили все существующие нити
            ldr     r3, [r8, r1]            // адрес первой задачи
            cmp     r3, #0
            beq     SVC_step4_1
            mov     r5, r3                  // r3 первая задача в списке
//0 -удачный хвост, 1 счётчик, 2 новая голова,  5 задача в списке, 8 банк
SVC_step4:
            add     r2, r5, r6
            add     r2, r2, #32
            cmp     r2, r0 //>
            itttt   hi
            ldrhhi  r2, [r5, #24]           // читаем новый хвост
            subhi   r2, r2, #32
            subhi   r2, r5, r2
            cmphi   r0, r2
            ittt    hi
            movhi   r0, r2
            movhi   r1, #12                 // хвост изменился
            bhi     SVC_step4_1
            ldr     r2, [r5]
            cmp     r3, r2
            itt     ne
            movne   r5, r2
            bne     SVC_step4
            b       SVC_step4_1

SVC_step_stop: // r0 вершина стека новой нити
            sub     r5, r0, r6
            ldr     r4, [r8, #40]           // граница маллок
            cmp     r5, r4
            ittt    lo                      // нету места
            poplo   {r4}
            movlo   r5, #1
            blo     sTask_new_error
            ldr     r4, [r8, #44]           // task_stop - последний адрес стека
            cmp     r5, r4
            it      lo
            strlo   r5, [r8, #44]           // граница стека опустилась
            // выделение нового номера
            sub     r1, r0, #64
            mvn     r2, #0
SVC_step4_6:
            cmp     r1, r0
            itt     ne
            strne   r2, [r1], #4
            bne     SVC_step4_6
            sub     r1, r0, #64
            mov     r2, #17
            lsl     r2, r2, #25
            add     r2, r2, r1, lsl #5      // начальный адрес таблицы номеров
            mov     r1, #0
            str     r1, [r2]
            mov     r3, #16                 // активные, спящие, оживающие пинка, обработка памяти
SVC_step4_7:
            sub     r3, r3, #4
            cmp     r3, #18
            bhi     SVC_step4_9             // проверили все существующие нити
            ldr     r7, [r8, r3]            // адрес первой задачи
            cmp     r7, #0
            beq     SVC_step4_7
            mov     r4, r7
SVC_step4_8:
            ldrh    r5, [r4, #20]           // читаем номер
            str     r1, [r2, r5, lsl #2]    // обнуляем точку
            ldr     r4, [r4]
            cmp     r4, r7
            bne     SVC_step4_8
            b       SVC_step4_7
SVC_step4_9:
            mov     r3, #0
            sub     r2, r0, #64
SVC_step4_10:
            ldr     r1, [r2, r3, lsl #2]
            rbit    r1, r1
            clz     r1, r1
            cmp     r1, #32
            itt     eq
            addeq   r3, r3, #1
            beq     SVC_step4_10
            add     r7, r1, r3, lsl #5        // новый номер
            // обнуляем поле стека
            sub     r1, r0, #96
            mov     r2, #0
SVC_step4_11:
            cmp     r1, r0
            itt     ne
            strne   r2, [r1], #4
            bne     SVC_step4_11
            // заполняем стек
            sub     r0, r0, #32             //новая задача
            strh    r7, [r0, #20]
            ldr     r2, [r8]                //активная задача
            ldr     r3, [r2, #4]            //адрес преведущей задачи ( указывает на голову)
            str     r0, [r3]                //голова указывает на новую
            str     r0, [r2, #4]            //активная указывает на новую старую задачу
            str     r2, [r0]                //новая указывает на активную
            str     r3, [r0, #4]            //новая указывает на старую голову

            mrs     r4, psp                 // r4 будет адресом параметров
            ldr     r1, [r4]                // [psp +0]) void (*taskS_func())
            str     r1, [r0, #-8]           //сохранили pc
            ldr     r1, =sTask_kill
            str     r1, [r0, #-12]          //сохранили ложный lr
            ldr     r1, [r8, #32]           //tick_real - 100% тиков на задачу
            ldr     r5, [r4, #+8]           //[psp +8]) task_time_rate %

            cmp     r5, #100
            it      hi
            movhi   r5, #100
            cmp     r5, #1
            it      lo
            movlo   r5, #1
            strh    r5, [r0, #14]           //сохранили выделенный процент времени
            mul     r1, r1, r5
            mov     r3, #0
            movt    r3, #0x100
            str     r3, [r0, #-4]           //сохранили xPSR в стек 0x1000000
            mov     r5, #100
            udiv    r1, r1, r5              //подсчёт времени
            sub     r2, r0, #64             //Cortex-M3
            str     r2, [r0, #8]            //стек задачи (активный хвост)
            strh    r6, [r0, #24]           //размер стека (задаётся при запуске)
            mov     r3, #64
            strh    r3, [r0, #26]           //мах заюзанный размер стека
            strh    r1, [r0, #12]           //таймер активности в потоке
            ldr     r1, [r4, #12]           //[psp +12]) char* const task_func_name
            str     r1, [r0, #28]           //адрес имени задачи (char* const text)
            add     r2, r2, #32
            pop     {r4}                    // void* func_parametr
            cbz     r4, SVC_step4_12
            ldmia   r4!, {r5-r8}            //чтение параметров функции
            stmia   r2!, {r5-r8}            //установка параметров функции
SVC_step4_12:
            mov     r5, #0
sTask_new_error:
            pop     {r7, r8}
            isb
            bx      lr

 .align 4
__Delete_Task: // 5
            ldr     r2,= sSystem_task       // адрес банка задачь
            ldr     r3, [r2]                // активная
            ldr     r1, [r3, #4]            // адрес хвоста активных задач
            ldr     r0, [r3]                // новая голова
            str     r0, [r1]                // хвост на голову
            str     r1, [r0, #4]            // голова на хвост
            str     r0, [r2]                // теперь активная
          //  dsb
            b       PendSV_step



 .align 4
__sDelay_work:                              // обслуживание задержки
            ldr     r0, = sSystem_task      // адрес переменной с адресом активной задачи
            ldr     r1, [r0, #4]            // первая спящая задача
            ldr     r2, [r0, #48]
            add     r2, r2, #1
            str     r2, [r0, #48]           //alarm_mc - время для задержки выполнения условия
            cbz     r1, __sDelay_exit
            push    {R4, r5, r6}
            ldr     r5, [r1, #4]            // хвост не обработанной спящей
            mov     r6, #1
__sDelay_next:
            ldr     r2, [r1, #16]           // читаем время
            sub     r2, r2, #1
            str     r2, [r1, #16]           // сохраняем
            ldr     r4, [r1]                // голова следующей не обработанной спящей
            cmp     r5, r1
            it      eq
            moveq   r6, #0
            cbnz    r2, __sDelay_cut        // больше нуля
            cmp     r4, r1
            itt     eq                      // единственная задача в сне
            streq   r2, [r0, #4]            // больше нет спящих
            beq     __sDelay_cut3
            ldr     r2, [r1, #4]            // хвост не обработанной спящей
            str     r4, [r0, #4]            // теперь отчёт с неё
            str     r2, [r4, #4]            // голова на хвост
            str     r4, [r2]                // голова хвоста на голову
__sDelay_cut3:
            ldr     r2, [r0]                // активная задача в потоке
            ldr     r3, [r2, #4]            // хвост активной задачи
            str     r1, [r3]                // хвост на новую
            str     r3, [r1, #4]            // новая на хвост хвоста
            str     r2, [r1]                // новая на голову активной
            str     r1, [r2, #4]            // хвост активной на новую
__sDelay_cut:
            mov     r1, r4
            cmp     r6, #0
            bne     __sDelay_next
            pop     {R4, r5, r6}
            dsb
__sDelay_exit:
            bx      lr

 .align 4
__sDelay_new:       // r2 время сна
            ldr     r0,  =sSystem_task
            ldr     r1, [r0]                // активная задача
            str     r2, [r1, #16]           // сохранили время сна
            mov     r3, #4                  // задачи сна
            b       __nil_1

 .align 4
__sTask_wait: // R2(глобал флаг - адрес)
            ldr     r0,  =sSystem_task
            mov     r1, #1
            str     r1, [r2]                // взвели флаг
            ldr     r1, [r0]                // активная задача
            str     r2, [r1, #16]           // сохранили адрес флага для пинка
            mov     r3, #8                  // задачи ожидающих пинка
            b       __nil_1


 .align 4
__sTask_wake: // R2(глобал флаг - адрес) разбудить
            ldr     r3, [r2]                // читаем глобальный флаг
            cbz     r3, __sTask_wake_exit   // уже запускали
            ldr     r0, = sSystem_task      // адрес переменной с адресом активной задачи
            ldr     r1, [r0, #8]            // Адрес задачь ожидающих пинка
            cbz     r1, __sTask_wake_exit   // пусто, наверное глюк
            push    {R4}
            mov     r4, #0
            str     r4, [r2]                // сбрасываем глобальный флаг
            ldr     r4, [r1, #4]            // хвост
__sTask_wake_next:
            ldr     r3, [r1, #16]           // читаем адрес флага в нити
            sub     r3, r3, r2
            cbz     r3, __sTask_wake_1      // нашлось
            cmp     r4, r1
            itt     ne                      // не последняя
            ldrne   r1, [r1]                // там точно есть
            bne     __sTask_wake_next       // ищим
            pop     {r4}
            bx      lr
__sTask_wake_1:
            ldr     r2, [r1, #4]            // хвост в ожидающих пинка
            ldr     r3, [r1]                // новая голова в ожидающих пинка
            subs    r4, r3, r1
            iteee   eq                      // последняя из ожидающих пинка
            streq   r4, [r0, #8]            // больше нет задач для пинка
            strne   r3, [r0, #8]            // следующая голова будет первой
            strne   r2, [r3, #4]            // голова на хвост
            strne   r3, [r2]                // хвост на голову
            // проблемы с моментальным запуском, на потом
            ldr     r2, [r0]                // читаем активную задачу
            ldr     r3, [r2, #4]            // её хвост
            str     r1, [r3]                // хвост на новую задачу
            str     r3, [r1, #4]            // хвост новой на хвост старой
            str     r2, [r1]                // голова новой на голову активной
            str     r1, [r2, #4]            // хвост активной на новую задачу
            pop     {R4}
            dsb
__sTask_wake_exit:
            bx      lr


 .align 4
__nil_: // нить отложенных заданий  r1 активная задача // r2 время // r3 адрес назначения
            ldr     r0,  =sSystem_task
            ldr     r1, [r0]                // активная задача
            mov     r2, #0
            str     r2, [r1, #16]           // сброс таймера
            mov     r3, #12                 // нить отложенных заданий
__nil_1:
            mrs     r2, psp
            stmdb   r2!, {r4-r11}           // сохраняем регистры
            str     r2, [r1, #8]            // сохранили адрес стека активной задачи
            push    {r4}
            ldr     r4, [r1, #4]            // адрес хвоста
            ldr     r2, [r1]                // адрес головы
            str     r2, [r4]                // хвост на новую голову
            str     r4, [r2, #4]            // новая голова на хвост
            str     r2, [r0]                // новая голова активна
            pop     {r4}
            ldr     r2, [r0, r3]            // адрес слабых
            cmp     r2, #0
            itttt   eq
            streq   r1, [r0, r3]            // первая задача слабых
            streq   r1, [r1]
            streq   r1, [r1, #4]
            beq     __nil_2
            ldr     r3, [r2, #4]            // хвост спящей
            str     r1, [r3]                // хвост на новую голову
            str     r3, [r1, #4]            // новая на хвост
            str     r1, [r2, #4]            // хвост первой спящей на новую
            str     r2, [r1]                // голова новой на спящую
__nil_2:
            ldr     r0, [r0]                // активная задача
          //  dsb
            b       PendSV_step


 .align 4
__nil_ww:  // возврат из нити отложенных задач r7 - адрес размер
         //   ldr     r12,  =sSystem_task
            ldr     r3, [r12, #12]           // задача из обработки маллок
            mov     r2, #0
            strb    r2, [r3, #15]           // сброс флага
            str     r2, [r3, #16]           // сброс адрес глобального флага пинка
            ldr     r1, [r3, #4]            // хвост из обработки маллок
            ldr     r0, [r3]                // новая голова из обработки маллок
            cmp     r0, r3
            ittte   ne                      // не единственная
            strne   r0, [r1]                // хвост на голову
            strne   r1, [r0, #4]            // голова на хвост
            strne   r0, [r12, #12]           // первая голова в обработчике маллок
            streq   r2, [r12, #12]           // закрыли
            ldr     r0, [r12]               // активная задача в потоке
            ldr     r2, [r0, #4]            // хвост активной задачи
            str     r3, [r2]                // хвост на новую
            str     r2, [r3, #4]            // новая на хвост хвоста
            str     r0, [r3]                // новая на голову активной
            str     r3, [r0, #4]            // хвост активной на новую
            dsb
            bx      lr





 .align 4
__sTask_ask: // запрос ресурса  name_resource >> ("r4") >> resource_flag__
            mov     r1, r4
            ldr     r4, [r4]                // читаем хозяина
            ldr     r2,  =sSystem_task
            ldr     r2, [r2]                 // активная задача
            ldr     r2, [r2, #28]            // имя
            cmp     r4, r2
            it      ne
            cmpne   r4, #0
            itt     ne
            movne   r4, #0
            bne     __ask_error
            mov     r4, r2
            str     r2, [r1]                // новый хозяин
            bx      lr
__ask_error:
            ldr     r2, =__TIM6_CR1
            ldr     r0, [r2, #44]
            str     r0, [r2, #36]
            dsb
            bx      lr

 .align 4
__memory_d: // SVC 0xD
            ldr     r0,  =sSystem_task
            ldr     r3, [r0]
            mov     r2, #5
            strb    r2, [r3, #15]           // флаг - отдать память
            b       __nil_1

__memory_r: // SVC 0xE
            ldr     r0,  =sSystem_task
            ldr     r3, [r0]
            mov     r2, #6
            strb    r2, [r3, #15]           // флаг - забрать память
            b       __nil_1

__nil_nn: // SVC 0xF
            ldr     r0, [r12, #12]
            ldr     r0, [r0]
            str     r0, [r12, #12]
            bx      lr




 .size SVC_Handler, . - SVC_Handler


 .align 4
 .globl    Start_task
 .type    Start_task, %function

Start_task: // перемещение стеков
            cpsid   i
            push    {r0, r1, r2, r3, R4, R5, lr}
            ldr     r0,  =sSystem_task
            ldr     r1, [r0, #28]   // sSYSHCLK // системная частота
            ldr     r3, [r0, #32]   // us на задачу при старте
            ldr     r2, =__tick_real_ms
            cmp     r3, r2
            it      hi              // > проверка на максимум
            movhi   r3, r2
            cmp     r3, #10
            it      lo              // < проверка на минимум
            movlo   r3, #10
            udiv    r2, r2, r3
            movw    r4, #32767
            udiv    r2, r1, r2
            ldr     r5, =__RCC_APB1ENR
            udiv    r3, r2, r4      // число для TIM6_PSC
            ldr     r1, [r5]
            orr     r1, #0x10
            str     r1, [r5]        // включили TIM6
            udiv    r4, r2, r3      // число для TIM6_ARR
            ldr     r5, =__TIM6_CR1
            str     r3, [r5, #40]   // TIM6_PSC
            str     r4, [r0, #32]   // мах количество тиков на задачу
            str     r4, [r5, #44]   // TIM6_ARR
            mov     r1, #1
            str     r1, [r5, #12]   //TIM6_DIER (UIE)
            mov     r1, #5
            str     r1, [r5]        //TIM6_CR1 (Bit 2 URS,Bit 0 CEN) старт

            ldr     r1, [r0, #28]   // sSYSHCLK // системная частота
            mov     r2, #1000
            udiv    r3, r1, r2      // 1mc sDelay_ms
            ldr     r1, =__SysTick_CTRL
            bfc     r3, #24, #8
            str     r3, [r1, #4]    // SysTick->LOAD
            mov     r2, #7
            str     r2, [r1]        // запуск+прерывания+частота=1
            ldr     r1, [r1, #3320] //  Vector table offset register (VTOR)
            ldr     r1, [r1]        //вершина стека
            ldr     r2, [r0, #24]   // размер стека майна
            mrs	    r3, MSP
            sub     r3, r1, r3      //размер стека на текущий момент
            cmp     r2, r3
            ite     lo              // если запланированный размер стека меньше текущего -то используем текуший
            addlo   r2, r3, #31
            addhs   r2, r2, #31
            bfc     r2, #0, #5
            str     r2, [r0, #24]   // обравняли на поле и сохранились
            ldr     r2, [r0, #20]   // размер стека прерываний
            add     r2, r2, #31
            bfc     r2, #0, #5
            str     r2, [r0, #20]   // обравняли на поле и сохранились
            add     r2, r2, #160    // сдвиг на голову майна  (таблица майна8*4)+(задача_нуль(8*4*4)+(таблица(8*4)))+прерывания
            mrs	    r3, MSP
            sub     r3, r1, r3      //размер стека на текущий момент
            sub     r0, r1, r2      // голова нового стека майна
            sub     r0, r0, r3      // хвост нового стека
            mrs	    r2, MSP         // хвост текущего стека
            msr     PSP, r0         // новый хвост стека
            msr     MSP, r1         // новый хвост прерываний
_Start_task02:
            cmp     r1, r2
            ittt    ne
            ldrne   r3, [r2], #4
            strne   r3, [r0], #4
            bne     _Start_task02   // r0 + 4 адрес таблицы
            ldr     r2,  =sSystem_task
            str     r0, [r2]        //task_presently - Адрес активной задачи
            ldr     r1, [r2, #32]   //tick_real - 100% тиков на задачу
            movt    r1, #100        //100% выделенный процент активности потоке
            str     r1, [r0, #12]   //таймер активности в потоке//выделенный процент
            ldr     r1, [r2, #24]
 //           bfc     r1, #16, #16
            strh    r1, [r0, #24]   // размер стека не может быть выше 64к, сохранили
            mov     r1, #0
            str     r1, [r2, #4]    //обнуление спящих задачь
            str     r1, [r2, #8]    //обнуление задачь ожидающих пинка
            str     r1, [r2, #12]   //обнуление задач на обработку памяти
            mrs	    r2, PSP
            sub     r2, r0, r2
            strh    r2, [r0, #26]   //мах размер стека (мах заюзанный размер стека)
            ldr     r1, = __Main_Name
            str     r1, [r0, #28]
            mov     r1, #2
            strh    r1, [r0, #20]   // task_nomer
            add     r0, r0, #32
            mov     r3, r0
            mrs	    r2, MSP
            mov     r1, #0
_Start_task03:
            cmp     r0, r2
            itt     ne
            strne   r1, [r0], #4
            bne     _Start_task03   // обнулялка
            movt    r1, #0x100
            str     r1, [r3, #148]  // сохранили фиктивный lr в стек 0x1000000
            ldr     r2, =sTask_nil
            str     r2, [r3, #152]  // сохранили pc sTask_nil
            str     r1, [r3, #156]  // сохранили xPSR в стек 0x1000000
            add     r3, r3, #160    // r3 голова таблицы sTask_nil
            ldr     r5,  =sSystem_task
            ldr     r2, [r5, #32]   //tick_real - 100% тиков на задачу
            ldr     r0, [r5]        // активная задача
            str     r3, [r0]        //адрес новой задачи (указывает на голову)
            str     r0, [r3]        //адрес новой задачи (указывает на голову)
            str     r3, [r0, #4]    //адрес преведущей задачи ( указывает на голову)
            str     r0, [r3, #4]    //адрес преведущей задачи ( указывает на голову)
            movt    r2, #100
            str     r2, [r3, #12]   //таймер активности в потоке//выделенный процент
            sub     r1, r3, #64
            str     r1, [r3, #8]    //стек задачи (активный хвост)
            ldr     r1, =__sTask_nil_Nane
            str     r1, [r3, #28]   //адрес имени задачи (char* text) // номер задачи
            mov     r1, #1
            strh    r1, [r3, #20]   //task_nomer
            mov     r1, #128
            strh    r1, [r3, #24]   //размер стека сохранили
            mov     r1, #64
            strh    r1, [r3, #26]   //мах заюзанный размер стека
            ldr     r2, [r5, #20]   //размер стека прерываний
            ldr     r1, [r5, #24]   //размер стека майна
            add     r2, r2, r1
            ldr     r3, =__Vector_table
            ldr     r3, [r3]
            ldr     r3, [r3]        //вершина стека
            add     r2, r2, #160    //(задача_нуль(8*4*4)+(таблица(8*4)))+прерывания
            sub     r3, r3, r2      //хвост майна (вероятная голова новой задачи)
            str     r3, [r5, #24]   //удачный стек
            str     r3, [r5, #44]   //task_stop - последний адрес стека
            ldr     r1, =__HeapBase //стартовый адрес
            add     r1, r1, #15     //маллок_адрес
            bfc     r1, #0, #4
            str     r1, [r5, #36]   //malloc_start - первый адрес malloc
            str     r1, [r5, #40]   //malloc_stop - последний адрес malloc
            mov     r1, #0
            str     r1, [r5, #48]   //alarm_mc - время для задержки выполнения условия
            mov     r1, #3
            cpsie    i
            msr	   CONTROL, r1
            isb
            mov     r1, #54         //TIM6 global Interrupt
            mov     r2, #0x0F
            svc     0x0             //sNVIC_EnableIRQ(TIM6_IRQn, 15)
            mov     r1, #0xFF
            svc     0x0             //sNVIC_EnableIRQ(SysTick_IRQn, 15);
            pop     {r0, r1, r2, r3, R4, R5, lr}
            bx      lr






__Main_Name:
 .asciz "Main"
__sTask_nil_Nane:
 .asciz "sTask_nil"

 .size Start_task, . - Start_task


 .align 4
 .global    sTask_kill
 .type    sTask_kill, %function
sTask_kill:
            push    {r0, r1, r2, r3 }
            ldr     r3, = sSystem_task      // адрес переменной с адресом активной задачи
            ldr     r2, [r3]                // читаем адрес исполняемого банка
            ldr     r1, =__Main_Name
            ldr     r0, [r2, #28]
            sub     r0, r1, r0
            cbz     r0, sTask_kill_err
            mov     r1, #3                 // флаг обработки для sTask_nil, 3 __free_all
            strb    r1, [r2, #15]          // флаг запроса на обработку
            svc     0xA                    // нить отложенных заданий
            svc     0x5
            nop
            nop
sTask_kill_err:
            push    {r0, r1, r2, r3 }
            bx      lr

 .size sTask_kill, . - sTask_kill




 .align 4
 .globl    PendSV_Handler
 .type    PendSV_Handler, %function
PendSV_Handler:

 .ifdef  __Test_psp
            ldr     r1, = sSystem_task  // адрес переменной с адресом активной задачи
            ldr     r0, [r1]            // читаем адрес исполняемого банка
            ldrh    r3, [r0, #26]       // мах размер стека (мах заюзанный размер стека)
            mrs     r2, psp
            sub     r2, r0, r2
            add     r2, r2, #64         // текущий актуальный размер стека
            cmp     r2, r3
            ittt    hi                  // если больше запланированного
            strhhi  r2, [r0, #26]       // новый мах размер стека
            ldrhhi  r3, [r0, #24]       // размер стека (задаётся при запуске)
            cmphi   r2, r3
            bls     PendSV_step_norm
            bkpt        // пошаговый возврат в жадную задачу
            bx      lr // R0 адрес в стеке, R2- требуемый размер стека

PendSV_step_norm:
 .endif

            ldr     r1, = sSystem_task      // адрес переменной с адресом активной задачи
            ldr     r3, [r1]                // читаем адрес исполняемого банка
            ldr     r0, [r3]                // читаем адрес следующей нити
            mrs     r2, psp
            stmdb   r2!, {r4-r11}           // сохраняем регистры
            str     r2, [r3, #8]            // сохранили адрес стека текущей задачи
            str     r0, [r1]                // в переменной адрес новой задачи
PendSV_step: // r0 - новая задача
            ldr     r2, [r0, #8]            // читаем адрес стека
            ldr     r3, =__TIM6_CR1
            ldrh    r1, [r0, #12]           // читаем новое время
            str     r1, [r3, #44]           // сохранили время
            mov     r1, #1
            str     r1, [r3, #20]           // перезаруск
            mov     r1, #0
            str     r1, [r3, #16]           // сброс прерывания TIM6
            ldmia   r2!, {r4-r11}           // читаем сохранённое
            msr	    psp, r2                 // переписываем стек
            isb
            bx      lr

 .size PendSV_Handler, . - PendSV_Handler


 .ifdef  __Test_psp
 .align 4
 .globl    Handler_zize
 .type    Handler_zize, %function
Handler_zize:
            ldr     r0, =sSystem_task
            ldr     r1, [r0, #16]       // заюзанный размер стека под прерывания
            ldr     r3, =__Vector_table
            ldr     r3, [r3]
            ldr     r3, [r3]            //вершина стека
            mrs     r2, msp
            sub     r3, r3, r2
            cmp     r3, r1
            it      hs
            strhs   r3, [r0, #16]
            ldr     r2, [r0, #20]
            cmp     r2, r3
            bhs     Handler_zi
            bkpt                        // Мало стека под прерывания
Handler_zi:
            bx      lr
 .size Handler_zize, . - Handler_zize
 .endif


 .align 4
 .globl    SysTick_Handler
 .type    SysTick_Handler, %function
SysTick_Handler:
            ldr     r0, =__DWT          // временно
            ldr     r1, =sSystem_task   // временно
            mov     r2, #0              // временно
            ldr     r3, [r0, #4]        // временно
            str     r2, [r0, #4]        // временно
            str     r3, [r1, #52]       // временно

            svc     0x6                 //__sDelay_work
            bx      lr

 .size SysTick_Handler, . - SysTick_Handler



  .align 4
 .globl    TIM6_IRQHandler
 .type    TIM6_IRQHandler, %function
TIM6_IRQHandler:
            ldr     r0, =__TIM6_CR1
            mov     r1, #0
            str     r1, [r0, #16]           //сброс прерывания TIM6
            ldr     r0, =__Vector_table
            movt    r1, #0x1000             // сигнатура 0x10000000 PendSV
            str     r1, [r0, #-4]           // адрес 0xE000ED04 PendSV_Handler
            nop
            nop
            bx      lr


 .size TIM6_IRQHandler, . - TIM6_IRQHandler

 .align 4
 .globl    sTask_nil
 .type    sTask_nil, %function
sTask_nil:
            ldr     r12, = sSystem_task     // адрес переменной с адресом активной задачи
            ldr     r10, = __TIM6_CR1
            //(r11)  // адрес задач на обработку памяти


sTask_nil_re:
            ldr     r11, [r12, #12]          // адрес задач на обработку памяти
            mov     r0, r11
            cbnz    r0, sTask_nil_nw
            ldr     r0, [r12]
            ldr     r1, [r0]
            cmp     r0, r1
            itt     ne                       // то ждём физики
            ldrne   r0, [r10, #44]
            strne   r0, [r10, #36]
            nop
            nop
            b       sTask_nil_re
sTask_nil_nw:
            ldrb    r0, [r11, #15]           // читаем флаг
            tbh     [pc, r0, lsl #1]

__malloc_Table:
    .hword   ((__malloc_0 - __malloc_Table)/2)      //0
    .hword   ((__maloc_in - __malloc_Table)/2)      //1
    .hword   ((__free - __malloc_Table)/2)          //2
    .hword   ((__free_all - __malloc_Table)/2)      //3
    .hword   ((__sTask_kill - __malloc_Table)/2)    //4
    .hword   ((__memory_don - __malloc_Table)/2)    //5

  //  .hword   ((__sDel_new - __malloc_Table)/2)


 .align 4
__malloc_0: // флаг0 - ошибка
            svc     0xB                     // __nil_ww
            b       sTask_nil_re

//новое
 .align 4
__maloc_in: // флаг 1 - текущая задача r11 - ибо заепало
            ldr     r0, [r11, #16]          // читем sleep_wake задачи - >0 признак неудачной операции
            cbz     r0, __maloc_in_01
            subs    r0, r0, #1    //проверить
            str     r0, [r11, #16]          // сохранили
            itt     hs
            svchs   0xF                     // __nil_next - пропуск в обработке
            bhs     sTask_nil_re            // новый круг
__maloc_in_01:
            ldr     r5, [r11, #8]           // читаем стек задачи
            ldr     r4, [r5, #__STACK_Mx]   // читаем размер malloc
 .ifdef  __Test_psp
            lsr     r2, r4, #23
            cbz     r2, __maloc_norm
            cbnz    r4, __maloc_norm
            mvn     r2, #0                  // ошибка - размер malloc больше 32мбайт,или нулевой
            str     r2, [r5, #__STACK_Mx]   // пишем нереальный адрес
            svc     0xB                     // __nil_ww
            b       sTask_nil_re            // новый круг
__maloc_norm:
 .endif
            add     r4, r4, #7              // +заголовок
            lsr     r4, r4, #2              // смещение
            ldr     r3, [r12, #36]          // malloc_start - первый адрес malloc
__maloc_in_for:    // r3 - адрес старта
            ldr     r0, [r3]                // читаем имя/размер
            cbz     r0, __maloc_in_02       // последний
            lsr     r1, r0, #23             // хозяин
            bfc     r0,	#23, #9             // размер
            cmp     r1, #0
            itt     ne
            addne   r3, r3, r0, lsl #2
            bne     __maloc_in_for
            add     r1, r4, #3
            cmp     r0, r1                  // проверка на свободное место
            itt     lo
            addlo   r3, r3, r0, lsl #2
            blo     __maloc_in_for
__maloc_in_03:
            ldrh    r2, [r11, #20]          // читаем task_nomer задачи
            orr     r1, r4, r2, lsl #23
            str     r1, [r3]                // сохраняем нового хозяина и смещение
            sub     r2, r0, r4
            str     r2, [r3, r4, lsl #2]    // остаток
            add     r2, r3, r4, lsl #2
            add     r3, r3, #4              // адрес для передачи в функцию
            str     r3, [r5, #__STACK_Mx]   // пишем новый адрес malloc
            mov     r1, #0
__maloc_in_00:
            cmp     r2, r3
            itt     ne
            strne   r1, [r3], #4
            bne     __maloc_in_00
            svc     0xB                     // __nil_ww
            beq     sTask_nil_re            // новый круг
__maloc_in_02:
            add     r7, r3, r4, lsl #2      // r7 - проверка на границу
            svc     0x2                     // __malloc_in_0 - коррекция пределов
            cmp     r7, #0                  // неудача -   нулевой адрес
            itttt   eq
            moveq   r1, 20                  // пропуск - 20 циклов нулевой задачи
            streq   r1, [r11, #16]
            svceq   0xF                     // __nil_next - пропуск в обработке
            beq     sTask_nil_re            // новый круг
            mov     r0, r4
            b       __maloc_in_03



 .align 4
__free:
            ldr     r7, [r11, #8]           // читаем стек задачи
            ldr     r2, [r7, #__STACK_Mx]   // читаем free (malloc_adres) задачи

            ldrh    r3, [r11, #20]          // читаем номер хозяина
            ldr     r4, [r2, #-4]           // читаем настоящего хозяина\размер
            subs    r1, r3, r4, lsr #23     // 0
            itt     ne
            svcne   0xB                     // __nil_ww
            bne     sTask_nil_re            // новый круг
            bfc     r4, #23, #9
            str     r4, [r2, #-4]           // затираем хозяина
            mov     r2, #0
            ldr     r1, [r12, #36]          // malloc_start - первый адрес malloc
            b       __free_all_1


 .align 4
__free_all:
            ldrh    r2, [r11, #20]          // читаем номер хозяина
            ldr     r1, [r12, #36]          // malloc_start - первый адрес malloc
__free_all_1: // r1 старт
            mov     r5, #512
            ldr     r7, [r1]                // читаем хозяин\размер
__free_all_2:
            cbz     r7, __free_all_3        // выход
            mov     r0, r7, lsr #23         // номер
            bfc     r7, #23, #9             // размер
            subs    r5, r2, r0
            ite     eq                      // хозяин совпал
            streq   r7, [r1]                // затираем
            movne   r5, r0
            cmp     r5, #0
            itet     ne
            addne   r1, r1, r7, lsl #2
            addeq   r3, r1, r7, lsl #2
            bne     __free_all_1
            ldr     r4, [r3]                // читаем хозяина/размер
            cbz     r4, __free_all_3        // выход
            mov     r0, r4, lsr #23         // номер
            bfc     r4, #23, #9             // размер
            subs    r5, r2, r0
            it      ne                      // хозяин несовпал
            movne   r5, r0
            cmp     r5, #0
            ittt    eq
            addeq   r7, r7, r4
            streq   r7, [r1]
            beq     __free_all_2
            add     r1, r1, r7, lsl #2
            b       __free_all_1
__free_all_3:
            cmp     r5, #0
            ittt    eq
            streq   r5, [r1]
            addeq   r1, r1, #8
            streq   r1, [r12, #40]          // новая граница
            svc     0xB                     // __nil_ww
            b       sTask_nil_re            // новый круг

 .align 4
__sTask_kill:
            svc     0x5                     // __Delete_Task
            b       sTask_nil_re            // новый круг

 .align 4
__memory_don:   // svc 0xD;  стек r0 *link_memory, r1 char* const task_func_name
            ldr     r1, [r11, #8]            // Cтек задачи
            ldr     r2, [r1, __STACK_Mx +4]  // [r1] char* const task_func_name
            mov     r3, r11
__memory_don1:
            ldr     r3, [r3]
            cmp     r11, r3
            ittt    eq
            moveq   r0, #10
            streq   r0, [r11, #16]          // пишем sleep_wake задачи - 10 признак неудачной операции
            beq     sTask_nil_re
            ldrb    r4, [r3, #15]           // читаем Флаг запроса на обработку
            cmp     r4, #6                  // флаг ожидания памяти
            bne     __memory_don1           // не совпало
            ldr     r4, [r3, #28]           // Адрес имени задачи (char* const text)
            cmp     r2, r4
            bne     __memory_don1           // не совпало
            ldr     r4, [r1, __STACK_Mx]    // [r0] *link_memory
            ldr     r2, [r4, #-4]           // там был адрес ссылки
            ldrh    r0, [r3, #20]           // читаем номер получателя
            bfc     r2, #23, #9             // размер
            orr     r2, r2, r0, lsl #23
            str     r2, [r4, #-4]           // новый хозяин
            mvn     r0, #0
            str     r0, [r1, __STACK_Mx]    // затёрли адрес ссылки на ffff. при обращении будет ошибка доступа
            mov     r0, #0
            strb    r0, [r3, #15]           // стёрли флаг принимающей задачи
            svc     0xB                     // __nil_ww
            b       sTask_nil_re



 .size sTask_nil, . - sTask_nil






