<div style="text-align: center;">
<img src="images/stust.png" alt="STUST" class="center" style="width: 900px;"/>
</div>

<hr style="border:4px solid gray"> </hr>

<div style="text-align: center;">    
<br>    
    
# Transmission of C-Struct via I²C
### Raspberry Pi I²C Serial Communication

</div>

<br>
<hr style="border:4px solid gray"> </hr>


# Reference

* [The Raspberry Pi GPIO pinout guide.](https://pinout.xyz/)

* [Synchronous serial communication in Raspberry Pi using I2C protocol](https://www.engineersgarage.com/raspberrypi/articles-raspberry-pi-i2c-bus-pins-smbus-smbus2-python/)

* [struct — Interpret bytes as packed binary data](https://docs.python.org/3/library/struct.html)

* [Python String encode(); UTF-8 / ASCII](https://www.programiz.com/python-programming/methods/string/encode)

* [Communication between Raspberry Pi and Arduino with I2C](https://www.aranacorp.com/en/communication-between-raspberry-pi-and-arduino-with-i2c/)


<hr style="border:2px solid orange"> </hr>

# Arduino Sketch: rpi_i2c_lab01.uno

```C++

```

<hr style="border:2px solid orange"> </hr>

## Lab 01 : Raspberry Pi (Master) → Arduino (Slave)

* Arduino sketch : `rpi_i2c_lab01`



```python
import smbus2 as smbus 
from time import sleep
from random import randrange, random

I2C_ADDR = 11
PORT = 1

def i2c_write_bytes(data):
    i2cbus = smbus.SMBus(PORT)
    cmd = 123
    i2cbus.write_i2c_block_data(I2C_ADDR, cmd, data.encode("ascii"))
    sleep(0.5)

def main(msg):
    i2c_write_bytes(msg)
    
if __name__ == '__main__':
    main("I2C")
```

```
Arduino I2C Slave(11)
===================================
irsReceive # of bytes = 4
received : 123 73 50 67 
```

<hr style="border:2px solid orange"> </hr>

## Lab 02 : Raspberry Pi (Master) ← Arduino (Slave)

* Arduino sketch : `rpi_i2c_lab01`


```python
import smbus2 as smbus 
from time import sleep
from random import randrange, random

I2C_ADDR = 11
PORT = 1

def i2c_read_bytes(nb):
    i2cbus = smbus.SMBus(PORT)
    cmd = 123
    data = i2cbus.read_i2c_block_data(I2C_ADDR, cmd, nb)
    sleep(0.5)
    return data

def main():
    recv = i2c_read_bytes(5)
    print(f"receved : {recv}")
    
if __name__ == '__main__':
    main()
```

```
RPi Output
===================================
receved : [3, 5, 4, 4, 1]

Arduino I2C Slave(11)
===================================
irsReceive # of bytes = 1
irsRequest send => 3 5 4 4 1 
```

<hr style="border:2px solid orange"> </hr>

## [Pyuthon Reference - `Struct`](https://docs.python.org/3/library/struct.html)

| Character | Byte order             |
|-----------|------------------------|
| =         | native                 |
| <         | little-endian          |
| >         | big-endian             |


| format | C Type             | Python type       | Standard size |
|--------|--------------------|-------------------|---------------|
| x      | pad byte           | no value          |               |
| c      | char               | bytes of length 1 | 1             |
| b      | signed char        | integer           | 1             |
| B      | unsigned char      | integer           | 1             |
| ?      | _Bool              | bool              | 1             |
| h      | short              | integer           | 2             |
| H      | unsigned short     | integer           | 2             |
| i      | int                | integer           | 4             |
| I      | unsigned int       | integer           | 4             |
| l      | long               | integer           | 4             |
| L      | unsigned long      | integer           | 4             |
| q      | long long          | integer           | 8             |
| Q      | unsigned long long | integer           | 8             |
| f      | float              | float             | 4             |
| d      | double             | float             | 8             |
| s      | char[]             | str              |             |



<hr style="border:0.5px solid gray"> 

```python
from struct import pack, unpack, Struct

fmt = "=10sHHHf"
print(Struct(fmt).size)
name = "Jack"
eng = 78
math = 80
chi = 90
toArduino = pack(fmt,bytes(name,'ascii'),eng,math,chi,(eng+math+chi)/3.0)
print(toArduino)
name, eng, math, chi, avg = unpack(fmt,toArduino)
print(f"{name.decode()}{eng:3}{math:3}{chi:3}{avg:5.3}")
```
<hr style="border:0.5px solid gray"> 

    20
    b'Jack\x00\x00\x00\x00\x00\x00N\x00P\x00Z\x00UU\xa5B'
    Jack       78 80 90 82.7


<hr style="border:2px solid orange"> </hr>

## Lab 03 : Transfer C-Struct to Arduino UNO

* Arduino sketch : `rpi_i2c_lab05`


```python
# -------------------------------------------------------
# RPi --> Arduino (rpi_i2c_lab04)
# -------------------------------------------------------
import smbus2 as smbus 
from time import sleep
from random import randrange, random
from struct import pack, unpack

# Slave Addresses
I2C_ADDR = 11

def i2c_struct_tb01(cmd=123):
    with smbus.SMBus(1) as I2Cbus:
        s = 'stust'+str(randrange(100,999))
        rf = random()*10
        ri = randrange(100,200)
        pkg = pack('=10sfh', bytes(s, 'ascii'), rf, ri)
        I2Cbus.write_i2c_block_data(I2C_ADDR, cmd, pkg);
        print(f"send({cmd}, {s:10}, {rf:5.3}, {ri})")
        sleep(0.5)
    return 0

if __name__ == '__main__':
    _ = [i2c_struct_tb01(c) for c in range(5)]
    
```

<hr style="border:0.5px solid gray"> 

    send(0, stust941  , 0.566, 131)
    send(1, stust828  ,   9.3, 134)
    send(2, stust489  ,  7.18, 180)
    send(3, stust158  ,  8.47, 142)
    send(4, stust306  ,  7.91, 129)


<hr style="border:2px solid orange"> </hr>

## Lab 04 : Receive C-Struct from Arduino UNO

* Arduino sketch : `rpi_i2c_lab05`


```python
# -------------------------------------------------------
# RPi <--> Arduino (rpi_i2c_lab04)
# -------------------------------------------------------
import smbus2 as smbus 
from time import sleep
from random import randrange, random
from struct import pack, unpack, Struct

# Slave Addresses
I2C_ADDR = 11

def i2c_struct_send(cmd=123):
    fmt = "=10sfh"
    with smbus.SMBus(1) as I2Cbus:
        s = 'stust'+str(randrange(100,999))
        rf = random()
        ri = randrange(100,200)
        pkg = pack(fmt, bytes(s, 'ascii'), rf, ri)
        I2Cbus.write_i2c_block_data(I2C_ADDR, cmd, pkg);
        print(f"send({cmd:2}, {s:10}, {rf:3.2}, {ri})")
        sleep(0.5)
    return 0

def i2c_struct_recv(cmd=0):
    fmt = "=10sfh"
    Len = Struct(fmt).size+1
    try:
        with smbus.SMBus(1) as I2Cbus:
            data = I2Cbus.read_i2c_block_data(I2C_ADDR, cmd, Len)
            cmd = data[0]
            s, rf, ri = unpack(fmt,bytes(data[1:]))
            print(f"recv({cmd:2}, {s.decode():10}, {rf:3.2}, {ri})")
    except Exception as e:
        print(e)
                
if __name__ == '__main__':
    for c in range(1,6):
        i2c_struct_send(c)
        sleep(1)
        i2c_struct_recv()
    
```

<hr style="border:0.5px solid gray"> 


    send( 1, stust353  , 0.6, 167)
    recv(66, stust353  , 0.6, 167)
    send( 2, stust240  , 0.85, 141)
    recv(66, stust240  , 0.85, 141)
    send( 3, stust648  , 0.42, 149)
    recv(66, stust648  , 0.42, 149)
    send( 4, stust960  , 0.51, 166)
    recv(66, stust960  , 0.51, 166)
    send( 5, stust861  , 0.24, 141)
    recv(66, stust861  , 0.24, 141)


<br><hr style="border:3px solid red"> </hr>
<div style="text-align: center;">         
    
# *Homework Assignment*

</div>
<hr style="border:3px solid red"> </hr>
<br>

* Homework 01 - Verify the discussed Python script and Arduino sketch on real hardware (Raspberry Pi and Arduino UNO).

<hr style="border:2px solid orange"> </hr>
<br>

<div style="text-align: left;">
<img src="images/break-yang-tr.png" alt="Break" class="center" style="width: 500px;"/>
</div>

