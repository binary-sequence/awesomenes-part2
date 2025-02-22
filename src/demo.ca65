.export ball_nmi
.export rf_noise_init
.export rf_noise_nmi
.export snow_nmi
.export transition_to_blank_nmi
.export transition_to_snow_nmi

.include "nes_constants.inc"
.include "demo_constants.inc"

.importzp ball_dx
.importzp ball_dy
.importzp ball_x_direction
.importzp ball_y_direction
.importzp delay_palette_rotation
.importzp demo_state
.importzp frame_count
.importzp ppu_ctrl
.importzp ppu_mask
.importzp ppu_scroll_x
.importzp ppu_scroll_y
.importzp ptr_palette
.importzp ptr_snow
.importzp ptr_snow_transition
.importzp seconds
.import demo_song
.import noise_pal
.import famistudio_init
.import famistudio_music_play
.import famistudio_music_pause
.import famistudio_music_stop


.segment "CODE"
  .proc fill_row
    ; A: Tile 1
    ; Y: Tile 2
    LDX #32  ; 32 tiles
    copy_snow_transition:
      STA PPUDATA
      DEX
      STY PPUDATA
      DEX
      BNE copy_snow_transition

    RTS
  .endproc

  .proc fill_snow_row
    LDA ptr_snow+1
    STA PPUADDR
    LDA ptr_snow
    STA PPUADDR

    LDA #$F9
    LDY #$FA
    JSR fill_row
    RTS
  .endproc

  .proc fill_snow_transition_row
    LDA ptr_snow_transition+1
    STA PPUADDR
    LDA ptr_snow_transition
    STA PPUADDR

    LDA #$FB
    LDY #$FC
    JSR fill_row

    RTS
  .endproc

  .proc setup_ball_obj
    ;  -------------
    ; | OBJ0 | OBJ1 |
    ;  -------------
    ; | OBJ2 | OBJ3 |
    ;  -------------

    LDA #BALL_TILE
    STA OBJ0 + OAM_OBJ::tile_nr
    STA OBJ1 + OAM_OBJ::tile_nr
    STA OBJ2 + OAM_OBJ::tile_nr
    STA OBJ3 + OAM_OBJ::tile_nr

    ;     VHP---CC
    LDA #%01000000
    STA OBJ0 + OAM_OBJ::attr

    ;     VHP---CC
    LDA #%00000000
    STA OBJ1 + OAM_OBJ::attr

    ;     VHP---CC
    LDA #%11000000
    STA OBJ2 + OAM_OBJ::attr

    ;     VHP---CC
    LDA #%10000000
    STA OBJ3 + OAM_OBJ::attr

    RTS
  .endproc

  .proc move_ball_to
    ; X: x pos
    ; Y: y pos
    ;  -------------
    ; | OBJ0 | OBJ1 |
    ;  -------------
    ; | OBJ2 | OBJ3 |
    ;  -------------

    STX OBJ0 + OAM_OBJ::x_pos
    STY OBJ0 + OAM_OBJ::y_pos

    STX OBJ2 + OAM_OBJ::x_pos
    STY OBJ1 + OAM_OBJ::y_pos

    TXA
    CLC
    ADC #8
    STA OBJ1 + OAM_OBJ::x_pos
    STA OBJ3 + OAM_OBJ::x_pos

    TYA
    CLC
    ADC #8
    STA OBJ2 + OAM_OBJ::y_pos
    STA OBJ3 + OAM_OBJ::y_pos

    RTS
  .endproc

  .proc rf_noise_init
    LDA #DemoState::RF_NOISE
    STA demo_state

    ; Fill nametable 0 with noise tiles ($FD, $FE)
    LDX #.hibyte(NT0)
    STX PPUADDR
    LDX #.lobyte(NT0)  ; #0
    STX PPUADDR
    LDY #4
    fill_nt0:
      ;  256 * 4 = 1024
      LDA #$FE
      STA PPUDATA
      INX
      LDA #$FD
      STA PPUDATA
      INX
      BNE fill_nt0
      DEY
      BNE fill_nt0

    ; Copy attribute table 0
    LDA #.hibyte(ATTR0)
    STA PPUADDR
    LDA #.lobyte(ATTR0)
    STA PPUADDR
    LDA #0
    LDX #64
    copy_attr0:
      STA PPUDATA
      DEX
      BNE copy_attr0

    ; Noise palettes
    LDA #.hibyte(BGPAL0)
    STA PPUADDR
    LDA #.lobyte(BGPAL0)  ; #0
    STA PPUADDR
    LDX #0
    copy_noise_pal:
      LDA noise_pal,X
      STA PPUDATA
      INX
      CPX #4
      BNE copy_noise_pal

    ; Ball palettes
    LDA #.hibyte(FGPAL0)
    STA PPUADDR
    LDA #.lobyte(FGPAL0)
    STA PPUADDR
    LDX #0
    copy_ball_pal:
      LDA noise_pal,X
      STA PPUDATA
      INX
      CPX #4
      BNE copy_ball_pal

    LDA #.lobyte(noise_pal)
    STA ptr_palette
    LDA #.hibyte(noise_pal)
    STA ptr_palette+1

    LDA #1  ; NTSC
    LDX #.lobyte(demo_song)
    LDY #.hibyte(demo_song)
    JSR famistudio_init
    LDA #0  ; Track 0
    JSR famistudio_music_play

    LDA #0
    STA frame_count
    STA seconds

    BIT PPUSTATUS
    ;     BGRsbMmG
    LDA #%00011110
    STA ppu_mask
    STA PPUMASK

    BIT PPUSTATUS
    ;     VPHBSINN
    LDA #%10001000
    STA ppu_ctrl
    STA PPUCTRL

    LDA #.lobyte(NT0_BOTTOM_LEFT)
    STA ptr_snow_transition
    STA ptr_snow
    LDA #.hibyte(NT0_BOTTOM_LEFT)
    STA ptr_snow_transition+1
    STA ptr_snow+1

    RTS
  .endproc

  .proc rf_noise_effect
    ; palette cycling
    INC delay_palette_rotation
    LDA delay_palette_rotation
    LSR A
    BCS skip_rotate_palette
    rotate_palette:
      LDA #.hibyte(BGPAL0)
      STA PPUADDR
      LDA #.lobyte(BGPAL0)
      STA PPUADDR
      LDY #0
      copy_noise_pal:
        LDA (ptr_palette),Y
        STA PPUDATA
        INY
        CPY #4
        BNE copy_noise_pal
      LDA ptr_palette
      CMP #.lobyte(noise_pal)+12
      BEQ reset_ptr_palette
      .repeat 4
        INC ptr_palette
      .endrepeat
      JMP skip_reset_ptr_palette
      reset_ptr_palette:
        LDA #.lobyte(noise_pal)
        STA ptr_palette
      skip_reset_ptr_palette:
    skip_rotate_palette:

    RTS
  .endproc

  .proc rf_noise_nmi
    INC ppu_scroll_y

    JSR rf_noise_effect

    LDA seconds
    CMP #3  ; 3 seconds
    BNE same_state
    next_state:
      LDA #DemoState::TRANSITION_TO_SNOW
      STA demo_state
      LDA #0
      STA frame_count
      STA seconds
      STA ppu_scroll_y

      JSR fill_snow_transition_row
      SEC
      LDA ptr_snow_transition
      SBC #32
      STA ptr_snow_transition
      LDA ptr_snow_transition+1
      SBC #0
      STA ptr_snow_transition+1
    same_state:

    RTS
  .endproc

  .proc transition_to_snow_nmi
    JSR rf_noise_effect
    JSR fill_snow_row
    JSR fill_snow_transition_row

    LDA frame_count
    CMP #4  ; 4 frames
    BNE check_state
    SEC
    LDA ptr_snow_transition
    SBC #32
    STA ptr_snow_transition
    LDA ptr_snow_transition+1
    SBC #0
    STA ptr_snow_transition+1
    SEC
    LDA ptr_snow
    SBC #32
    STA ptr_snow
    LDA ptr_snow+1
    SBC #0
    STA ptr_snow+1
    ; reset counter
    LDA #0
    STA frame_count
    STA seconds

    check_state:
      LDA ptr_snow
      CMP #$00
      BNE continue
      LDA ptr_snow+1
      CMP #$20
      BNE continue
      next_state:
        JSR fill_snow_row
        LDA #DemoState::SNOW
        STA demo_state
        LDA #0
        STA frame_count
        STA seconds
      reset_counter:
        LDA #0
        STA frame_count
        STA seconds
    continue:

    RTS
  .endproc

  .proc snow_nmi
    JSR rf_noise_effect

    check_state:
      LDA seconds
      CMP #1  ; 2 second
      BNE same_state
      next_state:
        LDA #DemoState::TRANSITION_TO_BLANK
        STA demo_state
        LDA #0
        STA frame_count
        STA seconds
      same_state:

    RTS
  .endproc

  .proc transition_to_blank_nmi
    LDA ppu_scroll_x
    CLC
    ADC #4
    STA ppu_scroll_x
    BCS next_nametable
    JMP same_nametable
    next_nametable:
      INC ppu_ctrl
      LDX #DemoState::BALL
      STX demo_state
      LDA #0
      STA frame_count
      STA seconds
      TAX
      TAY
      JSR move_ball_to
      JSR setup_ball_obj
      LDA #4
      STA ball_dx
      STA ball_dy
      LDA #0
      STA ball_x_direction
      STA ball_y_direction
    same_nametable:

    RTS
  .endproc

  .proc ball_nmi
    LDA OBJ0 + OAM_OBJ::x_pos
    LDX ball_x_direction
    BEQ move_right
    JMP move_left
    move_right:
      CLC
      ADC ball_dx
      JMP end_x_move
    move_left:
      SEC
      SBC ball_dx
    end_x_move:
    CMP #(255-16)
    BCC keep_dx
    change_dx:
      LDX #1
      STX ball_x_direction
      SEC
      SBC ball_dx
    keep_dx:
    TAX

    LDA OBJ0 + OAM_OBJ::y_pos
    LDY ball_y_direction
    BEQ move_down
    JMP move_up
    move_down:
      CLC
      ADC ball_dy
      JMP end_y_move
    move_up:
      SEC
      SBC ball_dy
    end_y_move:
    CMP #(240-16)
    BCC keep_dy
    change_dy:
      LDY #1
      STY ball_y_direction
      SEC
      SBC ball_dy
    keep_dy:
    TAY

    JSR move_ball_to

    RTS
  .endproc
