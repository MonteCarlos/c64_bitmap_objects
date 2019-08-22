
; -------------------------------------------------------------------------
;
; Compo entry by Monte Carlos for the 128 Byte Font Compo on CSDB, Mar 2018
;
; -------------------------------------------------------------------------
;
; Welcome to the 128b font compo!
; The goal is to create the most legible, stylish, and/or awesome font using an
; executable program no larger than 128 bytes.
; The assumption is that the ROM font will be used as a basis somehow,
; although this is not a formal requirement.
; The deadline is end of April 8, 2018, in a time zone of your choice.
; The compo is held at CSDb, and the result is determined by
; the weighted average rating (including non-public votes).
; The ratings will be sampled one week after the deadline, end of April 15, in a time zone of my choice.
; Judges are asked to consider screen codes 00-3f only.
; This comprises the uppercase letters, digits, and punctuation.
; The rules do not mandate that reverse-video characters work, so it is all right if the cursor doesn't work properly.
; I reserve the right to modify the rules if there is a good reason.
; Technical requirements
; Maximum file size including load address is 128 bytes.
; The program must load to the default location ($801), and must be executable with RUN.
; The new font should be installed at address $3800, after which the program should return to the BASIC prompt,
; with the VIC bank register ($d018) updated to point to the new font.
; The entry should be uploaded in .prg format.
; A person may be creatively involved in at most two (2) compo entries.
; (Idea/test/help credits don't count towards the limit.)
; It's alright to withdraw an earlier entry in order to submit a new one, up to the deadline.

; Use a3*120+a2*28+a1*6+a0*1 to setup table (difference to values from 0 to 15)
shower:

tmp1    = 2

zoom   = $f0 ; Use $f7, because count table needs 9 entries
            ; (from count for 0 to count for 8 bits)


.org                $07ff
            ; Start address
            .word   $801

            ; Endaddress of basic line and line no
            .word   $80b,   1

            ; sys 2061 and terminators
            .byte   $9e,"2061", 0,  0,  0

            sei

            ; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            ; Set-up a table for expanding nibbles
            ; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            lsr     $01                     ; set memory config for accessing charset
            ldx     #15
setupTbl:
            stx     tmp1

            ldy     #3                      ; process four bits
calc:
            lsr                             ; insert a blank bit and the rightmost bit of the nibble
            lsr     tmp1                    ; currently processed
            ror
            dey
            bpl     calc
            sta     zoom,x                  ; store expanded nibble into zoom table
            dex
            bpl     setupTbl
            ; in total the code to set-up the table is one byte smaller than the table would be
            ; if inserted as .byte list. However, there is more saving because the table is set-up
            ; in the zeropage


            ; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            ; iterate through charset
            ; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
loop:
            ldy     #127                      ; counter for the 8 bytes of the charset
            ; process a charblock
convert:
src:
            lda     $d180,y
            pha
            and     #15                     ; isolate low nibble
            tax

            tya                             ; mul index by two to address byte in dest charset
            asl                             ; which is expanded both in x and in y
            tay

            ;c = 0
            jsr     storeb                  ; get expanded nibble, modify and store in dest charset
            dey
            pla                             ; isolate high nibble
            lsr
            lsr
            lsr
            lsr
            tax
            dec     dest+2
            jsr     storeb                  ; get expanded nibble, modify and store in dest charset
            inc     dest+2

            tya
            lsr
            tay

            dey
            bpl     convert                 ; loop until end of char

            lax     src+1                   ; proceed src and dest ptrs
            axs     #$80                    ; use axs to save clc
            stx     src+1
            bcs     :+
            dec     src+2
:
            dec     dest+2
            lda     #$37
            dcp     dest+2                  ; use dcp instead of dec ...; lda ...; cmp #..
            bcc     loop

            sta     1                       ; just use lucky fact that >startadr of dest minus 1
            lda     #$1e                    ; equals $01 default value
            sta     $d018

        .ifdef shower
            jsr _shower
        .endif

;- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

storeb:
            lda     zoom,x
            ;rol
            ;ora     zoom,x
ok:
            jsr     dest
            iny
dest:
            sta     $3f00,y
            rts

; ----------------------------------------------------------------
        .ifdef  shower
_shower:
            lda     #0
            sta     $d020
            sta     $d021

            ;lda     #$3b
            ;sta     $d011
            lda     #$20
            sta     tmp1

            ldx     #$27
        fs:
            txa
            lsr
            php
            asl
            plp
            bcc     :+
            adc     #$1f
        :
            cpx     #$20
            bcc     :+
            adc     #$1f
        :
            sta     $0400,x
            ora     #1
            sta     $0428,x
            cpx     #$18
            bcc     :+
            cpx     #$20
            bcs     :+
            adc     #$20
        :   clc
            adc     #$47
            sta     $0450,x
            ora     #1
            sta     $0478,x
            cpx     #$10
            bcc     :+
            cpx     #$18
            bcs     :+
            adc     #$20
        :   clc
            adc     #$47
            sta     $04a0,x
            ora     #1
            sta     $04c8,x
            adc     #$47
            sta     $04f0,x
            ora     #1
            sta     $0518,x

            lda     #$05+8
            sta     $d800,x
            sta     $d828,x
            sta     $d850,x
            sta     $d878,x
            sta     $d8a0,x
            sta     $d8c8,x
            sta     $d8f0,x
            sta     $d918,x

            dex
            bpl     fs

            lda     #$18
            sta     $d016

            lda     #$09
            sta     $d022
            lda     #$0d
            sta     $d023

            rts
        .endif

