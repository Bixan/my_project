  ; ===========================================
  ; dasm.inc copyright MASM32 1998 - 2005
  ; ===========================================

IFNDEF DASM_INC
DASM_INC equ <1>

Decode2Asm PROTO :DWORD,:DWORD,:DWORD,:DWORD
Decode2AsmOpcode PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD

ELSE
echo -----------------------------------------
echo WARNING Duplicate include file dasm.inc
echo -----------------------------------------
ENDIF
