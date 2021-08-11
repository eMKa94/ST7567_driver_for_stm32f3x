/*
    Board_lcd.h
    Board lcd driver header file
*/

#include "board_lcd.h"
#include "board_core.h"

#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_spi.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_dma.h"


uint8_t board_lcd_cmd_buffer[BOARD_LCD_CMDn];
uint8_t *board_lcd_data_buffer;
uint8_t board_lcd_position_buffer[3];

uint64_t board_lcd_disable_time_ms = 0;

bool board_lcd_transmission_in_progress = false;
bool board_lcd_active_flag = false;

#define BOARD_SPI_PORT      SPI3
#define BOARD_SPI_DMA_PORT  DMA2

// Display reset signal
#define BOARD_LCD_RST_GPIO_PORT    GPIOA
#define BOARD_LCD_RST_GPIO_PIN     LL_GPIO_PIN_0
ErrorStatus board_lcd_rst_signal_init(void);
#define board_lcd_rst_signal_set_low()              LL_GPIO_ResetOutputPin(BOARD_LCD_RST_GPIO_PORT, BOARD_LCD_RST_GPIO_PIN)
#define board_lcd_rst_signal_set_high()             LL_GPIO_SetOutputPin(BOARD_LCD_RST_GPIO_PORT, BOARD_LCD_RST_GPIO_PIN)

// Chip select signal
#define BOARD_LCD_CS_GPIO_PORT     GPIOA
#define BOARD_LCD_CS_GPIO_PIN      LL_GPIO_PIN_2
ErrorStatus board_lcd_cs_signal_init(void);
#define board_lcd_cs_signal_set_low()               LL_GPIO_ResetOutputPin(BOARD_LCD_CS_GPIO_PORT, BOARD_LCD_CS_GPIO_PIN)
#define board_lcd_cs_signal_set_high()              LL_GPIO_SetOutputPin(BOARD_LCD_CS_GPIO_PORT, BOARD_LCD_CS_GPIO_PIN)

// A0 signal
#define BOARD_LCD_A0_GPIO_PORT     GPIOC
#define BOARD_LCD_A0_GPIO_PIN      LL_GPIO_PIN_3
ErrorStatus board_lcd_a0_signal_init(void);
#define board_lcd_a0_signal_set_low()               LL_GPIO_ResetOutputPin(BOARD_LCD_A0_GPIO_PORT, BOARD_LCD_A0_GPIO_PIN)
#define board_lcd_a0_signal_set_high()              LL_GPIO_SetOutputPin(BOARD_LCD_A0_GPIO_PORT, BOARD_LCD_A0_GPIO_PIN)

// Backlight signal
#define BOARD_LCD_BL_GPIO_PORT     GPIOC
#define BOARD_LCD_BL_GPIO_PIN      LL_GPIO_PIN_2
ErrorStatus board_lcd_backlight_signal_init(void);
#define board_lcd_backlight_signal_set_low()        LL_GPIO_ResetOutputPin(BOARD_LCD_BL_GPIO_PORT, BOARD_LCD_BL_GPIO_PIN)   
#define board_lcd_backlight_signal_set_high()       LL_GPIO_SetOutputPin(BOARD_LCD_BL_GPIO_PORT, BOARD_LCD_BL_GPIO_PIN)
#define board_lcd_backlight_signal_toggle()         LL_GPIO_TogglePin(BOARD_LCD_BL_GPIO_PORT, BOARD_LCD_BL_GPIO_PIN)

// SPI Interface

// MOSI signal
#define BOARD_SPI_MOSI_GPIO_PORT   GPIOA
#define BOARD_SPI_MOSI_GPIO_PIN    LL_GPIO_PIN_3
ErrorStatus board_spi_interface_mosi_signal_init(void);

// SCK signal
#define BOARD_SPI_SCK_GPIO_PORT   GPIOA
#define BOARD_SPI_SCK_GPIO_PIN    LL_GPIO_PIN_1
ErrorStatus board_spi_interface_sck_signal_init(void);

#define board_spi_interface_clock_enable()          LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3)
ErrorStatus board_spi_interface_init(void);
#define board_spi_interface_enable()                LL_SPI_Enable(BOARD_SPI_PORT)
#define board_spi_interface_disable()               LL_SPI_Disable(BOARD_SPI_PORT)
#define board_spi_interface_DMA_request()           LL_SPI_EnableDMAReq_TX(BOARD_SPI_PORT)
void board_spi_send_byte(uint8_t byte);

