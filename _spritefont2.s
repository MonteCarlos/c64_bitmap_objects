.include "cc65runtime.sh"
.include "myMacros.sh"

spriteCount = 64
charset = $d000
sprites = $2000

.proc _generatefont
.export _generatefont
.export generatefont = _generatefont
.import _loBitsTable, _hiBitsTable

lo = tmp1
hi = tmp2
ByteOfCharMatrixIdx = 2
ByteOfSpriteMatrixIdx = 3

    lda #$33
    sta 1

    lax #0
    sax ptr1
    sax ptr2
    lda #>(charset+(spriteCount-1)*8)
    sta ptr1+1
    lda #>(sprites+(spriteCount-1)*64)
    sta ptr2+1
gensprite0:
    ;lda #6
    lda #<(charset+(spriteCount-1)*8)+6
    sta ByteOfCharMatrixIdx
    ;lda #9*6+8
    lda #<(sprites+(spriteCount-1)*64)+9*6+8
    sta ByteOfSpriteMatrixIdx
gensprite:
    lax #0
    sax hi+1

    ldx #7
    lda ByteOfCharMatrixIdx
    axs #7
    beq nextCharLine
    tay
    lda (ptr1),y ;fetch byte from char matrix

    ldx #15
    axs #0 ;puts lower 4 bits of a into x
    jsr expand
    ldy ByteOfSpriteMatrixIdx
    ldx #2
setsprite:
    lda lo
    lsr
    lsr
    ora lo
    ;and #%10110110
    sta (ptr2),y
    dey
    lda hi
    sta (ptr2),y
    dey
    lda hi+1
    sta (ptr2),y
    dey
    tya
    bne :+
    dec ptr2+1
:
    and #$3f
    bne :+
    dey
:
    dex
    bpl setsprite

    dey
    sty ByteOfSpriteMatrixIdx
nextCharLine:
    lda #$ff
    dcp ByteOfCharMatrixIdx
    bne gensprite

    lda #$cf
    dcp ptr1+1
    bne gensprite0
    rts

expand:
    ldy _hiBitsTable,x
    sty hi
    ldy _loBitsTable,x
    sty lo
    lsr
    lsr
    lsr
    lsr
    tax
    lda _loBitsTable,x
    asl
    rol hi+1
    asl
    rol hi+1
    asl
    rol hi+1
    asl
    rol hi+1
    ora hi
    ;and #%01101101
    sta hi
    lda _hiBitsTable,x
    asl
    asl
    asl
    asl
    ora hi+1
    asl
    asl
    asl
    ora hi+1
    ;and #%11011011
    sta hi+1
    rts


.endproc

.SEGMENT "SETTINGS"
.export multicol = *
    .byte   $ff

.export bgcol = *
    .byte   $0b

.export sprcol = *
    .byte   $07

.export sprmcol1 = *
    .byte   $0a

.export sprmcol2 = *
    .byte   $08
