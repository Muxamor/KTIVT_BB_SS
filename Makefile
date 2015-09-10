PROJECT=KTIVT_BB_SS
# Set the FLAG OS_SYSTEM - LINUX or WINDOWS or BEAGLEBONEBLACK it depends which sistem are you use to build. 
OS_SYSTEM=LINUX

# Two additional CFLAGS must be used for Angstrom
# They must not be used for Debian or Ubuntu. I couldn't find out why. 
# The hint came from C:\gcc-linaro\share\doc\gcc-linaro-arm-linux-gnueabihf\README.txt 
#
# Uncomment the following line if you use Angstrom on your BeagleBone
#TARGET=angstrom

# Directory for C-Source
vpath %.c $(CURDIR)/source

# Directory for includes
CINCLUDE = $(CURDIR)/include  

# Directory for object files
OBJDIR = $(CURDIR)/object

# Other dependencies
DEPS = \
 Makefile \
 include/main.h\
 include/GPIO_SS.h\
 include/BB_Setup.h\
 include/SPI_SS.h

# Compiler object files 
COBJ = \
 $(OBJDIR)/main.o \
 $(OBJDIR)/GPIO_SS.o\
 $(OBJDIR)/BB_Setup.o\
 $(OBJDIR)/SPI_SS.o

# gcc binaries to use. Write correct path on your system 
ifeq ($(OS_SYSTEM),LINUX)
CC = "/usr/local/linaro/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc"
LD = "/usr/local/linaro/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc"
else ifeq ($(OS_SYSTEM),WINDOWS)
CC = "C:\gcc-linaro\bin\arm-linux-gnueabihf-gcc.exe"
LD = "C:\gcc-linaro\bin\arm-linux-gnueabihf-gcc.exe"
else ifeq ($(OS_SYSTEM),BEAGLEBONEBLACK)
CC = "gcc"
LD = "gcc"
endif


# rm is part of yagarto-tools
ifeq ($(OS_SYSTEM),WINDOWS)
SHELL = cmd
endif

REMOVE = rm -f


# Compiler options
# Two additional flags neccessary for Angstrom Linux. Don't use them with Ubuntu or Debian  
CFLAGS = -marm
ifeq ($(TARGET),angstrom)
CFLAGS += -march=armv4t
CFLAGS += -mfloat-abi=soft
endif
CFLAGS += -O0 
CFLAGS += -g 
CFLAGS += -I.
CFLAGS += -I$(CINCLUDE)
CFLAGS += $(CDEFINE)

# for a better output
MSG_EMPTYLINE = . 
MSG_COMPILING = ---COMPILE--- 
MSG_LINKING = ---LINK--- 
MSG_SUCCESS = ---SUCCESS--- 

# Our favourite
all: $(PROJECT)

# Linker call
$(PROJECT): $(COBJ)
	@echo $(MSG_EMPTYLINE)
	@echo $(MSG_LINKING)
	$(LD) -o $@ $^ $(CFLAGS)
	@echo $(MSG_EMPTYLINE)
	@echo $(MSG_SUCCESS) $(PROJECT)

# Compiler call
$(COBJ): $(OBJDIR)/%.o: %.c $(DEPS)
	@echo $(MSG_EMPTYLINE)
	@echo $(MSG_COMPILING) $<
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(REMOVE) $(OBJDIR)/*.o
	$(REMOVE) $(PROJECT)

