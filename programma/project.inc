;------------------------------------------------------------------
;Lijst van functies:
;   -statusCheck            Status check van scherm opslaan in STA register
;   -initGLCD               Initializatie van grafisch LCD scherm
;   -P1out                  Initializatie poort 1 als output
;   -P3in                   Initializatie poort 3 als output
;   -P3out                  Initializatie poort 3 als input
;   -zendData               Data verzeden naar scherm
;   -zenComando             Comando verzeden naar scherm
;   -debug                  debug info op seriele poot zetten
;------------------------------------------------------------------                                                                
;------------------------------------------------------------------
;declaratie van pinnen          
;------------------------------------------------------------------
    WR          bit     p3_data.0   ;write line van scherm actief laag
    RD          bit     p3_data.1   ;read line van scherrm actief laag
    CE          bit     p3_data.2   ;chip enble van scherm actief laag
    CD          bit     p3_data.3   ;Command write
    RESET       bit     p1_data.1   ;reset van scherm actief laag
    FS          bit     p1_data.0   ;font groote selecteren FS=1: 6*8, FS=0: 8*8
                                                           
                        
                        
;------------------------------------------------------------------
;declaratie van registers                   
;------------------------------------------------------------------
    STA         equ     10h         ;geheugenplaats 10h gelijk stellen aan STA
    DATAREG     equ     11h         ;geheugenplaats voor het verzenden van data naar het scherm
    COMANDREG   equ     12h         ;geheugenplaats voor het verzenden van comandos naar het scherm


;------------------------------------------------------------------
;Status check van scherm opslaan in STA register
;-STA
;------------------------------------------------------------------
statusCheck:
    push    acc
    push    psw
    lcall   P5in                        ;poort 4 als input schakelen
    lcall   P3out
STAOPNIEUW:   
    mov     p3_data,#00h 
    mov     STA,#00h                    ;STA register leeg maken
    mov     p3_data,#00001110b          
    lcall   delay1ms 
    mov     a,p5_data                   ;status word inlezen
    anl     a,#00000011b                ;masker voor STA0 en STA1
;    mov     r0,a                        ;debug status op aurt zetten
;    lcall   debug
    cjne    a,#00000011b,STAOPNIEUW      ;vergelijken
    mov     STA,a                       ;data opslaan in STA voor eventuel gebruik later
    mov     r0,a                        ;debug status op aurt zetten
    lcall   debug
    mov     p3_data,#00h
    pop     psw
    pop     acc
ret

;------------------------------------------------------------------
;Initializatie van grafisch LCD scherm
;-Poort 1
;------------------------------------------------------------------
initGLCD:
    lcall   P1out                   ;poort 1 initializeren
    clr     RESET                   ;reseten van scherm
    mov     a,#20h                   ;reset lang genenoeg laten duren
    lcall   delaya0k05s
    setb    FS                      ;font op 6*8 zetten
    setb    RESET                   ;reset terug af zetten
     
        
    lcall   initsio                 ;voor debug

    lcall   statusCheck             ;initiele status check uitvoeren
    
    mov     COMANDREG,#80h          ;mode instellen
    lcall   zendComando             ;mode instellen

    mov     DATAREG,#00h            ;grafisch home adres
    lcall   zendData
    mov     DATAREG,#00h
    lcall   zendData
    mov     COMANDREG,#42h 
    lcall   zendComando     
    
    mov     DATAREG,#28h            ;Grafic aeria set
    lcall   zendData
    mov     DATAREG,#00h
    lcall   zendData
    mov     COMANDREG,#43h
    lcall   zendComando


    mov     DATAREG,#00h            ;text home aderes
    lcall   zendData
    mov     DATAREG,#00h                
    lcall   zendData
    mov     COMANDREG,#40h
    lcall   zendComando
    
    mov     DATAREG,#28h            ;aantal colommen text
    lcall   zendData
    mov     DATAREG,#00h
    lcall   zendData
    mov     COMANDREG,#41h
    lcall   zendComando
