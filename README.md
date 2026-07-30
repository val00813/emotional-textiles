# Emotional Textiles

**Name:** Emotional Textiles  
**Date:** March 2026  
**Author:** Jingyao Li  

## About

Emotional Textiles is an interactive installation that uses touch-sensitive fabric, Arduino and TouchDesigner to explore emotion, self-identity and inner growth.

## Description

The project contains six touch-sensitive textile areas connected to an Arduino. When a participant touches the fabric, the Arduino sends the corresponding input to TouchDesigner through serial communication.

TouchDesigner translates these inputs into changing colours and generative visual responses. Each touch produces a slightly different result, allowing the participant to influence the visual environment through physical contact.

By combining soft textile materials with digital imagery, the project considers emotions as something changeable and responsive rather than fixed. The interaction turns an internal emotional experience into a visible and shared encounter.

## Project Demo

[Watch the Emotional Textiles video on YouTube](https://youtu.be/fW6KRBQ_fI4)

## How to Use

1. Connect the six touch-sensitive textile areas to the Arduino.
2. Install the `TouchyTouch` Arduino library.
3. Upload `touch_6pins.ino` to the Arduino.
4. Open `Emotional Flower.toe` in TouchDesigner.
5. Select the correct Arduino serial port and set the baud rate to `9600`.
6. Touch the different textile areas to activate the visual responses.

## Tools Used

- TouchDesigner
- Arduino
- TouchyTouch library
- Conductive textiles
- Serial communication

## Acknowledgements

The project uses the open-source TouchyTouch library for capacitive touch sensing.

## Author
Jingyao Li
