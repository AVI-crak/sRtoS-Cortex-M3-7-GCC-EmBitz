/**
 @file    RtoS_cortex_m7.S
 @author  AVI-crak
 @version V-12%
 @date    28-декабря-2016
 @brief   Аxis sRtoS, Cortex-M7 ARM GCC EmBitz

 license
 Million helicopters and one cent.

 форум для обсуждения
 http://forum.ixbt.com/topic.cgi?id=48:11735

 репозиторий
 https://bitbucket.org/AVI-crak/rtos-cortex-m3-gcc

 общий размер 200, минимальный размер стека под задачу 400
 STACK_Fx -96,-92,-88,-84,-80,-76,-72,-68,-64, -4,   0,  4,  8, 12,  16, 20, 24,  28, 32,~ 96,  100,  104
           r4, r5, r6, r7, r8, r9,r10,r11,s16~s31,H,r0, r1, r2, r3, r12, LR, PC,xPSR, s0,-s15,fpscr,/0x0/

 STACK_Mx -32,-28,-24,-20,-16,-12, -8, -4,  0,  4,  8, 12,  16, 20, 24,   28
           r4, r5, r6, r7, r8, r9,r10,r11, r0, r1, r2, r3, r12, LR, PC, xPSR

**/
 .syntax unified
 .cpu cortex-m7
 .arch armv7e-m
 .thumb
 .thumb_func


 .equ   __Test_psp,0               // проверка на ошибки
 .equ   __Vector_table,     0xE000ED08  // (0xE000ED04 SCB->ICSR)
 .equ   __DWT,              0xE0001000  // таймер arm ядра, можно отключить "//" в начале строки
 .equ   __SysTick_CTRL,     0xE000E010  // #0xE010 SysTick->CTRL, #0xE014 SysTick->LOAD, #0xE018 SysTick->VAL
 .equ   __STACK_Fx,         96          //смещение до первого сохранёного элемента с математикой




 .align 2
 .section  .text.SVC_Handler
 .globl     SVC_Handler
 .type    SVC_Handler, %function

