.export nmi_handler

.include "nes_constants.inc"
.include "demo_constants.inc"

.importzp demo_state
.importzp frame_count
.importzp ppu_ctrl
.importzp ppu_mask
.importzp ppu_scroll_x
.importzp ppu_scroll_y
.importzp seconds
.import ball_nmi
.import famistudio_update
.import rf_noise_nmi
.import snow_nmi
.import transition_to_blank_nmi
.import transition_to_snow_nmi


.segment "CODE"

  .proc nmi_handler
    PHA
    TXA
    PHA
    TYA
    PHA

    LDA #OAM
    STA OAMADDR
    LDX #.hibyte(OAM_BUFFER)
    STX OAMDMA

    LDA demo_state
    CMP #DemoState::RF_NOISE
    BEQ jsr_rf_noise_nmi
    CMP #DemoState::TRANSITION_TO_SNOW
    BEQ jsr_transition_to_snow_nmi
    CMP #DemoState::SNOW
    BEQ jsr_snow_nmi
    CMP #DemoState::TRANSITION_TO_BLANK
    BEQ jsr_transition_to_blank_nmi
    CMP #DemoState::BALL
    BEQ jsr_ball_nmi
    JMP no_jsr
    jsr_rf_noise_nmi: JSR rf_noise_nmi
    JMP no_jsr
    jsr_transition_to_snow_nmi: JSR transition_to_snow_nmi
    JMP no_jsr
    jsr_snow_nmi: JSR snow_nmi
    JMP no_jsr
    jsr_transition_to_blank_nmi: JSR transition_to_blank_nmi
    JMP no_jsr
    jsr_ball_nmi: JSR ball_nmi
    no_jsr:

    LDA ppu_scroll_x
    STA PPUSCROLL
    LDA ppu_scroll_y
    STA PPUSCROLL
    LDA ppu_ctrl
    STA PPUCTRL
    LDA ppu_mask
    STA PPUMASK

    INC frame_count
    LDA frame_count
    CMP #FPS
    BEQ reset_frame_count
    JMP keep_frame_count
    reset_frame_count:
      LDX #0
      STX frame_count
      INC seconds
    keep_frame_count:

    JSR famistudio_update

    PLA
    TAY
    PLA
    TAX
    PLA
    RTI
  .endproc
