# OLed using ESP32 Example

In this repository we implement a simple animation in the OLed to learn more how to use I²C communication using ESP32.

The main animation is a Snake in the display going foward and backwards.

// TODO: Explication about OLed

Based on SSD1306 Display controller chip using 6800 interface.

// TODO: Explication about communication

Serial communication. A cool [video](https://www.youtube.com/watch?v=IyGwvGzrqp8&ab_channel=Electronoobs) about it. OLed + SSD1306 uses I²C, so wee have this pinout 

To communicate we will be using MCU I2C Interface:
    - Both the data and clock signals must be connected to pull-up resistors.


The project Hardware and Software is defined in the next diagram:

<div class="side">
    <div class="middle">
        <p style="text-align: center">Harwre Block Diagram</p>
        <img src="./docs/Hardware_block.png" alt="Hardware Block", title="Hardware Block Diagram">
    </div>
    <div class="middle">
        <p style="text-align: center">Software Block Diagram</p>
        <img src="./docs/software_block.png" alt="Software Block" title title="Software Block Diagram">
    </div>
</div>


<style>

.side {
    display: grid;
    grid-template-columns: 1fr 1fr 1fr;
    column-gap: 20px;
    place-items: center;
}

.middle {
}

.img {
    max-width: 100%;
    max-height: 100%;
}

</style>