SVC_Handler:
            tbh     [pc, r3, lsl #1]
SVC_Table:
    .hword   ((__EnableIRQS - SVC_Table)/2)      //0
    .hword   ((__DisableIRQS - SVC_Table)/2)     //1
    .hword   ((__Enabling_FPU - SVC_Table)/2)    //2
    .hword   ((___sRandom - SVC_Table)/2)        //3
    .hword   ((__sTask_new - SVC_Table)/2)       //4
    .hword   ((__Delete_Task - SVC_Table)/2)     //5
    .hword   ((__sTask_wake_mc - SVC_Table)/2)   //6
    .hword   ((__sDelay_new - SVC_Table)/2)      //7
    .hword   ((__sTask_wait - SVC_Table)/2)      //8
    .hword   ((__sTask_wake - SVC_Table)/2)      //9
    .hword   ((__nil_in - SVC_Table)/2)            //A
    .hword   ((__nil_exit - SVC_Table)/2)          //B
    .hword   ((__sTask_list - SVC_Table)/2)      //C
    .hword   ((__memory_donate - SVC_Table)/2)   //D
    .hword   ((__memory_get - SVC_Table)/2)      //E
    .hword   ((__nil_step - SVC_Table)/2)          //F
    .hword   ((__switch - SVC_Table)/2)          //10  переглючатель


 .align 2
__EnableIRQS: // ("r0") = IRQn, ("r1") = priority; ++
            movw    r12, #0xE70C
            movt    r12, #0xE000
            and     r0, #0xFF       //IRQn безопасность
            and     r1, #0xF        //Priority безопасность
            cmp     r1, #15
            it      eq              //проверка на мин приоритет
            moveq   r1, #14
            cmp     r0, #0xFF       //проверка SysTick_IRQn
            it      eq
            moveq   r1, #15         //SysTick_IRQn должен иметь минимальный приоритет
            lsl     r1, r1, #4      //сдвиг на границу
            mov     r3, r0
            and     r3, #0x0F
            sub     r3, r3, #4
            cmp     r0, #0xF1
            ittee   cs
            addwcs  r2, r12, #0x60C //SCB->SHPx 0xE000ED18
            strbcs  r1, [r2, r3]
            subwcc  r2, r12, #0x30c //NVIC->IPx 0xE000E400
            strbcc  r1, [r2, r0]
            and     r1, r0, #0x1F
            mov     r3, #0x1
            lsls	r1, r3, r1
            lsrs    r0, r0, #5
            lsls	r0, r0, #2
            subw    r2, r12, #0x60C //NVIC_ISERx 0xE000E100
            str     r1, [r2, r0]
            bx      lr

 .align 2
__DisableIRQS: // ("r0") = IRQn; ++
//NVIC->ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* disable interrupt */
            mov     r3, #0x1F
            and     r1, r0, r3
            mov     r3, #0x1
            lsls	r2, r3, r1
            lsrs    r1, r0, #5
            lsls	r1, r1, #2
            movw    r3, #0xE180
            movt    r3, #0xE000
            str     r2, [r3, r1]
            bx      lr
 .align 2
__Enabling_FPU: // не понадобилась

            bx      lr

 .align 2
___sRandom: // ("r0") = Random_max, ("r1") = Random_min
            ldr     r12,  =sSystem_task
            add     r12, r12, #68
            ldmia   r12, {r0-r2}           // читаем сохранённое
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
            stmia   r12, {r0-r2}
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






 .align 2
__sTask_new: // r0 - новая задача
            ldr     r2, [r12]                //активная задача
            ldr     r3, [r2, #4]            //адрес преведущей задачи ( указывает на голову)
            str     r0, [r3]                //голова указывает на новую
            str     r0, [r2, #4]            //активная указывает на новую старую задачу
            str     r2, [r0]                //новая указывает на активную
            str     r3, [r0, #4]            //новая указывает на старую голову
            b       __nil_exit
 .align 2
__sTask_wake_mc:  // r0 uint32_t* timer_name, r1 uint32_t timer_mc
            ldr     r2, [r0]                // читаем точку таймера
            ldr     r12,  =sSystem_task
            ldr     r3, [r12, #48]           // Системное время, System time counter
            sub     r2, r3, r2               // прошло времени с момента вызова
            subs    r2, r1, r2               // осталось времени до нового запуска
            itt     hi                       // если есть время внутри цикла
            subhi   r3, r3, r2               // считаем пропущенную точку сна
            subhi   r1, r1, r2               // считаем остаток сна
            str     r3, [r0]                 // сохраняем точку сна
            mov     r0, r1                   // сон на остаток
            b       __sDelay_new


 .align 2
__Delete_Task: // 5
            ldr     r12,= sSystem_task       // адрес банка задачь
            ldr     r3, [r12, #12]           // активная sTask_nil
            ldr     r0, [r12, #60]           // Список тасков
__Delete_Task_1:
            ldr     r1, [r0], #4            // читаем список
            cmp     r3, r1
            itte    eq                      // нашли
            moveq   r1, #0
            streq   r1, [r0, #-4]
            bne     __Delete_Task_1
            ldrh    r1, [r12, #20]           // Количество тасков в системе
            sub     r1, r1, #1
            strh    r1, [r12, #20]
            add     r1, r1, #48
            ldrh    r0, [r12, #22]           // Количество тасков в списке
            cmp     r1, r0
            itt     ls
            movls   r0, #0
            strhls  r0, [r12, #22]           // Количество тасков в списке ошибка
            ldr     r0, [r3]                // новая голова sTask_nil
            ldr     r1, [r3, #4]            // адрес хвоста активных задач sTask_nil
            subs    r2, r3, r0
            iteee     eq                      // одна задача
            streq   r2, [r12, #12]
            strne   r0, [r12, #12]           // теперь новая голова sTask_nil
            strne   r0, [r1]                // хвост на голову
            strne   r1, [r0, #4]            // голова на хвост
            dsb
            bx      lr


 .align 2
__sDelay_new:       // ("r0") = Delay_mc +
            ldr     r12,  =sSystem_task
            ldr     r1, [r12]                // активная задача
            ldr     r3, [r12, #48]           // Системное время, System time counter
            add     r3, r0, r3
            ldrb    r2, [r1, #13]           // mode_nomer
            str     r3, [r1, #16]           // сохранили точку сна
            bfc     r2, #6, #2
            add     r2, r2, #128
            strb    r2, [r1, #13]           // mode 2 = delay
            mov     r3, #4                  // задачи сна
            b       __nil_1

 .align 2
__sTask_wait: // ("r0") = task_global_flag +
            ldr     r12,  =sSystem_task
            ldr     r1, [r12]                // активная задача
            ldrb    r2, [r1, #13]           // mode_nomer
            bfc     r2, #6, #2
            add     r2, r2, #64
            mov     r3, #1
            strb    r2, [r1, #13]           // mode 1 = wait
            str     r3, [r0]                // взвели флаг
            str     r0, [r1, #16]           // сохранили адрес флага для пинка
            mov     r3, #8                  // задачи ожидающих пинка
            b       __nil_1


 .align 2
__sTask_wake: // r0(глобал флаг - адрес) разбудить
            cbz     r0, __sTask_wake_exit   // а там пусто...
            ldr     r3, [r0]                // читаем глобальный флаг
            ldr     r12, = sSystem_task      // адрес переменной с адресом активной задачи
            ldr     r1, [r12, #8]            // Адрес задачь ожидающих пинка
            cbz     r1, __sTask_wake_exit   // пусто, наверное глюк
            mov     r2, #0
            str     r2, [r0]                // сбрасываем глобальный флаг
            ldr     r2, [r1, #4]            // хвост
__sTask_wake_next:
            ldr     r3, [r1, #16]           // читаем адрес флага в нити
            subs    r3, r3, r0
            beq     __sTask_wake_1          // нашлось
            cmp     r2, r1
            itt     ne                      // не последняя
            ldrne   r1, [r1]                // там точно есть
            bne     __sTask_wake_next       // ищим
            bx      lr
__sTask_wake_1:
            str     r3, [r1, #16]           // сбрасываем адрес флага в нити
            ldr     r0, [r1, #4]            // хвост в ожидающих пинка
            ldr     r3, [r1]                // новая голова в ожидающих пинка
            subs    r2, r3, r1
            iteee   eq                      // последняя из ожидающих пинка
            streq   r2, [r12, #8]            // больше нет задач для пинка
            strne   r3, [r12, #8]            // следующая голова будет первой
            strne   r0, [r3, #4]            // голова на хвост
            strne   r3, [r0]                // хвост на голову
            ldr     r0, [r12]                // читаем активную задачу
            ldr     r3, [r0, #4]            // её хвост
            str     r1, [r3]                // хвост на новую задачу
            str     r3, [r1, #4]            // хвост новой на хвост старой
            str     r0, [r1]                // голова новой на голову активной
            str     r1, [r0, #4]            // хвост активной на новую задачу
            ldrb    r2, [r1, #13]           // mode_nomer
            bfc     r2, #6, #2
            add     r2, r2, #192
            strb    r2, [r1, #13]           // mode_nomer 3 activ
            dsb
__sTask_wake_exit:
            bx      lr


 .align 2
__nil_in: // нить отложенных заданий // r1 активная задача
            ldr     r12,  =sSystem_task
            ldr     r1, [r12]                // активная задача
            mov     r2, #0
            str     r2, [r1, #16]           // сброс d_wake
            ldrb    r2, [r1, #13]           // mode_nomer
            bfc     r2, #6, #2
            strb    r2, [r1, #13]           // mode 0 = hold
            mov     r3, #12                 // нить отложенных заданий

__nil_1:
            mrs     r2, psp
            vstmdb  r2!, {s16-s31}         // сохраняем регистры математики
            stmdb   r2!, {r4-r11}           // сохраняем регистры

            str     r2, [r1, #8]            // сохранили адрес стека активной задачи
            ldr     r4, [r1, #4]            // адрес хвоста
            ldr     r0, [r1]                // адрес головы
            str     r0, [r4]                // хвост на новую голову
            str     r4, [r0, #4]            // новая голова на хвост
            str     r0, [r12]                // новая голова активна
            ldr     r2, [r12, r3]            // адрес слабых
            cmp     r2, #0
            itttt   eq
            streq   r1, [r12, r3]            // первая задача слабых
            streq   r1, [r1]
            streq   r1, [r1, #4]
            beq     __switch_step
            ldr     r3, [r2, #4]            // хвост слабых
            str     r1, [r3]                // хвост на новую голову
            str     r3, [r1, #4]            // новая на хвост
            str     r1, [r2, #4]            // хвост первой слабых на новую
            str     r2, [r1]                // голова новой на слабых
            b       __switch_step


 .align 2
__nil_exit:  // возврат из нити отложенных задач r7 - адрес размер
         //   ldr     r12,  =sSystem_task
         //   ldr     r11, [r12, #12]           // задача из обработки маллок
            mov     r2, #0
            strb    r2, [r11, #15]           // сброс флага
            str     r2, [r11, #16]           // сброс d_wake
            ldr     r1, [r11, #4]            // хвост из обработки маллок
            ldr     r0, [r11]                // новая голова из обработки маллок
            cmp     r0, r11
            ittte   ne                      // не единственная
            strne   r0, [r1]                // хвост на голову
            strne   r1, [r0, #4]            // голова на хвост
            strne   r0, [r12, #12]           // первая голова в обработчике маллок
            streq   r2, [r12, #12]           // закрыли
            ldr     r0, [r12]               // активная задача в потоке
            ldr     r2, [r0, #4]            // хвост активной задачи
            str     r11, [r2]                // хвост на новую
            str     r2, [r11, #4]            // новая на хвост хвоста
            str     r0, [r11]                // новая на голову активной
            str     r11, [r0, #4]            // хвост активной на новую
            ldrb    r2, [r11, #13]           // mode_nomer
            bfc     r2, #6, #2
            add     r2, r2, #192
            strb    r2, [r11, #13]           // mode 3 = activ
            dsb
            bx      lr

 .align 2
__sTask_list:   //   malloc_adres = *task_list #60- Список тасков
                // task_list_zize_use #22- Количество тасков в списке
                //task_list_zize_sys #20- Количество тасков в системе
        //    ldr     r12, = sSystem_task     // временно
            mov     r1, #16                 // активные, спящие, оживающие пинка, обработка памяти
            ldr     r0, [r12, #60]          // Список тасков
__sTask_list_1:
            sub     r1, r1, #4
            cmp     r1, #18
            it      hi
            bxhi    lr                      // проверили все существующие нити
            ldr     r2, [r12, r1]           // адрес первой задачи
            cmp     r2, #0
            beq     __sTask_list_1
            mov     r3, r2                  // r3 первая задача в списке
__sTask_list_2:
            str     r2, [r0], #4            // пишем адрес задачи в массив
            ldr     r2, [r2]                // новая
            cmp     r2, r3
            bne     __sTask_list_2
            b       __sTask_list_1
//__sTask_list_3:
  //          bx      lr


 .align 2
__memory_donate: // SVC 0xD
            ldr     r12,  =sSystem_task
            ldr     r3, [r12]
            mov     r2, #5
            strb    r2, [r3, #15]           // флаг - отдать память
            b       __nil_1

__memory_get: // SVC 0xE
            ldr     r12,  =sSystem_task
            ldr     r3, [r12]
            mov     r2, #6
            strb    r2, [r3, #15]           // флаг - забрать память
            b       __nil_1

__nil_step: // SVC 0xF     __nil_step
        //    ldr     r0, [r12, #12]
            ldr     r11, [r11]
            str     r11, [r12, #12]
            bx      lr


__switch: // переглючатель +

 .ifdef  __Test_psp
            ldr     r12, = sSystem_task  // адрес переменной с адресом активной задачи
            ldr     r0, [r12]            // читаем адрес исполняемого банка
            ldrh    r3, [r0, #26]       // мах размер стека (мах заюзанный размер стека)
            mrs     r2, psp
            sub     r2, r0, r2
            add     r2, r2, #100         // текущий актуальный размер стека
            cmp     r2, r3
            ittt    hi                  // если больше запланированного
            strhhi  r2, [r0, #26]       // новый мах размер стека
            ldrhhi  r3, [r0, #24]       // размер стека (задаётся при запуске)
            cmphi   r2, r3
            bls     __switch_norm
            bkpt        // пошаговый возврат в жадную задачу
            bx      lr // rx0 адрес в стеке, rx2- требуемый размер стека
 .endif
__switch_norm:
            ldr     r12, = sSystem_task     // адрес переменной с адресом активной задачи
            ldr     r3, [r12]               // читаем адрес исполняемого банка
            ldr     r0, [r3]                // читаем адрес следующей нити
            mrs     r2, psp
            vstmdb  r2!, {s16-s31}         // сохраняем регистры математики
            stmdb   r2!, {r4-r11}           // сохраняем регистры
            str     r2, [r3, #8]            // сохранили адрес стека текущей задачи
            str     r0, [r12]                // в переменной адрес новой задачи
__switch_step: // rx0 - новая задача. учёт времени
 .ifndef  __DWT
            ldr     r2, =__SysTick_CTRL
            ldr     r12, =sSystem_task
            ldr     r3, [r2, #4]            // SysTick->LOAD было
            ldr     r4, [r2, #8]            // SysTick->VAL стало
            ldrh    r5, [r2, #2]            // SysTick->CTRL.COUNTFLAG
            cmp     r5, #1
            ite     eq                      // пеерполнение
            rsbeq   r4, r4, r3, lsl #1
            subne   r4, r3, r4
            ldr     r3, [r12, #52]           // дробный остаток
            add     r3, r3, r4
            ldr     r4, [r12, #56]           // норма остатка (1mc)
            ldr     r5, [r12, #48]           //системное время
            add     r5, r5, #1
            subs    r4, r3, r4 // <
            iteet   lo
            strlo   r3, [r12, #52]
            strhs   r4, [r12, #52]
            strhs   r5, [r12, #48]
            blo     __switch_sDelay_exit
 .endif

 .ifdef __DWT
            ldr     r2, =__DWT
            ldr     r12, =sSystem_task
            ldr     r4, [r12, #56]           // норма остатка (1mc)
            ldr     r5, [r12, #48]           // системное время
            add     r5, r5, #1
            ldr     r3, [r2, #4]            // системный таймер ядра
            subs    r3, r3, r4
            itte    hs  //>=
            strhs   r3, [r2, #4]            // обновили остаток
            strhs   r5, [r12, #48]
            blo     __switch_sDelay_exit
 .endif

__switch_sDelay:                              // обслуживание задержки
            ldr     r12, = sSystem_task      // адрес переменной с адресом активной задачи
            ldr     r2, [r12, #4]            // первая спящая задача
            cbz     r2, __switch_sDelay_exit
            ldr     r7, [r2, #4]            // хвост следующей не обработанной спящей
__switch_sDelay_next:
            ldr     r3, [r2, #16]           // читаем время
            ldr     r6, [r2]                // голова следующей не обработанной спящей
            subs    r3, r3, r5
            ite     eq
            streq   r3, [r2, #16]           // обнулили
            bne     __switch_sDelay_past
            ldr     r8, [r2, #4]            // хвост не обработанной спящей
            subs    r3, r6, r2
            iteee   eq
            streq   r3, [r12, #4]            // больше нет спящих
            strne   r6, [r12, #4]            // теперь отчёт с неё
            strne   r8, [r6, #4]            // голова на хвост
            strne   r6, [r8]                // голова хвоста на голову
            ldr     r3, [r12]                // активная задача в потоке
            ldr     r8, [r3, #4]            // хвост активной задачи
            str     r2, [r8]                // хвост на новую
            str     r8, [r2, #4]            // новая на хвост хвоста
            str     r3, [r2]                // новая на голову активной
            str     r2, [r3, #4]            // хвост активной на новую
            ldrb    r8, [r2, #13]
            bfc     r8, #6, #2
            add     r8, r8, #192
            strb    r8, [r2, #13]           // mode 3 = activ
__switch_sDelay_past:
            subs    r3, r7, r2
            itt     ne
            movne   r2, r6
            bne     __switch_sDelay_next

__switch_sDelay_exit:

            dsb
            ldr     r12, =__SysTick_CTRL
            ldr     r2, [r0, #8]            // читаем адрес стека
            ldmia   r2!, {r4-r11}           // читаем сохранённое
            vldmia  r2!, {s16-s31}           // грузим математику
            ldr     r3, [r0, #20]           // читаем новое время Task activity timer
            str     r3, [r12, #4]           // сохранили время #0xE014 SysTick->LOAD
            mov     r3, #7
            str     r3, [r12, #8]           // перезаруск #0xE018 SysTick->VAL
            strh    r3, [r12]
            msr	    psp, r2                 // переписываем стек
            dsb
            isb
            bx      lr


 .size SVC_Handler, . - SVC_Handler


 .align 2
 .section  .text.Start_task
 .globl    Start_task
 .type    Start_task, %function

Start_task: // перемещение стеков +
            cpsid   i
            push    {r0, r1, r2, r3, R4, R5, lr}
            ldr     r0,  =sSystem_task
            ldr     r1, =_ebss       //стартовый адрес
            add     r1, r1, #15     //маллок_адрес
            bfc     r1, #0, #4
            str     r1, [r0, #36]   //malloc_start - первый адрес malloc
            str     r1, [r0, #40]   //malloc_stop - последний адрес malloc
            mrs	    r3, MSP
            mov     r2, #0
_Start_task01:
            cmp     r1, r3
            itt     ne
            strne   r2, [r1], #4
            bne     _Start_task01   // обнулялка malloc
            ldr     r1, =__SysTick_CTRL
            ldr     r1, [r1, #3320] //  Vector table offset register (VTOR)
            ldr     r1, [r1]        //вершина стека
            ldr     r2, [r0, #24]   // размер стека майна , booked stack address
            mrs	    r3, MSP
            sub     r3, r1, r3      //размер стека на текущий момент
            cmp     r2, r3
            ite     lo              // если запланированный размер стека меньше текущего -то используем текуший
            addlo   r2, r3, #31
            addhs   r2, r2, #31
            bfc     r2, #0, #5
            str     r2, [r0, #24]   // обравняли на поле и сохранились
            ldrh    r2, [r0, #18]   // #18- Размер стека прерываний, Task NVIC stack size
            add     r2, r2, #664    // сдвиг на голову майна  (таблица майна8*4)+(задача_нуль(600)+(таблица(8*4)))+прерывания
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
            str     r0, [r2]        // task_presently - Адрес активной задачи
            ldr     r1, [r2, #32]   // tick_real - 100% тиков на задачу
            str     r1, [r0, #20]   // Таймер активности задачи
            mov     r1, #100        // 100% выделенный процент активности потоке
            strb    r1, [r0, #14]   // выделенный процент
            ldr     r1, [r2, #24]
            strh    r1, [r0, #24]   // размер стека не может быть выше 64к, сохранили
            mov     r1, #0
            mrs	    r2, PSP
            sub     r2, r0, r2
            strh    r2, [r0, #26]   //мах размер стека (мах заюзанный размер стека)
            ldr     r1, = __Main_Name
            str     r1, [r0, #28]
            movw    r1, #0xC002
            strh    r1, [r0, #12]   // Номер таска, Task unique ID
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
            str     r1, [r3, #(600-84)]  //148 сохранили фиктивный lr в стек 0x1000000
            ldr     r2, =sTask_nil
            str     r2, [r3, #(600-80)]  //152 сохранили pc sTask_nil
            str     r1, [r3, #(600-76)]  //125 сохранили xPSR в стек 0x1000000
            add     r3, r3, #600    //160 r3 голова таблицы sTask_nil
            ldr     r5,  =sSystem_task
            ldr     r2, [r5, #32]   //tick_real - 100% тиков на задачу
            str     r2, [r3, #20]   // Таймер активности задачи
            ldr     r0, [r5]        // активная задача
            str     r3, [r0]        //адрес новой задачи (указывает на голову)
            str     r0, [r3]        //адрес новой задачи (указывает на голову)
            str     r3, [r0, #4]    //адрес преведущей задачи ( указывает на голову)
            str     r0, [r3, #4]    //адрес преведущей задачи ( указывает на голову)
            mov     r2, #100
            strb    r2, [r3, #14]   //Процент активности задачи
            sub     r1, r3, #200
            str     r1, [r3, #8]    //стек задачи (активный хвост)
            ldr     r1, =__sTask_nil_Nane
            str     r1, [r3, #28]   //адрес имени задачи (char* text) // номер задачи
            movw    r1, #0xC001
            strh    r1, [r3, #12]   //Номер таска
            mov     r1, #600
            strh    r1, [r3, #24]   //размер стека сохранили
            mov     r1, #32
            strh    r1, [r3, #26]   //мах заюзанный размер стека
            ldr     r2, [r5]        //адрес таблицы майна
            ldrh    r1, [r5, #24]   //размер стека майна
            sub     r3, r2, r1
            str     r3, [r5, #24]   //#24- Удачный стек, booked stack address
            str     r3, [r5, #44]   //task_stop - последний адрес стека
            ldr     r1, =__SysTick_CTRL
            ldr     r3, [r5, #32]   // tick_real
            str     r3, [r1, #4]    // SysTick->LOAD
            mov     r2, #7
            str     r2, [r1]        // запуск+прерывания+частота=1
            mov     r3, #15
            lsl     r3, r3, #20
            str     r3, [r1, #3448] // разрешение на математику
            cpsie    i
            msr	   CONTROL, r2
            isb
            mov     r3, #0          // __EnableIRQS 0x0
            mov     r1, #0x0F
            mov     r0, #0xFF
            svc     0x0             //sNVIC_EnableIRQ(SysTick_IRQn, 15);
            pop     {r0, r1, r2, r3, R4, R5, pc}
  //          bx      lr






__Main_Name:
 .asciz "Main"
__sTask_nil_Nane:
 .asciz "sTask_nil"
 .align 3
 .size Start_task, . - Start_task


 .align 2
 .section  .text.sTask_kill
 .global    sTask_kill
 .type    sTask_kill, %function
sTask_kill: // +
            push    {r0, r1, r2, r3, lr }
            ldr     r3, = sSystem_task      // адрес переменной с адресом активной задачи
            ldr     r2, [r3]                // читаем адрес исполняемого банка
            ldr     r1, =__Main_Name
            ldr     r0, [r2, #28]
            subs    r0, r1, r0
            it      eq
            popeq   {r0, r1, r2, r3, pc }
            mov     r1, #4                 // флаг обработки для sTask_nil, __sTask_kill: //4
            strb    r1, [r2, #15]          // флаг запроса на обработку
            mov     r3, 0xA                 //__nil_in 0xA;
            svc     0x0                    // нить отложенных заданий
            nop


 .size sTask_kill, . - sTask_kill







 .ifdef  __Test_psp
 .align 2
 .section  .text.sHandler_zize
 .globl    sHandler_zize
 .type    sHandler_zize, %function
sHandler_zize:
            ldr     r0, =sSystem_task
            ldrh    r1, [r0, #16]       // #16- Рабочий стек прерываний, Task maximum NVIC used stack
            ldr     r3, =__Vector_table
            ldr     r3, [r3]
            ldr     r3, [r3]            //вершина стека
            mrs     r2, msp
            sub     r3, r3, r2
            cmp     r3, r1
            it      hs
            strhhs  r3, [r0, #16]       // #16- Рабочий стек прерываний, Task maximum NVIC used stack
            ldrh    r2, [r0, #18]       // #18- Размер стека прерываний, Task NVIC stack size
            cmp     r2, r3
            bhs     sHandler_zi
            bkpt                        // Мало стека под прерывания
sHandler_zi:
            bx      lr
 .size sHandler_zize, . - sHandler_zize
 .endif


 .align 2
 .globl     SysTick_Handler
 .type      SysTick_Handler, %function
// .section   .qsdramload.SysTick_Handler
 .section  .text.SysTick_Handler
SysTick_Handler:
            mov     r3, #0x10
            svc     0x0
            bx      lr
 .size SysTick_Handler, . - SysTick_Handler





 .align 2
 .section  .text.sTask_nil
 .globl    sTask_nil
 .type    sTask_nil, %function
sTask_nil:
            ldr     r12, = sSystem_task     // адрес переменной с адресом активной задачи
            //(r11)  // адрес задач на обработку памяти
            //(r10)  // стек задачи, 0 признак очереди
            // s0  32b первая не обработанная в отложенных
            // s1  32b malloc_zize > malloc_adres
            // s2  32b free, malloc, = name
            // 12 16b xxxx



sTask_nil_re:
            ldr     r5, [r12, #12]          // адрес задач на обработку памяти
            cbnz    r5, sTask_nil_nw
            ldr     r0, [r12]
            ldr     r1, [r0]
            cmp     r0, r1                  // активная единственная нулевая
            itt    ne                       // то ждём физики
            movne   r3, 0x10
            svcne   0x0                     // __switch 0x10
         //  wfieq
            b       sTask_nil_re
sTask_nil_nw:
            ldr     r10, [r5, #8]           // читаем стек задачи
            ldr     r1, [r5, #16]           // task_wake
            ldrb    r4, [r5, #15]          // читаем флаг
            cbz     r1, sTask_nil_ns        // предварительная обработка не требуется
            ubfx    r0, r1, #0, #8
            vmov    r2, s0                  // первая тормозная
            mov     r3, #0xF
            cmp     r0, #0
            itttt   hi
            subhi   r0, r0, #1
            strbhi  r0, [r5, #16]           // task_wake.delay
            svchi   0x0                     // __nil_step 0xF
            bhi     sTask_nil_re
            /*
            sublo   r2, r2, r5
            mov     r3, #0xF
            cmp     r2, #0
            itt      eq
            streq   r2, [sp, #__STACK_Px]   // первая тормозная =0
            beq     sTask_nil_nz
            mov     r3, #0xF
            svc     0x0                     // __nil_step 0xF
            cmp     r2, r5
            itt     eq
            moveq   r3, #0x10
            svceq   0x0                     // __switch 0x10
            */
sTask_nil_nz:
            ubfx    r4, r1, #8, #8
            lsr     r0, r1, #8
            bfc     r0, #0, #8
            str     r0, [r5, #16]           // сохраняем очередь, task_wake
            mov     r10, #0
sTask_nil_ns:
            mov     r11, r5
            tbh     [pc, r4, lsl #1]

__malloc_Table:
    .hword   ((__eror_0 - __malloc_Table)/2)      //0
    .hword   ((__maloc_in - __malloc_Table)/2)      //1
    .hword   ((__free - __malloc_Table)/2)          //2
    .hword   ((__free_all - __malloc_Table)/2)      //3
    .hword   ((__sTask_kill - __malloc_Table)/2)    //4
    .hword   ((__memory_don - __malloc_Table)/2)    //5
    .hword   ((__sTask_new_01 - __malloc_Table)/2)  //6
  //  .hword   ((__sTask_listr - __malloc_Table)/2)    //7
 //   .hword   ((__maloc_in1 - __malloc_Table)/2)     //8



  //  .hword   ((__sDel_new - __malloc_Table)/2)


 .align 2
__eror_0: // флаг0 - ошибка
            //ldr     r5, =__nil_exit
            mov     r3, #0xB
            svc     0x0                         // __nil_exit
            b       sTask_nil_re


//новое
 .align 2
__maloc_in: // флаг 1 - текущая задача r11 - ибо заепало ++
            cmp     r10, #0
            ite     eq
            vmoveq  r4, s1
            ldrne   r4, [r10, #__STACK_Fx]      // читаем размер malloc

 .ifdef  __Test_psp
            lsr     r2, r4, #23
            cbz     r2, __maloc_norm
            cbnz    r4, __maloc_norm
            mvn     r2, #0                      // ошибка - размер malloc больше 32мбайт,или нулевой
            str     r2, [r10, #__STACK_Fx]      // пишем нереальный адрес
            mov     r3, #0xB
            svc     0x0                         // __nil_exit
            b       sTask_nil_re                // новый круг
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
            cmp     r10, #0
            ite     eq
            vmoveq  r2, s2
            ldrhne  r2, [r11, #12]          // читаем номер таска, Task unique ID
            bfc     r2, #9, #7
            orr     r1, r4, r2, lsl #23
            str     r1, [r3]                // сохраняем нового хозяина и смещение
            sub     r2, r0, r4
            str     r2, [r3, r4, lsl #2]    // остаток
            add     r2, r3, r4, lsl #2
            add     r3, r3, #4              // адрес для передачи в функцию
            cmp     r10, #0
            ite     eq
            vmoveq  s1, r3
            strne   r3, [r10, #__STACK_Fx]      // пишем новый адрес malloc
            mov     r1, #0
__maloc_in_00:
            cmp     r2, r3
            itt     ne
            strne   r1, [r3], #4
            bne     __maloc_in_00
            mov     r3, #0xB
            cmp     r10, #0
            it      ne
            svcne   0x0                     // __nil_exit
            b       sTask_nil_re            // новый круг
__maloc_in_02:
            add     r6, r3, r4, lsl #2      // r6 - проверка на границу
            add     r6, r6, #12
            ldr     r7, [r12, #44]          // читем task_stop - последний адрес стека
            cmp     r7, r6
            itet    hi                      // есть место
            ldrhi   r7, [r12, #40]          // читаем malloc_stop - последний адрес malloc
            movls   r6, #0
            cmphi   r6, r7
            it      hi
            strhi   r6, [r12, #40]          // новый malloc_stop - последний адрес malloc
            mov     r7, #0x120              // пропуск - 32 цикла нулевой задачи
            cmp     r6, #0                  // неудача -   нулевой адрес
            itttt   eq
            ldreq   r1, [r11, #16]
            addeq   r1, r7, r1, lsl #8
            streq   r1, [r11, #16]
            beq     sTask_nil_re            // новый круг
            mov     r0, r4
            b       __maloc_in_03



 .align 2
__free:
            ldr     r7, [r11, #8]           // читаем стек задачи
            ldr     r2, [r10, #__STACK_Fx]   // читаем free (malloc_adres) задачи
            ldr     r4, [r2, #-4]           // читаем настоящего хозяина\размер
            bfc     r4, #23, #9
            str     r4, [r2, #-4]           // затираем хозяина
            mov     r2, #0
            ldr     r1, [r12, #36]          // malloc_start - первый адрес malloc
            b       __free_all_1

 .align 2
__free_all: // 3
            cmp     r10, #0
            ite     eq
            vmoveq  r2, s2
            ldrhne  r2, [r11, #12]          // читаем номер таска, Task unique ID
            bfc     r2, #9, #7
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
            cmp     r10, #0
            itt     ne
            movne   r3, #0xB
            svcne   0x0                     // __nil_exit
            b       sTask_nil_re            // новый круг

 .align 2
__sTask_kill: //4
            ldrh    r0, [r11, #12]          //- Номер таска, Task unique ID
            bfc     r0, #9, #7
            mov     r1, #0x0300
            movt    r1, #0x0004             // удаление памяти и возврат
            cmp     r10, #0
            ittt    ne
            strne   r1, [r11, #16]           // task_wake
            vmovne  s2, r0
            bne     sTask_nil_re            // запуск цепочки
            mov     r3, #0x5                // __Delete_Task: // 5
            svc     0x0                     // __Delete_Task
            b       sTask_nil_re            // новый круг


 .align 2
__memory_don:   // svc 0xD;  стек r0 *link_memory, r1 char* const task_func_name
            ldr     r1, [r11, #8]            // Cтек задачи
            ldr     r2, [r1, __STACK_Fx +4]  // [r1] char* const task_func_name
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
            ldr     r4, [r1, __STACK_Fx]    // [r0] *link_memory
            ldr     r2, [r4, #-4]           // там был адрес ссылки
            ldrh    r0, [r3, #12]           // читаем номер таска, Task unique ID ?
            bfc     r0, #9, #7
            bfc     r2, #23, #9             // размер
            orr     r2, r2, r0, lsl #23
            str     r2, [r4, #-4]           // новый хозяин
            mvn     r0, #0
            str     r0, [r1, __STACK_Fx]    // затёрли адрес ссылки на ffff. при обращении будет ошибка доступа
            mov     r0, #0
            ldr     r5, =__nil_exit
            strb    r0, [r3, #15]           // стёрли флаг принимающей задачи
            svc     0x0                     // __nil_exit
            b       sTask_nil_re

 .align 2
__sTask_new_01: // новая задача флаг 6
                // [r0]) void (*taskS_func()), [r1] task_size ,
                // [r2] task_time_rate , [r4] char* const task_func_name
                // [r5] task_func_massif4_data
            ldrex   r0, [r12, #64]
            bfc     r0, #0, #1
            strex   r1, r0, [r12, #64]          // сброс флага доступности
            cmp     r1, #0
            bne     __sTask_new_01
            ldrh    r0, [r12, #20]              // Количество тасков в системе task_list_zize_sys
            ldrh    r1, [r12, #22]              // Количество тасков в списке task_list_zize_use
            add     r0, r0, #1
            cmp     r1, r0
            bhi     __sTask_new_02
            ldr     r5, [r12, #60]              // Список тасков
            mov     r4, #1
            vmov    s2, r4
            add     r1, r0, #15
            strh    r1, [r12, #22]              // Количество тасков в списке task_list_zize_use
            lsl     r2, r1, #2
            vmov    s1, r2
            mov     r3, #0x0100                 // __maloc_in 1
            str     r3, [r11, #16]              // t_wake task_wake
            cmp     r5, #0
            beq     sTask_nil_re
            ldr     r4, [r5, #-4]               // читаем настоящего хозяина\размер
            bfc     r4, #23, #9
            str     r4, [r5, #-4]               // затираем хозяина
            mov     r2, #0
            ldr     r1, [r12, #36]              // malloc_start - первый адрес malloc
            mov     r5, #0
            mov     r10, #0
            str     r5, [r12, #60]              // Список тасков
            b       __free_all_1



__sTask_new_02:

            ldr     r10, [r11, #8]              // Cтек задачи r10 будет адресом параметров
            ldr     r8, [r12, #60]              // Список тасков
            mov     r3, #0xC                    //__sTask_list      //C
            cmp     r8, #0
            ittt    eq
            vmoveq  r8, s1
            streq   r8, [r12, #60]              // новый лист
            svceq   #0
            ldrh    r7, [r12, #22]          // #22- Количество тасков в списке
            lsl     r7, r7, #2
            ldr     r0, [r12, #24]          // удачный стек
            ldr     r6, [r10, #__STACK_Fx +4] // [psp +4] task_size
            add     r6, r6, #31
            bfc     r6, #0, #5              // выровняли размер новой задачи
            mov     r4, #0
__sTask_new_003:
            cmp     r4, r7
            beq     __sTask_new_05          // проверили все существующие нити
            ldr     r5, [r8, r4]    // адрес первой задачи  !!!
            add     r4, r4, #4
            cmp     r5, #0
            beq     __sTask_new_003
            add     r2, r5, r6
            add     r2, r2, #32
            cmp     r2, r0 //>
            itttt   hi
            ldrhhi  r2, [r5, #24]           // читаем размер стека
            subhi   r2, r2, #32
            subhi   r2, r5, r2
            cmphi   r0, r2
            itt     hi
            movhi   r0, r2
            movhi   r4, #0                 // хвост изменился
            b       __sTask_new_003
__sTask_new_05: // r0 вершина стека новой нити
            sub     r5, r0, r6
            ldr     r4, [r12, #40]           // граница маллок
            cmp     r5, r4
            ittt    lo                      // нету места
            movlo   r1, #60                  // пропуск - 60 циклов нулевой задачи
            strlo   r1, [r11, #16]
            blo     sTask_nil_re            // новый круг
            ldr     r4, [r12, #44]           // task_stop - последний адрес стека
            cmp     r5, r4
            it      lo
            strlo   r5, [r12, #44]           // граница стека опустилась
            // выделение нового номера
            sub     r1, r0, #64
            mvn     r2, #0
__sTask_new_06:
            cmp     r1, r0
            itt     ne
            strne   r2, [r1], #4
            bne     __sTask_new_06          // FFFF.. много
            sub     r1, r0, #64
            bic     r2, #0x1
            str     r2, [r1]
            mov     r4, #0
__sTask_new_07:
            cmp     r4, r7
            beq     __sTask_new_08          // проверили все существующие нити
            ldr     r5, [r8, r4]    // адрес первой задачи
            add     r4, r4, #4
            cmp     r5, #0
            beq     __sTask_new_07
            ldrh    r5, [r5, #12]           // читаем номер таска, Task unique ID
            bfc     r5, #9, #7
            and     r3, r5, #7              // пределы байта
            mov     r2, #1
            lsl     r2, r2, r3              // бит на сброс
            lsr     r5, r5, #3
            ldrb    r3, [r1, r5]            // читаем место захоронения
            bic     r3, r3, r2              // сбрасываем
            strb    r3, [r1, r5]            // сохраняем место захоронения
            b       __sTask_new_07
__sTask_new_08:
            mov     r3, #0
            sub     r2, r0, #64
__sTask_new_10:
            ldr     r1, [r2, r3, lsl #2]
            rbit    r1, r1
            clz     r1, r1
            cmp     r1, #32
            itt     eq
            addeq   r3, r3, #1
            beq     __sTask_new_10
            add     r3, r1, r3, lsl #5        // новый номер r3
            movw    r4, #0xE000
            add     r3, r3, r4                // новый номер + mode_nomer

            mov     r4, #0
__sTask_new_10_1:
            cmp     r4, r7
            beq     __sTask_new_10_2          // проверили все существующие нити
            ldr     r5, [r8, r4]    // адрес первой задачи
            cmp     r5, #0
            itt     ne
            addne    r4, r4, #4
            bne     __sTask_new_10_1

__sTask_new_10_2:
            sub     r5, r0, #32             //новая задача
            str     r5, [r8, r4]            // адрес новой задачи в списке
            // обнуляем поле стека
            sub     r1, r0, #232
            mov     r2, #0
__sTask_new_11:
            cmp     r1, r0
            itt     ne
            strne   r2, [r1], #4
            bne     __sTask_new_11
            // заполняем стек
            sub     r0, r0, #32             //новая задача
            strh    r3, [r0, #12]           // Номер таска, Task unique ID + mode_nomer
            ldr     r1, [r10, #__STACK_Fx]    // void (*taskS_func()) +0
            str     r1, [r0, #-80]          //сохранили pc
            ldr     r1, =sTask_kill
            str     r1, [r0, #-84]          //сохранили ложный lr
            ldr     r1, [r12, #32]          //tick_real - 100% тиков на задачу
            ldr     r5, [r10, #__STACK_Fx +8] //[psp +8]) task_time_rate %
            strb    r5, [r0, #14]           //сохранили выделенный процент времени
            mul     r1, r1, r5
            mov     r3, #0
            movt    r3, #0x100
            str     r3, [r0, #-76]           //сохранили xPSR в стек 0x1000000
            mov     r5, #100
            udiv    r1, r1, r5              //подсчёт времени
            sub     r2, r0, #200           //Cortex-M7 (STACK_Fx 200)
            str     r2, [r0, #8]            //стек задачи (активный хвост)
            add     r2, r2, #96
            strh    r6, [r0, #24]           //размер стека (задаётся при запуске)
            mov     r3, #200
            strh    r3, [r0, #26]           //мах заюзанный размер стека
            str     r1, [r0, #20]           //Таймер активности задачи, Task activity timer
            ldr     r1, [r10, #__STACK_Fx -96] //[psp -32]) char* const task_func_name
            str     r1, [r0, #28]           //адрес имени задачи (char* const text)
            ldr     r4, [r10, #__STACK_Fx -92] // void* func_parametr
            cbz     r4, __sTask_new_13
            ldmia   r4!, {r5-r8}            //чтение параметров функции
            stmia   r2!, {r5-r8}            //установка параметров функции


__sTask_new_13:
            ldrh    r2, [r12, #20]              // Количество тасков в системе task_list_zize_sys
            add     r2, r2, #1
            strh    r2, [r12, #20]              // Количество тасков в системе task_list_zize_sys
            mov     r3, #0x4                // __sTask_new 4 + __nil_exit
            svc     0x0
__sTask_new_14:
            ldrex   r2, [r12, #64]
            add     r3, r2, #1
            strex   r4, r3, [r12, #64]              // task_list доступен
            cmp     r4, #0
            bne     __sTask_new_14
            b       sTask_nil_re







 .size sTask_nil, . - sTask_nil
 /*

.word  _sisdramdata
.word  _sqsdramload
.word  _eqsdramload

// Call the clock system initialization function
            bl  SystemInit

__LOADSDRAM:
            ldr     r0, =_sisdramdata
            ldr     r1, =_sqsdramload
            ldr     r2, =_eqsdramload
__LOADSDRAMx:
            cmp     r1, r2
            ittt    ne
            ldrne   r3, [r0], #4
            strne   r3, [r1], #4
            bne     __LOADSDRAMx

            bl  main

*/
