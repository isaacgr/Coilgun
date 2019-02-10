# Buck-Boost Converter

[Great Scott](https://www.youtube.com/watch?v=ZiD_X-uo_TQ&t=1s)

[Instrucatble](https://www.instructables.com/id/DIY-BuckBoost-Converter-Flyback/)

---

### Motivation

I have been in the process of building a boost converter for some time. The main purpose was to act as the charging circuit for my capacitor bank (which would be used in a coil gun project). I wanted to use an arduino, but write the code in C for the learning experience. 

It includes an LCD for dispaying the output voltage, setpoint and currentdraw as well as built in current limiting using a couple of transistors.

It generaly works well, except for some inconsitencies with the voltage output for various loads and exhibiting some bucking properties.

The goal of the redesign for this project is to modify the existing design and implement the following:

> 1. Buck-boost capabilities
> 2. Differential Op-amp feedback loop for better stability (currently using resitor divider)
> 3. Documentation of implementation

### Technical Requirements

1. Display setpoint, output voltage and current draw
2. Current limiting
3. Max output of 150V and minimum input of 12V.
4. Max current draw of 1A@some voltage
5. Implemented with pure C using ATmega328p (arduino mini)

