Q-SID: a 4-voice polyphonic MIDI synth based on four SID 8580 chips 

Features:

 Hardware/OS: 

 Q-SID will have a modular construction: every functional block of the device will have it's own PCB:
  - CPU board - Raspberry Pi, or other SoC board with two I2C busses, UART, at least 16 GPIO's, USB and ethernet
  - four voice boards, each with one 8580 SID chip
  - MIDI interface board
  - DSP/mixer board 
  - front controls board with rotary encoders and buttons for sound parameters control 
  - front display board 
  - I2C voltage level converter for voice boards
  - power supply with +9V, +5V switched for CPU, +5V linear for voice boards, +3.3V

 Q-SID will have an operating system in form of single multithreaded process of Linux OS, running on custom Raspbian image scaled down to
 stripped Linux kernel, Q-SID OS process, and prefferably nothing more (to spare CPU cycles and other resources).

 Synthetizer features:

  - 4 SID engines with full SID features
  - polyphonic mode with 4 voice polyphony
  - performance mode with keyboard/voice/patch split 
  - arpeggiator (not sure about exact parameters yet)
  - 2 software LFO's modulating filter cutoff/resonance, PWM, oscillator detune, and other parameters
  - onboard sound effects via internal DSP
  - MIDI in/out with programmable MIDI channel
  - embedded .sid chiptune player

====
