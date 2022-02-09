# Alternative firmwares for Synthfox Digenerator

The firmwares do not use arduino libraries and only depend on avr-gcc standard library. Files *digenerator.c* and *digenerator.h* contain code from original firmwares by Synthfox, but rewritten in pure C.
Compiled firmwares are included in the repository, you can flash them using a bash script:

> ./flash.sh \*firmware-name\*

Alternatively you can compile them yourself using make:

> make \*firmware-name\*

## Bernoullator

A probability-based oscillator/noise generator, draws samples from two different waveforms depending on probability setting.

 - character - probability
 - opt a     - full-wave rectification
 - opt b     - selects between square-to-sine and square-to-saw waves
 - gate 1    - octave up
 - gate 2    - freeze current waveshape
 - gate 3    - sync
 - gate 4    - glitchy softsync
 - gate out  - 1-bit noise
 
## Crossmod

Digital ringmodulator / modulated distortion

 - character - modulator input
 - opt a     - selects between ringmodulation and distortion
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
