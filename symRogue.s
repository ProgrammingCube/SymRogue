;
; File generated by cc65 v 2.19
;
	.fopt		compiler,"cc65 v 2.19"
	.setcpu		"65C02"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	off
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.forceimport	__STARTUP__
	.import		_putchar
	.import		_getchar
	.import		_memset
	.import		_rand
	.export		__puts
	.export		_map
	.export		_k_input
	.export		_plyr_pos
	.export		_temp_pos
	.export		_mons_xy
	.export		_mons_hp
	.export		_mons_ch
	.export		_parsInpt
	.export		_updtPlyr
	.export		_updtMons
	.export		_monsCmbt
	.export		_plyrCmbt
	.export		_printMap
	.export		_main

.segment	"DATA"

_k_input:
	.byte	$00

.segment	"RODATA"

S0001:
	.byte	$1B,$5B,$32,$4A,$00
S0002:
	.byte	$1B,$5B,$48,$00

.segment	"BSS"

_map:
	.res	160,$00
_plyr_pos:
	.res	1,$00
_temp_pos:
	.res	1,$00
_mons_xy:
	.res	3,$00
_mons_hp:
	.res	3,$00
_mons_ch:
	.res	3,$00

; ---------------------------------------------------------------
; void __near__ _puts (char *s)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	__puts: near

.segment	"CODE"

	jsr     pushax
	lda     regbank+4
	ldx     regbank+5
	jsr     pushax
	ldy     #$03
	lda     (sp),y
	sta     regbank+4+1
	dey
	lda     (sp),y
	sta     regbank+4
	bra     L0004
L0007:	lda     (regbank+4),y
	inc     regbank+4
	bne     L0005
	inc     regbank+4+1
L0005:	jsr     _putchar
L0004:	ldy     #$00
	lda     (regbank+4)
	bne     L0007
	lda     (sp)
	sta     regbank+4
	iny
	lda     (sp),y
	sta     regbank+5
	jmp     incsp4

.endproc

