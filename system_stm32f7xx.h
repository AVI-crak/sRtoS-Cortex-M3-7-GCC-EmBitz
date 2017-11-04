
///HSE 16mgz
// этот файл не является частью ОС, и должен быть изменён под текущий проект, или использован как шаблон.



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




__attribute__( ( always_inline ) ) static inline void delay(volatile uint32_t time_tmp){time_tmp++; do {time_tmp--;} while ( time_tmp );}
void SystemInit (void)
{
    volatile uint32_t tmp;
    volatile uint32_t timeout;
    __set_CONTROL(0);

    SCB->CPACR |= 15<<20; /// FPU settings

    DBGMCU->APB1FZ = 0x06e019ff;
    DBGMCU->APB2FZ = 0x00070003; /// стоп в отладке всех таймеров

    MPU->RNR = 0;
    MPU->RBAR = 0x20000000;
    MPU->RASR = 0x03060025; /// C=0, B=0, TEX=1, S=0, SRD=0, XN=0, AP=3, size=0x12 (512k)
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



    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM7EN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
    RNG->CR = RNG_CR_RNGEN;

    RCC->AHB1ENR |=  RCC_AHB1ENR_DMA2DEN | RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_BKPSRAMEN | 0x001007ff; /// GPIO ALL;

    RCC->AHB3ENR |= RCC_AHB3ENR_QSPIEN | RCC_AHB3ENR_FMCEN;
    RCC->AHB3RSTR = RCC_AHB3RSTR_QSPIRST | RCC_AHB3RSTR_FMCRST;
    RCC->AHB3RSTR = 0;

/// RCC
    delay(80000);

    RCC->CR  = RCC_CR_HSION | RCC_CR_HSEON;
    while((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);
    while((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);
    RCC->PLLCFGR =  RCC_PLLCFGR_PLLSRC_HSE  /// PLL SRC[22] Source Mux = HSE (1)
                    | (12)                  /// PLL M[0] = 12
                    | (324 <<6)             /// PLL N[6] = 324
                    | (0 <<16)              /// PLL P[16] = 2 (00: PLLP = 2)
                    | (9 <<24);             /// PLL Q[24] = 9
    RCC->PLLSAICFGR = (252 <<6)             /// PLLSAI N[6] = 252
                    | (3 <<28)              /// PLLSAI R[28] = 3
                    | (3 <<16);              /// PLLSAI P[16] = 8
    RCC->CR |=  RCC_CR_PLLSAION | RCC_CR_PLLON; /// вкл PLL
    RCC->CFGR = (5 <<10)                    /// APB1[10] = 4 (101: AHB clock divided by 4)
                | (4 <<13)                  /// APB2[13] = 2 (100: AHB clock divided by 2)
                | (3 <<21)                  /// MCO1[21] = 11: PLL clock selected
                | (7 <<26);                 /// MCO1PRE: MCO1[26] prescaler = 111: division by 5
    FLASH->ACR = 7                          /// Latency >210mg
                | FLASH_ACR_ARTEN           /// вкл кеш флеша
                | FLASH_ACR_PRFTEN;
    while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);
    while((RCC->CR & RCC_CR_PLLSAIRDY) != RCC_CR_PLLSAIRDY);
    RCC->CFGR |= 2;                         /// SW[0] 10: PLL selected as system clock
    SCB_InvalidateICache();
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
if((RCC->BDCR & RCC_BDCR_LSERDY) /// кварц работал раньше
   &&(((RCC->BDCR) & RCC_BDCR_RTCSEL ) == RCC_BDCR_RTCSEL_0) /// кварц работал на часы
   &&((RCC->BDCR) & RCC_BDCR_RTCEN)) /// часы уже включенны
 goto batareika ;else;

RCC->BDCR |= RCC_BDCR_BDRST;/// ресет модуля
RCC->BDCR &= ~RCC_BDCR_BDRST;
RCC->AHB1LPENR |= RCC_AHB1LPENR_BKPSRAMLPEN;
PWR->CSR1 |= PWR_CSR1_BRE;   /// вкл LP стаб на BKPSRAM
while(!(PWR->CSR1 & PWR_CSR1_BRR));
RCC->BDCR |= RCC_BDCR_LSEDRV_0 | RCC_BDCR_RTCEN; /// вкл часового модуля
RCC->BDCR |= RCC_BDCR_LSEON; /// включаем внешний кварц
RCC->CSR |= RCC_CSR_LSION; /// включаем внутренний генератор 32Кгц
for(tmp = 0x40024000; tmp < 0x40024FFC ; tmp +=4) // обнуление BKPSRAM
 {
    *(__IO uint32_t*) tmp = 0x0;
    delay(10);
 }
for (tmp=6000000;tmp>1;tmp--)
    {
    if ((RCC->BDCR & RCC_BDCR_LSERDY) == RCC_BDCR_LSERDY)
       {
        RCC->BDCR |= RCC_BDCR_RTCSEL_0; /* переключаемся на внешний кварц */
        break  ;
       }else ;
    }
if (((RCC->BDCR) & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY)
 {
   for (tmp=50000;tmp>1;tmp--)
    {
      if ((RCC->CSR & RCC_CSR_LSIRDY) == RCC_CSR_LSIRDY)
         {
           RCC->BDCR |= RCC_BDCR_RTCSEL_1; // переключаемся на внутренний генератор
           break ;
         }else;
    };
 }else;

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
//RTC->CALR = ((0 <<15) /// CALP + 488.5 ppm
//             |200);     ///  CALM[8:0] - 0.9537 ppm // +30 секунд в сутки

RTC->DR = ( ((( 2015 /10) & 0x0f)<<20)                             // year - год 2015
          | ((( 2015  - ((( 2015 /10) & 0x0f) *10)) & 0x0f) << 16) // year - год 2015
          | (((5 /10) & 0x01)<<12)                                 // month - месяц 5
          | ((( 5 - ((( 5 /10) & 0x01) *10)) & 0x0f) << 8)         // month месяц 5
          | ((( 10 /10) & 0x03)<<4)                                // day день 10
          | (( 10 - ((( 10 /10) & 0x03) *10)) & 0x0f)              // day день 10
          | (( 7  & 0x07) << 13) );                                // day_of_the_week день недели 7

RTC->TR = ( ((( 4 /10) & 0x03)<<20)                                // hour час 4
          | ((( 4  - ((( 4 /10) & 0x03) *10)) & 0x0f)<<16)         // hour час 4
          | ((( 20 /10) & 0x0f)<<12)                               // minutes - минуты 20
          | ((( 20  - ((( 20 /10) & 0x0f) *10)) & 0x0f)<<8)        // minutes - минуты 20
          | ((( 5 /10) & 0x0f)<<4)                                 // seconds секунды 5
          | (( 5  - ((( 5 /10) & 0x0f) *10)) & 0x0f) );            // seconds секунды 5
delay(100);
RTC->ISR &= (~RTC_ISR_INIT); // закрываем доступ на запись регистрам часов реального времени
RTC->WPR =0xEE;
delay(1000);
//PWR->CR1 &= (~ PWR_CR1_DBP);


batareika :
//PWR->CR1 &= (~ PWR_CR1_DBP);




//delay(10000);

//RCC->AHB1ENR |= 0x001007ff;              /// GPIO ALL


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







/*
    GPIO_Inctall(GPIOH,5,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_sdnwe
    GPIO_Inctall(GPIOF,11,OUT_AF12,PUII_PP_UP,F100Mgz); /// FMC_sdnras
    GPIO_Inctall(GPIOH,3,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_sdne0
    GPIO_Inctall(GPIOG,15,OUT_AF12,PUII_PP_UP,F100Mgz); /// FMC_sdncas
    GPIO_Inctall(GPIOG,8,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_sdclk
    GPIO_Inctall(GPIOH,2,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_sdcke0
    GPIO_Inctall(GPIOE,1,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_nbl1
    GPIO_Inctall(GPIOE,0,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_nbl0
    GPIO_Inctall(GPIOD,10,OUT_AF12,PUII_PP_DO,F100Mgz); /// FMC_d15
    GPIO_Inctall(GPIOD,9,OUT_AF12,PUII_PP_DO,F100Mgz);  /// FMC_d14
    GPIO_Inctall(GPIOD,8,OUT_AF12,PUII_PP_DO,F100Mgz);  /// FMC_d13
    GPIO_Inctall(GPIOE,15,OUT_AF12,PUII_PP_DO,F100Mgz); /// FMC_d12
    GPIO_Inctall(GPIOE,14,OUT_AF12,PUII_PP_DO,F100Mgz); /// FMC_d11
    GPIO_Inctall(GPIOE,13,OUT_AF12,PUII_PP_DO,F100Mgz); /// FMC_d10
    GPIO_Inctall(GPIOE,12,OUT_AF12,PUII_PP_DO,F100Mgz); /// FMC_d9
    GPIO_Inctall(GPIOE,11,OUT_AF12,PUII_PP_DO,F100Mgz); /// FMC_d8
    GPIO_Inctall(GPIOE,10,OUT_AF12,PUII_PP_DO,F100Mgz); /// FMC_d7
    GPIO_Inctall(GPIOE,9,OUT_AF12,PUII_PP_DO,F100Mgz);  /// FMC_d6
    GPIO_Inctall(GPIOE,8,OUT_AF12,PUII_PP_DO,F100Mgz);  /// FMC_d5
    GPIO_Inctall(GPIOE,7,OUT_AF12,PUII_PP_DO,F100Mgz);  /// FMC_d4
    GPIO_Inctall(GPIOD,1,OUT_AF12,PUII_PP_DO,F100Mgz);  /// FMC_d3
    GPIO_Inctall(GPIOD,0,OUT_AF12,PUII_PP_DO,F100Mgz);  /// FMC_d2
    GPIO_Inctall(GPIOD,15,OUT_AF12,PUII_PP_DO,F100Mgz); /// FMC_d1
    GPIO_Inctall(GPIOD,14,OUT_AF12,PUII_PP_DO,F100Mgz); /// FMC_d0
    GPIO_Inctall(GPIOG,5,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_ba1
    GPIO_Inctall(GPIOG,4,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_ba0
    GPIO_Inctall(GPIOG,2,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a12
    GPIO_Inctall(GPIOG,1,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a11
    GPIO_Inctall(GPIOG,0,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a10
    GPIO_Inctall(GPIOF,15,OUT_AF12,PUII_PP_UP,F100Mgz); /// FMC_a9
    GPIO_Inctall(GPIOF,14,OUT_AF12,PUII_PP_UP,F100Mgz); /// FMC_a8
    GPIO_Inctall(GPIOF,13,OUT_AF12,PUII_PP_UP,F100Mgz); /// FMC_a7
    GPIO_Inctall(GPIOF,12,OUT_AF12,PUII_PP_UP,F100Mgz); /// FMC_a6
    GPIO_Inctall(GPIOF,5,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a5
    GPIO_Inctall(GPIOF,4,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a4
    GPIO_Inctall(GPIOF,3,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a3
    GPIO_Inctall(GPIOF,2,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a2
    GPIO_Inctall(GPIOF,1,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a1
    GPIO_Inctall(GPIOF,0,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a0

    GPIO_Inctall(GPIOD,5,OUT_AF12,PUII_PP_UP,F50Mgz);  /// FMC_nwe
    GPIO_Inctall(GPIOD,4,OUT_AF12,PUII_PP_UP,F50Mgz);  /// FMC_noe
    GPIO_Inctall(GPIOD,7,OUT_AF12,PUII_PP_UP,F50Mgz);  /// FMC_NE1
    GPIO_Inctall(GPIOE,3,OUT_AF12,PUII_PP_UP,F100Mgz);  /// FMC_a19

    GPIO_Inctall(GPIOJ,7,INPUT,PUII_PP_UP,F2Mgz);       /// LCD_Fmark
    GPIO_Inctall(GPIOJ,8,OUT_PP,PUII_PP_OFF,F100Mgz);   /// LCD_Reset
*/

/// Инсталл FMC 1 SRAM NE1 + SDRAM

    /// SRAM1
    FMC_Bank1E->BWTR[0] = ( (0 <<28)            /// ACCMOD
                          | (0 << 24)          /// DATLAT задержка выдачи данных
                          | (7 <<16)            /// BUSTURN время свободной шины после записи /7
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
  for(tmp = 0xc0000000; tmp < 0xC3FFFFFC ; tmp += 4) ///64Mb 0.873 ms
  {
      *((volatile uint32_t *)tmp) = 0; //timeout +=9;
  }
/*
  timeout =0;
  for(tmp = 0xc0000000; tmp < 0xC3FFFFFC ; tmp += 4) ///32Mb 0.873 ms
  {
     if (( *((volatile uint32_t *)tmp)) != timeout ) tmp = 0xfffffff0;
      timeout +=9;
  }
*/




///---------------------

 //   DBGMCU->CR |= 1 <<5; ///TRACE_MODE

///---------------------
/// запуск spi 25q64
/// после иницилизации внешняя флеш память будет вулючена в адресное пространство мк

/*

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
*/

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


//RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    ///Запуск таймера TIM3 для датчика растояния SR04
   TIM3->PSC = 165 ;
   TIM3->CR1 |= TIM_CR1_ARPE;
   TIM3->CCMR1 |= TIM_CCMR1_OC2PE;
   TIM3->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_IC2F_1; //TI1FP1+фильтр
   TIM3->CCMR1 |=  TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1; //2
   TIM3->CCMR2 |= (TIM_CCMR2_OC3M_2|TIM_CCMR2_OC3M_1);
   TIM3->CCMR2 |= (TIM_CCMR2_OC4M_2|TIM_CCMR2_OC4M_1);
   TIM3->ARR =65534 ;
   TIM3->CCR1 = 0;
   TIM3->CCR2 = 65400 ;
   TIM3->CCR3 = 10;
   TIM3->CCR4 = 10 ;
   TIM3->SMCR = (5<<4) | 4; //TI1FP1+Reset Mode
   TIM3->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC1P |TIM_CCER_CC2E|TIM_CCER_CC2P);
   TIM3->BDTR |= TIM_BDTR_MOE;
   TIM3->DIER = TIM_DIER_UIE;
   TIM3->CR1 |= TIM_CR1_CEN;
   NVIC_SetPriority(TIM3_IRQn, 14);
   NVIC_EnableIRQ(TIM3_IRQn); // разрешение прерывания

  //  RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
 //Инициализация таймера TIM7
   TIM7->PSC =1;
   TIM7->CR1 |= TIM_CR1_ARPE;
   TIM7->ARR =10;
   TIM7->CCR1 = 1 ;
   TIM7->DIER = TIM_DIER_UDE;
  // TIM7->CR1 |= TIM_CR1_CEN;



};


///---------------------------------------------
/// DMA 1-2


/*
Available for STM32F42xxx and STM32F43xxx.
___________________________________________________________________________________________________________________
 DMA1     | Stream 0    | Stream 1  | Stream 2    | Stream 3    | Stream 4   | Stream 5    | Stream 6  | Stream 7  |
----------|-------------|-----------|-------------|-------------|------------|-------------|-----------|-----------|
Channel 0 | SPI3_RX     |           | SPI3_RX     | SPI2_RX     | SPI2_TX    | SPI3_TX     |           | SPI3_TX   |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 1 | I2C1_RX     |           | TIM7_UP     |             | TIM7_UP    | I2C1_RX     | I2C1_TX   | I2C1_TX   |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 2 | TIM4_CH1    |           | I2S3_EXT_RX | TIM4_CH2    | I2S_EXT_TX | I2S3_EXT_TX |  TIM4_UP  | TIM4_CH3  |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 3 | I2S3_EXT_RX | TIM2_UP   | I2C3_RX     | I2S2_EXT_RX | I2C3_TX    | TIM2_CH1    | TIM2_CH2  | TIM2_UP   |
          |             | TIM2_CH3  |             |             |            |             | TIM2_CH4  | TIM2_CH4  |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 4 | UART5_RX    | USART3_RX | UART4_RX    | USART3_TX   | UART4_TX   | USART2_RX   | USART2_TX | UART5_TX  |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 5 | UART8_TX    | UART7_TX  | TIM3_CH4    | UART4_RX    | TIM3_CH1   | TIM3_CH2    | UART8_RX  | TIM3_CH3  |
          |             |           | TIM3_UP     |             | TIM3_TRIG  |             |           |           |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 6 | TIM5_CH3    | TIM5_CH4  | TIM5_CH1    | TIM5_CH4    | TIM5_CH2   |             | TIM5_UP   |           |
          | TIM5_UP     | TIM5_TRIG |             | TIM5_TRIG   |            |             |           |           |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 7 |             | TIM6_UP   | I2C2_RX     | I2C2_RX     | USART3_TX  | DAC1        | DAC2      | I2C2_TX   |
----------|-------------|-----------|-------------|-------------|------------|-------------|-----------|---------- |

___________________________________________________________________________________________________________________
 DMA2     | Stream 0    | Stream 1  | Stream 2    | Stream 3    | Stream 4   | Stream 5    | Stream 6  | Stream 7  |
----------|-------------|-----------|-------------|-------------|------------|-------------|-----------|-----------|
Channel 0 | ADC1        | SA1_A     | TIM8_CH1    | SAI1_A      | ADC1       | SAI1_B      | TIM1_CH1  |           |
          |             |           | TIM8_CH2    |             |            |             | TIM1_CH2  |           |
          |             |           | TIM8_CH3    |             |            |             | TIM1_CH3  |           |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 1 |             | DCMI      | ADC2        | ADC2        | SAI1_B     | SPI6_TX     | SPI6_RX   | DCMI      |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 2 | ADC3        | ADC3      |             | SPI5_RX     | SPI5_TX    | CRYP_OUT    | CRYP_IN   | HASH_IN   |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 3 | SPI1_RX     |           | SPI1_RX     | SPI1_TX     |            | SPI1_TX     |           |           |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 4 | SPI4_RX     | SPI4_TX   | USART1_RX   | SDIO        |            | USART1_RX   | SDIO      | USART1_TX |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 5 |             | USART2_RX | USART6_RX   | SPI4_RX     | SPI4_TX    |             | USART6_TX | USART6_RX |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 6 | TIM1_TRIG   | TIM1_CH1  | TIM1_CH2    | TIM1_CH1    | TIM1_CH4   | TIM1_UP     | TIM1_CH3  |           |
          |             |           |             |             | TIM1_TRIG  |             |           |           |
          |             |           |             |             | TIM1_COM   |             |           |           |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 7 |             | TIM8_UP   | TIM8_CH1    | TIM8_CH2    | TIM8_CH3   | SPI5_RX     | SPI5_TX   | TIM8_CH4  |
          |             |           |             |             |            |             |           | TIM8_TRIG |
          |             |           |             |             |            |             |           | TIM8_COM  |
----------|-------------|-----------|-------------|-------------|------------|-------------|-----------|------------
*/

/*
Available for stm32f746xx.
___________________________________________________________________________________________________________________
 DMA1     | Stream 0    | Stream 1  | Stream 2    | Stream 3    | Stream 4   | Stream 5    | Stream 6  | Stream 7  |
----------|-------------|-----------|-------------|-------------|------------|-------------|-----------|-----------|
Channel 0 | SPI3_RX     |SPDIFRX_DT | SPI3_RX     | SPI2_RX     | SPI2_TX    | SPI3_TX     |SPDIFRX_CS | SPI3_TX   |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 1 | I2C1_RX     | I2C3_RX   | TIM7_UP     |             | TIM7_UP    | I2C1_RX     | I2C1_TX   | I2C1_TX   |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 2 | TIM4_CH1    |           |  I2C4 _RX   | TIM4_CH2    |            |  I2C4 _TX   |  TIM4_UP  | TIM4_CH3  |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 3 |             | TIM2_UP   | I2C3_RX     |             | I2C3_TX    | TIM2_CH1    | TIM2_CH2  | TIM2_UP   |
          |             | TIM2_CH3  |             |             |            |             | TIM2_CH4  | TIM2_CH4  |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 4 | UART5_RX    | USART3_RX | UART4_RX    | USART3_TX   | UART4_TX   | USART2_RX   | USART2_TX | UART5_TX  |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 5 | UART8_TX    | UART7_TX  | TIM3_CH4    | UART7_RX    | TIM3_CH1   | TIM3_CH2    | UART8_RX  | TIM3_CH3  |
          |             |           | TIM3_UP     |             | TIM3_TRIG  |             |           |           |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 6 | TIM5_CH3    | TIM5_CH4  | TIM5_CH1    | TIM5_CH4    | TIM5_CH2   |             | TIM5_UP   |           |
          | TIM5_UP     | TIM5_TRIG |             | TIM5_TRIG   |            |             |           |           |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 7 |             | TIM6_UP   | I2C2_RX     | I2C2_RX     | USART3_TX  | DAC1        | DAC2      | I2C2_TX   |
----------|-------------|-----------|-------------|-------------|------------|-------------|-----------|---------- |

___________________________________________________________________________________________________________________
 DMA2     | Stream 0    | Stream 1  | Stream 2    | Stream 3    | Stream 4   | Stream 5    | Stream 6  | Stream 7  |
----------|-------------|-----------|-------------|-------------|------------|-------------|-----------|-----------|
Channel 0 | ADC1        | SA1_A     | TIM8_CH1    | SAI1_A      | ADC1       | SAI1_B      | TIM1_CH1  |  SAI2_B   |
          |             |           | TIM8_CH2    |             |            |             | TIM1_CH2  |           |
          |             |           | TIM8_CH3    |             |            |             | TIM1_CH3  |           |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 1 |             | DCMI      | ADC2        | ADC2        | SAI1_B     | SPI6_TX     | SPI6_RX   | DCMI      |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 2 | ADC3        | ADC3      |             | SPI5_RX     | SPI5_TX    | CRYP_OUT    | CRYP_IN   | HASH_IN   |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 3 | SPI1_RX     |           | SPI1_RX     | SPI1_TX     |  SAI2_A    | SPI1_TX     |  SAI2_B   | QUADSPI   |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 4 | SPI4_RX     | SPI4_TX   | USART1_RX   | SDMMC1      |            | USART1_RX   | SDMMC1    | USART1_TX |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 5 |             | USART6_RX | USART6_RX   | SPI4_RX     | SPI4_TX    |             | USART6_TX | USART6_TX |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 6 | TIM1_TRIG   | TIM1_CH1  | TIM1_CH2    | TIM1_CH1    | TIM1_CH4   | TIM1_UP     | TIM1_CH3  |           |
          |             |           |             |             | TIM1_TRIG  |             |           |           |
          |             |           |             |             | TIM1_COM   |             |           |           |
==========|=============|===========|=============|=============|============|=============|===========|===========|
Channel 7 |             | TIM8_UP   | TIM8_CH1    | TIM8_CH2    | TIM8_CH3   | SPI5_RX     | SPI5_TX   | TIM8_CH4  |
          |             |           |             |             |            |             |           | TIM8_TRIG |
          |             |           |             |             |            |             |           | TIM8_COM  |
----------|-------------|-----------|-------------|-------------|------------|-------------|-----------|------------
*/

/*
//#pragma pack(push, 4)
ALCD static struct
{
  //  volatile uint16_t nc;
    volatile uint16_t reg;
    volatile uint16_t data;

}lcd; /// FMC_A19  LCD_RS
//#pragma pack(pop)



//#define LCD           ((LCD_TypeDef *) 0x60000000)

#define F2Mgz                 ((uint32_t) 0x00)
#define F10Mgz                ((uint32_t) 0x00000001)
#define F50Mgz                ((uint32_t) 0x00000002)
#define F100Mgz               ((uint32_t) 0x00000003)

#define PUII_PP_OFF          ((uint32_t) 0x00000000)
#define PUII_PP_UP           ((uint32_t) 0x00000001)
#define PUII_PP_DO           ((uint32_t) 0x00000002)
#define PUII_OD_OFF          ((uint32_t) 0x00000010)
#define PUII_OD_UP           ((uint32_t) 0x00000011)
#define PUII_OD_DO           ((uint32_t) 0x00000012)

#define INPUT                ((uint32_t) 0x00000000)
#define OUT_PP               ((uint32_t) 0x00000001)
#define ANALOG               ((uint32_t) 0x00000003)
#define OUT_AF0              ((uint32_t) 0x00000002)
#define OUT_AF1              ((uint32_t) 0x00000012)
#define OUT_AF2              ((uint32_t) 0x00000022)
#define OUT_AF3              ((uint32_t) 0x00000032)
#define OUT_AF4              ((uint32_t) 0x00000042)
#define OUT_AF5              ((uint32_t) 0x00000052)
#define OUT_AF6              ((uint32_t) 0x00000062)
#define OUT_AF7              ((uint32_t) 0x00000072)
#define OUT_AF8              ((uint32_t) 0x00000082)
#define OUT_AF9              ((uint32_t) 0x00000092)
#define OUT_AF10             ((uint32_t) 0x000000A2)
#define OUT_AF11             ((uint32_t) 0x000000B2)
#define OUT_AF12             ((uint32_t) 0x000000C2)
#define OUT_AF13             ((uint32_t) 0x000000D2)
#define OUT_AF14             ((uint32_t) 0x000000E2)
#define OUT_AF15             ((uint32_t) 0x000000F2)

#define RGB_out(Pr,Pg,Pb)   ((int16_t) (((Pr >> 3) << 11) | ((Pg >> 2) << 5) | (Pb >> 3)))
#define RGB(Pr,Pg,Pb)   ((int16_t) (((Pr & 31) << 11) | ((Pg & 63) << 5) | (Pb & 31)))

//static void Delay (volatile uint32_t time_tmp);
//__attribute__( ( always_inline ) ) static inline
__attribute__( ( always_inline ) ) static inline void Delay(volatile uint32_t time_tmp){time_tmp++; do {time_tmp--;} while ( time_tmp );}



/// GPIO_Inctall (GPIOE,0,OUT_PP,PUII_PP_OFF,F50Mgz)
///- настройка ножек (порт, номер ноги, тип входа-выхода, режим выхлопа, чатота выхлопа
void GPIO_Inctall (GPIO_TypeDef* GPIOx,uint16_t PIN,uint16_t RMODE,uint16_t RPUII,uint16_t RF);
void GPIO_Inctall (GPIO_TypeDef* GPIOx,uint16_t PIN,uint16_t RMODE,uint16_t RPUII,uint16_t RF)
{
    GPIOx->MODER &= (~( 0x00000003<<(PIN*2)));
    GPIOx->MODER |= (( uint32_t) (RMODE & 0x0000000F)<<(PIN*2));
    GPIOx->OTYPER &= (~(0x00000001 << PIN));
    GPIOx->OTYPER |= ((uint32_t) ((RPUII)>>4)<<PIN);
    GPIOx->PUPDR &= ~(( uint32_t) 0x00000003 << (PIN*2));
    GPIOx->PUPDR |= (( uint32_t) (RPUII & 0x00000003)<<(PIN*2));
    GPIOx->OSPEEDR &= (~((uint32_t)(0x00000003)<<((PIN)*2)));
    GPIOx->OSPEEDR |= ((uint32_t)(RF)<<((PIN)*2));
    if (((RMODE)&0x000F)!= 0x0002) return ; else;
    if ((PIN) < 8)
    {
    	GPIOx->AFR[0] &= (~((uint32_t)0x0000000F<<(PIN*4)));
        GPIOx->AFR[0] |= (uint32_t)(RMODE>>4)<<(PIN*4);
    }else
    {
        GPIOx->AFR[1] &= (~((uint32_t)0x0000000F<<((PIN-8)*4)));
        GPIOx->AFR[1] |= ((uint32_t)(RMODE>>4)<<((PIN-8)*4));
    };
}

/// обновление даты и времени: год , месяц ,  день , день_недели , час , минуты ,  секунды
static void RTC_Update_Data (uint8_t year,uint8_t month,uint8_t day,uint8_t day_of_the_week,uint8_t hour,uint8_t minutes,uint8_t seconds);
void RTC_Update_Data (uint8_t year,uint8_t month,uint8_t day,uint8_t day_of_the_week,uint8_t hour,uint8_t minutes,uint8_t seconds)
{
RTC->WPR = 0xCA;
Delay(1000);
RTC->WPR = 0x53;
Delay(1000);
RTC->ISR |= RTC_ISR_INIT;
while(!(RTC->ISR & RTC_ISR_INITF)) {}
RTC->PRER = 0x007f00ff;
uint32_t tmp;
tmp  = (((year/10) & 0x0f)<<20);
tmp |= (((year - (((year/10) & 0x0f) *10)) & 0x0f) << 16);
tmp |= (((month/10) & 0x01)<<12);
tmp |= (((month - (((month/10) & 0x01) *10)) & 0x0f) << 8);
tmp |= (((day/10) & 0x03)<<4);
tmp |= ((day - (((day/10) & 0x03) *10)) & 0x0f);
tmp |= ((day_of_the_week & 0x07) << 13);
RTC->DR = tmp;
tmp  = (((hour/10) & 0x03)<<20);
tmp |= (((hour - (((hour/10) & 0x03) *10)) & 0x0f)<<16);
tmp |= (((minutes/10) & 0x0f)<<12);
tmp |= (((minutes - (((minutes/10) & 0x0f) *10)) & 0x0f)<<8);
tmp |= (((seconds/10) & 0x0f)<<4);
tmp |= ((seconds - (((seconds/10) & 0x0f) *10)) & 0x0f);
RTC->TR = tmp;
RTC->ISR &= (~RTC_ISR_INIT);
RTC->WPR =0xEE;
Delay(1000);
}

 */
