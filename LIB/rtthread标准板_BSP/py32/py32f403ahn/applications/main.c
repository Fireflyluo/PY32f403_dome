#include <rtthread.h>
#include <rtdevice.h>
#include <stdlib.h>
#include <string.h>

#include <py32f4xx_hal.h>

#include "board.h"
#include "drv_spi.h"
#include "drv_usart.h"
#include "drv_dma.h"

#define LED_PIN GET_PIN(B, 2)

#define SPI1_FLASH_CS_PIN GET_PIN(A, 4)
#define SPI3_RF_CS_PIN GET_PIN(B, 1)

static struct rt_spi_device *ensure_spi_device(const char *bus_name,
                                                const char *dev_name,
                                                rt_base_t cs_pin,
                                                rt_uint32_t max_hz)
{
    struct rt_spi_device *spi_dev;
    struct rt_spi_configuration cfg;

    spi_dev = (struct rt_spi_device *)rt_device_find(dev_name);
    if (spi_dev == RT_NULL)
    {
        if (rt_hw_spi_device_attach(bus_name, dev_name, cs_pin) != RT_EOK)
        {
            return RT_NULL;
        }
        spi_dev = (struct rt_spi_device *)rt_device_find(dev_name);
    }

    if (spi_dev == RT_NULL)
    {
        return RT_NULL;
    }

    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
    cfg.max_hz = max_hz;
    if (rt_spi_configure(spi_dev, &cfg) != RT_EOK)
    {
        return RT_NULL;
    }

    return spi_dev;
}

static int cmd_spi_flash(int argc, char **argv)
{
    rt_uint8_t tx = 0x9FU;
    rt_uint8_t jedec[3] = {0};
    struct rt_spi_device *spi;

    RT_UNUSED(argc);
    RT_UNUSED(argv);

    spi = ensure_spi_device("spi1", "spi10", SPI1_FLASH_CS_PIN, 18000000);
    if (spi == RT_NULL)
    {
        rt_kprintf("spi1 attach/config failed\n");
        return -RT_ERROR;
    }

    if (rt_spi_send_then_recv(spi, &tx, 1, jedec, 3) != RT_EOK)
    {
        rt_kprintf("spi1 transfer failed\n");
        return -RT_ERROR;
    }

    rt_kprintf("W25Q128 JEDEC: %02X %02X %02X\n", jedec[0], jedec[1], jedec[2]);
    return RT_EOK;
}
MSH_CMD_EXPORT(cmd_spi_flash, Read W25Q128 JEDEC ID via spi1);

static int cmd_i2c_scan(int argc, char **argv)
{
    struct rt_i2c_bus_device *bus;
    int found = 0;
    int addr;

    RT_UNUSED(argc);
    RT_UNUSED(argv);

    bus = rt_i2c_bus_device_find("hwi2c2");
    if (bus == RT_NULL)
    {
        rt_kprintf("hwi2c2 not found\n");
        return -RT_ERROR;
    }

    rt_kprintf("I2C2 scan:\n");
    for (addr = 1; addr < 127; addr++)
    {
        struct rt_i2c_msg msg;

        msg.addr = (rt_uint16_t)addr;
        msg.flags = RT_I2C_WR;
        msg.buf = RT_NULL;
        msg.len = 0;

        if (rt_i2c_transfer(bus, &msg, 1) == 1)
        {
            rt_kprintf("  0x%02X\n", addr);
            found++;
        }
    }

    rt_kprintf("Found %d device(s)\n", found);
    return RT_EOK;
}
MSH_CMD_EXPORT(cmd_i2c_scan, Scan i2c bus hwi2c2);

/* ========== WS2812 (PA1, TIM2_CH2 PWM + DMA) ========== */
#define WS2812_NUM 8
#define WS2812_RESET_SLOTS 64
#define WS2812_DMA_CHANNEL DMA2_Channel3
#define WS2812_DMA_IRQ DMA2_Channel3_IRQn
#define WS2812_TIM_DMA_REMAP DMA_CHANNEL_MAP_TIM2_CH2

static uint16_t ws2812_buf[WS2812_NUM * 24 + WS2812_RESET_SLOTS];
static uint32_t ws2812_color[WS2812_NUM];
static TIM_HandleTypeDef htim2;
static DMA_HandleTypeDef hdma_tim2_ch2;
static struct rt_completion ws2812_done;
static rt_bool_t ws2812_inited = RT_FALSE;

void DMA2_Channel3_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_DMA_IRQHandler(&hdma_tim2_ch2);
    rt_interrupt_leave();
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if ((htim != RT_NULL) && (htim->Instance == TIM2))
    {
        rt_completion_done(&ws2812_done);
    }
}

