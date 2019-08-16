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
    ;sax ptr1
    sax ptr2
    ;lda #>(charset+(spriteCount-1)*8)
    ;sta ptr1+1
    lda #>(sprites+(spriteCount-1)*64)
    sta ptr2+1

gensprite0:
    lda #<(charset+'9'*8)+7; (spriteCount-1)
    sta ByteOfCharMatrixIdx

    lda #<(sprites+(spriteCount-1)*64)+62
    sta ByteOfSpriteMatrixIdx

gensprite:
    ldy ByteOfCharMatrixIdx
    ;lda ptr1+1
    lda src+1
    cmp #>(charset+'9'*8)
    bcc :+
    cpy #<(charset+'0'*8)
    bcs :+
    ldy #<(charset+('z'*8))+7
    sty ByteOfCharMatrixIdx
    ;dec ptr1+1
    dec src+1
:
src = *+1
    lda 256*>(charset+(spriteCount-1)*8), y
    ;lda (ptr1),y ;fetch byte from char matrix
    pha

    ldx #7
    axs #0 ;puts lower 3 bits of a into x
    cpx #4;if bit 3 is set, then also set bits 4-7
    bcc :+
    axs #<($100-$f8) ;set upper 4 bits
:
    ;a still preserved here
    and #$f0
    asl
    bit bitmask
    beq :+
    ora #$1f
:
    tay
    pla
    and #%00001000
    beq :+

    txa
    ora #%11010000
    tax
    tya
    ora #%00001011
    tay

    lda #$0f
:   stx lo
    sty hi+1

    asl lo
    rol
    asl lo
    rol
    asl lo
    rol
    asl lo
    rol
    sta hi

    lda ByteOfCharMatrixIdx
    and #7
    tay
    ldx reptable,y
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

    lda #$cf
    dcp src+1
    bcs finished

    lda #0
:   dec ByteOfCharMatrixIdx
    and #7
    jne gensprite

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
    jne gensprite
finished:
    rts
bitmask:
    .byte $20
reptable:
    .byte 0,1,2,2,2,1,0,0


.endproc

.SEGMENT "SETTINGS"
.export multicol = *
    .byte   $00

.export bgcol = *
    .byte   $06

.export sprcol = *
    .byte   $0f

.export sprmcol1 = *
    .byte   $01

.export sprmcol2 = *
    .byte   $0d
