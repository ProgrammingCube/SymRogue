@echo off
:: win build
gcc symRogue.c -o win_build/symRogue.exe

:: sym build
:: make sure you have cc65 installed!
:: change this to your cc65 root path (the one with all the folders and readme)
set curdir=%cd%
set cc65dir=C:\Users\Patrick\Documents\cc65_sym1_source

if exist symRogue.c (
	echo.
	echo --- Building symRogue ---
:: set your config file here
	set config=%cc65dir%\cfg\sym1-4k.cfg
	echo Config file: sym1-4k.cfg
	if exist symRogue.s (
		del symRogue.s
	)
	if exist symRogue.o (
		del symRogue.o
	)
	if exist symRogue.bin (
		del symRogue.bin
	)
	if exist sym_build\symRogue.hex (
		del sym_build\symRogue.hex
	)
	%cc65dir%\bin\cc65 --cpu 65C02 -t sym1 -Cl -Osir -DSYM symRogue.c
	%cc65dir%\bin\ca65 --cpu 65C02 symRogue.s
	%cc65dir%\bin\ld65 -C %config% -m symRogue.map -o symRogue.bin symRogue.o %cc65dir%\lib\sym1.lib
	if exist symRogue.bin (
		%cc65dir%\bin\bin2hex symRogue.bin sym_build\symRogue.hex > nul
	)

	if exist sym_build\symRogue.hex (
		echo --- symRogue.hex made ---
		python hexptp.py sym_build\symRogue.hex sym_build\symRogue.ptp
	) else echo --- symRogue.hex FAIL ---
)