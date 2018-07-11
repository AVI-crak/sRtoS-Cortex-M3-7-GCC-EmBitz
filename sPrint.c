///sPrint.c

/// sPrint.h
/// слепая печать без указания параметров типа переменной
/// без зависимостей от внешних библиотек
/// printo("текст", float / uint(8-32)_t / int(8-32)_t )
/// размер - 1064 байт при агрессивной оптимизации
/// на вкус и цвет... добавить собственную функцию печати
/// макрос отдаёт адрес массива для печати (char*)
/// #define soft_printing(data_char)  monitor_print(data_char)
#include <stdint.h>
#include "sPrint.h"






union float_raw
{
    struct
    {
        uint32_t massa  :23;
        uint32_t order  :8;
        uint32_t sign   :1;
    };
    uint32_t    u_raw;
    float       f_raw;
};

char float_text[16];

/// печать float
char* float_char (float value)
{
    union float_raw Ftemp;
    Ftemp.f_raw = value;
    uint32_t ofconst = 100000000;
    int32_t oftemp2 = (int32_t)((uint8_t) Ftemp.order - 127);
    int32_t ofreze = 0;
    uint32_t of10raw = 100000000;
    if ((oftemp2 == -127) && (Ftemp.massa == 0))
    {
        float_text[0] = '-';
        float_text[1] = '0';
        float_text[2] = ',';
        float_text[3] = '0';
        float_text[4] = 0;
        if (Ftemp.sign == 0) return &float_text[1]; else return &float_text[0];
    }else if (oftemp2 == -127)
    {
        ofconst = 117549430;  ofreze = -38;  of10raw = 0;
        while ((Ftemp.massa >> 22) == 0)
        {
            ofconst >>= 1; Ftemp.massa <<= 1;
            if (ofconst < 99999999 )
            {
                ofreze -= 1;
                ofconst *=10;
            };
        };
    }else if ((oftemp2 == 128) && (Ftemp.massa == 0))
    {
        float_text[0] = '-';
        float_text[1] = '1';
        float_text[2] = '.';
        float_text[3] = '#';
        float_text[4] = 'I';
        float_text[5] = 'N';
        float_text[6] = 'F';
        float_text[7] = 0;
        if (Ftemp.sign == 0) return &float_text[1]; else return &float_text[0];
    }else if ((oftemp2 == 128) && (Ftemp.massa != 0))
    {
        float_text[0] = 'N';
        float_text[1] = 'A';
        float_text[2] = 'N';
        float_text[3] = 0;
        return &float_text[0];
    }else if (oftemp2 < 0)
    {
        while ( oftemp2 !=0)
        {
            ofconst >>= 1; oftemp2 += 1;
            if (ofconst < 100000000 )
            {
                ofreze -= 1;
                ofconst *=10;
            };
        };
        of10raw = ofconst;
    }else if (oftemp2 > 0)
    {
        while ( oftemp2 !=0)
        {
            ofconst <<= 1; oftemp2 -= 1;
            if (ofconst > 1000000000 )
            {
                ofreze += 1;
                ofconst /=10;
            };
        };
        of10raw = ofconst;
    }else of10raw = ofconst;
    of10raw += (uint32_t) (((uint64_t) Ftemp.massa * ofconst) >> 23) ;
    uint32_t ofout;
    of10raw += 6;
    of10raw /=10;
    ofout = 12;
    do
    {
        float_text[ofout--] = of10raw % 10 + '0';
        of10raw /= 10;
    }while (of10raw);
    ofreze += 4 - ofout;
    uint32_t ofline;
    /// фигня получилась
    if ((0 <= ofreze ) && (ofreze < 9))
    {
        ofline = ofout;
        do
        {
           float_text[ofline] = float_text[ofline +1];
           ofline++;
        }while ( ofreze-- );
        float_text[ofline] = '.'; ofline = 13;
        while ( float_text[--ofline] == '0');
        if (float_text[ofline] == '.') ofline += 2; else ofline++;
        float_text[ofline] = 0;
        if (Ftemp.sign != 0) float_text[--ofout] = '-';
        return &float_text[ofout];
    };

    float_text[ofout] = float_text[(ofout) + 1];
    float_text[(ofout) + 1] = '.';
    ofconst = 11;
    while ( float_text[--ofconst] == '0');
    if (float_text[ofconst] == '.') ofconst += 2; else ofconst++;
    if (ofreze !=0 )
    {
        float_text[ofconst] = 'e';
        if (ofreze < 0) {float_text[(ofconst) +1] = '-'; ofreze = 0 - ofreze;}
            else float_text[(ofconst) +1] = '+';
        if (ofreze < 10)
        {
            float_text[(ofconst) +2] = ofreze % 10 + '0'; ofreze /= 10;
            float_text[(ofconst) +3] = 0;
        }else
        {
            float_text[(ofconst) +3] = ofreze % 10 + '0'; ofreze /= 10;
            float_text[(ofconst) +2] = ofreze % 10 + '0';
            float_text[(ofconst) +4] = 0;
        };
    }else   float_text[ofconst] = 0;
    if (Ftemp.sign != 0) float_text[--ofout] = '-';
    return &float_text[ofout];
}

/// печать uint32_t, uint16_t, uint8_t
char * ui32_char (uint32_t value)
{
    uint32_t ofreze = 16;
    float_text[--ofreze] = 0;
    do
    {
        float_text[--ofreze] = value % 10 + '0';
        value /= 10;
    } while (value != 0);
    return &float_text[ofreze];
}

/// печать int32_t, int16_t, int8_t
char * i32_char (int32_t value)
{
    int32_t ofzee = value;
    if (value < 0) value = 0 - value;

    uint32_t ofreze = 16;
    float_text[--ofreze] = 0;
    do
    {
        float_text[--ofreze] = value % 10 + '0';
        value /= 10;
    } while (value != 0);
    if (ofzee < 0) float_text[--ofreze] = '-';
    return &float_text[ofreze];
}