#define board_spi_dma_clock_enable()                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2)
ErrorStatus board_spi_dma_channel_init(void);
#define board_spi_dma_channel_enable()              LL_DMA_EnableChannel(BOARD_SPI_DMA_PORT, LL_DMA_CHANNEL_2);
#define board_spi_dma_channel_disable()             LL_DMA_DisableChannel(BOARD_SPI_DMA_PORT, LL_DMA_CHANNEL_2);
#define board_spi_dma_set_source_address(address)   LL_DMA_SetMemoryAddress(BOARD_SPI_DMA_PORT, LL_DMA_CHANNEL_2, (uint32_t)address);
#define board_spi_dma_set_data_qty(ndata)           LL_DMA_SetDataLength(BOARD_SPI_DMA_PORT, LL_DMA_CHANNEL_2, ndata);

typedef enum lcd_current_transfer_type_t
{
    LCD_PAGE_DATA,
    LCD_POSITION_DATA,
    LCD_CONFIG_DATA
}lcd_current_transfer_type_t;
lcd_current_transfer_type_t lcd_current_transfer_type = LCD_CONFIG_DATA;

#define board_lcd_set_transfer_type(transfer_type)  lcd_current_transfer_type=transfer_type

void board_lcd_transfer_prepare(lcd_current_transfer_type_t transfer_type, uint32_t *data_source_address, uint32_t ndata);
void board_lcd_tranfer_continue(void);
void board_lcd_send_byte_pooling(uint8_t byte);

void board_lcd_page_data_transfer_finish_handler(void);
void board_lcd_position_data_transfer_finish_handler(void);
void board_lcd_config_data_transfer_finish_handler(void);
void board_lcd_position_buffer_update(uint8_t column, uint8_t page);

uint8_t lcd_page_counter = 0;

void board_lcd_init(void)
{
    board_wait_ms(2);
    board_lcd_a0_signal_init();
    board_lcd_backlight_signal_init();
    board_lcd_cs_signal_init();
    board_lcd_rst_signal_init();
    board_spi_interface_mosi_signal_init();
    board_spi_interface_sck_signal_init();
    board_spi_interface_init();
    board_spi_dma_channel_init();

    LL_DMA_EnableIT_TC(DMA2, LL_DMA_CHANNEL_2);
    //NVIC_EnableIRQ(DMA2_Channel2_IRQn);
    
    board_spi_interface_DMA_request();

    //board_spi_dma_channel_enable();
    board_spi_interface_enable();
   
    board_lcd_hardware_reset();
    board_lcd_send_config_data();

}

void board_lcd_hardware_reset(void)
{
    board_lcd_rst_signal_set_low();
    board_wait_ms(1);
    board_lcd_rst_signal_set_high();
    board_wait_ms(1);
}

void board_lcd_set_display_data_address(uint8_t *data_address)
{
    board_lcd_data_buffer = data_address;
}
// SPI Interface

