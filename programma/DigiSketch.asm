clrL    equ     14h                 ;register voor het legen van het het scherm
clrH    equ     15h                 ;register voor het legen van het het scherm

org 0000h
ljmp   main

;------------------------------------------------------------------
;Hoofd programma
;------------------------------------------------------------------
main:
    mov     sp,#7fh                 ;stackpointer instellen
    lcall   inits
   
laatzien:
    lcall   readRotary
    ljmp    laatzien


;------------------------------------------------------------------
;Initializatie van alle componenten nodig in het hoofd programma
;------------------------------------------------------------------
inits:
    mov     port_page,#000h         ;selecteer poort page 0
    lcall   initGLCD                ;initializatie grafisch LCD scherm, MOET EERST GEBEUREN
    lcall   clearScreen
ret

;------------------------------------------------------------------
;Het scherm leeg maken
;------------------------------------------------------------------
clearScreen:
    push    acc
    push    psw
    mov     clrL,#255               ;255 keer doolopen               
    mov     clrH,#8                 ;32 keer de 255 keer doorlopen, heel het geheugen van controller leeg maken (8192 keer)
    ;Mode set
    mov     COMANDREG,#10000010b
    lcall   comandWrite
L1:      
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     COMANDREG,#c0h
    lcall   comandWrite
    djnz    clrL,L1
    mov     clrL,#255
    djnz    clrH,L1
    ;adres data pointer
    mov     DATAREG,#00h            ;lower address
    lcall   dataWrite
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     COMANDREG,#24h
    lcall   comandWrite
    pop     acc
    pop     psw
ret



#include "project.inc"
#include "xcez1.inc" ;include file toevoegen
