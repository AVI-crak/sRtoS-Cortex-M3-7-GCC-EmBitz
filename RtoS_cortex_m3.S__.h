// Cortex-M3 GCC EmBitz 0.40
/* RtoS_cortex_m3.S */
/* videocrak@maol.ru */

 .syntax unified
 .arch armv7-m

 .equ   __Test_psp,1
 .equ   __Vector_table, 0xE000ED08
 .equ   __SysTick_CTRL, 0xE000E010 // #0xE010 SysTick->CTRL, #0xE014 SysTick->LOAD, #0xE018 SysTick->VAL
 .equ   __TIM6_CR1,     0x40001000 // +12 DIER, +16 SR, +20 EGR, +36 CNT, +40 PSC, >+44 ARR
 .equ   __RCC_APB1ENR,  0x4002101C
 .equ   __tik_real_ms,  0x000F4240


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
    .hword   ((__SetPriorityS - SVC_Table)/2)    //2
    .hword   ((___sRandom - SVC_Table)/2)        //3
    .hword   ((__sTask_new - SVC_Table)/2)       //4
    .hword   ((__Delete_Task - SVC_Table)/2)     //5
    .hword   ((__sDelay_work - SVC_Table)/2)     //6
    .hword   ((__sDelay_new - SVC_Table)/2)      //7
    .hword   ((__sTask_wait - SVC_Table)/2)      //8
    .hword   ((__sTask_wake - SVC_Table)/2)      //9







__EnableIRQS:
            mov     r3, #0x1F
            and     r0, r1, r3
            mov     r3, #0x1
            lsls	r2, r3, r0
            lsrs    r0, r1, #5
            lsls	r0, r0, #2
            mov     r3, #0xE100
            movt    r3, #0xE000
            str     r2, [r3, r0]
            bx      lr

__DisableIRQS:
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

__SetPriorityS: //"r4" (IRQn),"r5" (priority)
            lsls    r0, r5, #4
            tst     r4, #0x80
            bne     SVC_step2

            movw    r2, #0xE400 //NVIC->IP 0
            movt    r2, #0xE000
            strb    r0, [r2, r4]
            bx      lr
