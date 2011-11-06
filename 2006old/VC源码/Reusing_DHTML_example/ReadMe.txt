Демо приложение к статье :

  Использование web-интерфейса в MFC приложениях.
  Исследование в сторону  облегчения использования activeX елемента WebBrowser и DHTML для создания интефейса приложения на DHTML.
  Автор: Шилоносов Александр shilo@dekart.com
  Ноябрь, 2003 год. 

Требования Для компиялции проекта:

  0. Intenet Exporer 4.0 (Лучше 5.0)
 
  1. Необходимо установить Platform SDK. (хотябы July 2002 SDK)

  2. Необходимо установить библиотеку IE5.5 libarary (ie55_lib.exe 1.5 Mb)
     Адрес для скачивания и инструкции по установке:
     http://msdn.microsoft.com/downloads/samples/internet/default.asp?url=/downloads/samples/internet/libraries/ie55_lib/default.asp""



Возможные ошибки при компиляции проекта :


--------------------Configuration: dhtml2 - Win32 Release--------------------
Compiling...
HtmlCtrl.cpp
C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\transact.h(226) : error C2059: syntax error : 'constant'
C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\transact.h(271) : error C2143: syntax error : missing ';' before '}'
C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oledb.h(17149) : error C2143: syntax error : missing ';' before '}'
C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oledb.h(17149) : error C2143: syntax error : missing ';' before '}'
C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\oledb.h(17149) : error C2143: syntax error : missing ';' before '}'

Не знаю чесно говоря причину этой ошибки но она исправляется путем коментирования 
"typedef enum XACT_E {... }" структуры в файле transact.h (строка 226).

--------------------Configuration: dhtml2 - Win32 Release--------------------
Compiling...
HtmlCtrl.cpp
C:\Documents and Settings\Alex Shilonosov\Desktop\My Docs\DHTML\dhtml2\DocHostSite.h(96) : error C2504: 'IDocHostUIHandler2' : base class undefined
Error executing cl.exe.

Означает что вы не установили библиотеку IE5.5 libarary. см. пункт "Требования Для компиялции проекта"


--
 Шилоносов Александр.
 shilo@dekart.com.
 Ноябрь, 2003 год. 

