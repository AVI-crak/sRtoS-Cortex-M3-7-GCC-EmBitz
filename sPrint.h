/// sPrint.h
/// слепая печать без указания параметров типа переменной
/// без зависимостей от внешних библиотек
/// printo("текст", double / float / uint(8-32-64)_t / int(8-32-64)_t )
/// размер - 1907 байт при агрессивной оптимизации
/// на вкус и цвет... добавить собственную функцию печати
/// макрос отдаёт адрес массива для печати (char*)
/// #define soft_printing(data_char)  monitor_print(data_char)


#ifndef _sPrinto_

#include "monitor.h"



/// 8 < OUT_TXT_SIZE_FLOATING < 16
#define OUT_TXT_SIZE_FLOATING   14


char* ui32_char (uint32_t value);
char* i32_char (int32_t value);
char* i64_char (int64_t value);
char* ui64_char (uint64_t value);
char* float_char (float value);
char* double_char (double value);


typedef union
{
    double      x1;
    uint64_t    x2;
    int64_t     x3;
    uint32_t    x4;
    int32_t     x5;
    float       x6;
}all_type;


//__attribute__( ( always_inline ) ) inline void   printo( char* text, all_type value);


/// на вкус и цвет...
//#define soft_printing(data_char)  Sprint(data_char)
#define soft_printing(data_char)  monitor_print(data_char)


#define ICE_TYPE(type_x) (__typeof__(type_x))
#define ICE_SIZE(type_x) (sizeof(type_x))
#define ICE_i(var) (ICE_TYPE (var) (0) > -1 ? 0 : 1 )
#define ICE_F(var) (ICE_TYPE (var) (0.1f) ==  0 ? 0 : 1 )
#define ICE_D(var) (ICE_TYPE (var) (0.1d) ==  0 ? 0 : 1 )
#define ICE_64(var) (ICE_SIZE (var) == 8 ? 0 : 1 )
#define ICE_C(var) (ICE_TYPE (var) ("0") ==  (var + 1 ) ? 0 : 1 )





/// printo("текст", float/ uint(8-32)_t/ int(8-32)_t )
#define printo(text, value)     soft_printing(text); \
    soft_printing ( ICE_64(value) ? (ICE_F(value) ? float_char(value) :  (ICE_i(value) ? ui32_char(value) : i32_char(value)) ) : \
    (ICE_D(value) ? double_char(value) :  (ICE_i(value) ? ui64_char(value) : i64_char(value)) ) )



#endif
#define _sPrinto_

