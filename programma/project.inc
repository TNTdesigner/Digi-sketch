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
    ;scherm
    WR          bit     p3_data.0   ;write line van scherm actief laag
    RD          bit     p3_data.1   ;read line van scherrm actief laag
    CE          bit     p3_data.2   ;chip enble van scherm actief laag
    CD          bit     p3_data.3   ;Command write
    RESET       bit     p0_data.1   ;reset van scherm actief laag
    FS          bit     p0_data.0   ;font groote selecteren FS=1: 6*8, FS=0: 8*8

    ;encoders
    SW1         bit     p2_data.2   ;knop in rotary encoder 1
    SW2         bit     p2_data.7   ;knop in rotary encoder 2
    R1A         bit     p2_data.4   ;rotary encoder 1 pin A
    R1B         bit     p2_data.3   ;rotary encoder 1 pin B
    R2A         bit     p2_data.5   ;rotary encoder 2 pin A
    R2B         bit     p2_data.6   ;rotary encoder 2 pin B
                                                                                
;------------------------------------------------------------------
;declaratie van registers                   
;------------------------------------------------------------------
    STA         equ     10h         ;geheugenplaats voor de status check
    DATAREG     equ     11h         ;geheugenplaats voor het verzenden van data naar het scherm
    COMANDREG   equ     12h         ;geheugenplaats voor het verzenden van comandos naar het scherm
    ROTARY      equ     13h         ;geheugenplaats voor de momentele waarde van de encoders
    ROTARYTEMP  equ     14h         ;geheugenplaats voor de momentele waarde van de encoders te vergelijken
    clrL        equ     15h         ;geheugenplaats voor het legen van het het scherm
    clrH        equ     16h         ;geheugenplaats voor het legen van het het scherm
    POSX        equ     17h         ;geheugenplaats voor X positie op scherm
    POSY        equ     18h         ;geheugenplaats voor Y positie op scherm
                                        
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
;    mov     r0,a
;    lcall   debug
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
    clr     POSX                    ;X positie op 0 zetten
    clr     POSY                    ;Y positie op 0 zetten

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

;------------------------------------------------------------------
;encoders initializeren
;------------------------------------------------------------------  
initRotary:
    mov     p2_dir,#000h            ;poort 2 als input zetten
    clr     ROTARY
    clr     ROTARYTEMP
    mov     tmod,#00000010b         ;timer op 8bit autoreload zetten
    mov     TH0,#200
    mov     TL0,#200
    setb    EA
    setb    ET0
    setb    TR0  

ret


