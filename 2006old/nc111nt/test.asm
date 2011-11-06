	.file	"d:\lcc\include\stdio.h"
_$M0:
	.file	"e:\program\nc111nt\test.c"
	.text
	.file	"d:\lcc\include\safelib.h"
_$M1:
	.file	"d:\lcc\include\stdio.h"
_$M2:
	.file	"e:\program\nc111nt\test.c"
_$M3:
	.text
;    1 #include <stdio.h>
;    2 
;    3 
;    4 int main()
	.type	_main,function
_main:
;    5 { 
	.line	5
;    6 	printf("helloworld");
	.line	6
	pushl	$_$2
	call	_printf
	addl	$4,%esp
;    7 	return 1;
	.line	7
	movl	$1,%eax
_$1:
	.line	8
	ret
_$3:
	.size	_main,_$3-_main
	.globl	_main
	.extern	_printf
	.data
_$2:
; "helloworld\x0"
	.byte	104,101,108,108,111,119,111,114,108,100,0
