;------------------------------------------------------------------
;Lijst van functies:
;   -statusCheck            Status check van scherm opslaan in STA register
;   -initGLCD               Initializatie van grafisch LCD scherm
;   -debug                  debug info op seriele poot zetten
;   -dataWrite              data naar het scherm sturen
;   -dataRead               data van het scherm lezen
;   -comandWrite            comanndo naar scherm sturen
;------------------------------------------------------------------                                                                
;------------------------------------------------------------------
;declaratie van pinnen          
;------------------------------------------------------------------
    WR          bit     p3_data.0   ;write line van scherm actief laag
    RD          bit     p3_data.1   ;read line van scherrm actief laag
    CE          bit     p3_data.2   ;chip enble van scherm actief laag
    CD          bit     p3_data.3   ;Command write
    RESET       bit     p0_data.1   ;reset van scherm actief laag
    FS          bit     p0_data.0   ;font groote selecteren FS=1: 6*8, FS=0: 8*8
                                                                                
;------------------------------------------------------------------
;declaratie van registers                   
;------------------------------------------------------------------
    STA         equ     10h         ;geheugenplaats voor de status check
    DATAREG     equ     11h         ;geheugenplaats voor het verzenden van data naar het scherm
    COMANDREG   equ     12h         ;geheugenplaats voor het verzenden van comandos naar het scherm
    ROTARY      equ     13h         ;geheugenplaats voor de momentele waarde van de encoders
    ROTARYTEMP  equ     14h         ;geheugenplaats voor de momentele waarde van de encoders te vergelijken
                                        
;------------------------------------------------------------------
;Status check van scherm opslaan in STA register
;-STA                                               
;------------------------------------------------------------------
statusCheck:                              
    push    acc                                 
    push    psw                         
    clr     STA                     ;STA register leeg maken
    mov     p4_dir,#000h            ;p4 als input zetten
    setb    CD
    setb    WR
stan:
    clr     RD
    mov     a,p4_data
    setb    RD
    setb    CD
    mov     r0,a
    lcall   debug
    mov     STA,a                   ;bijhouden voor eventueel gebruik
    anl     a,#03h                  ;masker voor STA0 en STA1
    cjne    a,#03h,stan
    mov     r0,a
    lcall   debug
    mov     p4_dir,#0ffh
    pop     psw
    pop     acc
ret

;------------------------------------------------------------------
;Initializatie van grafisch LCD scherm
;------------------------------------------------------------------
initGLCD:
    lcall   initsio                 ;voor debug
    mov     p0_dir,#0ffh            ;poort 1 als output
    mov     p3_dir,#0ffh            ;poort 3 als output
    clr     CE                      ;standaart laag houden
    setb    WR                      ;standaart hoog maken
    setb    RD                      ;standaart hoog maken
    setb    FS                      ;font op 6*8 zetten
    clr     RESET                   ;reseten van scherm
    lcall   delay1ms 
    setb    RESET                   ;reset terug af zetten 
    lcall   statusCheck             ;initiele status check uitvoeren

    ;Mode set
    mov     COMANDREG,#10000000b
    lcall   comandWrite

    ;Graphics home address
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     COMANDREG,#42h
    lcall   comandWrite

    ;Graphics area set
    mov     DATAREG,#28h
    lcall   dataWrite
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     COMANDREG,#43h
    lcall   comandWrite

    ;text home address
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     DATAREG,#0ah
    lcall   dataWrite
    mov     COMANDREG,#40h
    lcall   comandWrite

    ;text area set
    mov     DATAREG,#28h
    lcall   dataWrite
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     COMANDREG,#41h
    lcall   comandWrite

    ;adres data pointer
    mov     DATAREG,#00h            ;lower address
    lcall   dataWrite
    mov     DATAREG,#00h
    lcall   dataWrite
    mov     COMANDREG,#24h
    lcall   comandWrite

    ;Display mode
    mov     COMANDREG,#10010000b
    lcall   comandWrite
    
ret

;------------------------------------------------------------------
;Data naar het schem sturen 
;-DATAREG
;------------------------------------------------------------------
DataWrite:
    push    acc
    push    psw
    lcall   statusCheck             ;status checken
    mov     p4_data,DATAREG         ;zet de data op de bus
    clr     CD
    nop
    clr     WR                      
    nop
    setb    WR
    pop     psw
    pop     acc
ret

dataAutoWrite:
    push    acc
    push    psw
dataAutoWriteStatus:
    lcall   statusCheck             ;status checken
    mov     a,STA
    anl     a,#04h                  ;sta 2 nakijken
    cjne    a,#04h,dataAutoWriteStatus
    mov     p4_data,DATAREG         ;zet de data op de bus
    clr     CD
    nop
    clr     WR                      
    nop
    setb    WR
    pop     psw
    pop     acc
ret

;------------------------------------------------------------------
;data van het scherm inlezen
;-DATAREG
;------------------------------------------------------------------
DataRead:
    push    acc
    push    psw
    lcall   statusCheck             ;status checken
    mov     p4_dir,#000h            ;p4 als ingang
    clr     CD
    clr     RD
    nop                             ;wachten
    nop
    mov     DATAREG,p4_data         ;data inlezeen van scherm
    pop     psw
    pop     acc
ret

;------------------------------------------------------------------
;comando naar het scherrm sturen
;-COMANDREG
;------------------------------------------------------------------
ComandWrite:
    push    acc
    push    psw
    lcall   statusCheck             ;status checken
    mov     p4_data,COMANDREG       ;comando op de bus zetten
    setb    CD
    clr     WR
    nop
    setb    WR
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

initRotary:
    push    acc
    push    psw
    mov     p2_dir,#000h            ;poort 2 als input zetten
    mov     ROTARY,p2_data          ;de waarde van de encoders bewaren
    pop     psw
    pop     psw
ret

;------------------------------------------------------------------
;lees de encoders in
;------------------------------------------------------------------
readRotary:
    push    acc
    push    psw
    mov     a,ROTARY
    mov     ROTARYTEMP,p2_data
    cjne    a,ROTARYTEMP,nietGelijk
    ljmp    gelijk    
nietGelijk:
    mov     ROTARY,p2_data
gelijk:
    pop     psw
    pop     acc
ret

;------------------------------------------------------------------
;laat start menu zien
;------------------------------------------------------------------
StartMenu:
    push    acc
    push    psw
    
    pop     psw
    pop     acc
ret

 




