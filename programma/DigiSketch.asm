org 0000h
lcall   main

org 000bh
lcall   timer

timer:
    push    acc
    push    psw
    mov     r0,p4_data

    pop     psw
    pop     acc
reti

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
    ;lcall   initGLCD                ;initializatie grafisch LCD scherm, MOET EERST GEBEUREN
    lcall   initRotary
ret

;------------------------------------------------------------------
;Initializatie van de rotary encoders
;------------------------------------------------------------------
initRotary:
    lcall   P4in
    lcall   initsio
    mov     tmod,#00000010b ;timer in auto relaud mode zetten
    mov     tl0,#06h
    mov     th0,#06h
    setb    tr0
    setb    ea
    setb    et0
ret

P4in:
    push    syscon0              ;juiste map selecteren
    mov     syscon0,#004h
    push    port_page            ;tijdelijk bewaren zodat we dat kunnen herstellen
    mov     port_page,#000h      ;pagina 0 selecteren
    mov     p4_dir,#000h         ;poort 4 als input schakelen
    pop     port_page            ;herstellen in oorspronkelijke staat
    pop     syscon0              ;pagina terug herstellen     
ret



#include "project.inc"
#include "xcez1.inc" ;include file toevoegen
