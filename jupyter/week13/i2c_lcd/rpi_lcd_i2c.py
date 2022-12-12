import smbus2 as smbus
from time import sleep

RPi_I2C_PORT = 1

class I2C_dev:
    
    def __init__(self, addr, port=RPi_I2C_PORT):
        self.addr = addr
        self.bus = smbus.SMBus(port)

    def write_byte(self, data):
        self.bus.write_byte(self.addr, data)
        sleep(0.0001)

    def write_byte_data(self, cmd, data):
        self.bus.write_byte_data(self.addr, cmd, data)
        sleep(0.0001)

    def write_block_data(self, cmd, data):
        self.bus.write_block_data(self.addr, cmd, data)
        sleep(0.0001)

    def read_byte(self):
        return self.bus.read_byte(self.addr)

    def read_byte_data(self, cmd):
        return self.bus.read_byte_data(self.addr, cmd)

    def read_block_data(self, cmd):
        return self.bus.read_block_data(self.addr, cmd)


# LCD I2C Address (A2=1, A1=1, A0=1)
LCD_ADDR = 0x27

# commands
LCD_CLEARDISPLAY = 0x01
LCD_RETURNHOME = 0x02
LCD_ENTRYMODESET = 0x04
LCD_DISPLAYCONTROL = 0x08
LCD_CURSORSHIFT = 0x10
LCD_FUNCTIONSET = 0x20
LCD_SETCGRAMADDR = 0x40
LCD_SETDDRAMADDR = 0x80

# flags for display entry mode
LCD_ENTRYRIGHT = 0x00
LCD_ENTRYLEFT = 0x02
LCD_ENTRYSHIFTINCREMENT = 0x01
LCD_ENTRYSHIFTDECREMENT = 0x00

# flags for display on/off control
LCD_DISPLAYON = 0x04
LCD_DISPLAYOFF = 0x00
LCD_CURSORON = 0x02
LCD_CURSOROFF = 0x00
LCD_BLINKON = 0x01
LCD_BLINKOFF = 0x00

# flags for display/cursor shift
LCD_DISPLAYMOVE = 0x08
LCD_CURSORMOVE = 0x00
LCD_MOVERIGHT = 0x04
LCD_MOVELEFT = 0x00

# flags for function set
LCD_8BITMODE = 0x10
LCD_4BITMODE = 0x00
LCD_2LINE = 0x08
LCD_1LINE = 0x00
LCD_5x10DOTS = 0x04
LCD_5x8DOTS = 0x00

# I2C module (PCF7654) -> LCD controller (HD44780)
# [P7 ~ P0] ->  [D7 D6 D5 D4 BL En Rw Rs]

BL = 0b00001000 # Backlight bit
En = 0b00000100 # Enable bit
Rw = 0b00000010 # Read/Write bit
Rs = 0b00000001 # Register select bit

class LCD_i2c:
    
    # initilize LCD module with I2C interface - HD44780 + PCF7654
    def __init__(self):
        self.lcd_device = I2C_dev(LCD_ADDR)
        self.BACKLIGHT = 0
        
        # refer to the HD44780 Datasheet p.46 - Power-on 4-bit interface initialization procee
        #
        self.lcd_device.write_byte(0x30)
        sleep(.005) # 5ms > 4.1ms
        self.lcd_device.write_byte(0x30)
        sleep(.001) # 1ms > 100us
        self.lcd_device.write_byte(0x30)
        self.lcd_device.write_byte(0x20)
        
        self.lcd_write(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE)
        self.lcd_write(LCD_DISPLAYCONTROL | LCD_DISPLAYON)
        self.lcd_write(LCD_CLEARDISPLAY)
        self.lcd_write(LCD_ENTRYMODESET | LCD_ENTRYLEFT)
        sleep(0.2)

   # driven EN as a pluse to latch output data/command
    def lcd_strobe(self, data):
        self.lcd_device.write_byte(data | En)
        sleep(.0005)
        self.lcd_device.write_byte(data & ~En)
        sleep(.0001)

    def lcd_write_4bits(self, data):
        self.lcd_strobe(data | self.BACKLIGHT)

    # write command/character to lCD; command (mode=0), character (mode=Rs)
    def lcd_write(self, data, mode=0):
        self.lcd_write_4bits((data & 0xF0) |  mode)
        self.lcd_write_4bits(((data << 4) & 0xF0) | mode)

    # write a character to LCD
    def lcd_write_char(self, ch):
        self.lcd_write(ch,mode=Rs)
        
    # write a string to LCD @(row,col)
    # 1[7-bit DDRAM addr.] => row1 : 0x80 ~ , row2 0xC0 ~  
    def lcd_write_string(self, string, row, col=1):
        base = [0x80, 0xC0]
        self.lcd_write(base[row-1]+col-1)
        _ = [self.lcd_write_char(ord(c)) for c in string]
            
    # clear lcd and set to home
    def lcd_clear(self):
        self.lcd_write(LCD_CLEARDISPLAY)
        self.lcd_write(LCD_RETURNHOME)

    # turn on/off backlight
    def backlight(self, state):
        self.BACKLIGHT = BL if (state==1) else 0
        self.lcd_device.write_byte(self.BACKLIGHT)

    # add custom characters (0 - 7)
    # 01[6-bit CGRAM addrs] -> 0x40 ~ 
    def lcd_load_custom_chars(self, cgdata):
        self.lcd_write(0x40);
        _ = [[self.lcd_write_char(ch) for ch in row] for row in cgdata]
         