.include "cc65runtime.sh"
.include "myMacros.sh"

.proc _generateSprites
.export _generateSprites
.import _loBitsTable, _hiBitsTable
    sei
    lda 1
    pha
    lda #$33
    sta 1

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

    lda #6
    sta ByteOfCharMatrixIdx
    asl
    asl
    asl
    adc ByteOfCharMatrixIdx ;x9
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

    ldy ByteOfSpriteMatrixIdx
    ldx #8
:
    lda hi+1
    sta (ptr2),y
    iny
    lda hi
    sta (ptr2),y
    iny
    lda lo
    sta (ptr2),y
    iny
    dex
    bpl :-
    tya
    sec
    sbc #18
    sta ByteOfSpriteMatrixIdx

    dec ByteOfCharMatrixIdx
    bpl gensprite
    pla
    sta 1
    cli
    rts
ByteOfCharMatrixIdx:
    .res 1
ByteOfSpriteMatrixIdx:
    .res 1
lo:
    .res 1
hi:
    .res 2
.endproc

