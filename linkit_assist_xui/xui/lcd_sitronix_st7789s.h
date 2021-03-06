#define LCD_BASE                       (0xA0450000)
#define LCD_INT_ENABLE_REG          (LCD_BASE+0x0004)
#define LCD_START_REG               (LCD_BASE+0x000C)
#define LCD_SIF_CON_REG             (LCD_BASE+0x0028)
#define LCD_ROI_CTRL_REG            (LCD_BASE+0x0080)
#define LCD_SIF_PIX_CON_REG         (LCD_BASE+0x0018)
#define LCD_SERIAL0_A0_LOW_ADDR     (LCD_BASE+0x0F80)
#define LCD_SERIAL0_A0_HIGH_ADDR    (LCD_BASE+0x0F90)

#define LCD_SERIAL_PIX_CONFIG_SIF0_CS_STAY_LOW_BIT	0x0080
#define LCD_ROI_CTRL_CMD_ENABLE_BIT         0x00008000
#define LCD_ROI_CTRL_DATA_FORMAT_MASK       0x020000FF
#define LCD_SERIAL_CONFIG_SIF_HW_CS_CTRL_BIT         0x01000000
#define LCD_SERIAL_PIX_CONFIG_SIF0_SINGLE_A0_BIT		0x0040
#define LCD_SERIAL_CONFIG_SIF0_3WIRE_BIT             0x0008
#define LCD_SERIAL_PIX_CONFIG_SIF0_PIX_2PIN_BIT	 	0x0010
#define LCD_INT_ENABLE_CMD_COMPLETE_BIT     0x0004
#define LCD_START_BIT                       0x8000
#define LCD_SERIAL_CONFIG_SIF0_SIZE_MASK             0x0007
#define LCD_SERIAL_PIX_CONFIG_SIF0_2PIN_SIZE_MASK	0x0007
#define LCD_SERIAL_CONFIG_SIF0_SIZE_OFFSET           0
#define LCD_SERIAL_PIX_CONFIG_SIF0_2PIN_SIZE_OFFSET	 0
#define LCD_SERIAL_CONFIG_SDI_ENALBE_BIT	 0x0010

#define REG_LCD_ROI_CTRL               *((volatile unsigned int *)   (LCD_ROI_CTRL_REG))
#define REG_LCD_SIF_CON_REG             *((volatile unsigned int *)   (LCD_SIF_CON_REG))
#define REG_LCD_SIF_PIX_CON_REG			*((volatile unsigned short *) LCD_SIF_PIX_CON_REG)
#define REG_LCD_INT_ENABLE             *((volatile unsigned short *) (LCD_INT_ENABLE_REG))
#define REG_LCD_START                  *((volatile unsigned short *) (LCD_START_REG))

#define LCD_SET_SERIAL0_IF_SIZE(n)  			REG_LCD_SIF_CON_REG &= ~(LCD_SERIAL_CONFIG_SIF0_SIZE_MASK);\
                                         REG_LCD_SIF_CON_REG |= ((n & LCD_SERIAL_CONFIG_SIF0_SIZE_MASK) << LCD_SERIAL_CONFIG_SIF0_SIZE_OFFSET)

#define LCD_SET_LCD_ROI_CTRL_OUTPUT_FORMAT(n)	REG_LCD_ROI_CTRL &= ~LCD_ROI_CTRL_DATA_FORMAT_MASK;\
                                                REG_LCD_ROI_CTRL |= n;

#define SET_LCD_SERIAL0_IF_2PIN_SIZE(n)	 REG_LCD_SIF_PIX_CON_REG &= ~(LCD_SERIAL_PIX_CONFIG_SIF0_2PIN_SIZE_MASK);\
									 REG_LCD_SIF_PIX_CON_REG |= ((n & LCD_SERIAL_PIX_CONFIG_SIF0_2PIN_SIZE_MASK) << LCD_SERIAL_PIX_CONFIG_SIF0_2PIN_SIZE_OFFSET)

#define LCD_DISABLE_SERIAL0_SDI				REG_LCD_SIF_CON_REG &= ~(LCD_SERIAL_CONFIG_SDI_ENALBE_BIT)
#define LCD_DISABLE_SERIAL0_SINGLE_A0		REG_LCD_SIF_PIX_CON_REG &= ~(LCD_SERIAL_PIX_CONFIG_SIF0_SINGLE_A0_BIT)
#define LCD_DISABLE_SERIAL0_CS_STAY_LOW		REG_LCD_SIF_PIX_CON_REG &= ~(LCD_SERIAL_PIX_CONFIG_SIF0_CS_STAY_LOW_BIT)
#define LCD_ENABLE_SERIAL0_3WIRE			REG_LCD_SIF_CON_REG |= LCD_SERIAL_CONFIG_SIF0_3WIRE_BIT
#define LCD_ENABLE_HW_CS					REG_LCD_SIF_CON_REG |= LCD_SERIAL_CONFIG_SIF_HW_CS_CTRL_BIT
#define LCD_ENABLE_SERIAL0_2PIN				REG_LCD_SIF_PIX_CON_REG |= LCD_SERIAL_PIX_CONFIG_SIF0_PIX_2PIN_BIT
#define LCD_ENABLE_LCD_CMD_COMPLETE_INT		REG_LCD_INT_ENABLE |= LCD_INT_ENABLE_CMD_COMPLETE_BIT;
#define LCD_DISABLE_LCD_ROI_CTRL_CMD_FIRST	REG_LCD_ROI_CTRL &= ~LCD_ROI_CTRL_CMD_ENABLE_BIT;
#define LCD_START_LCD_TRANSFER				REG_LCD_START = 0;\
                                            REG_LCD_START = LCD_START_BIT;

