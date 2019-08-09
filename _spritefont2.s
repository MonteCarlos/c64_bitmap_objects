.include "cc65runtime.sh"
.include "myMacros.sh"

spriteCount = 37
charset = $d000
sprites = $2000

.proc _generatefont
.export _generatefont
.export generatefont = _generatefont
.import _loBitsTable, _hiBitsTable

lo = tmp2
hi = tmp3

ByteOfCharMatrixIdx = 2
ByteOfSpriteMatrixIdx = 3

    lsr $01

    lax #0
    sax ptr1
    sax ptr2
    lda #>(charset+(spriteCount-1)*8)
    sta ptr1+1
    lda #>(sprites+(spriteCount-1)*64)
    sta ptr2+1
gensprite0:
    ;lda #6
    lda #<(charset+'9'*8)+7; (spriteCount-1)
    sta ByteOfCharMatrixIdx
    ;lda #9*6+8
    lda #<(sprites+(spriteCount-1)*64)+62
    sta ByteOfSpriteMatrixIdx
gensprite:
    ldy ByteOfCharMatrixIdx
    lda ptr1+1
    cmp #>(charset+'9'*8)
    bcc :+
    cpy #<(charset+'0'*8)
    bcs :+
    ldy #<(charset+('z'*8))+7
    ;lda #>(charset+(27*8))
    ;sta ptr1+1
    sty ByteOfCharMatrixIdx
    ;sta ptr1+1
    dec ptr1+1
    :
    lda (ptr1),y ;fetch byte from char matrix
    ldy #0
    ldx #15
    axs #0 ;puts lower 4 bits of a into x
    cpx #8 ;if bit 3 is set, then also set bits 4-7
    bcc :+
    ldy #$ff
    axs #$10 ;set upper 4 bits
:
    stx lo
    sty hi
    and #$f0
    bit bitmask
    beq :+
    ora #$0f
:
    sta hi+1

    ;ldx #0
    lda ByteOfCharMatrixIdx
    cmp #<(charset+('k'*8))
    bcc :+
    cmp #<(charset+('k'*8))+8
    bcs :+
    ldx #2
    bne setsprite0
:
    and #7
    tay
    ldx reptable,y
    ;cmp #4
    ;bne :+
    ;ldx #6
;:   ;cmp #2
    ;bne :+
    ;ldx #3
;:
setsprite0:
    ldy ByteOfSpriteMatrixIdx
setsprite:

    lda lo
    sta (ptr2),y
    dey
    lda hi
    sta (ptr2),y
    dey
    lda hi+1
    sta (ptr2),y
    dey
    dex
    bpl setsprite

    sty ByteOfSpriteMatrixIdx

    lda ByteOfCharMatrixIdx
    bne :+
    ;pha
    lda #$cf
    dcp ptr1+1
    ;pla
    bcs finished
    lda #0
:   dec ByteOfCharMatrixIdx
    and #7
    bne gensprite
nextpage:
    tya
    and #$3f
    tax
    lda #0
fillrest:
    sta (ptr2),y
    dey
    dex
    bpl fillrest
    dey

    cpy #$fe
    bne :+
    dec ptr2+1
:
    sty ByteOfSpriteMatrixIdx

    ;lda ByteOfCharMatrixIdx
    ;cmp #$08
    bne gensprite
finished:
    rts
bitmask:
    .byte $10
reptable:
    .byte 0,0,0,0,3,3,0,0
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
