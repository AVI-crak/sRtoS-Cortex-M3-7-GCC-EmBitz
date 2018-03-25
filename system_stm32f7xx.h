
///HSE 16mgz
/// https://bitbucket.org/AVI-crak/system_f746/overview



#define  FMC_SDCR1_NC_8bits                 ((uint32_t)0x00000000)        /// 8 bits
#define  FMC_SDCR1_NC_9bits                 ((uint32_t)0x00000001)        /// 9 bits
#define  FMC_SDCR1_NC_10bits                ((uint32_t)0x00000002)        /// 10 bits
#define  FMC_SDCR1_NC_11bits                ((uint32_t)0x00000003)        /// 11 bits.

#define  FMC_SDCR1_NR_11bits                ((uint32_t)0x00000000)        /// 11 bit
#define  FMC_SDCR1_NR_12bits                ((uint32_t)0x00000004)        /// 12 bits
#define  FMC_SDCR1_NR_13bits                ((uint32_t)0x00000008)        /// 13 bits

#define  FMC_SDCR1_MWID_8bits               ((uint32_t)0x00000000)        /// 8 bits
#define  FMC_SDCR1_MWID_16bits              ((uint32_t)0x00000010)        /// 16 bits
#define  FMC_SDCR1_MWID_32bits              ((uint32_t)0x00000020)        /// 32 bits

#define  FMC_SDCR1_NB_2banks                ((uint32_t)0x00000000)        /// 2banks
#define  FMC_SDCR1_NB_4banks                ((uint32_t)0x00000040)        /// 4banks

#define  FMC_SDCR1_CAS_1cycle               ((uint32_t)0x00000080)        /// 1 cycle
#define  FMC_SDCR1_CAS_2cycle               ((uint32_t)0x00000100)        /// 2 cycles
#define  FMC_SDCR1_CAS_3cycle               ((uint32_t)0x00000180)        /// 3 cycles

#define  FMC_SDCR1_SDCLK_2x                 ((uint32_t)0x00000800)        /// SDCLK period = 2 x HCLK periods
#define  FMC_SDCR1_SDCLK_3x                 ((uint32_t)0x00000C00)        /// SDCLK period = 3 x HCLK periods

#define  FMC_SDCR1_RPIPE_0delay             ((uint32_t)0x00000000)        ///  HCLK clock cycle 0delay
#define  FMC_SDCR1_RPIPE_1delay             ((uint32_t)0x00002000)        ///  HCLK clock cycle 1delay
#define  FMC_SDCR1_RPIPE_2delay             ((uint32_t)0x00004000)        ///  HCLK clock cycle 2delay
#define  FMC_SDCR1_RPIPE_3delay             ((uint32_t)0x00006000)        ///  HCLK clock cycle 3delay

#define  FMC_SDCR2_NC_8bits                 ((uint32_t)0x00000000)        /// 8 bits
#define  FMC_SDCR2_NC_9bits                 ((uint32_t)0x00000001)        /// 9 bits
#define  FMC_SDCR2_NC_10bits                ((uint32_t)0x00000002)        /// 10 bits
#define  FMC_SDCR2_NC_11bits                ((uint32_t)0x00000003)        /// 11 bits.

#define  FMC_SDCMR_MODE_Normal              ((uint32_t)0x00000000)        /// Normal Mode
#define  FMC_SDCMR_MODE_Config_Enable       ((uint32_t)0x00000001)        /// Clock Configuration Enable
#define  FMC_SDCMR_MODE_PALL                ((uint32_t)0x00000002)        /// PALL (“All Bank Precharge”) command
#define  FMC_SDCMR_MODE_Auto_refresh        ((uint32_t)0x00000003)        /// Auto-refresh command
#define  FMC_SDCMR_MODE_Load_Mode           ((uint32_t)0x00000004)        /// Load Mode Register
#define  FMC_SDCMR_MODE_Self_refresh        ((uint32_t)0x00000005)        /// Self-refresh command
#define  FMC_SDCMR_MODE_Power_of            ((uint32_t)0x00000006)        /// Power-down command





#define  MPU_HFNMI_PRIVDEF_NONE      ((uint32_t)0x00000000U)
#define  MPU_HARDFAULT_NMI           ((uint32_t)0x00000002U)
#define  MPU_PRIVILEGED_DEFAULT      ((uint32_t)0x00000004U)
#define  MPU_HFNMI_PRIVDEF           ((uint32_t)0x00000006U)

#define  MPU_REGION_ENABLE     ((uint8_t)0x01U)
#define  MPU_REGION_DISABLE    ((uint8_t)0x00U)

#define  MPU_INSTRUCTION_ACCESS_ENABLE      ((uint8_t)0x00U)
#define  MPU_INSTRUCTION_ACCESS_DISABLE     ((uint8_t)0x01U)

#define  MPU_ACCESS_SHAREABLE        ((uint8_t)0x01U)
#define  MPU_ACCESS_NOT_SHAREABLE    ((uint8_t)0x00U)

#define  MPU_ACCESS_CACHEABLE         ((uint8_t)0x01U)
#define  MPU_ACCESS_NOT_CACHEABLE     ((uint8_t)0x00U)

