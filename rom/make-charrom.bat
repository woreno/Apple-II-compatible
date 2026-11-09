@cls
@set COM_PORT=6
@set ROM_PATH=C:\Development\docs\arquiteturas\apple][\rom

remap charrom-remap.txt %ROM_PATH%\3410036-CHAR.BIN charrom.bin

@rem confirmacao
fdiffs %ROM_PATH%\3410036-CHAR.BIN charrom.bin

prom w -f=charrom.bin  -com=%COM_PORT%
