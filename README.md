# Buck-Boost Converter

<!-- toc -->

- [Motivation](#Motivation)
- [Technical Requirements](#Technical-Requirements)
- [Overview](#Overview)
- [Calculations](#Calculations)
  * [Converter](#Converter)
    + [Basics](#Basics)
    + [Values](#Values)
  * [Current Limiter](#Current-Limiter)

<!-- tocstop -->

---

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
5. Reasonably fast response time for changing load
6. Maintain voltage with no load

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

[DC-DC Converters](http://www.eng.auburn.edu/~agrawvd/COURSE/READING/LOWP/Erikson_DC_2_DC.pdf)
[Buck-Boost Basics](http://www.ti.com/lit/an/slva721a/slva721a.pdf)
[Buck-Boost Power Stages](http://www.ti.com/lit/an/slva059a/slva059a.pdf)
[Choppers](https://www.electrical4u.com/chopper-dc-to-dc-converter/)

#### Converter

##### Basics

The basic theory of operation for a buck-boost converter is given below. 

When the switch for the converter is on, the inductor voltage is equal to the supply voltage and the inductor charges its magnetic field:

<a href="https://www.codecogs.com/eqnedit.php?latex=\begin{gathered}&space;{V_L&space;=&space;V_S}&space;\\&space;{V_S&space;=&space;L\frac{di}{dt}&space;\Rightarrow&space;\bigtriangleup&space;I&space;=&space;\frac{V_S}{L}T_{ON}}&space;\\&space;{\Rightarrow&space;\bigtriangleup&space;I&space;=&space;T(\frac{V_S}{L})(\frac{T_{ON}}{T})}&space;\\&space;\therefore&space;\\&space;\bigtriangleup&space;I&space;=&space;\frac{DV_S}{Lf}&space;\end{gathered}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\begin{gathered}&space;{V_L&space;=&space;V_S}&space;\\&space;{V_S&space;=&space;L\frac{di}{dt}&space;\Rightarrow&space;\bigtriangleup&space;I&space;=&space;\frac{V_S}{L}T_{ON}}&space;\\&space;{\Rightarrow&space;\bigtriangleup&space;I&space;=&space;T(\frac{V_S}{L})(\frac{T_{ON}}{T})}&space;\\&space;\therefore&space;\\&space;\bigtriangleup&space;I&space;=&space;\frac{DV_S}{Lf}&space;\end{gathered}" title="\begin{gathered} {V_L = V_S} \\ {V_S = L\frac{di}{dt} \Rightarrow \bigtriangleup I = \frac{V_S}{L}T_{ON}} \\ {\Rightarrow \bigtriangleup I = T(\frac{V_S}{L})(\frac{T_{ON}}{T})} \\ \therefore \\ \bigtriangleup I = \frac{DV_S}{Lf} \end{gathered}" /></a>

When the chopper is off, the inductor will reverse its polarity and discharges through the load and the diode:

<a href="https://www.codecogs.com/eqnedit.php?latex=\begin{gathered}&space;V_O&space;=&space;-V_L&space;\\&space;L\frac{di}{dt}&space;=&space;-V_O&space;\Rightarrow&space;L\frac{\bigtriangleup&space;I}{T_{OFF}}&space;=&space;-V_O&space;\Rightarrow&space;\bigtriangleup&space;I&space;=&space;-\frac{V_O}{L}&space;T_{OFF}&space;\end{gathered}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\begin{gathered}&space;V_O&space;=&space;-V_L&space;\\&space;L\frac{di}{dt}&space;=&space;-V_O&space;\Rightarrow&space;L\frac{\bigtriangleup&space;I}{T_{OFF}}&space;=&space;-V_O&space;\Rightarrow&space;\bigtriangleup&space;I&space;=&space;-\frac{V_O}{L}&space;T_{OFF}&space;\end{gathered}" title="\begin{gathered} V_O = -V_L \\ L\frac{di}{dt} = -V_O \Rightarrow L\frac{\bigtriangleup I}{T_{OFF}} = -V_O \Rightarrow \bigtriangleup I = -\frac{V_O}{L} T_{OFF} \end{gathered}" /></a>

Evaluating the above equations gives:

<a href="https://www.codecogs.com/eqnedit.php?latex=\begin{gathered}&space;\frac{DV_S}{Lf}&space;=&space;-\frac{V_O}{L}&space;T_{OFF}&space;\\&space;\Downarrow&space;\\&space;V_O&space;=&space;\frac{DV_S}{1-D}&space;\end{gathered}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\begin{gathered}&space;\frac{DV_S}{Lf}&space;=&space;-\frac{V_O}{L}&space;T_{OFF}&space;\\&space;\Downarrow&space;\\&space;V_O&space;=&space;\frac{DV_S}{1-D}&space;\end{gathered}" title="\begin{gathered} \frac{DV_S}{Lf} = -\frac{V_O}{L} T_{OFF} \\ \Downarrow \\ V_O = \frac{DV_S}{1-D} \end{gathered}" /></a>

This means we can vary D from 0 to 1 to decrease or increase the voltage

##### Values

#### Current Limiter