#define  MPU_ACCESS_BUFFERABLE         ((uint8_t)0x01U)
#define  MPU_ACCESS_NOT_BUFFERABLE     ((uint8_t)0x00U)

#define  MPU_TEX_LEVEL0    ((uint8_t)0x00U)
#define  MPU_TEX_LEVEL1    ((uint8_t)0x01U)
#define  MPU_TEX_LEVEL2    ((uint8_t)0x02U)


#define  MPU_REGION_NO_ACCESS      ((uint8_t)0x00U)
#define  MPU_REGION_PRIV_RW        ((uint8_t)0x01U)
#define  MPU_REGION_PRIV_RW_URO    ((uint8_t)0x02U)
#define  MPU_REGION_FULL_ACCESS    ((uint8_t)0x03U)
#define  MPU_REGION_PRIV_RO        ((uint8_t)0x05U)
#define  MPU_REGION_PRIV_RO_URO    ((uint8_t)0x06U)




__attribute__( ( always_inline ) ) static inline void delay( uint32_t volatile time_tmp){do {time_tmp--;} while ( time_tmp );};

void SystemInit (void)
{
    volatile uint32_t tmp;
    volatile uint32_t timeout;
  //  tmp = (.DB __MONTH__);




  //  __set_CONTROL(0);
    if (CONTROL_SPSEL_Msk & __get_CONTROL ())
{// MSP не активен
  __set_CONTROL (__get_CONTROL () & ~ CONTROL_SPSEL_Msk);
}

    SCB->CPACR |= 15<<20; /// FPU settings

    DBGMCU->APB1FZ = 0x06e019ff;
    DBGMCU->APB2FZ = 0x00070003; /// стоп в отладке всех таймеров

    MPU->RNR = 0;
    MPU->RBAR = 0x20000000;
    MPU->RASR = 0x03090025; /// C=0, B=1, TEX=1, S=0, SRD=0, XN=0, AP=3, size=0x12 (512k)
    MPU->RNR = 1;
    MPU->RBAR = 0x08000001;
    MPU->RASR = 0x03020027; /// C=1, B=0, TEX=0, S=0, SRD=0, XN=0, AP=3, size=0x13 (1M)
    MPU->RNR = 2;
    MPU->RBAR = 0x600fff02;
    MPU->RASR = 0x03040029; /// C=0, B=0, TEX=0, S=1, SRD=0, XN=0, AP=3, size=0x13 (1M)
    MPU->RNR = 3;
    MPU->RBAR = 0x90000003;
    MPU->RASR = 0x0306002d; /// C=1, B=0, TEX=0, S=1, SRD=0, XN=0, AP=3, size=0x16 (8M)
    MPU->RNR = 4;
    MPU->RBAR = 0xc0000004;
    MPU->RASR = 0x03070033; /// C=1, B=1, TEX=0, S=1, SRD=0, XN=0, AP=3, size=0x19 (64M)
    MPU->RNR = 5;
    MPU->RBAR = 0x40000005;
    MPU->RASR = 0x13040039; /// C=0, B=0, TEX=0, S=1, SRD=0, XN=1, AP=3, size=0x16 (0.5G)
    MPU->RNR = 6;
    MPU->RBAR = 0xe0000006;
    MPU->RASR = 0x1300002d; /// C=0, B=0, TEX=0, S=0, SRD=0, XN=1, AP=3, size=0x16 (0.5G)


    MPU->RNR = 7;
    MPU->RBAR = 0;
    MPU->RASR = 0;
    MPU->CTRL = 7;



    RCC->APB1ENR |= RCC_APB1ENR_PWREN ;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
    RNG->CR = RNG_CR_RNGEN;

    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN | RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_BKPSRAMEN;

    RCC->AHB3ENR |= RCC_AHB3ENR_QSPIEN | RCC_AHB3ENR_FMCEN;
    RCC->AHB3RSTR = RCC_AHB3RSTR_QSPIRST | RCC_AHB3RSTR_FMCRST;
    RCC->AHB3RSTR = 0;

/// RCC
 //   delay(80000);

    RCC->CR  = RCC_CR_HSION | RCC_CR_HSEON;
    while(! srRCC_CR_HSIRDY);
    while(! srRCC_CR_HSERDY);

    RCC->PLLCFGR =  swPLLCFGR_DIVM(12)          // Primary frequency divider 2-63
                    |swPLLCFGR_PLLN(324)        // Clock multiplier for PLLCLK 50-432
                    |swPLLCFGR_DIVP(2)          // The last divisor for PLLCLK 2,4,6,8
                    |swPLLCFGR_DIVQ(9)          // Divisor for USB, SDMMC, and RNG 2-15
                    |RCC_PLLCFGR_PLLSRC_HSE;    // 0x00000000U Clock from HSE

    RCC->PLLSAICFGR = swPLLSAICFGR_PLLN(252)    // PLLN frequency multiplication PLLSAI 50-432
                      |swPLLSAICFGR_DIVP(8)     // P division factor for USB,RNG,SDMMC(48MHz) 2,4,6,8
                      |swPLLSAICFGR_DIVQ(15)    // Q division factor for SAI clock  2-15
                      |swPLLSAICFGR_DIVR(3);    // R division factor for LCD clock  2-7

    RCC->CR |=  RCC_CR_PLLSAION | RCC_CR_PLLON; // PLL enable
    RCC->CFGR = sRCC_CFGR_HPRE_AHB.DIV1         // Divisor SYSCLK to HCLK
                |sRCC_CFGR_PPRE1_APB1.DIV4      // Divisor HCLK to the 1 periphery
                |sRCC_CFGR_PPRE2_APB2.DIV2      // Divisor HCLK to the 2 periphery
                |swRCC_CFGR_RTCPRE_DIV_RTC(20)  // HSE division factor for RTC clock 2-31
                |sRCC_CFGR_MCO1_OUT.PLLCLK      // Microcontroller clock output 1
                |sRCC_CFGR_MCO1PRE.DIV5;        // MCO1 prescaler

    RCC->DCKCFGR2 = sRCC_DCKCFGR2_USART1SEL.APB2        /// USART 1 clock source selection
                   |sRCC_DCKCFGR2_I2C2SEL.APB1          /// I2C2 clock source selection
                   |sRCC_DCKCFGR2_LPTIM1SEL.HSI        /// Low-power timer 1 clock source selection
                   |sRCC_DCKCFGR2_CK48MSEL.PLLQ         /// 48MHz clock source selection
                   |sRCC_DCKCFGR2_SDMMC1SEL.CK48MSEL;   /// SDMMC clock source selection



__DMB();
    FLASH->ACR = 7                          /// Latency >210mg
                | FLASH_ACR_ARTEN           /// вкл кеш флеша
                | FLASH_ACR_PRFTEN;
    while(! srRCC_CR_PLLRDY);
    while(! srRCC_CR_PLLSAIRDY);

    RCC->CFGR |= sRCC_CFGR_SW.PLL;              // PLL selected as system clock







    SCB_EnableICache();
    SCB_InvalidateDCache();
    SCB_EnableDCache();
    ///-------------

  //  RCC->CFGR |= ((4 <<4) +8)&0xFF; // HPRE = 64 меедленннаяяя отладка




/// часы - дата
/// питание Vbat должно быть равно 2,9v, и подключаться через резистор 100R<->1K
delay(500);
//EXTI->FTSR |= EXTI_FTSR_TR16; /// прерываник по нарастанию
EXTI->RTSR |= EXTI_RTSR_TR16; /// прерываник по спаду питания
EXTI->IMR |= EXTI_IMR_MR16; /// маска линии PVDE
EXTI->PR = 0; //EXTI_IMR_MR16;
NVIC_SetPriority(PVD_IRQn,1);
NVIC_EnableIRQ(PVD_IRQn);
PWR->CR1 |= ( PWR_CR1_PLS_LEV7  /// питание + 2.5В<<reset
            | PWR_CR1_DBP);     /// снимаем защиту записи с защищённого домена

PWR->CR1 |= PWR_CR1_PVDE;      /// следим за питанием

RCC->CSR |= RCC_CSR_LSION; /// включаем внутренний генератор 32Кгц
/*
PWR->CSR1 |= PWR_CSR1_BRE;   /// вкл LP стаб на BKPSRAM
while(!(PWR->CSR1 & PWR_CSR1_BRR));
*/


if((RCC->BDCR & ( RCC_BDCR_LSERDY | sRCC_BDCR_RTCSEL.LSE | RCC_BDCR_RTCEN ))
        ^ ( RCC_BDCR_LSERDY | sRCC_BDCR_RTCSEL.LSE | RCC_BDCR_RTCEN ))
{  /// Сбой часового кварца, коммутатора кварца, часового модуля
    RCC->BDCR = RCC_BDCR_BDRST;             /// ресет модуля
    RCC->BDCR &= ~RCC_BDCR_BDRST;
    RCC->AHB1LPENR |= RCC_AHB1LPENR_BKPSRAMLPEN;
    PWR->CSR1 |= PWR_CSR1_BRE;              /// вкл LP стаб на BKPSRAM
    while(!(PWR->CSR1 & PWR_CSR1_BRR));
    RCC->BDCR |= RCC_BDCR_RTCEN             /// вкл часового модуля
                |RCC_BDCR_LSEON             /// включаем внешний кварц
                |swRCC_BDCR_LSEDRV(3);      /// Power LSE oscillator's drive capability. 0-3
    RCC->CSR |= RCC_CSR_LSION;              /// включаем внутренний генератор 32Кгц
    for(tmp = 0x40024000; tmp < 0x40024FFC ; tmp +=4) // обнуление BKPSRAM
    { *(__IO uint32_t*) tmp = 0x0; delay(10);}
    tmp = 33554432;
    do { tmp--; }
    while ( (srRCC_BDCR_LSERDY == 0) && ( tmp != 0) );
    if ( tmp != 0) RCC->BDCR |= sRCC_BDCR_RTCSEL.LSE;
        else RCC->BDCR |= sRCC_BDCR_RTCSEL.LSI;

/// обновление даты и времени: год , месяц , день , день_недели , час , минуты , секунды
///                           year,  month,  day,  day_of_the_week,  hour,  minutes,  seconds);
    PWR->CR1 |= PWR_CR1_DBP;
    RTC->WPR = 0xCA;
    delay(1000);
    RTC->WPR = 0x53;
    delay(1000);
    RTC->ISR |= RTC_ISR_INIT;
    while(!(RTC->ISR & RTC_ISR_INITF));
    RTC->CR |= RTC_CR_BYPSHAD; /// чтение/запись без кеша
    RTC->PRER = 0x007F00FF; //0x007f00ff;
    delay(100);

    RTC->TR =   ((__TIME__[0]-'0') << 20)                           // hour 10
                | ((__TIME__[1]-'0') << 16)                         // hour 1
                | ((__TIME__[3]-'0') << 12)                         // minutes 10
                | ((__TIME__[4]-'0') << 8)                          // minutes 1
                | ((__TIME__[6]-'0') << 4)                          // seconds 10
                | (__TIME__[7]-'0');                                // seconds 1

    RTC->DR = ((__DATE__[9]-'0') << 20)                             // year 10
            | ((__DATE__[10]-'0') << 16)                            // year 1
            | ((__TIMESTAMP__[2]=='e'?2:__TIMESTAMP__[2]=='d'?3 \
            :__TIMESTAMP__[2]=='u'?4:__TIMESTAMP__[2]=='i'?5 \
            :__TIMESTAMP__[2]=='t'?6:__TIMESTAMP__[0]=='M'?1:7) << 13)      // Week day
            | ((((__DATE__[2]=='n'?(__DATE__[1]=='a'?0:5):__DATE__[2]=='b'?1 \
            :__DATE__[2]=='r'?(__DATE__[0]=='M'?2:3):__DATE__[2]=='y'?4 \
            :__DATE__[2]=='l'?6:__DATE__[2]=='g'?7:__DATE__[2]=='p'?8 \
            :__DATE__[2] =='t'?9:__DATE__[2]=='v'?10:11)+1)/10) << 12)      // Month 10
            | ((((__DATE__[2]=='n'?(__DATE__[1]=='a'?0:5):__DATE__[2]=='b'?1 \
            :__DATE__[2]=='r'?(__DATE__[0]=='M'?2:3):__DATE__[2]=='y'?4 \
            :__DATE__[2]=='l'?6:__DATE__[2]=='g'?7:__DATE__[2]=='p'?8 \
            :__DATE__[2] =='t'?9:__DATE__[2]=='v'?10:11)+1)%10) << 8)       // Month 1
            | ((__DATE__[4]==' ' ? 0 : __DATE__[4]-'0') << 4)               // day 10
            | (__DATE__[5]-'0');                                            // day  1



    delay(100);
    RTC->ISR &= (~RTC_ISR_INIT); // закрываем доступ на запись регистрам часов реального времени
    RTC->WPR =0xEE;
    delay(1000);
    //PWR->CR1 &= (~ PWR_CR1_DBP);




//#define COMPILE_MONTH (__TIMESTAMP__[2]=='e'?2:__TIMESTAMP__[2]=='d'?3:__TIMESTAMP__[2]=='u'?4:__TIMESTAMP__[2]=='i'?5:__TIMESTAMP__[2]=='t'?6:__TIMESTAMP__[0]=='M'?1:7)



}else;




//delay(10000);

RCC->AHB1ENR |= 0x001007ff;              /// GPIO ALL

     /*
    GPIOA->MODER |= 0xa80200c3;
    GPIOA->OSPEEDR |= 0x0c030000;
    GPIOA->PUPDR |= 0x64000000;
    GPIOA->AFR[0] |= 0x0a000900;

    GPIOB->MODER |= 0x000022aa;
    GPIOB->OSPEEDR |= 0x000030ff;
    GPIOB->PUPDR |= 0x00001110;
    GPIOB->AFR[0] |= 0x0a000922;

    GPIOC->MODER |= 0x0400a0c0;
    GPIOC->OSPEEDR |= 0x0c00f000;
    GPIOC->PUPDR |= 0;
    GPIOC->AFR[0] |= 0x22000000;

    GPIOD->MODER |= 0xa02a8a0a;
    GPIOD->OSPEEDR |= 0xf03f8a0f;
    GPIOD->PUPDR |= 0xa02a450a;
    GPIOD->AFR[0] |= 0xc0cc00cc;
    GPIOD->AFR[1] |= 0xcc000ccc;

    GPIOE->MODER |= 0xaaaaa9aa;
    GPIOE->OSPEEDR |= 0xffffffff;
    GPIOE->PUPDR |= 0xaaaa9455;
    GPIOE->AFR[0] |= 0xc550c5cc;
    GPIOE->AFR[1] |= 0xcccccccc;


    GPIOF->MODER |= 0xaa8aaaaa;
    GPIOF->OSPEEDR |= 0xffcfffff;
    GPIOF->PUPDR |= 0x55455555;
    GPIOF->AFR[0] |= 0x99cccccc;
    GPIOF->AFR[1] |= 0xccccc0aa;

    GPIOG->MODER |= 0x80020a2a;
    GPIOG->OSPEEDR |= 0xc0030f3f;
    GPIOG->PUPDR |= 0x40010515;
    GPIOG->AFR[0] |= 0x00cc0ccc;
    GPIOG->AFR[1] |= 0xc000000c;

    GPIOH->MODER |= 0x000008a0;
    GPIOH->OSPEEDR |= 0x00000cf0;
    GPIOH->PUPDR |= 0x00000450;
    GPIOH->AFR[0] |= 0x00c0cc00;
    GPIOH->AFR[1] |= 0;

    GPIOJ->MODER |= 0x00010000;
    GPIOJ->OSPEEDR |= 0x00030000;
    GPIOJ->PUPDR |= 0x00004000;
    GPIOJ->AFR[0] |= 0x00000000;
    GPIOJ->AFR[1] |= 0;
    */







    gpio_install (GPIOH, s_gpio_line.pin_05.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on );    /// FMC_sdnwe
    gpio_install (GPIOF, s_gpio_line.pin_11.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_sdnras
    gpio_install (GPIOH, s_gpio_line.pin_03.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_sdne0
    gpio_install (GPIOG, s_gpio_line.pin_15.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_sdncas
    gpio_install (GPIOG, s_gpio_line.pin_08.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_sdclk
    gpio_install (GPIOH, s_gpio_line.pin_02.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_sdcke0
    gpio_install (GPIOE, s_gpio_line.pin_01.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_nbl1
    gpio_install (GPIOE, s_gpio_line.pin_00.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_nbl0
    gpio_install (GPIOD, s_gpio_line.pin_10.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d15
    gpio_install (GPIOD, s_gpio_line.pin_09.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d14
    gpio_install (GPIOD, s_gpio_line.pin_08.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d13
    gpio_install (GPIOE, s_gpio_line.pin_15.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d12
    gpio_install (GPIOE, s_gpio_line.pin_14.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d11
    gpio_install (GPIOE, s_gpio_line.pin_13.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d10
    gpio_install (GPIOE, s_gpio_line.pin_12.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d9
    gpio_install (GPIOE, s_gpio_line.pin_11.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d8
    gpio_install (GPIOE, s_gpio_line.pin_10.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d7
    gpio_install (GPIOE, s_gpio_line.pin_09.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d6
    gpio_install (GPIOE, s_gpio_line.pin_08.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d5
    gpio_install (GPIOE, s_gpio_line.pin_07.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d4
    gpio_install (GPIOD, s_gpio_line.pin_01.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d3
    gpio_install (GPIOD, s_gpio_line.pin_00.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d2
    gpio_install (GPIOD, s_gpio_line.pin_15.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d1
    gpio_install (GPIOD, s_gpio_line.pin_14.out_af_12.speed_100Mgz.pull_down.push_pull.lock_on);   /// FMC_d0
    gpio_install (GPIOG, s_gpio_line.pin_05.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_ba1
    gpio_install (GPIOG, s_gpio_line.pin_04.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_ba0
    gpio_install (GPIOG, s_gpio_line.pin_02.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a12
    gpio_install (GPIOG, s_gpio_line.pin_01.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a11
    gpio_install (GPIOG, s_gpio_line.pin_00.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a10
    gpio_install (GPIOF, s_gpio_line.pin_15.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a9
    gpio_install (GPIOF, s_gpio_line.pin_14.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a8
    gpio_install (GPIOF, s_gpio_line.pin_13.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a7
    gpio_install (GPIOF, s_gpio_line.pin_12.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a6
    gpio_install (GPIOF, s_gpio_line.pin_05.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a5
    gpio_install (GPIOF, s_gpio_line.pin_04.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a4
    gpio_install (GPIOF, s_gpio_line.pin_03.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a3
    gpio_install (GPIOF, s_gpio_line.pin_02.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a2
    gpio_install (GPIOF, s_gpio_line.pin_01.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a1
    gpio_install (GPIOF, s_gpio_line.pin_00.out_af_12.speed_100Mgz.pull_of.push_pull.lock_on);     /// FMC_a0

    gpio_install (GPIOD, s_gpio_line.pin_05.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_nwe
    gpio_install (GPIOD, s_gpio_line.pin_04.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_noe
    gpio_install (GPIOD, s_gpio_line.pin_07.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_NE1
    gpio_install (GPIOE, s_gpio_line.pin_03.out_af_12.speed_100Mgz.pull_up.push_pull.lock_on);     /// FMC_a19

    gpio_install (GPIOJ, s_gpio_line.pin_07.inprit.pull_up.lock_on);                               /// LCD_Fmark
    gpio_install (GPIOJ, s_gpio_line.pin_08.out.speed_010Mgz.pull_up.push_pull.lock_on);           /// LCD_Reset

          /// SPI4
    gpio_install (GPIOE, s_gpio_line.pin_02.out_af_05.speed_100Mgz.pull_up.push_pull.lock_on);    /// spi4_sck
    gpio_install (GPIOE, s_gpio_line.pin_04.out.speed_100Mgz.pull_up.push_pull.lock_on);          /// spi4_nss
    gpio_install (GPIOE, s_gpio_line.pin_05.out_af_05.speed_100Mgz.pull_up.push_pull.lock_on);    /// spi4-mo
    gpio_install (GPIOE, s_gpio_line.pin_06.out_af_05.speed_100Mgz.pull_up.push_pull.lock_on);    /// spi4_mi
    gpio_install (GPIOA, s_gpio_line.pin_00.analog.pull_of.lock_on);                              /// adc1-0 - PA0. 43
    gpio_install (GPIOA, s_gpio_line.pin_03.analog.pull_of.lock_on);                              /// adc2-2 - PA3. 50
    gpio_install (GPIOC, s_gpio_line.pin_03.analog.pull_of.lock_on);                              /// adc3-13 - PC3. 38



    gpio_install (GPIOC, s_gpio_line.pin_13.out.speed_100Mgz.pull_of.push_pull.lock_on);            /// светик
    gpio_install (GPIOA, s_gpio_line.pin_08.out_af_00.speed_100Mgz.pull_of.push_pull.lock_of);      /// 40mg  out pin

        /// TIM3

    gpio_install (GPIOC, s_gpio_line.pin_06.out_af_02.speed_100Mgz.pull_of.push_pull.lock_on);      /// TIM3_CH1
    gpio_install (GPIOC, s_gpio_line.pin_07.out_af_02.speed_100Mgz.pull_of.push_pull.lock_on);      /// TIM3_CH2
    gpio_install (GPIOB, s_gpio_line.pin_00.out_af_02.speed_100Mgz.pull_of.push_pull.lock_on);      /// TIM3_CH3
    gpio_install (GPIOB, s_gpio_line.pin_01.out_af_02.speed_100Mgz.pull_of.push_pull.lock_on);      /// TIM3_CH4



/// Инсталл FMC 1 SRAM NE1 + SDRAM

    /// SRAM1
    FMC_Bank1E->BWTR[0] = ( (0 <<28)            /// ACCMOD
                          | (0 << 24)          /// DATLAT задержка выдачи данных
                          | (3 <<16)            /// BUSTURN время свободной шины после записи /7
                          | (3 <<8)            /// DATAST время записи данных /30
                          | (0 <<4)            /// ADDHLD заморозка адреса
                          | 5);                 /// ADDSET время установки адреса /9

    FMC_Bank1->BTCR[1]  = ( (80 <<8)            /// время чтения данных //99
                          | (0 <<20)           /// CLKDIV
                          | 5                  /// время установки адреса //15
                          | (15 <<16));         /// время свободной шины после чтения //15

    FMC_Bank1->BTCR[0]  = ( FMC_BCR1_EXTMOD     ///время чтения != время записи
                          | FMC_BCR1_WREN        ///разрешение записи
                          | (1 <<4)             ///размер шины данных: 0=8bits,1=16bits,2=32bits
                          | 1 <<21              ///21 WFDIS: Write FIFO Disable
                          | FMC_BCR1_MBKEN );   ///Включить банк




///Install SDRAM
FMC_Bank5_6 -> SDCR[0] = FMC_SDCR1_NC_10bits
                        |FMC_SDCR1_NR_13bits
                        |FMC_SDCR1_MWID_16bits
                        |FMC_SDCR1_NB_4banks
                        |FMC_SDCR1_CAS_2cycle
                        //|FMC_SDCR1_SDCLK_3x
                        |FMC_SDCR1_SDCLK_2x
                        |FMC_SDCR1_RBURST
                        |FMC_SDCR1_WP
                        |FMC_SDCR1_RPIPE_2delay; //FMC_SDCR1_RPIPE_1delay FMC_SDCR1_RPIPE_3delay

FMC_Bank5_6->SDTR[0] = (0x00000001)        /// TMRD время между записью в MODE-REGISTER и ACTIVATE/1  /2
                      |(0x00000006 << 4)   /// TXSR время между SELF-REFRESHING и ACTIVATE (exit self-refresh mode)/5  /7
                      |(0x00000004 << 8)   /// TRAS минимальное время активности строки
                      |(0x00000006 << 12)  /// TRC  минимальное время между активацией строк одного банка
                      |(0x00000002 << 16)  /// TWR  время записи данных
                      |(0x00000002 << 20)  /// TRP  минимальное время закрытия строки
                      |(0x00000002 << 24); /// TRCD время между подачей команды ACTIVATE и появлением данных на шинеС/1  /2
///TWR>=TRAS-TRCD and TWR>=TRC-TRCD-TRP  TRC=TRAS+TRP
  FMC_Bank5_6->SDCMR = FMC_SDCMR_CTB1 | FMC_SDCMR_MODE_Config_Enable; // 0x00000011;
  tmp = 15;
  timeout = 0xFFFF;
  while((tmp != 0) && (timeout > 0))
  {
    tmp = FMC_Bank5_6->SDSR & 0x00000020; timeout-- ;
  }

delay(100000);
/// PALL command
  FMC_Bank5_6->SDCMR = FMC_SDCMR_CTB1 | FMC_SDCMR_MODE_PALL; //0x00000012;
  timeout = 0xFFFF; tmp = 10;
  while((tmp != 0) && (timeout > 0))
  {
    tmp = FMC_Bank5_6->SDSR & 0x00000020; timeout-- ;
  }
/// Auto refresh command
  FMC_Bank5_6->SDCMR = (0x00000003 << 5) | FMC_SDCMR_CTB1 | FMC_SDCMR_MODE_Self_refresh; //0x00000073; FMC_SDCMR_MODE_Self_refresh  FMC_SDCMR_MODE_Auto_refresh
/// Количество рефлеш минимум 2
  timeout = 0xFFFF; tmp = 10;
  while((tmp != 0) && (timeout > 0))
  {
    tmp = FMC_Bank5_6->SDSR & 0x00000020; timeout-- ;
  }
/// MRD register program
  tmp = ((108000)*64)/8192; // 48тактов
  FMC_Bank5_6->SDCMR = (tmp << 9) | FMC_SDCMR_CTB1 | FMC_SDCMR_MODE_Load_Mode ;
/// 64mc/(размер блока Row Addresses(8192)) * (тактовая частота чипа)
  timeout = 0xFFFF; tmp = 10;
  while((tmp != 0) && (timeout > 0))
  {
    tmp = FMC_Bank5_6->SDSR & 0x00000020; timeout-- ;
  }
  tmp = (((108000 * 64)/8192)<<1) | FMC_Bank5_6->SDRTR;
  FMC_Bank5_6->SDRTR = tmp | 1<<14 ; // время регена + вкл регена
/// Refresh rate = (COUNT) * SDRAM clock frequency
/// SDRAM refresh period = 64 mc
/// COUNT = (SDRAM refresh period / Number of rows )
/// Refresh rate = 0.064 / (8192rows + 4) ) * 84000000 , ~ 656 ( 0x290 )

  FMC_Bank5_6->SDCR[0] &= (~FMC_SDCR1_WP);// снятие защиты от записи
  timeout =0;

  uint32_t tmp3 = 0xc0000000;
  do{*((uint32_t *)tmp3++) = 0;} while(tmp3 != 0xC3FFFFFC);



///---------------------

 //   DBGMCU->CR |= 1 <<5; ///TRACE_MODE

///---------------------
/// запуск spi 25q64



    GPIOB->MODER |= 0x00002020;
    GPIOB->OSPEEDR |= 0x00003030;
    GPIOB->PUPDR |= 0x00001010;
    GPIOB->AFR[0] |= 0x0a000900;
    GPIOB->LCKR = 0x00010044;
    GPIOB->LCKR = 0x00000044;
    GPIOB->LCKR = 0x00010044;
    GPIOF->MODER |= 0x000aa000;
    GPIOF->OSPEEDR |= 0x000ff000;
    GPIOF->PUPDR |= 0x00055000;
    GPIOF->AFR[0] |= 0x99000000;
    GPIOF->AFR[1] |= 0x000000aa;
    GPIOF->LCKR = 0x000103C0;
    GPIOF->LCKR = 0x000003C0;
    GPIOF->LCKR = 0x000103C0;


    QUADSPI->DCR = (22 <<16)    /// FSIZE[16]: Flash memory size количество адресных линий (23 = 8mb)-1
                | (0 <<8)       /// CSHT[8]: Chip select high time удержание активности чипа
                | (0);          /// CKMODE[0] Mode 0 линия такта сначала в нуле
    QUADSPI->PIR = 0xffff;
    QUADSPI->LPTR = 0xffff;

/// сброс
    QUADSPI->DLR = 0;           /// количество байт при чтении через регистр
    QUADSPI->FCR = 0x1B;        /// сброс флагов
    QUADSPI->CR = (3 <<24)|1;     /// PRESCALER[24]: Clock prescaler FAHB/5 (+1) // 2
/// ресет
QUADSPI->DLR = 1;
QUADSPI->CCR = 0x01000101;
while(!(QUADSPI->SR & 0x04)); /// FTF flag  FIFO пуст -  =1
QUADSPI->DR = 0xFFFF; /// младший байт первым
while(!(QUADSPI->SR & 0x02)); /// Wait for TCF flag to be set - передача данных завершена =1



    QUADSPI->DLR = 0;
    QUADSPI->CCR = 0x05000135;              /// Read Status Register-1 (35h
    while(!(QUADSPI->SR & QUADSPI_SR_TCF)); /// операция завершена
    tmp = QUADSPI->DR;

    QUADSPI->CCR = 0x0104; ///  Write Disable (04h)
    QUADSPI->PSMKR = 0x03;
tmp = QUADSPI->SR;
tmp = QUADSPI->SR;
while(QUADSPI->SR & QUADSPI_SR_BUSY);
QUADSPI->CCR = 0x08000105; /// чтение флага - подождите
tmp = QUADSPI->SR;
tmp = QUADSPI->SR;
tmp = QUADSPI->SR;


  //  if (tmp ==0)
 //   {
        QUADSPI->CCR = 0x0106; /// Write Enable (06h)
        while(QUADSPI->SR & QUADSPI_SR_BUSY);
        while(QUADSPI->SR & 0x20); /// Wait for busy flag to be cleared
            tmp = QUADSPI->SR;
        QUADSPI->PSMKR = 0xff;
        QUADSPI->CCR = 0x08000105; /// чтение флага - подождите
        tmp = QUADSPI->DR;
        tmp = QUADSPI->SR;
        tmp = QUADSPI->DR;
        tmp = QUADSPI->SR;
        tmp = QUADSPI->SR;


 //   }


delay(1000);

QUADSPI->CCR = 0x0106; /// Write Enable (06h)
while(QUADSPI->SR & 0x20); /// Wait for busy flag to be cleared
tmp = QUADSPI->SR;
    tmp = QUADSPI->DR;
    tmp = QUADSPI->SR;
delay(100);

QUADSPI->DLR = 1;
QUADSPI->CCR = 0x01000101;
tmp = QUADSPI->SR;
while(!(QUADSPI->SR & 0x04)); /// Wait for FTF flag to be set
tmp = QUADSPI->SR;
QUADSPI->DR = 0x0200; /// младший байт первым
tmp = QUADSPI->SR;
while(!(QUADSPI->SR & 0x02)); /// Wait for TCF flag to be set */
tmp = QUADSPI->SR;
delay(100);

QUADSPI->CCR = 0x0104; ///  Write Disable (04h)
tmp = QUADSPI->SR;
while(QUADSPI->SR & QUADSPI_SR_BUSY);
tmp = QUADSPI->SR;
tmp = QUADSPI->SR;
delay(100);


///---------
    QUADSPI->FCR = 0x1B;
    QUADSPI->DLR = 0;
    QUADSPI->CCR = 0x05000135;              /// Read Status Register-2 (35h)
    tmp = QUADSPI->SR;
    while(!(QUADSPI->SR & 0x02)); /// Wait for TCF flag to be set */
    tmp = QUADSPI->SR;
    tmp = QUADSPI->DR;
    delay(50);
///-------
    QUADSPI->FCR = 0x1B;
    QUADSPI->DLR = 0;
    QUADSPI->CCR = 0x05000105;              /// Read Status Register-1 (05h
    while(!(QUADSPI->SR & QUADSPI_SR_TCF)); /// операция завершена
    tmp = QUADSPI->DR;
///-------
delay(50);

    QUADSPI->FCR = 0x1B;        /// сброс флагов
    QUADSPI->ABR = 0xaa;
    QUADSPI->DLR = 3;           /// количество байт при чтении через регистр
    QUADSPI->CCR = (1 <<28)      /// SIOO[28] Отправить команду только один раз режим IMODE = 00
                | (3 <<26)      /// FMODE[26] Функциональный режим
                | (3 <<24)      /// DMODE[24] Количество линий данных
                | (4 <<18)      /// DCYC[18] Количество фиктивных перепадов такта
                | (0 <<16)      /// ABSIZE[16] Альтернативный режим - размер данных
                | (3 <<14)      /// ABMODE[14] Альтернативный режим - количество линий
                | (2 <<12)      /// ADSIZE[12] Адрес размер
                | (3 <<10)      /// ADMODE[10] Режим адрес
                | (1 <<8)       /// IMODE[8] Количество линий под инструкцию
                | 0xEB;         /// Прямая команда Fast Read Dual Output (3Bh)
    delay(50);


RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    ///Запуск таймера TIM3 для датчика растояния SR04
   TIM3->PSC = 82 ;
   TIM3->CR1 |= TIM_CR1_ARPE;
   TIM3->CCMR1 |= TIM_CCMR1_OC2PE;
   TIM3->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_IC2F_1; //TI1FP1+фильтр
   TIM3->CCMR1 |=  TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; //2
   TIM3->CCMR2 |= (TIM_CCMR2_OC3M_2|TIM_CCMR2_OC3M_1);
   TIM3->CCMR2 |= (TIM_CCMR2_OC4M_2|TIM_CCMR2_OC4M_1);
   TIM3->ARR =65534 ;
   TIM3->CCR1 = 0;
   TIM3->CCR2 = 52366 ;
   TIM3->CCR3 = 13170;
   TIM3->CCR4 = 13170 ;
   TIM3->SMCR = (5<<4) | 4; //TI1FP1+Reset Mode
   TIM3->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC1P |TIM_CCER_CC2E|TIM_CCER_CC2P);
   TIM3->BDTR |= TIM_BDTR_MOE;
   TIM3->DIER = TIM_DIER_UIE;
   TIM3->CR1 |= TIM_CR1_CEN;
   NVIC_SetPriority(TIM3_IRQn, 14);
   NVIC_EnableIRQ(TIM3_IRQn); // разрешение прерывания

    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
 //Инициализация таймера TIM7
   TIM7->PSC =1;
   TIM7->CR1 |= TIM_CR1_ARPE;
   TIM7->ARR =10;
   TIM7->CCR1 = 1 ;
   TIM7->DIER = TIM_DIER_UDE;
  // TIM7->CR1 |= TIM_CR1_CEN;



}



void PVD_IRQHandler (void)
{
PWR->CR1 &= ~PWR_CR1_DBP;     ///  защита записи  защищённого домена вкл

QUADSPI->DLR = 1;
QUADSPI->CCR = 0x01000101;
QUADSPI->FCR = 0x1B;
while(!(QUADSPI->SR & 0x04)); /// FTF flag  FIFO пуст -  =1
QUADSPI->DR = 0xFFFF; /// младший байт первым
while(!(QUADSPI->SR & 0x02)); /// Wait for TCF flag to be set - передача данных завершена =1
while(1);
}

