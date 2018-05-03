Главная особенность sRtoS - потоковый запуск всех активных тасков, без запрета и потери прерываний, без обёрток для прерываний, без коллизий доступа к ресурсам, без потери времени мк на простых задачах. Все активные задачи выполняются в безусловном варианте, но с разным временем работы. Это позволяет выполняться самой жирной задаче в максимальном объёме, и при этом не блокируются все остальные.

sRtoS в данном варианте работает чипах серии cortex m3, это чипы: STM32L1x, STM32F1x, STM32F2x, с небольшой оговоркой - пока временно без внешней памяти.
У всех этих чипов есть таймер 6, в ос он используется для задания времени лимита задачи. Стандартный таймер SysTick используется в реализации задержек.
На данный момент объём кода чуть больше 3к, статической памяти 60 байт, и минимум 128 байт под новую задачу. Время переключения контекста с отключённой отладкой - 70 тактов мк.
Ос потоковая, это означает что все активные задачи имеют одинаковый приоритет по моменту запуска , но разное время работы (лимит). По сути моя ос похожа на https://www.segger.com, с небольшой разницей: у меня отрытый проект.
Второе отличие - для использования ос в проекте необходимо просто подключить два файла.
Всё - ос доступна.

Запуск ос в любом участке майна, но до этого момента должен быть запущен и залочен PLL, иначе таймер 6 ребутнётся, и вся ось зависнет.
По идее всю настройку ног, и PLL необходимо делать в функции SystemInit , она запускается из стартового *.s файла раньше майна.

Старт ос: setup_run (частота ядра в гц, размер стека майна, размер стека прерываний, стартовое время задачи в микросекундах)
Нет дифлайнов для всех этих параметров, значения вводятся непосредственно в параметры стартовой функции.
Функция производит перемещение стеков, инициализацию нулевой задачи, и перевод мк в защищённый режим.
В связи с чем использование связанных с потоками функций в теле прерываний - не допустимо !!! Можно использовать в прерывании автономные функции,
которые не используются в задачах. В таком случае gcc может гарантировать корректность возврата из функции.

Далее - обзор функций:
sDelay_mc - задержка в мс, по политическим причинам нельзя юзать в прерываниях, и до запуска ос.

sTask_alarm_mc - функция условного выполнения кода от циклического таймера, требует статической внешней переменной.
Отдаёт 1 в случае превышения времени от прошлого срабатывания. Для смены времени необходимо обнулять внешнюю переменную.
Есть небольшой глюк, двойное время каждые 49 дней.

malloc - выделение памяти , размер в байтах. Аналог стандартной функции, но есть привязка к имени задачи. Это позволяет делать кашу из данных.

free - удаление памяти, (адрес) , в случае когда адрес уже не принадлежит задаче - удаления не происходит.

sTask_kill - необязательная функция, можно создать задачу без бесконечного цикла, тогда при завершении алгоритма задача
самостоятельно убьётся об стену без дополнительных команд. Сначала удаляется вся выделенная память, а после и сама задача.
Можно удалить только ту задачу - где написана эта функция.

sTask_new(функция, размер стека, процент времени 1-100, указатель на массив параметров новой задачи [4])
Место для нового стека очищается, расположение стека - первое свободное место сверху.
Указанная функция будет новой задачей, для неё можно указать массив входных параметров до 4 штук.
Процент времени - аналог приоритета, задача будет безусловно исполняемой, но её время работы может быть меньше 100%.
Процент в дословном понимании, от 1до 100.
Размер стека автоматически корректируется до нормального значения, и есть возможность узнать реально потреблённый размер стека через просмотр в отладчике (в режиме стоп) вложенных структур activ - основной структуры sSustem_task, параметр stack_max_zize.
После полной уверенности можно уменьшить размер стека до значения параметра stack_max_zize, и отключить отладку __Test_psp.
Имя задачи "task_name" - обязательный уникальный параметр, отправная точка идентификации задачи для пользователя и для работы других функций. Повторы не допустимы!!!


sTask_resource_ask (глобальная переменная) Запрос ресурса, бесконечный цикл - пока не освободится. В случае занятости ресурса - задача освобождает время мк до следующего опроса. Так-как все активные задачи запускаются в потоке - коллизии не возникает. Глобальная переменная должна быть видна всем таскам. Таск получивший доступ - просто вписывает своё имя.

sTask_resource_free (глобальная переменная) Освободить ресурс.

sNVIC_EnableIRQ - (имя прерывания, приоритет) Включение прерывания отдельной функцией в момент работы ос может привести к глюкам.
По этой причине функция выполняет всё разом. Для пользователя разрешены уровни 1-14.


sNVIC_DisableIRQ - отключить прерывание.

sRandom (мах-мин) программное решение рандомного числа с пределами, число целое.

sTask_skip - уступить время, обязательная команда к конце цикла каждой задачи.
Просто переключает диспетчер на новую задачу, когда текущая выполнила всё что от неё требуется на данный момент.

sTask_wait ( имя глобальной переменной - флага) Впасть в зависимость. Имя флага обязано быть уникальным, размер значения не имеет.
После применения - текущая задача становится зависимой от состояния данного флага, и выпадает в осадок списка activ.

sTask_wake -( имя глобального флага) - Разбудить задачу, в условии взведённого флага начинается поиск задачи в списке wait, и перемещение её в activ.

Наполеоновские планы есть, свободного времени мало.

Форум для обсуждения http://forum.ixbt.com/topic.cgi?id=48:11735
Средство контроля версий - черепаха https://tortoisehg.bitbucket.io/ программа на русском языке, с почти нативным интерфейсом.
Как подключится к проекту https://www.youtube.com/watch?v=MFj-IxwCN8s


Первй тест.
http://www.youtube.com/watch?v=lGg5RqCE5Ig