# I²C SSD1306

Slave Address Bit `SAO`.  
I²C-bus data signal `SDA` (SDA_OUT/D2 for output and SDA_IN/D1 for input) -> Pull-Up Resistors.  
I²C-bus clock Signal `SCL` -> Pull-Up Resistors

SSD1306 recognize the Slave address before transmitting or recieving any information.

Slave Address: `0b011110X⁰X¹`, where X⁰ is any 0 or 1, and X¹ is 1 for read and 0 for Write. 

Every bit is read in the **Clock UP**.

// TODO: Will we have a read option?


## Write Mode

<img src="./write_i2c.png">

Sent in order to achieve communication

1. Start condition:
    - SDA: HIGH -> LOW
    - SCL: HIGH
1. Slave Address: `0b011110X⁰0` with R/W# bit == 0 to write mode
1. Acknowledgement signal will be generated after receiving one byte of data. On the ninth UP clock pulse after 8 bits. Read it or ignore?
    - If Co is 0, the next informatiion will be data bytes only.
    - The D/C# bit determines the next data byte is acted as a comamand or data. 0: The next byte is a command. 1: The next byte is a data byte which will be stored into GDDRAM column.
1. Next will be sent a control byte or data byte which consists of `Co-D/C#-0-0-0-0-0-0`
1. Again we will have an acknowledge bit by each byte sent.
1. The write mode will be finished when there's a stop condition.
    - SDA: LOW -> HIGH
    - SCL: HIGH.

<img src="./start_sstop_ack.png">

## Commands

Each command will be sent after a `0b00000000` and a ack bit.

To see full tables of commands, read Section 9 of SSD1306 datasheet.

## Data

