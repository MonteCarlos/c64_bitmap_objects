.export fillGarbage
.proc fillGarbage
    ldy #$0f
fill1:
    ldx #0
fill2:
src = *+1
    lda $e000,x
dest = *+1
    sta $2000,x
    inx
    bne fill2

    inc src+1
    inc dest+1

    dey
    bpl fill1
    rts
.endproc
; 24 bytes
