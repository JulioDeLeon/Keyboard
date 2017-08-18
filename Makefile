TARGET := keyboard_fw

SRC := $(TARGET).c

# Chipset of the teensy 3.2
# MCU :=

# Frequency of the core to be
# F_CPU :=

# Format of the output file. Teensy loader accepts hex files
FORMAT := ihex

# Directory for object files.
OBJDIR := .

# Optimization flag level, can be [0,1,2,3,s]. In this case, 's' is for space
OPT := s

# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG := dwarf-2

