.export clear_nametables
.export clear_oam_buffer
.export clear_screen

.include "nes_constants.inc"


.segment "CODE"

  .proc clear_nametables
    LDA #.hibyte(NT0)
    STA PPUADDR
    LDA #.lobyte(NT0)  ; LDA #0
    STA PPUADDR
    LDX #0
    LDY #8
    clear:
      ; 256 * 8 = 2048
      STA PPUDATA
      INX
      BNE clear
      DEY
      BNE clear

    RTS
  .endproc

  .proc clear_oam_buffer
    LDA #$FF  ; y pos off screen
    LDX #0
    clear:
      STA OAM_BUFFER+OAM_OBJ::y_pos,X
      INX
      INX
      INX
      INX
      BNE clear

    RTS
  .endproc

  .proc clear_screen
    JSR clear_nametables
    JSR clear_oam_buffer
  .endproc
