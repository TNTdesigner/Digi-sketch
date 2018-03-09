org 0000h



;------------------------------------------------------------------
;Hoofd programma
;------------------------------------------------------------------
main:
    mov     sp,#7fh                 ;stackpointer instellen
    lcall   initftoetsen
    lcall   inits

    mov     DATAREG,#00h            ;adres pointer voor tekst instellen
    lcall   zendData
    mov     DATAREG,#00h
    lcall   zendData
    mov     COMANDREG,#24h
    lcall   zendComando
  
    mov     DATAREG,#24h
    lcall   zendData
    mov     COMANDREG,#c0h
    lcall   zendComando

    mov     COMANDREG,#9ch
    lcall   zendComando
lus2:
    mov     DATAREG,#24h
    lcall   zendData
    mov     COMANDREG,#c0h
    lcall   zendComando

    
    mov     DATAREG,#24h
    lcall   zendData
    mov     COMANDREG,#c0h
    lcall   zendComando

    
    mov     DATAREG,#24h
    lcall   zendData
    mov     COMANDREG,#c0h
    lcall   zendComando

    
    mov     DATAREG,#24h
    lcall   zendData
    mov     COMANDREG,#c0h
    lcall   zendComando

    mov     COMANDREG,#9ch
    lcall   zendComando
    ljmp    lus2   



;------------------------------------------------------------------
;Initializatie van alle componenten nodig in het hoofd programma
;------------------------------------------------------------------
inits:
    lcall   initGLCD                ;initializatie grafisch LCD scherm, MOET EERST GEBEUREN
ret





#include "project.inc"
#include "xcez1.inc" ;include file toevoegen
