.include "cc65runtime.sh"
.include "myMacros.sh"

spriteCount = 64
charset = $d000
sprites = $2000

.proc _generatefont
.export _generatefont
.export generatefont = _generatefont
.import _loBitsTable, _hiBitsTable

bitmask = tmp1
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
    lda #<(charset+(spriteCount-1)*8)+7
    sta ByteOfCharMatrixIdx
    ;lda #9*6+8
    lda #<(sprites+(spriteCount-1)*64)+62
    sta ByteOfSpriteMatrixIdx
gensprite:

    ldy ByteOfCharMatrixIdx

    lda (ptr1),y ;fetch byte from char matrix

    ldx #16
    stx bitmask
    dex
    axs #0 ;puts lower 4 bits of a into x
    cpx #8 ;if bit 3 is set, then also set bits 4-7
    bcc :+
    axs #$10 ;set upper 4 bits
:
    and #$f0
    bit bitmask
    beq :+
    ora #$0f
:
    pha

    ldy ByteOfSpriteMatrixIdx

setsprite:

    txa
    sta (ptr2),y
    dey
    lda #0
    cpx #8
    bcc :+
    lda #$ff
:   sta (ptr2),y
    ;lda hi
    ;sta (ptr2),y
    dey
    pla
    sta (ptr2),y
    ;lda hi+1
    ;sta (ptr2),y
    ;dey
    dey
    sty ByteOfSpriteMatrixIdx

    ;ldx #$f8
    lda ByteOfCharMatrixIdx
    ;axs #$01
    ;txa
    and #$f8
    dcp ByteOfCharMatrixIdx
    beq gensprite
    bcc gensprite

    tya
    sec
    sbc #64-(8*3)
    bcs :+
    dec ptr2+1
:
    sta ByteOfSpriteMatrixIdx

    lda ByteOfCharMatrixIdx
    cmp #$08
    bcs gensprite
    ;lda #$cf
    ;dcp ptr1+1
    ;bne gensprite0
    rts


.endproc

.SEGMENT "SETTINGS"
.export multicol = *
    .byte   $00

.export bgcol = *
    .byte   $0b

.export sprcol = *
    .byte   $07

.export sprmcol1 = *
    .byte   $0a

.export sprmcol2 = *
    .byte   $08
