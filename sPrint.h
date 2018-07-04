/// sPrint.h
/// слепая печать без указания параметров типа переменной
/// без зависимостей от внешних библиотек
/// printo("текст", float / uint(8-32)_t / int(8-32)_t )
/// размер - 1064 байт при агрессивной оптимизации
/// на вкус и цвет... добавить собственную функцию печати
/// макрос отдаёт адрес массива для печати (char*)
/// #define soft_printing(data_char)  monitor_print(data_char)


#ifndef _sPrinto_




/// печать float
char* float_char (float value);

/// печать uint32_t, uint16_t, uint8_t
char * ui32_char (uint32_t value);

/// печать int32_t, int16_t, int8_t
char * i32_char (int32_t value);

typedef union
{
    uint32_t x1;
    int32_t x2;
    float x3;
}all_type;

void   printo( char* text, all_type value);



/// на вкус и цвет...
//#define soft_printing(data_char)  Sprint(data_char)
#define soft_printing(data_char)  monitor_print(data_char)


#define ICE_TYPE(type_x) (__typeof__(type_x))
#define ICE_i(var) (ICE_TYPE (var) (0) > -1 ? 0 : 1 )
#define ICE_F(var) (ICE_TYPE (var) (0.1f) ==  0 ? 0 : 1 )




/// слепая печать без указания параметров
/// printo("текст", float/ uint(8-32)_t/ int(8-32)_t )
#define printo(text, value)     soft_printing(text); \
    soft_printing ( ICE_F(value) ? float_char(value) :  ICE_i(value) ?  \
    ui32_char(value) : i32_char(value) )



#endif
#define _sPrinto_