static rt_err_t ws2812_hw_init(void)
{
    TIM_OC_InitTypeDef oc = {0};
    struct dma_config ws_dma_cfg = {0};
    GPIO_InitTypeDef gpio = {0};

    if (ws2812_inited)
    {
        return RT_EOK;
    }

    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_1;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF6_TIM2;
    HAL_GPIO_Init(GPIOA, &gpio);

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 89;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
    {
        return -RT_ERROR;
    }

    oc.OCMode = TIM_OCMODE_PWM1;
    oc.Pulse = 0;
    oc.OCPolarity = TIM_OCPOLARITY_HIGH;
    oc.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &oc, TIM_CHANNEL_2) != HAL_OK)
    {
        return -RT_ERROR;
    }

    ws_dma_cfg.Instance = WS2812_DMA_CHANNEL;
    ws_dma_cfg.dma_rcc = RCC_AHB1ENR_DMA2EN;
    ws_dma_cfg.dma_irq = WS2812_DMA_IRQ;
    ws_dma_cfg.dma_remap = WS2812_TIM_DMA_REMAP;
    py32_dma_apply_remap(&ws_dma_cfg);

    __HAL_RCC_DMA2_CLK_ENABLE();
    rt_memset(&hdma_tim2_ch2, 0, sizeof(hdma_tim2_ch2));
    hdma_tim2_ch2.Instance = WS2812_DMA_CHANNEL;
    hdma_tim2_ch2.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim2_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim2_ch2.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim2_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_tim2_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_tim2_ch2.Init.Mode = DMA_NORMAL;
    hdma_tim2_ch2.Init.Priority = DMA_PRIORITY_HIGH;

    if (HAL_DMA_DeInit(&hdma_tim2_ch2) != HAL_OK)
    {
        return -RT_ERROR;
    }
    if (HAL_DMA_Init(&hdma_tim2_ch2) != HAL_OK)
    {
        return -RT_ERROR;
    }

    __HAL_LINKDMA(&htim2, hdma[TIM_DMA_ID_CC2], hdma_tim2_ch2);
    HAL_NVIC_SetPriority(WS2812_DMA_IRQ, 2, 0);
    HAL_NVIC_EnableIRQ(WS2812_DMA_IRQ);

    rt_completion_init(&ws2812_done);
    ws2812_inited = RT_TRUE;
    return RT_EOK;
}

static void ws2812_fill_buffer(void)
{
    rt_size_t i;
    rt_size_t bit_index = 0;

    for (i = 0; i < WS2812_NUM; i++)
    {
        rt_uint32_t grb = ((ws2812_color[i] & 0x00FF00U) << 8) |
                          (ws2812_color[i] & 0xFF0000U) |
                          (ws2812_color[i] & 0x0000FFU);
        int b;

        for (b = 0; b < 24; b++)
        {
            ws2812_buf[bit_index++] = (grb & 0x800000U) ? 58U : 29U;
            grb <<= 1;
        }
    }

    while (bit_index < (WS2812_NUM * 24 + WS2812_RESET_SLOTS))
    {
        ws2812_buf[bit_index++] = 0U;
    }
}

static rt_err_t ws2812_show(void)
{
    rt_tick_t wait_tick = rt_tick_from_millisecond(20);

    if (wait_tick == 0)
    {
        wait_tick = 1;
    }

    if (ws2812_hw_init() != RT_EOK)
    {
        return -RT_ERROR;
    }

    ws2812_fill_buffer();
    rt_completion_init(&ws2812_done);

    if (HAL_TIM_PWM_Start_DMA(&htim2,
                              TIM_CHANNEL_2,
                              (uint32_t *)ws2812_buf,
                              sizeof(ws2812_buf) / sizeof(ws2812_buf[0])) != HAL_OK)
    {
        return -RT_ERROR;
    }

    if (rt_completion_wait(&ws2812_done, wait_tick) != RT_EOK)
    {
        HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_2);
        return -RT_ETIMEOUT;
    }

    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_2);
    return RT_EOK;
}

static int cmd_ws2812(int argc, char **argv)
{
    int n;
    uint32_t v;

    if (argc < 3)
    {
        rt_kprintf("Usage: cmd_ws2812 <0-7> <RRGGBB>\n");
        return -RT_ERROR;
    }

    n = atoi(argv[1]);
    if (n < 0 || n >= WS2812_NUM)
    {
        rt_kprintf("Invalid index: %d\n", n);
        return -RT_ERROR;
    }

    v = (uint32_t)strtoul(argv[2], RT_NULL, 16) & 0xFFFFFFU;
    ws2812_color[n] = v;
    if (ws2812_show() != RT_EOK)
    {
        rt_kprintf("ws2812 dma update failed\n");
        return -RT_ERROR;
    }

    rt_kprintf("WS2812[%d] = #%06lX\n", n, (unsigned long)v);
    return RT_EOK;
}
MSH_CMD_EXPORT(cmd_ws2812, Set WS2812 LED color by TIM2 PWM DMA);

