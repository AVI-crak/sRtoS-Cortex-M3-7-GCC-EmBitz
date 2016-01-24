// Cortex-M3 GCC EmBitz 0.40
/* имя файла */
/* RtoS_cortex_m3.S */
/* процент готовности 35% */

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
 .equ   __SysTick_CTRL,     0xE000E010  // #0xE010 SysTick->CTRL, #0xE014 SysTick->LOAD, #0xE018 SysTick->VAL
 .equ   __TIM6_CR1,         0x40001000  // +12 DIER, +16 SR, +20 EGR, +36 CNT, +40 PSC, >+44 ARR
 .equ   __RCC_APB1ENR,      0x4002101C
 .equ   __tik_real_ms,      0x000F4240
 .equ   __TIM_Delay_IRQn,   54          //TIM6_IRQn


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

__malloc_in_0: //r7 - адрес возврата
            add     r0, r3, #12
            ldr     r2, [r12, #44]          // читем task_stop - последний адрес стека
            cmp     r2, r0
            itet    hi                      // есть место
            ldrhi   r2, [r12, #40]          // читаем malloc_stop - последний адрес malloc
            movls   r7, #0
            cmphi   r0, r2
            it      hi
            strhi   r0, [r12, #40]          // новый malloc_stop - последний адрес malloc
            bx      lr

___sRandom:
            ldr     r3, =Random_register
            ldmia   r3!, {r0-r2}           // читаем сохранённое
            mov     r3, #48
            cmp     r0, #0
            itt     eq
            movweq  r0, #0xD755
            movteq  r0, #0x8BD7
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
            ldr     r8,= sSustem_task       // адрес банка задачь
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
            sub     r1, r0, #96
            mov     r2, #0
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
SVC_step4_6:
            cmp     r1, r0
            itt     ne
            strne   r2, [r1], #4
            bne     SVC_step4_6
            sub     r0, r0, #32             //новая задача
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
            ldr     r1, [r8, #32]           //tik_real - 100% тиков на задачу
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
            cbz     r4, sTask_new_error
            ldmia   r4!, {r5-r8}            //чтение параметров функции
            stmia   r2!, {r5-r8}            //установка параметров функции
sTask_new_error:
            pop     {r7, r8}
            mov     r5, #0
            bx      lr


__Delete_Task:
            ldr     r3, [r12, #12]           // задача из обработки маллок
            ldr     r1, [r3, #4]            // хвост из обработки маллок
            ldr     r0, [r3]                // новая голова из обработки маллок
            subs    r2, r0, r1
            ittte   ne                      // не единственная
            strne   r0, [r1]                // хвост на голову
            strne   r1, [r0, #4]            // голова на хвост
            strne   r0, [r12, #12]           // первая голова в обработчике маллок
            streq   r2, [r12, #12]           // закрыли
            bx      lr


__sDelay_work:                              // обслуживание задержки
            ldr     r0, = sSustem_task      // адрес переменной с адресом активной задачи
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
__sDelay_exit:
            bx      lr


__sDelay_new:       // r2 время
            ldr     r0,  =sSustem_task
            ldr     r1, [r0]                // активная задача
            str     r2, [r1, #16]           // сохранили время сна
            mrs     r2, psp
            stmdb   r2!, {r4-r11}           // сохраняем регистры
            str     r2, [r1, #8]            // сохранили адрес стека активной задачи
            ldr     r2, [r1, #4]            // адрес хвоста
            ldr     r3, [r1]                // адрес новой головы
            str     r3, [r2]                // хвост на новую голову
            str     r2, [r3, #4]            // новая голова на хвост
            str     r3, [r0]                // новая голова активна
            ldr     r2, [r0, #4]            // адрес спящих задачь
            cmp     r2, #0
            itttt   eq
            streq   r1, [r0, #4]            // первая задача в спящих
            streq   r1, [r1]
            streq   r1, [r1, #4]
            beq     __sDelay_new_next
            ldr     r3, [r2, #4]            // хвост спящей
            str     r1, [r3]                // хвост на новую голову
            str     r3, [r1, #4]            // новая на хвост
            str     r1, [r2, #4]            // хвост первой спящей на новую
            str     r2, [r1]                // голова новой на спящую
__sDelay_new_next:
            ldr     r0, [r0]                // активная задача
            b       PendSV_step


__sTask_wait: // R2(глобал флаг - адрес)
            ldr     r0,  =sSustem_task
            mov     r1, #1
            str     r1, [r2]                // взвели флаг
            ldr     r1, [r0]                // активная задача
            str     r2, [r1, #16]           // сохранили адрес флага для пинка
            mrs     r2, psp
            stmdb   r2!, {r4-r11}           // сохраняем регистры
            str     r2, [r1, #8]            // сохранили адрес стека активной задачи
            ldr     r2, [r1, #4]            // адрес хвоста
            ldr     r3, [r1]                // адрес новой головы
            str     r3, [r2]                // хвост на новую голову
            str     r2, [r3, #4]            // новая голова на хвост
            str     r3, [r0]                // новая голова активна
            ldr     r2, [r0, #8]            // адрес задачь ожидающих пинка
            cmp     r2, #0
            itttt   eq
            streq   r1, [r0, #8]            // первая задача ожидающих пинка
            streq   r1, [r1]
            streq   r1, [r1, #4]
            beq     __sTask_wait_next
            ldr     r3, [r2, #4]            // хвост ожидающих пинка
            str     r1, [r3]                // хвост на новую голову
            str     r3, [r1, #4]            // новая на хвост
            str     r1, [r2, #4]            // хвост первой ожидающих пинка на новую
            str     r2, [r1]                // голова новой на ожидающих пинка
__sTask_wait_next:
            ldr     r0, [r0]                // активная задача
            b       PendSV_step


__sTask_wake: // R2(глобал флаг - адрес) разбудить
            ldr     r3, [r2]                // читаем глобальный флаг
            cbz     r3, __sTask_wake_exit   // уже запускали
            ldr     r0, = sSustem_task      // адрес переменной с адресом активной задачи
            ldr     r1, [r0, #8]            // Адрес задачь ожидающих пинка
            cbz     r1, __sTask_wake_exit   // пусто, наверное глюк
            push    {R4}
            mov     r4, #0
            str     r4, [r2]                // сбрасываем глобальный флаг
__sTask_wake_next:
            ldr     r3, [r1, #16]           // читаем адрес флага в нити
            cmp     r3, r2
            ite     eq                      // нашлось?
            ldreq   r1, [r1]                // там точно есть
            bne     __sTask_wake_next       // ищим до потери пульса
            ldr     r2, [r1, #4]            // хвост в ожидающих пинка
            ldr     r3, [r1]                // новая голова в ожидающих пинка
            cmp     r3, r1
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
__sTask_wake_exit:
            bx      lr


__nil_: // нить отложенных заданий
            ldr     r0,  =sSustem_task
            ldr     r3, [r0]              // активная задача
            ldr     r1, =__sTask_nil_Nane
            str     r1, [r3, #16]           // сохранили адрес флага для пинка
            mrs     r2, psp
            stmdb   r2!, {r4-r11}           // сохраняем регистры
            str     r2, [r3, #8]            // сохранили адрес стека активной задачи
            ldr     r2, [r3, #4]            // адрес хвоста
            ldr     r1, [r3]                // адрес новой головы
            str     r1, [r2]                // хвост на новую голову
            str     r2, [r1, #4]            // новая голова на хвост
            str     r1, [r0]                // новая голова активна
            ldr     r2, [r0, #12]           // aдрес задач на обработку памяти
            cmp     r2, #0
            itttt   eq
            streq   r3, [r0, #12]            // первая задача обработку памяти
            streq   r3, [r3]
            streq   r3, [r3, #4]
            beq     __nil_next
            ldr     r1, [r2, #4]            // хвост ожидающих пинка
            str     r3, [r1]                // хвост на новую голову
            str     r1, [r3, #4]            // новая на хвост
            str     r3, [r2, #4]            // хвост первой ожидающих пинка на новую
            str     r2, [r3]                // голова новой на ожидающих пинка
__nil_next:
            ldr     r0, [r0]                // активная задача
            b       PendSV_step


__nil_ww:  // возврат из нити отложенных задач r7 - адрес размер
         //   ldr     r12,  =sSustem_task
            ldr     r3, [r12, #12]           // задача из обработки маллок
            str     r7, [r3, #20]           // пишем адрес нового маллока-размера
            mov     r2, #0
            strb    r2, [r3, #15]           // сброс флага
            str     r2, [r3, #16]           // сброс адрес глобального флага пинка
            ldr     r1, [r3, #4]            // хвост из обработки маллок
            ldr     r0, [r3]                // новая голова из обработки маллок
            cmp     r1, r3
            ittte   ne                      // не единственная
            strne   r0, [r1]                // хвост на голову
            strne   r1, [r0, #4]            // голова на хвост
            strne   r0, [r12, #12]           // первая голова в обработчике маллок
            streq   r2, [r12, #12]           // закрыли
            ldr     r0, [r12]                // адрес активных задач
            ldr     r1, [r0, #4]            // адрес хвоста активных задач
            str     r3, [r1]                // хвост на новую голову
            str     r1, [r3, #4]            // маллок на хвост
            str     r0, [r3]                // маллок на активную голову
            str     r3, [r0, #4]            // хвост активной на маллок
            bx      lr

__sTask_ask: // запрос ресурса  name_resource >> ("r4") >> resource_flag__
            mov     r1, r4
            ldr     r4, [r4]                // читаем хозяина
            ldr     r2,  =sSustem_task
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
            bx      lr


 .size SVC_Handler, . - SVC_Handler


 .align 4
 .globl    Start_task
 .type    Start_task, %function

Start_task: // перемещение стеков
            cpsid   i
            push    {r0, r1, r2, r3, R4, R5, lr}
            ldr     r0,  =sSustem_task
            ldr     r1, [r0, #28]   // sSYSHCLK // системная частота
            ldr     r3, [r0, #32]   // us на задачу при старте
            ldr     r2, =__tik_real_ms
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
            ldr     r2,  =sSustem_task
            str     r0, [r2]        //task_presently - Адрес активной задачи
            ldr     r1, [r2, #32]   //tik_real - 100% тиков на задачу
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
            ldr     r5,  =sSustem_task
            ldr     r2, [r5, #32]   //tik_real - 100% тиков на задачу
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
            nop
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
            ldr     r3, = sSustem_task      // адрес переменной с адресом активной задачи
            ldr     r2, [r3]                // читаем адрес исполняемого банка
            ldr     r1, =__Main_Name
            ldr     r0, [r2, #28]
            sub     r0, r1, r0
            cbz     r0, sTask_kill_err
            mov     r1, #3                 // флаг обработки для sTask_nil, 3 __free_all
            strb    r1, [r2, #15]          // флаг запроса на обработку
            svc     0xA                    // нить отложенных заданий
            mov     r1, #4                 // флаг обработки для sTask_nil, 4 __sTask_kill
            strb    r1, [r2, #15]          // флаг запроса на обработку
            svc     0xA                    // нить отложенных заданий
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
            ldr     r1, = sSustem_task  // адрес переменной с адресом активной задачи
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

            ldr     r1, = sSustem_task      // адрес переменной с адресом активной задачи
            ldr     r3, [r1]                // читаем адрес исполняемого банка
            ldr     r0, [r3]                // читаем адрес следующей нити
            sub     r2, r0, r3
            cbz     r2, __PendSV_error
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
            ldmia   r2!, {r4-r11}           // читаем сохранённое
            msr	    psp, r2                 // переписываем стек
         //   ldr     r2, = 0x40007414
         //   str     r0, [r2]
__PendSV_error:
            bx      lr

 .size PendSV_Handler, . - PendSV_Handler




 .align 4
 .globl    SysTick_Handler
 .type    SysTick_Handler, %function
SysTick_Handler:
            svc     0x6     //__sDelay_work
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
            ldr     r12, = sSustem_task     // адрес переменной с адресом активной задачи
            ldr     r11, =__TIM6_CR1
            ldr     r10, [r12, #36]          //malloc_start - первый адрес malloc

sTask_nil_re:
            ldr     r0, [r12, #12]          // адрес задач на обработку памяти
            cbz     r0, __malloc_0          // то ждём физики
            ldrb    r1, [r0, #15]           // читаем флаг
            tbh     [pc, r1, lsl #1]

__malloc_Table:
    .hword   ((__malloc_0 - __malloc_Table)/2)      //0
    .hword   ((__malloc_in - __malloc_Table)/2)     //1
    .hword   ((__free - __malloc_Table)/2)          //2
    .hword   ((__free_all - __malloc_Table)/2)      //3
    .hword   ((__sTask_kill - __malloc_Table)/2)    //4





__malloc_0: // флаг0 - ошибка
            b       sTask_nil_step


__malloc_in: // флаг 1 -
            ldr     r2, [r0, #20]           // читем malloc_zize задачи
            add     r2, r2, #11             // +заголовок
            bfc     r2, #0, #2
            ldr     r9, [r0, #28]           // хозяин
            mov     r7, #0
            mov     r6, r10
__malloc_in_for:    // r6 - адрес старта, R7 - сдвиг
            add     r4, r6, r7
            ldr     r3, [r4]                // новая голова
            ldr     r5, [r4, #4]            // читаем хозяина
            cmp     r5, #0
            ittt    ne                      // есть хозяин
            movne   r7, #0
            addne   r6, r4, r3
            bne     __malloc_in_for
            cbnz    r3, __malloc_in_1       // не последняя
            add     r3, r6, r2
            add     r7, r6, #8              // адрес для передачи в функцию
            svc     0x2                     // __malloc_in_0 - коррекция пределов
            cbz     r7, __free_error        // неудача -   нулевой адрес
            str     r2, [r6]                // новый размер
            str     r9, [r6, #4]            // новый хозяин
            str     r5, [r3]                // новая последняя
            str     r5, [r3, #4]            // без хозяина
            svc     0xB                     // __nil_ww
            b       sTask_nil_re            // новый круг
__malloc_in_1:                              // не последняя
            add     r7, r7, r3
            cmp     r2, r7
            bhi     __malloc_in_for         // > мало места
            str     r2, [r6]                // новый размер
            str     r9, [r6, #4]            // новый хозяин
            add     r1, r6, #8
            sub     r3, r7, r2
            cmp     r3, #12
            ittte   hs                      // есть остаток >=  для нового
            addhs   r6, r6, r2
            strhs   r3, [r6]                // сдвиг до остатка
            strhs   r5, [r6, #4]            // без хозяина
            strlo   r7, [r6]
            mov     r7, r1                  // адрес для передачи в функцию
            svc     0xB                     // __nil_ww
            b       sTask_nil_re            // новый круг

__free:
            ldr     r2, [r0, #20]           // читем free (malloc_adres) задачи
            ldr     r3, [r0, #28]           // читем хозяина
            ldr     r4, [r2, #-4]           // читем настоящего хозяина
            subs    r1, r3, r4              // 0
            bne     __free_error
            str     r1, [r2, #-4]           // затираем хозяина
            ldr     r5, [r2, #-8]           // читаем размер
            add     r3, r5, r2
            ldr     r4, [r3, #-8]           // читаем следующий маллок
            cmp     r4, #0                  // если там пусто
            ittt    eq
            streq   r1, [r2, #-8]           // стёрли указатель
            addeq   r2, r2, #4
            streq   r2, [r12, #40]          // новая граница
__free_error:
            mov     r7, #0
            svc     0xB                     // __nil_ww
            b       sTask_nil_re            // новый круг

__free_all:
            ldr     r2, [r0, #28]           // читем хозяина
            mov     r1, r10                 // старый адрес
__free_all_1: // r1 старт
            ldr     r7, [r1]                // читаем размер
            ldr     r4, [r1, #4]            // читаем хозяина
            subs    r6, r2, r4
            ite     eq                      // хозяин совпал
            streq   r6, [r1, #4]            // затираем
            movne   r6, r4
            cbz     r7, __free_all_3        // выход
            cmp     r6, #0
            itt     ne
            addne   r1, r1, r7
            bne     __free_all_1
            add     r3, r1, r7
__free_all_2:
            ldr     r4, [r3]                // читаем размер
            cbz     r4, __free_all_3        // выход
            ldr     r5, [r3, #4]            // читаем хозяина
            subs    r8, r2, r5
            it      ne                      // хозяин совпал
            movne   r8, r5
            cmp     r8, #0
            it      ne
            bne     __free_all_1
            sub     r5, r3, r1
            add     r7, r5, r4
            str     r7, [r1]
            add     r3, r3, r4
            b       __free_all_2
__free_all_3:
            cmp     r6, #0
            ittt    eq
            streq   r6, [r1]
            addeq   r1, r1, #8
            streq   r1, [r12, #40]          // новая граница
            mov     r7, #0
            svc     0xB                     // __nil_ww
            b       sTask_nil_re            // новый круг


__sTask_kill:
            svc     0x5                     // __Delete_Task
            b       sTask_nil_re            // новый круг

sTask_nil_step:
            ldr     r0, [r12]               // адрес активной задачи (себя)
            ldr     r1, [r0]                // следующая задача
            cmp     r0, r1
            itt     ne                      // нулевая задача не одна
            ldrne   r0, [r11, #44]
            strne   r0, [r11, #36]
            nop
            nop
            nop
            wfi
            b       sTask_nil_re

 .size sTask_nil, . - sTask_nil

