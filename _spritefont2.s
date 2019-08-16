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
    ldx charnum

    lda (ptr1),y ;fetch byte from char matrix
    pha
    ;cpx #('1' )
    ;beq correct
    ;cpx #('j' & $3f)
    ;beq correct
    cpx #'j'&$3f
    bne :+
    cmp #0
    beq :+
    bit bitmask
    bne :+
    eor #%00001010
:
    cpx #'k'&$3f
    bcc :+
    cpx #'k'&$2f+1
    bcs :+
    lda k_matrix-(('k' & $3f)*8),y
:
    cpx #'m'&$3f
    bcc :+
    cpx #'o'&$3f
    bcs :+
    lda m_matrix-(('m' & $3f)*8),y
:
    ;lsr

    ldy #0
    ldx #7
    axs #0 ;puts lower 4 bits of a into x
    cpx #4;if bit 3 is set, then also set bits 4-7
    bcc :+
    ldy #$fe
    axs #$08 ;set upper 4 bits
:
    stx lo
    and #$e0
    bit bitmask
    beq :+
    iny
    ora #$1f
:
    sta hi+1
    pla
    and #%00011000
    tay
    ;tya
    ;;cmp #$01
    ;bne :+
    ;ldy #$f0
;:   ;cmp #$fe
    ;bne :+
    ;ldy #$0f
;:
    ldx charnum
    cpx #'i'&$3f
    bne :+
    lda hi+1
    cmp #%00011111
    bne :+
    ldy #%00011000
:

    sty hi

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
    ;pha
    lda #$cf
    dcp ptr1+1
    ;pla
    bcs finished
    lda #0
:   dec ByteOfCharMatrixIdx
    and #7
    jne gensprite
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
    dec charnum

    ;lda ByteOfCharMatrixIdx
    ;cmp #$08
    jpl gensprite
finished:
    rts
bitmask:
    .byte $20
bitmask2:
    .byte $30
reptable:
    .byte 0,0,4,0,4,0,0,0
charnum:
    .byte 36
i_matrix:
    .byte %11111111
    .byte %01100000
    .byte %01100000
    .byte %01100000
    .byte %01100000
    .byte %01100000
    .byte %11111111
    .byte 0
j_matrix:
    .byte %11111111
    .byte %00011000
    .byte %00011000
    .byte %00011000
    .byte %11101000
    .byte %11111000
    .byte %01111000
    .byte 0
k_matrix:
    .byte %00100111
    .byte %00111111
    .byte %00110000
    .byte %00100000
    .byte %00110000
    .byte %00111111
    .byte %00100111
    .byte 0
m_matrix:
    .byte %11100100
    .byte %11100110
    .byte %01111110
    .byte %01111110
    .byte %01100110
    .byte %01100110
    .byte %01100110
    .byte 0
n_matrix:
    .byte %11111100
    .byte %11100110
    .byte %01100110
    .byte %01100110
    .byte %01100110
    .byte %01100110
    .byte %01100110
    .byte 0

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
