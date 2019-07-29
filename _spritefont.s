
    ldx #15
    axs #0 ;puts lower 4 bits of a into x
    ldy loBytesTable,x
    sty lo
    ldy hiBytesTable,x
    sty hi
    lsr
    lsr
    lsr
    lsr
    tax
    lda loBytesTable,x
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
    lda hiBytesTable,x
    asl
    asl
    asl
    asl
    ora hi+1
    sta hi+1
    rts

