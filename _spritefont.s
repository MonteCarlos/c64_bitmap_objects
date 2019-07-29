.include "cc65runtime.sh"
.include "myMacros.sh"

.import _loBitsTable, _hiBitsTable
    lda #<$d000
    sta ptr1
    lda #>$d000
    sta ptr1+1

    lda #<$2000
    sta ptr2
    lda #>$2000
    sta ptr2+1

    lda #0
    sta hi+1

    lda #7
    sta ByteOfCharMatrixIdx
    asl
    adc ByteOfCharMatrixIdx
    sta ByteOfSpriteMatrixIdx
gensprite:
    ldy ByteOfCharMatrixIdx
    lda (ptr1),y ;fetch byte from char matrix

    ldx #15
    axs #0 ;puts lower 4 bits of a into x
    ldy _loBitsTable,x
    sty lo
    ldy _hiBitsTable,x
    sty hi
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
    sta hi
    lda _hiBitsTable,x
    asl
    asl
    asl
    asl
    ora hi+1
    sta hi+1

    lda hi+1
    ldy ByteOfSpriteMatrixIdx
    sta (ptr2),y
    iny
    lda hi
    sta (ptr2),y
    iny
    lda lo
    sta (ptr2),y
    tya
    sec
    sbc #9
    sta ByteOfSpriteMatrixIdx

    dec ByteOfCharMatrixIdx
    bpl gensprite
    rts
ByteOfCharMatrixIdx:
    .res 1
ByteOfSpriteMatrixIdx:
    .res 1
lo:
    .res 1
hi:
    .res 2

