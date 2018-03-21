org 0000h



;------------------------------------------------------------------
;Hoofd programma
;------------------------------------------------------------------
main:
    mov     sp,#7fh                 ;stackpointer instellen
    lcall   inits


lus2:

    ljmp    lus2   



;------------------------------------------------------------------
;Initializatie van alle componenten nodig in het hoofd programma
;------------------------------------------------------------------
inits:
    lcall   initGLCD                ;initializatie grafisch LCD scherm, MOET EERST GEBEUREN
ret





#include "project.inc"
#include "xcez1.inc" ;include file toevoegen
