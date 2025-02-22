; CPU RAM
ZP         = $00
STACK      = $0100
OAM_BUFFER = $0200
RAM        = $0300

; PPU registers
PPUCTRL   = $2000
PPUMASK   = $2001
PPUSTATUS = $2002
OAMADDR   = $2003
OAMDATA   = $2004
PPUSCROLL = $2005
PPUADDR   = $2006
PPUDATA   = $2007

; APU and I/O registers
SQ1_VOL    = $4000
SQ1_SWEEP  = $4001
SQ1_LO     = $4002
SQ1_HI     = $4003
SQ2_VOL    = $4004
SQ2_SWEEP  = $4005
SQ2_LO     = $4006
SQ2_HI     = $4007
TRI_LINEAR = $4008
; UNUSED   = $4009
TRI_LO     = $400A
TRI_HI     = $500B
NOISE_VOL  = $400C
; UNUSED   = $400D
NOISE_LO   = $400E
NOISE_HI   = $400F
DMC_FREQ   = $4010
DMC_RAW    = $4011
DMC_START  = $4012
DMC_LEN    = $4013
OAMDMA     = $4014
SND_CHN    = $4015
JOY1       = $4016
JOY2       = $4017

; OAM
OAM = $00

; VRAM
NT0    = $2000
ATTR0  = $23C0
NT1    = $2400
ATTR1  = $27C0
NT2    = $2800
ATTR2  = $2BC0
NT3    = $2C00
ATTR3  = $2FC0
BGPAL0 = $3F00
BGPAL1 = $3F04
BGPAL2 = $3F08
BGPAL3 = $3F0C
FGPAL0 = $3F10
FGPAL1 = $3F14
FGPAL2 = $3F18
FGPAL3 = $3F1C
NT0_BOTTOM_LEFT = $23A0

; Controller buttons
BUTTON_A      = %10000000
BUTTON_B      = %01000000
BUTTON_SELECT = %00100000
BUTTON_START  = %00010000
BUTTON_UP     = %00001000
BUTTON_DOWN   = %00000100
BUTTON_LEFT   = %00000010
BUTTON_RIGHT  = %00000001

OBJ0 = OAM_BUFFER
OBJ1 = OAM_BUFFER + 4
OBJ2 = OAM_BUFFER + 8
OBJ3 = OAM_BUFFER + 12

; Data structures
DRAWING_BUFFER_VERTICAL = %00000001
DRAWING_BUFFER_REPEAT = %00000010
.struct DrawingBufferHeader
  length .res 1
  vram_hibyte .res 1
  vram_lobyte .res 1
  drawing_flags .res 1
.endstruct
; Buffer format
; Byte 0: length of data
; Byte 1: high byte of PPU address
; Byte 2: low byte of PPU address
; Byte 3: drawing flags
; 7  bit  0
; ---- --RV
;        ||
;        |+- Vertical drawing (V)
;        |   (0: add 1, going across; 1: add 32, going down)
;        +-- Repeat (R)
;            (0: no repeat; 1: repeat)
; Byte 4-X: the data to draw (number of bytes determined by the length)

.struct OAM_OBJ
  y_pos .res 1
  tile_nr .res 1
  attr .res 1
  x_pos .res 1
.endstruct

.struct Point2D
  x_pos .res 1
  y_pos .res 1
.endstruct

.struct Pointer
  lobyte .res 1
  hibyte .res 1
.endstruct
