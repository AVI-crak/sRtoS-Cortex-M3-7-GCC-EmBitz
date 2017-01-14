
#ifndef _monitor_



#define buf_zize    (512)
uint8_t  _std_out_buffer[buf_zize];
uint8_t  _std_in_buffer[buf_zize];


union m_mk_b
{
uint8_t     buf_8[(buf_zize + 1)];
uint32_t    buf_32[(buf_zize >> 2)];
}m_mk_buf;



typedef struct
{
    uint16_t            length;
    volatile uint16_t   tail;
    volatile uint16_t   head;
    uint8_t             *ptr;
}_stdout,_stdin;
extern  _stdout	_eb_monitor_stdout;
extern  _stdin	_eb_monitor_stdin;

void monitor_reset (void)
{
    _eb_monitor_stdout.ptr = &_std_out_buffer[0];
    _eb_monitor_stdout.length = buf_zize ;
    _eb_monitor_stdin.ptr = &_std_in_buffer[0];
    _eb_monitor_stdin.length = buf_zize ;
};


void monitor_fining (void)
{
    uint16_t tmp = 0;
    do
    {
        m_mk_buf.buf_32[tmp++] = 0x20202020;
    }while (tmp != (buf_zize >> 2));
};



 /// печать строки
void monitor_print (uint8_t* text)
{
    uint32_t temp_t = 0;
    if (text[temp_t] == '\0') return;
    uint32_t temp_h = _eb_monitor_stdout.head;
    uint32_t temp_l;
    while (text[temp_t] != '\0')
    {
        temp_l = temp_h; temp_h++;
        if (temp_h == buf_zize) temp_h = 0;
        if (_eb_monitor_stdout.tail != temp_h ) *(_eb_monitor_stdout.ptr + temp_h) = text[temp_t];
        else
        {
            _eb_monitor_stdout.head = temp_l;
            while (_eb_monitor_stdout.tail != temp_l ) sTask_skip();// Delay(1000);// там занято
            *(_eb_monitor_stdout.ptr + temp_h) = text[temp_t];
            _eb_monitor_stdout.head = temp_h;
        }
        temp_t++;
    }
    _eb_monitor_stdout.head = temp_h;
};

uint8_t *monitor_scan (uint8_t *text)
{
    uint32_t temp_h = _eb_monitor_stdin.head;
    uint32_t temp_l = _eb_monitor_stdin.tail;
    uint32_t temp_x = 0;
    if(temp_l == temp_h) {*text = 0; return text;};
    while (temp_l != temp_h)
    {
        temp_l++; if (temp_l == buf_zize) temp_l = 0;
        *(text + temp_x++) = *(_eb_monitor_stdin.ptr + temp_l);
    }
    _eb_monitor_stdin.tail = temp_l;
    *(text + (temp_x)) = 0;
    return text;
}


void monitor (void)
{
    uint32_t    *Ltask_step;
uint32_t    *Ltask;
uint32_t    Ltask_list_zize_sys;
uint32_t    Ltask_list_zize_use;
uint32_t    temp_list;
uint32_t    temp_use;
uint16_t    temp_buf;
uint32_t    temp_n;
                        //  5    10     16
char     txt_mode[]= "hold\0wait\0delay\0activ\0";
uint32_t time_tax;
uint32_t time_tax2;

while(1)
{
   time_tax = sSystem_task.system_us;
//                   15           5        10        10        10
monitor_print("Task_name     ;  ID;     zize; max_zize;     time;   mode\n");
Ltask_list_zize_sys = sSystem_task.task_list_zize_sys;    //Количество тасков в системе
temp_list = 0;
while(Ltask_list_zize_sys != 0 )
{
   if ( *(sSystem_task.task_list + temp_list) != 0 )
   {
        monitor_fining();
        struct task* Ltask; Ltask = *(sSystem_task.task_list + temp_list);temp_use = 0; temp_buf = 0;
        do
        {
          m_mk_buf.buf_8[temp_buf++] = Ltask->task_names[temp_use++];
        }
        while (Ltask->task_names[temp_use] != '\0');
        temp_use = Ltask->task_nomer;temp_buf = 19;
        while (temp_use != 0){m_mk_buf.buf_8[--temp_buf] = temp_use %10 + '0'; temp_use /= 10; };
        temp_use = Ltask->stack_zize;temp_buf = 29;
        while (temp_use != 0){m_mk_buf.buf_8[--temp_buf] = temp_use %10 + '0'; temp_use /= 10; };
        temp_use = Ltask->stack_max_zize;temp_buf = 39;
        while (temp_use != 0){m_mk_buf.buf_8[--temp_buf] = temp_use %10 + '0'; temp_use /= 10; };
        temp_use = Ltask->life_time;temp_buf = 49;
        while (temp_use != 0){m_mk_buf.buf_8[--temp_buf] = temp_use %10 + '0'; temp_use /= 10; };
        temp_n = 16;
        if (sSystem_task.hold != 0)
        {
            Ltask_step = sSystem_task.hold;temp_use = sSystem_task.task_list_zize_sys;
            do
            {
                if (Ltask_step == Ltask) { temp_n = 0;}
                else Ltask_step = *Ltask_step;
            }while (( Ltask_step != sSystem_task.hold ) && temp_use--);
        };
        if (sSystem_task.wait != 0)
        {
            Ltask_step = sSystem_task.wait;temp_use = sSystem_task.task_list_zize_sys;
            do
            {
                if (Ltask_step == Ltask) { temp_n = 5;}
                else Ltask_step = *Ltask_step;
            }while (( Ltask_step != sSystem_task.wait ) && temp_use--);
        };
        if (sSystem_task.delay != 0)
        {
            Ltask_step = sSystem_task.delay;temp_use = sSystem_task.task_list_zize_sys;
            do
            {
                if (Ltask_step == Ltask) { temp_n = 10; }
                else Ltask_step = *Ltask_step;
            }while (( Ltask_step != sSystem_task.delay ) && temp_use--);
        }else
        {
            Ltask_step = sSystem_task.activ;temp_use = sSystem_task.task_list_zize_sys;
            do
            {
                if (Ltask_step == Ltask) { temp_n = 16;}
                else Ltask_step = *Ltask_step;
            }while (( Ltask_step != sSystem_task.activ ) && temp_use--);

        };
        temp_buf = 52;
        do
        {
          m_mk_buf.buf_8[temp_buf++] = txt_mode[temp_n++];
        }
        while ( txt_mode[temp_n] != '\0');
        temp_buf = 59;
        m_mk_buf.buf_8[temp_buf++] = '\n';
        m_mk_buf.buf_8[temp_buf++] = 0;
        monitor_print (&m_mk_buf.buf_8[0]);
        temp_list++; Ltask_list_zize_sys--;
   }else
   {
     temp_list++;
     if (temp_list > sSystem_task.task_list_zize_use) break;
   }
}

monitor_print ("NVIC_size_max: "); monitor_print (t32_char( sSystem_task.NVIC_size_max ,txt_buf));
monitor_print ("\nNVIC_size: "); monitor_print (t32_char( sSystem_task.NVIC_size ,txt_buf));
monitor_print ("\nTime_ptint: ");monitor_print (t32_char(time_tax2,txt_buf));
time_tax2 = sSystem_task.system_us - time_tax;
if (time_tax2 < 1500)temp_use = 1500;else temp_use = time_tax2 * 5;
sDelay_mc(temp_use);
monitor_print("\f");

}
}




#endif /* _monitor_ */
#define _monitor_
