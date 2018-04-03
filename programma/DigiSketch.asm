
org 0000h
ljmp   main

;------------------------------------------------------------------
;Hoofd programma
;------------------------------------------------------------------
main:
    mov     sp,#7fh                 ;stackpointer instellen
    lcall   inits
      
lus:

ljmp    lus


;------------------------------------------------------------------
;Initializatie van alle componenten nodig in het hoofd programma
;------------------------------------------------------------------
inits:
    mov     port_page,#000h         ;selecteer poort page 0
    lcall   initGLCD                ;initializatie grafisch LCD scherm, MOET EERST GEBEUREN

ret


#include "project.inc"
#include "xcez1.inc" ;include file toevoegen