; ---------------------------------------------------------------
; void __near__ parsInpt (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_parsInpt: near

.segment	"CODE"

	lda     _plyr_pos
	sta     _temp_pos
	lda     _k_input
	cmp     #$61
	beq     L0009
	cmp     #$64
	beq     L000B
	cmp     #$73
	beq     L000A
	cmp     #$77
	bne     L0003
	lda     _temp_pos
	sec
	sbc     #$10
	sta     _temp_pos
	rts
L0009:	dec     _temp_pos
	rts
L000A:	lda     #$10
	clc
	adc     _temp_pos
	sta     _temp_pos
	rts
L000B:	inc     _temp_pos
	rts
L0003:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ updtPlyr (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_updtPlyr: near

.segment	"CODE"

	ldy     _plyr_pos
	lda     #$2E
	sta     _map,y
	ldy     _temp_pos
	lda     _map,y
	cmp     #$23
	bne     L000D
	lda     _plyr_pos
	sta     _temp_pos
L000D:	stz     M0001
L000E:	lda     M0001
	cmp     #$03
	bcs     L0006
	ldy     _temp_pos
	ldx     #$00
	lda     _map,y
	ldy     M0001
	cmp     _mons_ch,y
	bne     L000F
	jsr     _plyrCmbt
	lda     _plyr_pos
	sta     _temp_pos
L000F:	inc     M0001
	bra     L000E
L0006:	ldy     _temp_pos
	lda     #$40
	sta     _map,y
	lda     _temp_pos
	sta     _plyr_pos
	rts

.segment	"BSS"

M0001:
	.res	1,$00

.endproc

; ---------------------------------------------------------------
; void __near__ updtMons (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_updtMons: near

.segment	"CODE"

	stz     M0001
L001D:	lda     M0001
	cmp     #$03
	jcs     L0023
	jsr     _rand
	and     #$03
	sta     M0002
	ldy     M0001
	lda     _mons_xy,y
	sta     M0003
	lda     M0002
	beq     L001E
	cmp     #$01
	beq     L001F
	cmp     #$02
	beq     L0020
	cmp     #$03
	beq     L0021
	bra     L0008
L001E:	lda     M0003
	sec
	sbc     #$10
	sta     M0003
	bra     L0008
L001F:	dec     M0003
	bra     L0008
L0020:	lda     #$10
	clc
	adc     M0003
	sta     M0003
	bra     L0008
L0021:	inc     M0003
L0008:	ldy     M0003
	lda     _map,y
	cmp     #$23
	bne     L0022
	ldy     M0001
	lda     _mons_xy,y
	sta     M0003
L0022:	lda     M0003
	cmp     _plyr_pos
	bne     L0011
	ldx     #$00
	lda     M0001
	jsr     _monsCmbt
	ldy     M0001
	lda     _mons_xy,y
	sta     M0003
L0011:	ldy     M0001
	lda     _mons_xy,y
	clc
	adc     #<(_map)
	sta     ptr1
	lda     #$00
	adc     #>(_map)
	sta     ptr1+1
	lda     #$2E
	sta     (ptr1)
	ldy     M0001
	lda     M0003
	sta     _mons_xy,y
	ldy     M0001
	lda     _mons_xy,y
	clc
	adc     #<(_map)
	sta     ptr1
	lda     #$00
	adc     #>(_map)
	sta     ptr1+1
	ldy     M0001
	lda     _mons_ch,y
	sta     (ptr1)
	inc     M0001
	jmp     L001D
L0023:	stz     M0001
L0024:	lda     M0001
	cmp     #$03
	bcs     L0018
	ldy     M0001
	lda     _mons_xy,y
	clc
	adc     #<(_map)
	sta     ptr1
	lda     #$00
	adc     #>(_map)
	sta     ptr1+1
	ldy     M0001
	lda     _mons_ch,y
	sta     (ptr1)
	inc     M0001
	bra     L0024
L0018:	rts

.segment	"BSS"

M0001:
	.res	1,$00
M0002:
	.res	1,$00
M0003:
	.res	1,$00

.endproc

; ---------------------------------------------------------------
; void __near__ monsCmbt (unsigned char i)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_monsCmbt: near

.segment	"CODE"

	jsr     pusha
	jmp     incsp1

.endproc

; ---------------------------------------------------------------
; void __near__ plyrCmbt (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_plyrCmbt: near

.segment	"CODE"

	rts

.endproc

; ---------------------------------------------------------------
; void __near__ printMap (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_printMap: near

.segment	"CODE"

	stz     M0001
L0009:	lda     M0001
	cmp     #$A0
	bcs     L0003
	ldy     M0001
	lda     _map,y
	jsr     _putchar
	lda     M0001
	ina
	and     #$0F
	bne     L000B
	lda     #$0A
	jsr     _putchar
	lda     #$0D
	jsr     _putchar
L000B:	inc     M0001
	bra     L0009
L0003:	rts

.segment	"BSS"

M0001:
	.res	1,$00

.endproc

; ---------------------------------------------------------------
; void __near__ main (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_main: near

.segment	"CODE"

	ldy     #$00
	lda     #$23
L0002:	sta     _map,y
	iny
	cpy     #$A0
	bne     L0002
	lda     #$11
L0018:	sta     M0001
	cmp     #$90
	bcs     L0004
	lda     #<(_map)
	ldx     #>(_map)
	clc
	adc     M0001
	bcc     L0007
	inx
L0007:	sta     ptr1
	stx     ptr1+1
	ldy     #$0D
	lda     #$2E
L0008:	sta     (ptr1),y
	dey
	bpl     L0008
	lda     #$10
	clc
	adc     M0001
	bra     L0018
L0004:	lda     #<(S0001)
	ldx     #>(S0001)
	jsr     __puts
	lda     #$19
	sta     _plyr_pos
	stz     M0001
	ldx     #$00
L0019:	lda     M0001
	cmp     #$03
	bcs     L000A
L000D:	lda     #<(_mons_xy)
	ldx     #>(_mons_xy)
	clc
	adc     M0001
	bcc     L0010
	inx
L0010:	jsr     pushax
	jsr     _rand
	jsr     pushax
	ldx     #$00
	lda     #$10
	jsr     tosmoda0
	jsr     mulax10
	ldy     #$00
	jsr     staspidx
	ldy     M0001
	lda     _mons_xy,y
	sta     ptr1
	clc
	lda     #>(_map)
	sta     ptr1+1
	ldy     #<(_map)
	lda     (ptr1),y
	cmp     #$2E
	bne     L000D
	ldy     M0001
	lda     #$4D
	sta     _mons_ch,y
	ldy     M0001
	lda     _mons_xy,y
	clc
	adc     #<(_map)
	sta     ptr1
	lda     #$00
	adc     #>(_map)
	sta     ptr1+1
	ldy     M0001
	lda     _mons_ch,y
	sta     (ptr1)
	ldx     #$00
	inc     M0001
	bra     L0019
L000A:	jsr     _parsInpt
	jsr     _updtPlyr
	jsr     _updtMons
	lda     #<(S0002)
	ldx     #>(S0002)
	jsr     __puts
	jsr     _printMap
	jsr     _getchar
	sta     _k_input
	ldx     #$00
	lda     _k_input
	cmp     #$71
	bne     L000A
	rts

.segment	"BSS"

M0001:
	.res	1,$00

.endproc
