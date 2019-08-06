.include "cc65runtime.sh"
.include "myMacros.sh"

spriteCount = 64
charset = $d000
sprites = $2000

.proc _generatefont
.export _generatefont
.export generatefont = _generatefont
.import _loBitsTable, _hiBitsTable
    ;sei
    lda 1
    pha
    lda #$33
    sta 1

    lda #0
    sta ptr1
    sta ptr2
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
    lda #0
    sta hi+1

    ldy ByteOfCharMatrixIdx
    ;lda ptr1+1
    ;cmp #>(charset+','*8)
    ;bne :+
    ;lda ptr1
    ;cpy #<(charset+','*8)
    ;bne :+
    ;iny
;:
    lda (ptr1),y ;fetch byte from char matrix

    ldx #15
    axs #0 ;puts lower 4 bits of a into x
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
    and #%01101101
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
    and #%11011011
    sta hi+1

    ldy ByteOfSpriteMatrixIdx
    ldx #2
:
    lda lo
    lsr
    lsr
    ora lo
    and #%10110110
    sta (ptr2),y
    dey
    lda hi
    sta (ptr2),y
    dey
    lda hi+1
    sta (ptr2),y
    dey
    dex
    bne :-
    txa
    sta (ptr2),y
    dey
    sta (ptr2),y
    dey
    sta (ptr2),y
    tya
    bne :+
    dec ptr2+1
:
    and #$3f
    bne :+
    dey
:
    dey
    sty ByteOfSpriteMatrixIdx

    ;lda ByteOfCharMatrixIdx
    ;and #$f8
    ;sec
    ;sbc #1
    lda #$ff
    dcp ByteOfCharMatrixIdx
    jne gensprite
 ;   dec ptr1+1

    ;lax ptr1
    ;axs #8
    ;stx ptr1
    ;bcs :+
    ;dec ptr1+1
;:
    ;lax ptr2
    ;axs #$40
    ;stx ptr2
    ;bcc :+
;jmpgensprite0
;    jmp gensprite0
;:
    lda #$d0
    dcp ptr1+1
    jne gensprite0
    pla
    sta 1
    ;cli
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

