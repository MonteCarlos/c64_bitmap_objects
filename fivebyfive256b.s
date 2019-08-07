; da65 V2.17 - Git 7e694295
; Created:    2019-08-03 22:52:43
; Input file: fivebyfive256b.prg
; Page:       1


        .setcpu "6502X"

L0033           := $0033
L087C           := $087C
L0900           := $0900
L0969           := $0969
L097B           := $097B
L9587           := $9587
        ora     ($08,x)
        anc     #$08
        asl     a
        brk
        shx     $3032,y
        sec
        bmi     LFE0D
LFE0D:  brk
        brk
        brk
        brk
        brk
        brk
        brk
        jam
        brk
        brk
        brk
        brk
        brk
        brk
        brk
        brk
        brk
        brk
        brk
        brk
        brk
        sei
        lda     $0810
        sta     $D01C
        lda     $0811
        sta     $D020
        sta     $D021
        lda     $0813
        sta     $D025
        lda     $0814
        sta     $D026
        lda     $0812
        ldx     #$07
LFE43:  sta     $D027,x
        dex
        bpl     LFE43
        ldx     #$00
        lda     #$20
LFE4D:  sta     $0400,x
        sta     $0500,x
        sta     $0600,x
        sta     $0700,x
        inx
        bne     LFE4D
        lda     #$FF
        sta     $D015
        lda     #$80
        sta     $D010
        ldx     #$07
        ldy     #$00
        lda     #$44
LFE6C:  sta     $D000,y
        clc
        adc     #$1D
        iny
        iny
        dex
        bpl     LFE6C
        jsr     L0900
        lda     #$35
        sta     $01
        lda     #$2F
        sta     $08C4
        lda     #$80
        sta     $08C5
LFE88:  lda     $08C4
LFE8B:  cmp     $D012
        bne     LFE8B
        clc
        adc     #$04
        ldx     #$07
        ldy     #$00
LFE97:  sta     $D001,y
        iny
        iny
        dex
        bpl     LFE97
        ldx     #$00
        ldy     $08C5
LFEA4:  tya
        sta     $07F8,x
        iny
        inx
        cpx     #$08
        bne     LFEA4
        clc
        lda     $08C5
        adc     #$08
        sta     $08C5
        lda     $08C4
        adc     #$19
        sta     $08C4
        cmp     #$F7
        bne     LFE88
        jmp     L087C

        brk
        brk
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     LFFFF,x
        isc     $20A9,x
        sta     $AD
        asl     a
        sta     $AE
        lda     #$00
        sta     $FB
        sta     $FC
LFF0F:  sta     $AC
        lda     #$04
        sta     $FD
LFF15:  ldy     #$05
LFF17:  clc
        beq     LFF31
        dec     $FC
        bpl     LFF2F
        lda     #$07
        sta     $FC
        inc     $FB
        ldx     $FB
        cpx     #$75
        bcs     LFF2F
        lda     $0989,x
        sta     $AF
LFF2F:  asl     $AF
LFF31:  php
        php
        php
LFF34:  clc
        php
        ldx     #$03
LFF38:  plp
        rol     $64
        rol     $63
        rol     $62
        dex
        bpl     LFF38
        dey
        bpl     LFF17
        jsr     L0969
        lda     $AC
        clc
        adc     #$0C
        sta     $AC
        dec     $FD
        bpl     LFF15
        lda     #$00
        tay
        sta     ($AC),y
        iny
        sta     ($AC),y
        iny
        sta     ($AC),y
        lda     $AC
        adc     #$04
        bcc     LFF66
        inc     $AD
LFF66:  dec     $AE
        bne     LFF0F
        rts

        lda     $62
        ldy     #$00
        jsr     L097B
        lda     $63
        ldy     #$01
        jsr     L097B
        lda     $64
        ldy     #$02
        ldx     #$02
LFF7F:  sta     ($AC),y
        iny
        iny
        iny
        dex
        bpl     LFF7F
        lda     #$00
        sta     ($AC),y
        rts

        nop     $7F,x
        clc
        nop
        rol     $9D8F,x
        clc
        eor     $DE
        sty     $EF63
        dcp     ($D0,x)
        isc     $E8E1,x
        eor     ($D0,x)
        ldy     $185D,x
        inc     $7131,x
        php
        sre     $04
        and     ($8B,x)
        lax     ($2E,x)
        lsr     a
        bmi     LFF34
        and     ($F8,x)
        inc     $8CB1
        rra     ($59),y
        cmp     $D1
        sty     $E85D
        nop
        bpl     L0033
        rra     ($26,x)
        nop
        rol     $5F94,x
        slo     $07
        dcp     $0821,x
        pha
        dec     $31
        nop     $63,x
        ora     $12,x
        and     ($AE),y
        isc     ($15,x)
        ora     ($51),y
        txa
        dey
        jam
        sei
        jmp     L9587

        sbc     $7D,x
        lsr     $729D
        nop     #$30
        sty     $77
        nop     $C8
        rra     $6082,y
        lda     $7829,y
        jam
        nop     $38,x
        rol     $1C3A
        ahx     ($3C),y
        jam
        jam
        asl     $A28B
        isc     $45
        .byte   $E1
LFFFF:  .byte   $70
