# Buck-Boost Converter

[Great Scott](https://www.youtube.com/watch?v=ZiD_X-uo_TQ&t=1s)

[Instrucatble](https://www.instructables.com/id/DIY-BuckBoost-Converter-Flyback/)

[Adding Current Limiting to Buck Boost](https://www.youtube.com/watch?v=8uoo5pAeWZI)

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
2. Current limiting to a max of 1A
3. Max output of 150V and minimum input of 12V.
4. Implemented with pure C using ATmega328p (arduino mini)

### Overview

[Switch Mode Power Supplies](https://www.electronics-tutorials.ws/power/switch-mode-power-supply.html)

The buck-boost regulator is a combination of the buck and boost converters. There are two types of configurations: inverting and non-inverting. 

The non-inverting implementation is basically the two regulators merged together, and would require two switches, one to activate the boost mode and one to activate the buck mode.

The inverting configuration is shown below

![](https://upload.wikimedia.org/wikipedia/commons/thumb/e/e6/Buckboost_conventions.svg/1024px-Buckboost_conventions.svg.png)

In this configuration, the output is inverted.

When the switch is fully on, the voltage across the inductor is equal to the supply voltage and the inductor stores the energy in its magnetic field. When the switch opens, the energy stored in the inductor discharges onto the load.

None of the power delivered comes form the DC source, so the voltage drop across the load when the switch is off is equal to the inductor voltage.

The result is that the output voltage can be greater or smaller than the input voltage based on the duty cycle of the switch. 

The output voltage is given by `Vout = Vin(D/1-D)`

### Calculations

###### Converter

###### Current Limiter
