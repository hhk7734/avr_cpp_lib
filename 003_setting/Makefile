TARGET = main
MCU_TARGET =
MCU_F_CPU  =

PROGRAMMER = 
PORT = /dev/cu.
BITCLOCK = 0.5 # ms 단위
BAUDRATE = 115200
UPLOAD = avrdude -p$(MCU_TARGET) -c$@ -v -Uflash:w:$(TARGET).elf:e
UPLOAD += -D

CC = avr-gcc
C_FLAGS = -fpack-struct

CXX = avr-g++
CXX_FLAGS = -std=c++11

CFLAGS  = -g -Wall -mmcu=$(MCU_TARGET) -DF_CPU=$(MCU_F_CPU)
CFLAGS += -O1
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -fshort-enums

LDFLAGS  = -g -mmcu=$(MCU_TARGET) -Wl,-Map,$(TARGET).map
LDFLAGS += -Wl,--gc-sections

# $(call rwildcard,directory,filetype) directory 하위에서 형식에 맞는 모든 파일을 나열
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

C_SRCS  = $(call rwildcard,,*.c)
CXX_SRCS = $(call rwildcard,,*.cpp)
HEADER = $(call rwildcard,,*.h)

OBJDIR = obj

C_OBJS  = $(addprefix $(OBJDIR)/,$(notdir $(patsubst %.c,%.o,$(C_SRCS))))
CXX_OBJS = $(addprefix $(OBJDIR)/,$(notdir $(patsubst %.cpp,%.o,$(CXX_SRCS))))

# 경로 추가
CFLAGS += $(addprefix -I,$(dir $(HEADER)))
CFLAGS += $(addprefix -I,$(dir $(C_SRCS)))
CFLAGS += $(addprefix -I,$(dir $(CXX_SRCS)))

BUILD = $(TARGET)

.PHONY: clean upload

build: $(BUILD)
	avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	avr-objdump -h -S "$(TARGET).elf" > "$(TARGET).lss"
	avr-size -C --mcu=$(MCU_TARGET) $(TARGET).elf

all: clean build upload
clean: ;rm -rf $(TARGET).* $(OBJDIR)

# 업로드
upload: $(PROGRAMMER)
stk500: ;$(UPLOAD) -P$(PORT)
avrispmkII: ;$(UPLOAD) -B$(BITCLOCK)
# arduino uno, nano, promini
arduino: ;$(UPLOAD) -P$(PORT) -b$(BAUDRATE)
# arduino mega
wiring: ;$(UPLOAD) -P$(PORT) -b$(BAUDRATE)
atmelice_isp: ;$(UPLOAD)

# 링크
$(TARGET): $(C_OBJS) $(CXX_OBJS)
	$(CC) $(LDFLAGS) -o $@.elf $^

# C 소스 컴파일
vpath %.c $(sort $(dir $(C_SRCS)))
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(C_FLAGS) -c -o $@ $<

# CPP 소스 컴파일
vpath %.cpp $(sort $(dir $(CXX_SRCS)))
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CFLAGS) $(CXX_FLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir $(OBJDIR)