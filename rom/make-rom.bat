@cls
@set COM_PORT=6
@set ROM_PATH=C:\Development\docs\arquiteturas\apple][\rom

glue %ROM_PATH%\apple2.rom %ROM_PATH%\341011d0.bin %ROM_PATH%\341012d8.bin %ROM_PATH%\341013e0.bin %ROM_PATH%\341014e8.bin %ROM_PATH%\341015f0.bin %ROM_PATH%\341020f8.bin


prom w -f=%ROM_PATH%\apple2.rom -a=0x5000 -com=%COM_PORT%

