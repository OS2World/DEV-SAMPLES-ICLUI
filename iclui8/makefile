all : bsp1.exe bsp2.exe bsp3.exe bspdll.dll

bsp1.exe : bsp1.cpp
	icc /Q /B/PM:PM /O /Ft- /Tdp bsp1.cpp cppooc3i.lib cppom30i.lib

bsp2.exe : bsp2.cpp
	icc /Q /B/PM:PM /O /Ft- /Tdp bsp2.cpp cppooc3i.lib cppom30i.lib

bsp3.exe : bsp3.cpp bspdll.lib
	icc /Q /B/PM:PM /O /Ft- /Tdp bsp3.cpp bspdll.lib cppooc3i.lib cppom30i.lib

bspdll.lib : bspdll.def
	implib bspdll.lib bspdll.def

bspdll.def : bspdll.obj
	echo LIBRARY BSPDLL >bspdll.def
	echo PROTMODE >>bspdll.def
	echo DATA MULTIPLE NONSHARED READWRITE LOADONCALL >>bspdll.def
	echo CODE LOADONCALL >>bspdll.def
	echo EXPORTS >>bspdll.def
	cppfilt /Q /B /P /s /n /o bspdll.obj >>bspdll.def

bspdll.dll : bspdll.obj bspdll.def
	icc /Q /B/PM:PM /O /Ft- /Tdp /Ge- /Fe bspdll.dll bspdll.obj bspdll.def cppooc3i.lib cppom30i.lib

bspdll.obj : bspdll.cpp
	icc /Q /C /O /Ft- /Tdp bspdll.cpp
