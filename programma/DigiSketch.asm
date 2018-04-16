clrL    equ     15h                 ;register voor het legen van het het scherm
clrH    equ     16h                 ;register voor het legen van het het scherm

org 0000h
ljmp   main

;------------------------------------------------------------------
;Hoofd programma
;------------------------------------------------------------------
main:
    mov     sp,#7fh                 ;stackpointer instellen
    lcall   inits

    ;Mode set
    mov     COMANDREG,#10000000b
    lcall   comandWrite
    ;Display mode
    mov     COMANDREG,#10011100b
    lcall   comandWrite

    ;adres data pointer
    mov     DATAREG,#00h            ;lower address
    lcall   dataWrite
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     COMANDREG,#24h
    lcall   comandWrite
    ;auto write mode
    mov     COMANDREG,#b0h
    lcall   comandWrite

lus0:
    mov     dptr,#startscherm       ;begin afbeelding in pointer steken
lus:
    clr     a
    movc    a,@a+dptr               ;get byte
    cjne    a,#10101010b,next
    ljmp    stop
next:    
    mov     DATAREG,a
    lcall   dataAutoWrite
    inc     dptr
    ljmp    lus
    ;data auto read af zetten
    mov     COMANDREG,#b2h
    lcall   comandWrite
stop:
    lcall   readRotary
    ljmp    stop    


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
    mov     clrL,#ffh               ;255 keer doolopen
    mov     clrH,#0fh               ;16 keer doorlopen           
    ;Mode set
    mov     COMANDREG,#10000000b    ;OR mode    
    lcall   comandWrite
    ;Display mode
    mov     COMANDREG,#10011100b    ;gafisch en tekst aan zetten
    lcall   comandWrite
    ;auto write mode
    mov     COMANDREG,#b0h
    lcall   comandWrite
lus2400:
    mov     DATAREG,#000h
    lcall   dataAutoWrite
    djnz    clrL,lus2400
    mov     clrL,#ffh
    djnz    clrH,lus2400
    ;data auto read af zetten
    mov     COMANDREG,#b2h
    lcall   comandWrite

    ;adres data pointer resetten
    mov     DATAREG,#00h            ;lower address
    lcall   dataWrite
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     COMANDREG,#24h
    lcall   comandWrite
    pop     acc
    pop     psw
ret




;include filles
#include "tables.inc"
#include "project.inc"
#include "xcez1.inc" ;include file toevoegen