typedef enum {
	LCD_8BIT_8_BPP_RGB332_1 = 0x00, /* RRRGGBB */
	LCD_8BIT_8_BPP_RGB332_2 = 0x01, /* BBGGGRR */
	LCD_8BIT_12_BPP_RGB444_1 = 0x08, /* RRRRGGGG, BBBBRRRR, GGGGBBBB */
	LCD_8BIT_12_BPP_RGB444_2 = 0x0B, /* GGGGRRRR, RRRRBBBB, BBBBGGGG */
	LCD_8BIT_16_BPP_RGB565_1 = 0x10, /* RRRRRGGG, GGGBBBBB */
	LCD_8BIT_16_BPP_RGB565_2 = 0x12, /* GGGBBBBB, RRRRRGGG */
	LCD_8BIT_16_BPP_BGR565_1 = 0x11, /* BBBBBGGG, GGGRRRRR */
	LCD_8BIT_16_BPP_BGR565_2 = 0x13, /* GGGRRRRR, BBBBBGGG */
	LCD_8BIT_18_BPP_RGB666_1 = 0x18, /* RRRRRRXX, GGGGGGXX, BBBBBBXX */
	LCD_8BIT_18_BPP_RGB666_2 = 0x1C, /* XXRRRRRR, XXGGGGGG, XXBBBBBB */
	LCD_8BIT_24_BPP_RGB888_1 = 0x20, /* RRRRRRRR, GGGGGGGG, BBBBBBBB */
	LCD_16BIT_8_BPP_RGB332_1 = 0x40, /* RRRGGGBBRRRGGGBB, MSB first*/
	LCD_16BIT_8_BPP_RGB332_2 = 0x42, /* RRRGGGBBRRRGGGBB, LSB first*/
	LCD_16BIT_8_BPP_RGB332_3 = 0x41, /* BBGGGRRRBBGGGRRR, MSB first */
	LCD_16BIT_8_BPP_RGB332_4 = 0x43, /* BBGGGRRRBBGGGRRR, LSB first */
	LCD_16BIT_12_BPP_RGB444_1 = 0x4C, /* XXXXRRRRGGGGBBBB */
	LCD_16BIT_12_BPP_RGB444_2 = 0x4D, /* XXXXBBBBGGGGRRRR */
	LCD_16BIT_12_BPP_RGB444_3 = 0x48, /* RRRRGGGGBBBBXXXX */
	LCD_16BIT_12_BPP_RGB444_4 = 0x49, /* BBBBGGGGRRRRXXXX */
	LCD_16BIT_16_BPP_RGB565_1 = 0x50, /* RRRRRGGGGGGBBBBB */
	LCD_16BIT_16_BPP_RGB565_2 = 0x52, /* GGGBBBBBRRRRRGGG */
	LCD_16BIT_16_BPP_BGR565_1 = 0x51, /* BBBBBGGGGGGRRRRR */
	LCD_16BIT_16_BPP_BGR565_2 = 0x53, /* GGGRRRRRBBBBBGGG */
	LCD_16BIT_18_BPP_RGB666_1 = 0x5C, /* XXXXRRRRRRGGGGGG, XXXXBBBBBBRRRRRR, XXXXGGGGGGBBBBBB */
	LCD_16BIT_18_BPP_RGB666_2 = 0x5F, /* XXXXGGGGGGRRRRRR, XXXXRRRRRRBBBBBB, XXXXBBBBBBGGGGGG */
	LCD_16BIT_18_BPP_RGB666_3 = 0x58, /* RRRRRRGGGGGGXXXX, BBBBBBRRRRRRXXXX, GGGGGGBBBBBBXXXX */
	LCD_16BIT_18_BPP_RGB666_4 = 0x5B, /* GGGGGGRRRRRRXXXX, RRRRRRBBBBBBXXXX, BBBBBBGGGGGGXXXX */
	LCD_16BIT_24_BPP_RGB888_1 = 0x60, /* RRRRRRRRGGGGGGGG, BBBBBBBBRRRRRRRR, GGGGGGGGBBBBBBBB */
	LCD_16BIT_24_BPP_RGB888_2 = 0x63, /* GGGGGGGGRRRRRRRR, RRRRRRRRBBBBBBBB, BBBBBBBBGGGGGGGG */

	LCD_9BIT_8_BPP_RGB332_1 = 0x80, /* RRRGGGBBX */
	LCD_9BIT_8_BPP_RGB332_2 = 0x81, /* BBGGGRRRX */
	LCD_9BIT_12_BPP_RGB444_1 = 0x88, /* RRRRGGGGX, BBBBRRRRX, GGGGBBBBX */
	LCD_9BIT_12_BPP_RGB444_2 = 0x8B, /* GGGGRRRRX, RRRRBBBBX, BBBBGGGGX */
	LCD_9BIT_16_BPP_RGB565_1 = 0x90, /* RRRRRGGGX, GGGBBBBBX */
	LCD_9BIT_16_BPP_RGB565_2 = 0x93, /* GGGRRRRRX, BBBBBGGGX */
	LCD_9BIT_18_BPP_RGB666_1 = 0x98, /* RRRRRRGGG, GGGBBBBBB */
	LCD_9BIT_18_BPP_RGB666_2 = 0x9B, /* GGGRRRRRR, BBBBBBGGG */
	LCD_9BIT_24_BPP_RGB888_1 = 0xA0, /* RRRRRRRRX, GGGGGGGGX, BBBBBBBBX */
	LCD_18BIT_8_BPP_RGB332_1 = 0xC0, /* RRRGGGBBRRRGGGBBXX, MSB first */
	LCD_18BIT_8_BPP_RGB332_2 = 0xC2, /* RRRGGGBBRRRGGGBBXX, LSB first */
	LCD_18BIT_8_BPP_RGB332_3 = 0xC1, /* BBGGGRRRBBGGGRRRXX, MSB first */
	LCD_18BIT_8_BPP_RGB332_4 = 0xC3, /* BBGGGRRRBBGGGRRRXX, LSB first */
	LCD_18BIT_12_BPP_RGB444_1 = 0xCC, /* XXXXXXRRRRGGGGBBBB */
	LCD_18BIT_12_BPP_RGB444_2 = 0xCD, /* XXXXXXBBBBGGGGRRRR */
	LCD_18BIT_12_BPP_RGB444_3 = 0xC8, /* RRRRGGGGBBBBXXXXXX */
	LCD_18BIT_12_BPP_RGB444_4 = 0xC9, /* BBBBGGGGRRRRXXXXXX */
	LCD_18BIT_16_BPP_RGB565_1 = 0xD0, /* RRRRRGGGGGGBBBBBXX */
	LCD_18BIT_16_BPP_RGB565_2 = 0xD1, /* BBBBBGGGGGGRRRRRXX */
	LCD_18BIT_18_BPP_RGB666_1 = 0xD8, /* RRRRRRGGGGGGBBBBBB */
	LCD_18BIT_18_BPP_RGB666_2 = 0xD9, /* BBBBBBGGGGGGRRRRRR */
	LCD_18BIT_24_BPP_RGB888_1 = 0xE0, /* RRRRRRRRGGGGGGGGXX, BBBBBBBBRRRRRRRRXX, GGGGGGGGBBBBBBBBXX */
	LCD_18BIT_24_BPP_RGB888_2 = 0xE3 /* GGGGGGGGRRRRRRRRXX, RRRRRRRRBBBBBBBBXX, BBBBBBBBGGGGGGGGXX */
} LCD_OUTPUT_FMT;