ret


;------------------------------------------------------------------
;Initializatie poort 1 als output
;-poort 1
;------------------------------------------------------------------
P1out:
    push    syscon0                 ;juiste map selecteren
    mov     syscon0,#004h
    push    port_page               ;tijdelijk bewaren zodat we dat kunnen herstellen
    mov     port_page,#001h         ;selecteer poort page 0
    mov     p1_pudsel,#0ffh         ;selecteer pull_up device
    mov     p1_puden,#0ffh          ;selectie inschakelen
    mov     port_page,#000h         ;selecteer poort page 0
    mov     p1_dir,#0ffh            ;poort 1 als output schakelen
    pop     port_page               ;herstellen in oorspronkelijke staat
    pop     syscon0                 ;pagina terug herstellen
ret

;------------------------------------------------------------------
;Initializatie poort 3 als output
;-poort 3
;------------------------------------------------------------------
P3out:
    push    syscon0                 ;juiste map selecteren
    mov     syscon0,#004h
    push    port_page               ;tijdelijk bewaren zodat we dat kunnen herstellen
    mov     port_page,#000h         ;selecteer poort page 0
    mov     p3_dir,#0ffh            ;poort 3 als output schakelen
    pop     port_page               ;herstellen in oorspronkelijke staat
    pop     syscon0                 ;pagina terug herstellen
ret         

;------------------------------------------------------------------
;Initializatie poort 5 als output
;-poort 5
;------------------------------------------------------------------
P5out:
    push    syscon0                 ;juiste map selecteren
    mov     syscon0,#004h
    push    port_page               ;tijdelijk bewaren zodat we dat kunnen herstellen
    mov     port_page,#000h         ;selecteer poort page 0
    mov     p5_dir,#0ffh            ;poort 5 als output schakelen
    pop     port_page               ;herstellen in oorspronkelijke staat
    pop     syscon0                 ;pagina terug herstellen
ret         

;------------------------------------------------------------------
;Initializatie poort 4 als input
;-poort 4
;------------------------------------------------------------------
P5in:
    push    syscon0                 ;juiste map selecteren
    mov     syscon0,#004h
    push    port_page               ;tijdelijk bewaren zodat we dat kunnen herstellen
    mov     port_page,#000h         ;selecteer poort page 0
    mov     p5_dir,#000h            ;poort 5 als input schakelen
    pop     port_page               ;herstellen in oorspronkelijke staat
    pop     syscon0                 ;pagina terug herstellen
ret 
;------------------------------------------------------------------
;Data verzeden naar scherm 
;-DATAREG
;------------------------------------------------------------------
zendData:
    push    acc
    push    psw
    lcall   P5out                   ;poort 5 als output schakelen
    mov     p3_data,#0000010b
    lcall   delay1ms
    mov     p5_data,DATAREG               ;data op de poort zetten
    mov     r0,DATAREG
    lcall   debug
    lcall   statusCheck             ;status van scherm checken voor het zenden van data
    pop     psw
    pop     acc
ret

;------------------------------------------------------------------
;Comando verzeden naar scherm 
;-COMANDREG
;------------------------------------------------------------------
zendComando:
    push    acc
    push    psw
    lcall   P5out                   ;poort 5 als output schakelen
    mov     p3_data,#00001010b
    lcall   delay1ms
    mov     p5_data,COMANDREG       ;data op de poort zetten
    mov     r0,COMANDREG
    lcall   debug
    lcall   statusCheck             ;status van scherm checken voor het zenden van data
    pop     psw
    pop     acc
ret

;------------------------------------------------------------------
;debug info op seriele poort zetten 
;-r0
;------------------------------------------------------------------
debug:
    push    acc
    push    psw
    mov     a,#0dh
    lcall   siooutchar
    mov     a,r0
    lcall   siooutbyte
    mov     a,#0ah
    lcall   siooutchar
    pop     psw
    pop     acc
ret


















              

