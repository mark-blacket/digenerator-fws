# Alternative firmwares for Synthfox Digenerator

The firmwares do not use arduino libraries and only depend on avr-gcc standard library. Files *digenerator.c* and *digenerator.h* mostly contain code taken from the original firmwares by Synthfox, but rewritten in pure C.

Compiled firmwares are included in the repository, you can flash them using a bash script: **./flash.sh \*firmware-name\***. Alternatively you can compile them yourself using **make**

## Bernoullator

A probability-based oscillator/noise generator, draws samples from two different waveforms depending on probability setting.

 - character - probability
 - opt a     - full-wave rectification (kinda)
 - opt b     - selects between square-to-sine and square-to-saw waves
 - gate 1    - octave up
 - gate 2    - freeze current waveshape
 - gate 3    - sync
 - gate 4    - glitchy softsync
 - gate out  - 1-bit noise
 
## Crossmod

Lo-fi sine osc for audio-rate modulation

 - character - modulator input
 - opt a     - selects between ringmodulation and crude phase modulation
 - opt b     - switch between 16-sample and 64-sample sine approximation
 - gate 1    - sync
 - gates 2-4 - waveform thrashing
 - gate out  - modulator > carrier
 
## DW8000

Wavetable oscillator loosely based on Korg DW-8000 waveforms

 - character - waveform morph
 - opts      - morphing modes
 - gates 1-2 - amount of skipped waveform samples
 - gate 3    - sync
 - gate 4    - glitchy softsync
 - gate out  - suboscillator

## Repeater

VCO-clocked dirty digital delay and glitch/stutter machine
Internal feedback is a bit buggy and sometimes doesn't fade to complete silence, can be temporarily fixed by fiddling with the character knob (which should be set somewhere around 12:00)

 - character - input
 - opt a - 8bit / 4bit samples
 - opt b - internal feedback on/off
 - gate 1 - stop
 - gate 2 - freeze
 - gate 3 - restart
 - gate 4 - reverse
 - gate out - beginning of buffer