typedef enum {
	LCD_SERIAL_CLOCK_132MHZ = 0,
	LCD_SERIAL_CLOCK_104MHZ = 1,
	LCD_SERIAL_CLOCK_91MHZ = 2,
	LCD_SERIAL_CLOCK_78MHZ = 3,
	LCD_SERIAL_CLOCK_65MHZ = 4,
	LCD_SERIAL_CLOCK_26MHZ = 5,
	LCD_SERIAL_CLOCK_RSVD = 6
} LCD_SERIAL_IF_CLOCK;

typedef enum {
	LCD_SCNF_IF_WIDTH_8 = 0,
	LCD_SCNF_IF_WIDTH_9,
	LCD_SCNF_IF_WIDTH_16,
	LCD_SCNF_IF_WIDTH_18,
	LCD_SCNF_IF_WIDTH_24,
	LCD_SCNF_IF_WIDTH_32
} LCD_SCNF_IF_WIDTH;

typedef enum {
	LCD_DRIVING_4MA = 0,
	LCD_DRIVING_8MA = 1,
	LCD_DRIVING_12MA = 2,
	LCD_DRIVING_16MA = 3
} LCD_DRIVING_CURRENT;

typedef enum {
	LCD_SCNF_IF_2PIN_WIDTH_16 = 2,
	LCD_SCNF_IF_2PIN_WIDTH_18 = 3,
	LCD_SCNF_IF_2PIN_WIDTH_24 = 4,
	LCD_SCNF_IF_2PIN_WIDTH_12 = 6
} LCD_SCNF_IF_2PIN_WIDTH;

void lcd_st7789s_init(void);
