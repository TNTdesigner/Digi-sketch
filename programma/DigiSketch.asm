org 0000h
ljmp   main

org 000bh
    push    acc
    push    psw
    mov     a,p2_data
    cjne    a,ROTARYTEMP,verzet
    ljmp    stopRotary
verzet:
    mov     r0,a
    lcall   debug
    mov     ROTARYTEMP,a
stopRotary:
    pop     psw
    pop     acc
reti

;------------------------------------------------------------------
;Hoofd programma
;------------------------------------------------------------------
main:
    mov     sp,#7fh                 ;stackpointer instellen
    lcall   inits

    ;Mode set
;    mov     COMANDREG,#10000000b
;    lcall   comandWrite
    ;Display mode
;    mov     COMANDREG,#10011100b
;    lcall   comandWrite

    ;adres data pointer
;    mov     DATAREG,#00h            ;lower address
;    lcall   dataWrite
;    mov     DATAREG,#00h
;    lcall   dataWrite
;    mov     COMANDREG,#24h
;    lcall   comandWrite
    ;auto write mode
;    mov     COMANDREG,#b0h
;    lcall   comandWrite

;lus0:
;    mov     dptr,#startscherm       ;begin afbeelding in pointer steken
;    mov     dptr,#rogue       ;begin afbeelding in pointer steken
;lus:
;    clr     a
;    movc    a,@a+dptr               ;get byte
;    cjne    a,#10101010b,next
;    ljmp    stop
;next:    
;    mov     DATAREG,a
;    lcall   dataAutoWrite
;    inc     dptr
;    ljmp    lus
    ;data auto read af zetten
;    mov     COMANDREG,#b2h
;    lcall   comandWrite
stop:
    
    ljmp    stop    


;------------------------------------------------------------------
;Initializatie van alle componenten nodig in het hoofd programma
;------------------------------------------------------------------
inits:
    lcall   initsio                 ;voor debug
    mov     port_page,#000h         ;selecteer poort page 0
;    lcall   initGLCD                ;initializatie grafisch LCD scherm, MOET EERST GEBEUREN
    lcall   initRotary
;    lcall   clearScreen
ret


;include filles
#include "tables.inc"
#include "project.inc"
#include "xcez1.inc" ;include file toevoegen
