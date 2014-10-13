
kalender.exe : kalender.obj kalender.res
	icc /Q /Ti /Tdp /Ft- /Gm kalender.obj /B/BATCH /B/PM:PM dde4muii.lib dde4cci.lib
	rc -x kalender.res kalender.exe >nul

kalender.obj : kalender.cpp kalender.hpp kalender.h
	icc /Q /Ti /Tdp /Ft- /Gm /C kalender.cpp

kalender.res : kalender.rc kalender.h
	rc -r kalender.rc >nul

