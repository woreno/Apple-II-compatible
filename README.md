# Apple-II-compatible (2021)
Ths is an Apple II compatible built from scratch. As it was made by transfer photo-paper and etching with ferric chloride, and made the traces wider to avoid errors; as result the board didn't fit the A4 copper sheet so memory and expansions slots were made into 2 different boards. 
* The logic was recovered from Apple's original schematic photos, corrected and studied with the book *The Apple II Circuit Description, 1983*.
* The DRAM logic is a bit different from the original.
* System ROM halves are selectable by a switch; one half will run a memory test, another half is Apple II original ROM. The Apple II ROM includes a memory test subroutine that can be accessed by a CALL command, but if the memory ins't reliable the machine probably won't boot the Apple II ROM. With the flip-switch, the machine will boot instead into a small program that only uses CPU registers to test the memory.  
* Character ROM pin layout was reordered to accommodate the rest the schematic; as pins differs, so the original Character ROM data needs to be shuffled by a program before programming. 
* The keyboard matrix and keyboard board containing the matrix decoder were also etched, and there's a ROM that translates the scancodes into ascii - the original had a GY AY-5-3600 decoder chip with a ROM-on-chip etched from factory.   
* It just has 4 slots instead of 8. Never tested a the slots as i have no Apple II cards. 
Despite this i had planned to assign a number to each of them using a small ROM to store them, and having 7-segment displays to display their number but this as too cumbersome and i stopped the project.


**References** 
 * The Apple II Circuit Description, 1983, Winston D. Gayler
 * Apple II schematics https://mirrors.apple2.org.za/ftp.apple.asimov.net/documentation/hardware/schematics/
 * Ben Eater's EEPROM arduino programmer