SVC_step2:
            movw    r2, #0xED14 //SCB->SHP 0
            movt    r2, #0xE000
            bfc     r4, #4, #28
            strb    r0, [r2, r4]
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
            ldr     r1, [r3, #4]
            ldr     r2, [r3, #8]
            sub     r1, r1, r2
            mvn     r2, #0
            udiv    r1, r2, r1
            udiv    r0, r0, r1
            ldr     r2, [r3, #8]
            add     r0, r0, r2
            str     r0, [r3]
            bx      lr

__sTask_new: //r4 void (*taskS_func()), r5 lifetime_time_task, r6 task_size, r9 char* const task_func_name
         //   push    {r7,r8}
            ldr     r8,= sSustem_task       // адрес банка задачь
            ldr     r0, [r8, #24]           // удачный стек
            add     r6, r6, #31
            bfc     r6, #0, #5              // выровняли размер новой задачи
            mov     r1, #12
SVC_step4_1:
            sub     r1, r1, #4
            cmp     r1, #12
            bhi     SVC_step_stop
            ldr     r3, [r8, r1]            // адрес первой задачи
            cmp     r3, #0
            beq     SVC_step4_1
            mov     r7, r3                  // первая задача в списке
//0 -удачный хвост, 1 счётчик, 2 новая голова,  7 кол линков, 8 банк
SVC_step4:
            add     r2, r7, r6
            add     r2, r2, #32
            cmp     r2, r0 //>
            itttt   hi
            ldrhhi  r2, [r7, #24]           // читаем новый хвост
            subhi   r2, r2, #32
            subhi   r2, r7, r2
            cmphi   r0, r2
            ittt    hi
            movhi   r0, r2
            movhi   r1, #12                 // хвост изменился
            bhi     SVC_step4_1
            ldr     r2, [r7]
            cmp     r3, r2
            itt     ne
            movne   r7, r2
            bne     SVC_step4
            b       SVC_step4_1

SVC_step_stop: // r0 вершина стека новой нити
            sub     r1, r0, #96
            mov     r2, #0
SVC_step4_6:
            cmp     r1, r0
            itt     ne
            strne   r2, [r1], #4
            bne     SVC_step4_6
            sub     r1, r0, #32             //новая задача
            ldr     r2, [r8]                //активная задача
            ldr     r3, [r2, #4]            //адрес преведущей задачи ( указывает на голову)
            str     r1, [r3]                //голова указывает на новую
            str     r1, [r2, #4]            //активная указывает на новую старую задачу
            str     r2, [r1]                //новая указывает на активную
            str     r3, [r1, #4]            //новая указывает на старую голову
            str     r4, [r1, #-8]           //сохранили pc
            ldr     r0, =__sTask_kill
            str     r0, [r1, #-12]          //сохранили ложный lr
            ldr     r7, [r8, #12]           //tik_real   // счётчик тиков
            cmp     r5, #100
            it      hi
            movhi   r5, #100
            cmp     r5, #1
            it      lo
            movlo   r5, #1
            mul     r7, r7, r5
            mov     r3, #0
            movt    r3, #0x100
            str     r3, [r1, #-4]           // сохранили xPSR в стек 0x1000000
            mov     r5, #100
            udiv    r7, r7, r5              //  подсчёт времени
            sub     r4, r1, #64
            str     r4, [r1, #8]            //стек задачи (активный хвост)
            strh    r6, [r1, #24]           //размер стека (задаётся при запуске)
            mov     r4, #64
            strh    r4, [r1, #26]           //мах заюзанный размер стека
            str     r7, [r1, #12]           //время активности задачи в потоке
            str     r9, [r1, #28]           //адрес имени задачи (char* const text)
        //    pop     {r7,r8}
            bx       lr


__Delete_Task:
            ldr     r1, = sSustem_task      // адрес переменной с адресом активной задачи
            ldr     r0, [r1]                // читаем адрес исполняемого банка
            ldr     r2, =__Main_Name
            ldr     r3, [r0, #28]
            sub     r2, r3, r2
            cbz     r2, _Delete_Task_error
            ldr     r2, [r0, #4]            //читаем адрес хвоста
            ldr     r3, [r0]                //читаем адрес новой задачи
            str     r3, [r2]                //хвост на новую
            str     r2, [r3, #4]            //хвост новой на голову старой
            str     r3, [r1]                //новая активная задача
            ldr     r0, [r3, #8]            // читаем адрес стека
            ldr     r1, [r3, #12]           // читаем новое время
            ldmia   r0!, {r4-r11}           // читаем сохранённое
            ldr     r2, =__TIM6_CR1
            str     r1, [r2, #44]            // сохранили время
            mov     r1, #1
            str     r1, [r3, #20]           // перезапуск
            msr	    psp, r0                 // переписываем стек
_Delete_Task_error:
            bx      lr


__sDelay_work:                              // обслуживание задержки
            ldr     r0, = sSustem_task      // адрес переменной с адресом активной задачи
            ldr     r1, [r0, #4]            // первая спящая задача
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
            ldr     r1, [r0]                // активная задача
            ldr     r2, [r1, #8]            // стек
            ldr     r3, =__TIM6_CR1
            ldr     r1, [r0, #12]           // читаем новое время
            str     r1, [r3, #44]           // сохранили время
            mov     r1, #1
            str     r1, [r3, #20]           // перезапуск
            ldmia   r2!, {r4-r11}           // читаем сохранённое
            msr	    psp, r2                 // переписываем стек
            bx      lr


__sTask_wait: // R2(флаг - адрес)
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
            ldr     r1, [r0]                // активная задача
            ldr     r2, [r1, #8]            // стек
            ldr     r3, =__TIM6_CR1
            ldr     r1, [r0, #12]           // читаем новое время
            str     r1, [r3, #44]           // сохранили время
            mov     r1, #1
            str     r1, [r3, #20]           // перезапуск
            ldmia   r2!, {r4-r11}           // читаем сохранённое
            msr	    psp, r2                 // переписываем стек
            bx      lr


__sTask_wake:
            nop
            nop
            nop
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
            ldr     r3, [r0, #12]   // us на задачу при старте
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
            str     r4, [r0, #12]   // мах количество тиков на задачу
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
            ite      lo              // если запланированный размер стека меньше текущего -то используем текуший
            addlo   r2, r3,#31
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
            ldr     r1, [r2, #12]   //tik_real   // счётчик тиков
            str     r1, [r0, #12]   //время активности задачи в потоке (ограничение 16bl)
            ldr     r1, [r2, #24]
            bfc     r1, #16, #16
            strh    r1, [r0, #24]   // размер стека не может быть выше 64к, сохранили
            mov     r1, #0
            str     r1, [r2, #4]    //обнуление спящих задачь
            str     r1, [r2, #8]    //обнуление задачь ожидающих пинка
            mrs	    r2, PSP
            sub     r2, r0, r2
            strh    r2, [r0, #26]   //мах размер стека (мах заюзанный размер стека)
            ldr     r1, =__Main_Name
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
            ldr     r2, [r5, #12]   //tik_real   // счётчик тиков
            ldr     r0, [r5]        // активная задача
            str     r3, [r0]        //адрес новой задачи (указывает на голову)
            str     r0, [r3]        //адрес новой задачи (указывает на голову)
            str     r3, [r0, #4]    //адрес преведущей задачи ( указывает на голову)
            str     r0, [r3, #4]    //адрес преведущей задачи ( указывает на голову)
            str     r2, [r3, #12]   //время активности задачи в потоке
            sub     r1, r3, #64
            str     r1, [r3, #8]    //стек задачи (активный хвост)
            ldr     r1, =__sTask_nil_Nane
            str     r1, [r3, #28]   //адрес имени задачи (char* text) // номер задачи
            mov     r1, #128
            strh    r1, [r3, #24]   // размер стека сохранили
            mov     r1, #64
            strh    r1, [r3, #26]   // мах заюзанный размер стека
            ldr     r2, [r5, #20]           // размер стека прерываний
            ldr     r1, [r5, #24]           //размер стека майна
            add     r2, r2, r1
            ldr     r3, =__Vector_table
            ldr     r3, [r3]
            ldr     r3, [r3]                //вершина стека
            add     r2, r2, #160            // (задача_нуль(8*4*4)+(таблица(8*4)))+прерывания
            sub     r3, r3, r2              //хвост майна (вероятная голова новой задачи)
            str     r3, [r5, #24]           //удачный стек
            mov     r1, #3
            cpsie    i
            msr	   CONTROL, r1
            nop
            mov     r1, #54         //TIM6 global Interrupt
            mov     r5, #0x0F
            mov     r4, r1
            svc     0x2             //sNVIC_SetPriority(TIM6_IRQn, 15);
            svc     0x0             //sNVIC_EnableIRQ(TIM6_IRQn)
            mov     r4, #0xFF
            svc     0x2             //sNVIC_SetPriority(SysTick_IRQn, 15);
            pop     {r0, r1, r2, r3, R4, R5, lr}
            bx      lr






__Main_Name:
 .asciz "Main"
__sTask_nil_Nane:
 .asciz "sTask_nil"

 .size Start_task, . - Start_task


 .align 4
 .globl    __sTask_kill
 .type    __sTask_kill, %function
__sTask_kill:
            SVC  0x5
            nop
            nop
            bx      lr

 .size __sTask_kill, . - __sTask_kill




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
            ldr     r2, [r0, #8]            // читаем адрес стека
            ldr     r3, =__TIM6_CR1
            ldr     r1, [r0, #12]           // читаем новое время
            str     r1, [r3, #44]           // сохранили время
            mov     r1, #1
            str     r1, [r3, #20]           // перезаруск
            ldmia   r2!, {r4-r11}           // читаем сохранённое
            msr	    psp, r2                 // переписываем стек
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
            ldr     r1, = sSustem_task      // адрес переменной с адресом активной задачи
            ldr     r3, [r1]                // читаем адрес исполняемого банка
            ldr     r0, [r3]                // читаем адрес следующей нити
            sub     r2, r0, r3              // если активная задача одна
            cbz     r2, sTask_nil_step      // то ждём физики
            ldr     r0, =__TIM6_CR1
            ldr     r1, [r0, #44]
            str     r1, [r0, #36]

            nop
            nop
sTask_nil_step:
            wfi
            b       sTask_nil

 .size sTask_nil, . - sTask_nil