static rt_err_t spi_dma_probe_once(struct rt_spi_device *spi, const char *name)
{
    struct rt_spi_message msg;
    rt_uint8_t tx[64];
    rt_uint8_t rx[64];
    int i;

    for (i = 0; i < (int)sizeof(tx); i++)
    {
        tx[i] = (rt_uint8_t)(0x80U + i);
    }
    tx[0] = 0x9FU;
    rt_memset(rx, 0, sizeof(rx));

    rt_memset(&msg, 0, sizeof(msg));
    msg.send_buf = tx;
    msg.recv_buf = rx;
    msg.length = sizeof(tx);
    msg.cs_take = 1;
    msg.cs_release = 1;
    msg.next = RT_NULL;

    if (rt_spi_transfer_message(spi, &msg) != RT_NULL)
    {
        rt_kprintf("%s dma xfer failed\n", name);
        return -RT_ERROR;
    }

    rt_kprintf("%s dma ok, rx[0..3]=%02X %02X %02X %02X\n",
               name, rx[0], rx[1], rx[2], rx[3]);
    return RT_EOK;
}

static int cmd_dma_probe(int argc, char **argv)
{
    struct rt_spi_device *spi1;
    struct rt_spi_device *spi3;
    struct rt_i2c_bus_device *bus;
    int addr;
    int hit_addr[8];
    int hit_cnt = 0;
    rt_err_t i2c_ok = -RT_ERROR;
    rt_uint8_t reg = 0;
    rt_uint8_t rx[2] = {0};

    RT_UNUSED(argc);
    RT_UNUSED(argv);

    spi1 = ensure_spi_device("spi1", "spi10", SPI1_FLASH_CS_PIN, 18000000);
    if (spi1 != RT_NULL)
    {
        spi_dma_probe_once(spi1, "spi1");
    }
    else
    {
        rt_kprintf("spi1 missing\n");
    }

    spi3 = ensure_spi_device("spi3", "spi30", SPI3_RF_CS_PIN, 4000000);
    if (spi3 != RT_NULL)
    {
        spi_dma_probe_once(spi3, "spi3");
    }
    else
    {
        rt_kprintf("spi3 missing\n");
    }

#ifdef RT_SERIAL_USING_DMA
    {
        const char *msg = "[uart2 dma] TX test: 0123456789ABCDEF\r\n";
        rt_err_t ret = rt_hw_usart_dma_test_send("uart2", (const rt_uint8_t *)msg, strlen(msg));
        rt_kprintf("uart2 dma tx: %s\n", (ret == RT_EOK) ? "ok" : "failed");
    }
#endif

    bus = rt_i2c_bus_device_find("hwi2c2");
    if (bus == RT_NULL)
    {
        rt_kprintf("hwi2c2 missing\n");
        return -RT_ERROR;
    }

    for (addr = 1; addr < 127; addr++)
    {
        struct rt_i2c_msg ping = {0};
        ping.addr = (rt_uint16_t)addr;
        ping.flags = RT_I2C_WR;
        ping.buf = RT_NULL;
        ping.len = 0;
        if (rt_i2c_transfer(bus, &ping, 1) == 1)
        {
            if (hit_cnt < (int)(sizeof(hit_addr) / sizeof(hit_addr[0])))
            {
                hit_addr[hit_cnt++] = addr;
            }
        }
    }

    for (addr = 0; addr < hit_cnt; addr++)
    {
        struct rt_i2c_msg msgs[2];
        msgs[0].addr = (rt_uint16_t)hit_addr[addr];
        msgs[0].flags = RT_I2C_WR;
        msgs[0].buf = &reg;
        msgs[0].len = 1;
        msgs[1].addr = (rt_uint16_t)hit_addr[addr];
        msgs[1].flags = RT_I2C_RD;
        msgs[1].buf = rx;
        msgs[1].len = sizeof(rx);

        if (rt_i2c_transfer(bus, msgs, 2) == 2)
        {
            rt_kprintf("i2c2 dma probe ok @0x%02X, data=%02X %02X\n",
                       hit_addr[addr], rx[0], rx[1]);
            i2c_ok = RT_EOK;
            break;
        }
    }

    if (i2c_ok != RT_EOK)
    {
        rt_kprintf("i2c2 dma probe: no readable target via reg 0x00\n");
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(cmd_dma_probe, Probe UART/SPI/I2C DMA paths);

static int cmd_bsp_probe(int argc, char **argv)
{
    const char *names[] = {"uart2", "pin", "spi1", "spi3", "hwi2c2", "pwm2"};
    rt_size_t i;

    RT_UNUSED(argc);
    RT_UNUSED(argv);

    for (i = 0; i < sizeof(names) / sizeof(names[0]); i++)
    {
        rt_kprintf("%-7s : %s\n", names[i], rt_device_find(names[i]) ? "ok" : "missing");
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(cmd_bsp_probe, Probe key BSP devices);

int main(void)
{
    int led_state = 0;

    rt_kprintf("\n========================================\n");
    rt_kprintf("  RT-Thread on PY32F403\n");
    rt_kprintf("========================================\n\n");

    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        led_state = !led_state;
        rt_pin_write(LED_PIN, led_state ? PIN_HIGH : PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