// MOSI signal
ErrorStatus board_spi_interface_mosi_signal_init(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(BOARD_SPI_MOSI_GPIO_PORT, BOARD_SPI_MOSI_GPIO_PIN, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(BOARD_SPI_MOSI_GPIO_PORT, BOARD_SPI_MOSI_GPIO_PIN, LL_GPIO_AF_6);

    return(SUCCESS);
}

// SCK signal
ErrorStatus board_spi_interface_sck_signal_init(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(BOARD_SPI_SCK_GPIO_PORT, BOARD_SPI_SCK_GPIO_PIN, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(BOARD_SPI_SCK_GPIO_PORT, BOARD_SPI_SCK_GPIO_PIN, LL_GPIO_AF_6);

    return(SUCCESS);
}
// Display reset signal

ErrorStatus board_lcd_rst_signal_init(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_InitTypeDef board_lcd_rst_signal_gpio;
    board_lcd_rst_signal_gpio.Pin  = BOARD_LCD_RST_GPIO_PIN;
    board_lcd_rst_signal_gpio.Mode = LL_GPIO_MODE_OUTPUT;
    return(LL_GPIO_Init(BOARD_LCD_RST_GPIO_PORT, &board_lcd_rst_signal_gpio));
}

// Chip select signal

ErrorStatus board_lcd_cs_signal_init(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_InitTypeDef board_lcd_cs_signal_gpio;
    board_lcd_cs_signal_gpio.Pin   = BOARD_LCD_CS_GPIO_PIN;
    board_lcd_cs_signal_gpio.Mode  = LL_GPIO_MODE_OUTPUT;
    return(LL_GPIO_Init(BOARD_LCD_CS_GPIO_PORT, &board_lcd_cs_signal_gpio));
}

// A0 signal

ErrorStatus board_lcd_a0_signal_init(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_InitTypeDef board_lcd_a0_signal_gpio;
    board_lcd_a0_signal_gpio.Pin   = BOARD_LCD_A0_GPIO_PIN;
    board_lcd_a0_signal_gpio.Mode  = LL_GPIO_MODE_OUTPUT;
    return(LL_GPIO_Init(BOARD_LCD_A0_GPIO_PORT, &board_lcd_a0_signal_gpio));
}

// Backlight signal

ErrorStatus board_lcd_backlight_signal_init(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_GPIO_InitTypeDef board_lcd_bl_signal_gpio;
    board_lcd_bl_signal_gpio.Pin   = BOARD_LCD_BL_GPIO_PIN;
    board_lcd_bl_signal_gpio.Mode  = LL_GPIO_MODE_OUTPUT;
    return(LL_GPIO_Init(BOARD_LCD_BL_GPIO_PORT, &board_lcd_bl_signal_gpio));
}

// SPI Interface

ErrorStatus board_spi_interface_init(void)
{
    LL_SPI_InitTypeDef board_spi_conf;

    board_spi_interface_clock_enable();

    board_spi_conf.TransferDirection  =   LL_SPI_FULL_DUPLEX;
    board_spi_conf.Mode               =   LL_SPI_MODE_MASTER;
    board_spi_conf.DataWidth          =   LL_SPI_DATAWIDTH_8BIT;
    board_spi_conf.ClockPolarity      =   LL_SPI_POLARITY_HIGH;
    board_spi_conf.ClockPhase         =   LL_SPI_PHASE_2EDGE;
    board_spi_conf.NSS                =   LL_SPI_NSS_SOFT;
    board_spi_conf.BaudRate           =   LL_SPI_BAUDRATEPRESCALER_DIV64;
    board_spi_conf.BitOrder           =   LL_SPI_MSB_FIRST;
    board_spi_conf.CRCCalculation     =   LL_SPI_CRCCALCULATION_DISABLE;
    board_spi_conf.CRCPoly            =   0;

    return(LL_SPI_Init(BOARD_SPI_PORT, &board_spi_conf));

}

LL_DMA_InitTypeDef board_spi_dma_channel_conf;

ErrorStatus board_spi_dma_channel_init(void)
{
    board_spi_dma_clock_enable();

    board_spi_dma_channel_conf.Mode                     = LL_DMA_MODE_NORMAL;
    board_spi_dma_channel_conf.Direction                = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    board_spi_dma_channel_conf.MemoryOrM2MDstAddress    = (uint32_t)board_lcd_data_buffer;
    board_spi_dma_channel_conf.MemoryOrM2MDstDataSize   = LL_DMA_MDATAALIGN_BYTE;
    board_spi_dma_channel_conf.MemoryOrM2MDstIncMode    = LL_DMA_MEMORY_INCREMENT;
    board_spi_dma_channel_conf.PeriphOrM2MSrcAddress    = (uint32_t)&(BOARD_SPI_PORT->DR);
    board_spi_dma_channel_conf.PeriphOrM2MSrcDataSize   = LL_DMA_PDATAALIGN_BYTE;
    board_spi_dma_channel_conf.PeriphOrM2MSrcIncMode    = LL_DMA_PERIPH_NOINCREMENT;
    board_spi_dma_channel_conf.NbData                   = 10;
    
    return(LL_DMA_Init(BOARD_SPI_DMA_PORT, LL_DMA_CHANNEL_2, &board_spi_dma_channel_conf));
}

void board_lcd_page_data_transfer_finish_handler(void)
{
    if (lcd_page_counter < (BOARD_LCD_PAGEn - 1))
    {
        board_lcd_position_buffer_update(0, ++lcd_page_counter);
        board_lcd_transfer_prepare(LCD_POSITION_DATA, (uint32_t *)&board_lcd_position_buffer, 3);
    }
    else
    {
        lcd_page_counter = 0;
        board_spi_dma_channel_disable();
        board_lcd_transmission_in_progress = false;
    }

}

void board_lcd_position_data_transfer_finish_handler(void)
{
    board_lcd_transfer_prepare(LCD_PAGE_DATA, (uint32_t *)(board_lcd_data_buffer+(lcd_page_counter * BOARD_LCD_COLUMNn)), BOARD_LCD_COLUMNn);
}

void board_lcd_config_data_transfer_finish_handler(void)
{
    board_spi_dma_channel_disable();
}

#define LCD_PAGE_CMD_MASK   0xB0
#define LCD_COLUMN_LSB_MASK 0x00
#define LCD_COLUMN_MSB_MASK 0x10

void board_lcd_position_buffer_update(uint8_t column, uint8_t page)
{
    board_lcd_position_buffer[0] = LCD_PAGE_CMD_MASK | page;
    board_lcd_position_buffer[1] = LCD_COLUMN_LSB_MASK | (column & 0xF0);
    board_lcd_position_buffer[2] = LCD_COLUMN_MSB_MASK | (column >> 4);
}

void board_lcd_send_display_data(void)
{
    lcd_page_counter = 0;
    board_lcd_position_buffer_update(0,0);
    board_lcd_transmission_in_progress = true;
    board_lcd_transfer_prepare(LCD_POSITION_DATA, (uint32_t *)&board_lcd_position_buffer, 3);
}

void board_lcd_send_config_data(void)
{
    board_lcd_send_byte_pooling(BOARD_LCD_CMD_POWER_ON);
    board_lcd_send_byte_pooling(BOARD_LCD_CMD_ALL_PIX_OFF);
    board_lcd_send_byte_pooling(BOARD_LCD_CMD_BIAS_1D7);
    board_lcd_send_byte_pooling(BOARD_LCD_CMD_DISPLAY_NORMAL);
    board_lcd_send_byte_pooling(BOARD_LCD_CMD_COM_REVERSE);
    board_lcd_send_byte_pooling(BOARD_LCD_CMD_SEG_DIR_NORM);
    board_lcd_send_byte_pooling(BOARD_LCD_CMD_DISP_ON);
}


bool board_lcd_check_if_active(void)
{
    return(board_lcd_active_flag);
}

void board_lcd_transfer_prepare(lcd_current_transfer_type_t transfer_type, uint32_t *data_source_address, uint32_t ndata)
{
    board_lcd_set_transfer_type(transfer_type);
    board_spi_dma_channel_disable();
    board_spi_dma_set_source_address(data_source_address);
    board_spi_dma_set_data_qty(ndata);
    if (transfer_type == LCD_PAGE_DATA) board_lcd_a0_signal_set_high();
    else                                board_lcd_a0_signal_set_low();
    board_spi_dma_channel_enable();
}

void board_lcd_tranfer_continue(void)
{
    switch (lcd_current_transfer_type)
    {
    case LCD_PAGE_DATA:
        board_lcd_page_data_transfer_finish_handler();
        break;
    case LCD_POSITION_DATA:
        board_lcd_position_data_transfer_finish_handler();
        break;
    case LCD_CONFIG_DATA:
        board_lcd_config_data_transfer_finish_handler();
        break;    
    default:
        break;
    }
}

void board_lcd_send_byte_pooling(uint8_t byte)
{
    while(!LL_SPI_IsActiveFlag_TXE(BOARD_SPI_PORT)){};
	LL_SPI_TransmitData8(BOARD_SPI_PORT, byte);
}

void board_lcd_activate(void)
{
    board_lcd_active_flag = true;
}

void board_lcd_deactivate(void)
{
    board_lcd_active_flag = false;
}

inline void board_lcd_backlight_enable(uint32_t lcd_on_time_ms)
{
    board_lcd_disable_time_ms = board_runtime_ms + (uint64_t)lcd_on_time_ms;
    board_lcd_backlight_signal_set_high();
}

inline void board_lcd_backlight_disable(void)
{
    board_lcd_backlight_signal_set_low();
}

void board_lcd_routine(void)
{
    if (board_lcd_transmission_in_progress)
    {
        if (!LL_SPI_IsActiveFlag_BSY(BOARD_SPI_PORT))
        {
            board_lcd_tranfer_continue();
        }
    }

    if (board_runtime_ms >= board_lcd_disable_time_ms) board_lcd_backlight_disable();
}

void boardLcdRefresh(void)
{
	if ((!board_lcd_transmission_in_progress) && board_lcd_check_if_active())
	{
		board_lcd_send_display_data();
	}    
}