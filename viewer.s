.import generatefont
.autoimport on

.SEGMENT "LOADADDR"
    .addr *+2

.SEGMENT "EXEHDR"
    .byte $0b, $08, $10, $03, $9e, $32, $30, $36, $31, $00, $00, $00


.SEGMENT "DISPLAYER"
    .export _viewer
    .proc   _viewer
        sei
        lda     multicol
        sta     $d01c
        lda     bgcol
        sta     $d020
        sta     $d021
        lda     sprmcol1
        sta     $d025
        lda     sprmcol2
        sta     $d026
        lda     sprcol
        ldx     #$07
    setsprcolor:
        sta     $d027,x
        dex
        bpl     setsprcolor

        ldx     #$00
        lda     #$20
    clrscrn:
        sta     $0400,x
        sta     $0500,x
        sta     $0600,x
        sta     $0700,x
        inx
        bne     clrscrn

        lda     #$ff
        sta     $d015
        lda     #$80
        sta     $d010

        ldx     #$07
        ldy     #$00
        lda     #$18+44
    setsprcoord:
        sta     $d000,y
        clc
        adc     #29
        iny
        iny
        dex
        bpl     setsprcoord

        jsr     generatefont

        lda     #$35
        sta     $01
mainloop:
        lda     #$2f
        sta     ycoord
        lda     #$80
        sta     spridx
    yloop:
        lda     ycoord
    waitrasterline:
        cmp     $d012
        bne     waitrasterline
        clc
        adc     #4
        ldx     #$07
        ldy     #$00
    updatesprycoord:
        sta     $d001,y
        iny
        iny
        dex
        bpl     updatesprycoord

        ldx     #$00
        ldy     spridx
    updatesprpointers:
        tya
        sta     $07f8,x
        iny
        inx
        cpx     #8
        bne     updatesprpointers
        clc
        lda     spridx
        adc     #8
        sta     spridx
        lda     ycoord
        adc     #25
        sta     ycoord
        cmp     #$2f+8*25
        bcc     yloop
        jmp     mainloop

ycoord:
        .byte   0
spridx:
        .byte   0
    .endproc



