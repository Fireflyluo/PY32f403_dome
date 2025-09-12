/**
  ******************************************************************************
  * @file    lps22hb_reg.c
  * @author  Sensors Software Solution Team
  * @brief   LPS22HB 驱动文件
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) 2021 STMicroelectronics。
  * 保留所有权利。
  *
  * 本软件根据可以在 LICENSE 文件中找到的条款获得许可，
  * 该文件位于此软件组件的根目录下。
  * 如果没有随此软件提供的 LICENSE 文件，则按原样提供。
  *
  ******************************************************************************
  */

#include "lps22hb_reg.h"

/**
  * @defgroup    LPS22HB
  * @brief       此文件提供了一组驱动超紧凑压阻式绝对压力传感器所需的函数。
  * @{
  *
  */

/**
  * @defgroup    LPS22HB_接口函数
  * @brief       本节提供了一组用于读写设备通用寄存器的函数。
  * @{
  *
  */

/**
  * @brief  读取通用设备寄存器
  *
  * @param  ctx   读/写接口定义指针
  * @param  reg   要读取的寄存器
  * @param  data  指向存储读取数据的缓冲区指针
  * @param  len   要连续读取的寄存器数量
  * @retval       接口状态 (必须返回 0 -> 无错误)
  *
  */
int32_t __weak lps22hb_read_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                                uint8_t *data,
                                uint16_t len)
{
  int32_t ret;

  // 检查上下文指针是否为空
  if (ctx == NULL)
  {
    return -1;
  }

  // 调用底层读取函数
  ret = ctx->read_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @brief  写入通用设备寄存器
  *
  * @param  ctx   读/写接口定义指针
  * @param  reg   要写入的寄存器
  * @param  data  指向要写入寄存器的数据指针
  * @param  len   要连续写入的寄存器数量
  * @retval       接口状态 (必须返回 0 -> 无错误)
  *
  */
int32_t __weak lps22hb_write_reg(const stmdev_ctx_t *ctx, uint8_t reg,
                                 uint8_t *data,
                                 uint16_t len)
{
  int32_t ret;

  // 检查上下文指针是否为空
  if (ctx == NULL)
  {
    return -1;
  }

  // 调用底层写入函数
  ret = ctx->write_reg(ctx->handle, reg, data, len);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_灵敏度
  * @brief       这些函数将原始数据转换为工程单位。
  * @{
  *
  */

/**
  * @brief  将原始值转换为百帕 (hPa)
  *
  * @param  lsb   原始值
  * @retval       转换后的压力值 (hPa)
  */
float_t lps22hb_from_lsb_to_hpa(int32_t lsb)
{
  return ((float_t)lsb / 4096.0f);
}

/**
  * @brief  将原始值转换为千帕 (kPa)
  *
  * @param  lsb   原始值
  * @retval       转换后的压力值 (kPa)
  */
float_t lps22hb_from_lsb_to_kpa(int32_t lsb)
{
  return lps22hb_from_lsb_to_hpa(lsb) / 10.0f;
}

/**
  * @brief  将原始值转换为磅/平方英寸 (psi)
  *
  * @param  lsb   原始值
  * @retval       转换后的压力值 (psi)
  */
float_t lps22hb_from_lsb_to_psi(int32_t lsb)
{
  return lps22hb_from_lsb_to_hpa(lsb) * 0.0145038f;
}

/**
  * @brief  将原始值转换为海拔高度 (米)
  *
  * @param  lsb   原始值
  * @retval       转换后的海拔高度 (米)
  */
float_t lps22hb_from_lsb_to_altitude(int32_t lsb)
{
  float_t atmospheric = lps22hb_from_lsb_to_hpa(lsb);
  // 海拔高度可以通过国际气压公式计算。
  // 平均海平面压力为 1013.25 hPa。
  return 44330.0 * (1.0 - pow(atmospheric / 1013.25f, (1.0 / 5.255)));
}

/**
  * @brief  将原始值转换为摄氏度 (°C)
  *
  * @param  lsb   原始值
  * @retval       转换后的温度值 (°C)
  */
float_t lps22hb_from_lsb_to_degc(int16_t lsb)
{
  return ((float_t)lsb / 100.0f);
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_数据生成_c
  * @brief       本节将所有与数据生成相关的函数分组
  * @{
  *
  */


/**
  * @brief  重置自动归零功能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 reset_az 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_autozero_rst_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.reset_az = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  重置自动归零功能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 reset_az 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_autozero_rst_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.reset_az;

  return ret;
}

/**
  * @brief  启用自动归零功能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 autozero 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_autozero_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.autozero = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  启用自动归零功能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 autozero 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_autozero_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.autozero;

  return ret;
}

/**
  * @brief  重置自动参考压力功能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 reset_arp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pressure_snap_rst_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.reset_arp = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  重置自动参考压力功能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 reset_arp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pressure_snap_rst_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.reset_arp;

  return ret;
}

/**
  * @brief  启用自动参考压力功能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 autorifp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pressure_snap_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.autorifp = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  启用自动参考压力功能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 autorifp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pressure_snap_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.autorifp;

  return ret;
}

/**
  * @brief  块数据更新。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 bdu 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_block_data_update_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.bdu = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  块数据更新。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 bdu 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_block_data_update_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  *val = ctrl_reg1.bdu;

  return ret;
}

/**
  * @brief  低通带宽选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 lpfp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_low_pass_filter_mode_set(const stmdev_ctx_t *ctx,
                                         lps22hb_lpfp_t val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.lpfp = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  低通带宽选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG1 寄存器中 lpfp 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_low_pass_filter_mode_get(const stmdev_ctx_t *ctx,
                                         lps22hb_lpfp_t *val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.lpfp)
  {
    case LPS22HB_LPF_ODR_DIV_2:
      *val = LPS22HB_LPF_ODR_DIV_2;
      break;

    case LPS22HB_LPF_ODR_DIV_9:
      *val = LPS22HB_LPF_ODR_DIV_9;
      break;

    case LPS22HB_LPF_ODR_DIV_20:
      *val = LPS22HB_LPF_ODR_DIV_20;
      break;

    default:
      *val = LPS22HB_LPF_ODR_DIV_2;
      break;
  }

  return ret;
}

/**
  * @brief  输出数据速率选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 odr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_data_rate_set(const stmdev_ctx_t *ctx, lps22hb_odr_t val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.odr = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  输出数据速率选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG1 寄存器中 odr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_data_rate_get(const stmdev_ctx_t *ctx, lps22hb_odr_t *val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.odr)
  {
    case LPS22HB_POWER_DOWN:
      *val = LPS22HB_POWER_DOWN;
      break;

    case LPS22HB_ODR_1_Hz:
      *val = LPS22HB_ODR_1_Hz;
      break;

    case LPS22HB_ODR_10_Hz:
      *val = LPS22HB_ODR_10_Hz;
      break;

    case LPS22HB_ODR_25_Hz:
      *val = LPS22HB_ODR_25_Hz;
      break;

    case LPS22HB_ODR_50_Hz:
      *val = LPS22HB_ODR_50_Hz;
      break;

    case LPS22HB_ODR_75_Hz:
      *val = LPS22HB_ODR_75_Hz;
      break;

    default:
      *val = LPS22HB_ODR_1_Hz;
      break;
  }

  return ret;
}

/**
  * @brief  单次触发模式。设备执行一次测量。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 one_shot 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_one_shoot_trigger_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.one_shot = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  单次触发模式。设备执行一次测量。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 one_shot 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_one_shoot_trigger_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.one_shot;

  return ret;
}

/**
  * @brief  pressure_ref: 参考压力值是一个 24 位数据，
  *         以二进制补码形式表示。当启用 AUTOZERO 或 AUTORIFP 功能时使用该值。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    要写入的数据
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pressure_ref_set(const stmdev_ctx_t *ctx, int32_t val)
{
  uint8_t buff[3];
  int32_t ret;

  buff[2] = (uint8_t)((uint32_t)val / 65536U);
  buff[1] = (uint8_t)((uint32_t)val - (buff[2] * 65536U)) / 256U;
  buff[0] = (uint8_t)((uint32_t)val - (buff[2] * 65536U) -
                      (buff[1] * 256U));
  ret =  lps22hb_write_reg(ctx, LPS22HB_REF_P_XL, buff, 3);

  return ret;
}

/**
  * @brief  pressure_ref: 参考压力值是一个 24 位数据，
  *         以二进制补码形式表示。当启用 AUTOZERO 或 AUTORIFP 功能时使用该值。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pressure_ref_get(const stmdev_ctx_t *ctx, int32_t *val)
{
  uint8_t buff[3];
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_REF_P_XL, buff, 3);
  *val = (int32_t)buff[2];
  *val = (*val * 256) + (int32_t)buff[1];
  *val = (*val * 256) + (int32_t)buff[0];

  return ret;
}

/**
  * @brief  压力偏移值是 16 位数据，可用于在焊接后实现单点校准 (OPC)。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    要写入的数据
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pressure_offset_set(const stmdev_ctx_t *ctx, int16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)((uint16_t)val / 256U);
  buff[0] = (uint8_t)((uint16_t)val - (buff[1] * 256U));
  ret =  lps22hb_write_reg(ctx, LPS22HB_RPDS_L, buff, 2);

  return ret;
}

/**
  * @brief  压力偏移值是 16 位数据，可用于在焊接后实现单点校准 (OPC)。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pressure_offset_get(const stmdev_ctx_t *ctx, int16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_RPDS_L, buff, 2);
  *val = (int16_t)buff[1];
  *val = (*val * 256) + (int16_t)buff[0];

  return ret;
}

/**
  * @brief  压力数据可用。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 STATUS 寄存器中 p_da 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_press_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *val = status.p_da;

  return ret;
}

/**
  * @brief  温度数据可用。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 STATUS 寄存器中 t_da 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_temp_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *val = status.t_da;

  return ret;
}

/**
  * @brief  压力和温度数据可用。[获取]
  *
  * @param  ctx          读/写接口定义
  * @param  press_val    改变 STATUS 寄存器中 p_da 的值
  * @param  temp_val     改变 STATUS 寄存器中 t_da 的值
  * @retval              接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_data_ready_get(const stmdev_ctx_t *ctx, uint8_t *press_val, uint8_t *temp_val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *press_val = status.p_da;
  *temp_val = status.t_da;

  return ret;
}

/**
  * @brief  压力数据溢出。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 STATUS 寄存器中 p_or 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_press_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *val = status.p_or;

  return ret;
}

/**
  * @brief  温度数据溢出。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 STATUS 寄存器中 t_or 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_temp_data_ovr_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_status_t status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_STATUS, (uint8_t *)&status, 1);
  *val = status.t_or;

  return ret;
}

/**
  * @brief  压力输出值[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pressure_raw_get(const stmdev_ctx_t *ctx, uint32_t *buff)
{
  uint8_t reg[3];
  int32_t ret,raw_pressure;

  ret =  lps22hb_read_reg(ctx, LPS22HB_PRESS_OUT_XL, reg, 3);
  
// 正确组合24位数据 (小端格式)
    // reg[2] = PRESS_OUT_H (MSB)
    // reg[1] = PRESS_OUT_L
    // reg[0] = PRESS_OUT_XL (LSB)
    raw_pressure = (int32_t)reg[2];       // 获取最高字节
    raw_pressure = raw_pressure << 8;     // 左移8位
    raw_pressure |= (int32_t)reg[1];       // 加入中间字节
    raw_pressure = raw_pressure << 8;     // 再左移8位
    raw_pressure |= (int32_t)reg[0];       // 加入最低字节

    // 处理24位有符号数（符号扩展）
    if (raw_pressure & 0x00800000) {
        raw_pressure |= 0xFF000000;  // 设置高8位为1（负数）
    }

    *buff = (uint32_t)raw_pressure;  // 保存为32位值
//  *buff = reg[2];
//  *buff = (*buff * 256) + reg[1];
//  *buff = (*buff * 256) + reg[0];
//  *buff *= 256;

  return ret;
}

/**
  * @brief  temperature_raw: 温度输出值[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_temperature_raw_get(const stmdev_ctx_t *ctx, int16_t *buff)
{
  uint8_t reg[2];
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_TEMP_OUT_L, (uint8_t *) reg, 2);
  *buff = reg[1];
  *buff = (*buff * 256) + reg[0];

  return ret;
}

/**
  * @defgroup    LPS22HB_FIFO_输出数据
  * @brief       这些函数将 FIFO 输出数据转换为单独的
  *              (压力, 温度) 字段。
  * @{
  *
  */

/**
  * @brief  将 FIFO 输出数据转换为原始压力值
  *
  * @param  val    FIFO 输出数据
  * @retval        原始压力值
  */
int32_t lps22hb_fifo_output_data_to_raw_pressure(lps22hb_fifo_output_data_t *val)
{
  int32_t pressure = val->bytes[2];
  pressure = (pressure * 256) + val->bytes[1];
  return (pressure * 256) + val->bytes[0];
}

/**
  * @brief  将 FIFO 输出数据转换为原始温度值
  *
  * @param  val    FIFO 输出数据
  * @retval        原始温度值
  */
int16_t lps22hb_fifo_output_data_to_raw_temperature(lps22hb_fifo_output_data_t *val)
{
  int16_t temperature = val->bytes[4];
  return (temperature * 256) + val->bytes[3];
}

/**
  * @}
  *
  */

/**
  * @brief  突发读取 FIFO 输出数据。
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @param  len    要从 FIFO 读取的压力-温度对的数量
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_output_data_burst_get(const stmdev_ctx_t *ctx,
                                           lps22hb_fifo_output_data_t *buff, uint8_t len)
{
  if (len > 32)
  {
    len = 32;
  }
  return lps22hb_read_reg(ctx, LPS22HB_PRESS_OUT_XL, (uint8_t *)&buff[0],
                          len * sizeof(lps22hb_fifo_output_data_t));
}

/**
  * @brief  低通滤波器复位寄存器。如果 LPFP 处于活动状态，
  *         为了避免瞬态阶段，可以通过在生成压力测量之前读取此寄存器来复位滤波器。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_low_pass_rst_get(const stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_LPFP_RES, (uint8_t *) buff, 1);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_通用
  * @brief       本节将通用的有用函数分组
  * @{
  *
  */

/**
  * @brief  设备标识[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  buff   存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_device_id_get(const stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_WHO_AM_I, (uint8_t *) buff, 1);

  return ret;
}

/**
  * @brief  软件复位。恢复用户寄存器中的默认值[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 swreset 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_reset_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.swreset = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  软件复位。恢复用户寄存器中的默认值[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 swreset 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_reset_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.swreset;

  return ret;
}

/**
  * @brief  重启内存内容。重新加载校准参数。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 boot 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_boot_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.boot = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  重启内存内容。重新加载校准参数。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 boot 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_boot_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.boot;

  return ret;
}

/**
  * @brief  低电流模式。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 RES_CONF 寄存器中 lc_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_low_power_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_res_conf_t res_conf;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_RES_CONF, (uint8_t *)&res_conf, 1);

  if (ret == 0)
  {
    res_conf.lc_en = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_RES_CONF, (uint8_t *)&res_conf, 1);
  }

  return ret;
}

/**
  * @brief  低电流模式。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 RES_CONF 寄存器中 lc_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_low_power_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_res_conf_t res_conf;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_RES_CONF, (uint8_t *)&res_conf, 1);
  *val = res_conf.lc_en;

  return ret;
}

/**
  * @brief  如果为 '1' 表示正在运行启动 (重启) 阶段。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INT_SOURCE 寄存器中 boot_status 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_boot_status_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_int_source_t int_source;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.boot_status;

  return ret;
}

/**
  * @brief  所有状态位，FIFO 和数据生成[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    从 FIFO_STATUS 到 STATUS 寄存器的结构体
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_dev_status_get(const stmdev_ctx_t *ctx,
                               lps22hb_dev_stat_t *val)
{
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_FIFO_STATUS, (uint8_t *) val, 2);

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_中断
  * @brief       本节将管理中断的所有函数分组
  * @{
  *
  */

/**
  * @brief  在压力低/高事件上启用中断生成。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 pe 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_sign_of_int_threshold_set(const stmdev_ctx_t *ctx,
                                          lps22hb_pe_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.pe = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  在压力低/高事件上启用中断生成。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 INTERRUPT_CFG 寄存器中 pe 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_sign_of_int_threshold_get(const stmdev_ctx_t *ctx,
                                          lps22hb_pe_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  switch (interrupt_cfg.pe)
  {
    case LPS22HB_NO_THRESHOLD:
      *val = LPS22HB_NO_THRESHOLD;
      break;

    case LPS22HB_POSITIVE:
      *val = LPS22HB_POSITIVE;
      break;

    case LPS22HB_NEGATIVE:
      *val = LPS22HB_NEGATIVE;
      break;

    case LPS22HB_BOTH:
      *val = LPS22HB_BOTH;
      break;

    default:
      *val = LPS22HB_NO_THRESHOLD;
      break;
  }

  return ret;
}

/**
  * @brief  向 INT_SOURCE (25h) 寄存器请求中断
  *         模式 (脉冲 / 锁存) [设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 lir 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_notification_mode_set(const stmdev_ctx_t *ctx,
                                          lps22hb_lir_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.lir = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  向 INT_SOURCE (25h) 寄存器请求中断
  *         模式 (脉冲 / 锁存) [获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 INTERRUPT_CFG 寄存器中 lir 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_notification_mode_get(const stmdev_ctx_t *ctx,
                                          lps22hb_lir_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  switch (interrupt_cfg.lir)
  {
    case LPS22HB_INT_PULSED:
      *val = LPS22HB_INT_PULSED;
      break;

    case LPS22HB_INT_LATCHED:
      *val = LPS22HB_INT_LATCHED;
      break;

    default:
      *val = LPS22HB_INT_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  启用中断生成。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 diff_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_generation_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);

  if (ret == 0)
  {
    interrupt_cfg.diff_en = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_INTERRUPT_CFG,
                            (uint8_t *)&interrupt_cfg, 1);
  }

  return ret;
}

/**
  * @brief  启用中断生成。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INTERRUPT_CFG 寄存器中 diff_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_generation_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_interrupt_cfg_t interrupt_cfg;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INTERRUPT_CFG,
                         (uint8_t *)&interrupt_cfg, 1);
  *val = interrupt_cfg.diff_en;

  return ret;
}

/**
  * @brief  用户定义的压力中断事件阈值值[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    要写入的数据
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_threshold_set(const stmdev_ctx_t *ctx, uint16_t val)
{
  uint8_t buff[2];
  int32_t ret;

  buff[1] = (uint8_t)(val / 256U);
  buff[0] = (uint8_t)(val - (buff[1] * 256U));
  ret =  lps22hb_write_reg(ctx, LPS22HB_THS_P_L, (uint8_t *) buff, 2);

  return ret;
}

/**
  * @brief  用户定义的压力中断事件阈值值[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    存储读取数据的缓冲区
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_threshold_get(const stmdev_ctx_t *ctx, uint16_t *val)
{
  uint8_t buff[2];
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_THS_P_L, (uint8_t *) buff, 2);
  *val = buff[1];
  *val = (*val * 256) + buff[0];

  return ret;
}

/**
  * @brief  INT_DRDY 引脚上的数据信号控制位。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 int_s 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_pin_mode_set(const stmdev_ctx_t *ctx,
                                 lps22hb_int_s_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.int_s = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  INT_DRDY 引脚上的数据信号控制位。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG3 寄存器中 int_s 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_pin_mode_get(const stmdev_ctx_t *ctx,
                                 lps22hb_int_s_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  switch (ctrl_reg3.int_s)
  {
    case LPS22HB_DRDY_OR_FIFO_FLAGS:
      *val = LPS22HB_DRDY_OR_FIFO_FLAGS;
      break;

    case LPS22HB_HIGH_PRES_INT:
      *val = LPS22HB_HIGH_PRES_INT;
      break;

    case LPS22HB_LOW_PRES_INT:
      *val = LPS22HB_LOW_PRES_INT;
      break;

    case LPS22HB_EVERY_PRES_INT:
      *val = LPS22HB_EVERY_PRES_INT;
      break;

    default:
      *val = LPS22HB_DRDY_OR_FIFO_FLAGS;
      break;
  }

  return ret;
}

/**
  * @brief  INT_DRDY 引脚上的数据就绪信号。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 drdy 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_drdy_on_int_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.drdy = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  INT_DRDY 引脚上的数据就绪信号。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 drdy 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_drdy_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.drdy;

  return ret;
}

/**
  * @brief  FIFO 溢出中断在 INT_DRDY 引脚上。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_ovr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_ovr_on_int_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.f_ovr = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  FIFO 溢出中断在 INT_DRDY 引脚上。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_ovr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_ovr_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.f_ovr;

  return ret;
}

/**
  * @brief  FIFO 水位状态在 INT_DRDY 引脚上。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_fth 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_threshold_on_int_set(const stmdev_ctx_t *ctx,
                                          uint8_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.f_fth = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  FIFO 水位状态在 INT_DRDY 引脚上。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_fth 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_threshold_on_int_get(const stmdev_ctx_t *ctx,
                                          uint8_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.f_fth;

  return ret;
}

/**
  * @brief  FIFO 满标志在 INT_DRDY 引脚上。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_fss5 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_full_on_int_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.f_fss5 = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  FIFO 满标志在 INT_DRDY 引脚上。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 f_fss5 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_full_on_int_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  *val = ctrl_reg3.f_fss5;

  return ret;
}

/**
  * @brief  中断引脚上的推挽/开漏选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 pp_od 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pin_mode_set(const stmdev_ctx_t *ctx, lps22hb_pp_od_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.pp_od = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  中断引脚上的推挽/开漏选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG3 寄存器中 pp_od 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_pin_mode_get(const stmdev_ctx_t *ctx, lps22hb_pp_od_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  switch (ctrl_reg3.pp_od)
  {
    case LPS22HB_PUSH_PULL:
      *val = LPS22HB_PUSH_PULL;
      break;

    case LPS22HB_OPEN_DRAIN:
      *val = LPS22HB_OPEN_DRAIN;
      break;

    default:
      *val = LPS22HB_PUSH_PULL;
      break;
  }

  return ret;
}

/**
  * @brief  中断高电平/低电平。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG3 寄存器中 int_h_l 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_polarity_set(const stmdev_ctx_t *ctx,
                                 lps22hb_int_h_l_t val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  if (ret == 0)
  {
    ctrl_reg3.int_h_l = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);
  }

  return ret;
}

/**
  * @brief  中断高电平/低电平。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG3 寄存器中 int_h_l 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_polarity_get(const stmdev_ctx_t *ctx,
                                 lps22hb_int_h_l_t *val)
{
  lps22hb_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG3, (uint8_t *)&ctrl_reg3, 1);

  switch (ctrl_reg3.int_h_l)
  {
    case LPS22HB_ACTIVE_HIGH:
      *val = LPS22HB_ACTIVE_HIGH;
      break;

    case LPS22HB_ACTIVE_LOW:
      *val = LPS22HB_ACTIVE_LOW;
      break;

    default:
      *val = LPS22HB_ACTIVE_HIGH;
      break;
  }

  return ret;
}

/**
  * @brief  中断源寄存器[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    INT_SOURCE 寄存器
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_source_get(const stmdev_ctx_t *ctx,
                               lps22hb_int_source_t *val)
{
  int32_t ret;

  ret =  lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE, (uint8_t *) val, 1);

  return ret;
}

/**
  * @brief  压力高中断标志。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INT_SOURCE 寄存器中 ph 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_on_press_high_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_int_source_t int_source;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.ph;

  return ret;
}

/**
  * @brief  压力低中断标志。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INT_SOURCE 寄存器中 pl 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_int_on_press_low_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_int_source_t int_source;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.pl;

  return ret;
}

/**
  * @brief  中断活动标志。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 INT_SOURCE 寄存器中 ia 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_interrupt_event_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_int_source_t int_source;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_INT_SOURCE,
                         (uint8_t *)&int_source, 1);
  *val = int_source.ia;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_FIFO
  * @brief       本节将所有与 FIFO 使用相关的函数分组
  * @{
  *
  */

/**
  * @brief  在 FIFO 水位处停止。启用 FIFO 水位级别使用。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 stop_on_fth 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_stop_on_fifo_threshold_set(const stmdev_ctx_t *ctx,
                                           uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.stop_on_fth = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  在 FIFO 水位处停止。启用 FIFO 水位级别使用。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 stop_on_fth 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_stop_on_fifo_threshold_get(const stmdev_ctx_t *ctx,
                                           uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.stop_on_fth;

  return ret;
}

/**
  * @brief  FIFO 使能。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 fifo_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.fifo_en = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  FIFO 使能。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 fifo_en 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.fifo_en;

  return ret;
}

/**
  * @brief  FIFO 水位级别选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_CTRL 寄存器中 wtm 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_watermark_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

  if (ret == 0)
  {
    fifo_ctrl.wtm = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  FIFO 水位级别选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_CTRL 寄存器中 wtm 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_watermark_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  *val = fifo_ctrl.wtm;

  return ret;
}

/**
  * @brief  FIFO 模式选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_CTRL 寄存器中 f_mode 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_mode_set(const stmdev_ctx_t *ctx, lps22hb_f_mode_t val)
{
  lps22hb_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

  if (ret == 0)
  {
    fifo_ctrl.f_mode = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);
  }

  return ret;
}

/**
  * @brief  FIFO 模式选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 FIFO_CTRL 寄存器中 f_mode 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_mode_get(const stmdev_ctx_t *ctx,
                              lps22hb_f_mode_t *val)
{
  lps22hb_fifo_ctrl_t fifo_ctrl;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_CTRL, (uint8_t *)&fifo_ctrl, 1);

  switch (fifo_ctrl.f_mode)
  {
    case LPS22HB_BYPASS_MODE:
      *val = LPS22HB_BYPASS_MODE;
      break;

    case LPS22HB_FIFO_MODE:
      *val = LPS22HB_FIFO_MODE;
      break;

    case LPS22HB_STREAM_MODE:
      *val = LPS22HB_STREAM_MODE;
      break;

    case LPS22HB_STREAM_TO_FIFO_MODE:
      *val = LPS22HB_STREAM_TO_FIFO_MODE;
      break;

    case LPS22HB_BYPASS_TO_STREAM_MODE:
      *val = LPS22HB_BYPASS_TO_STREAM_MODE;
      break;

    case LPS22HB_DYNAMIC_STREAM_MODE:
      *val = LPS22HB_DYNAMIC_STREAM_MODE;
      break;

    case LPS22HB_BYPASS_TO_FIFO_MODE:
      *val = LPS22HB_BYPASS_TO_FIFO_MODE;
      break;

    default:
      *val = LPS22HB_BYPASS_MODE;
      break;
  }

  return ret;
}

/**
  * @brief  FIFO 存储的数据级别。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_STATUS 寄存器中 fss 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_data_level_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_fifo_status_t fifo_status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_STATUS,
                         (uint8_t *)&fifo_status, 1);
  *val = fifo_status.fss;

  return ret;
}

/**
  * @brief  FIFO 溢出状态。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_STATUS 寄存器中 ovr 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_ovr_flag_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_fifo_status_t fifo_status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_STATUS,
                         (uint8_t *)&fifo_status, 1);
  *val = fifo_status.ovr;

  return ret;
}

/**
  * @brief  FIFO 水位状态。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 FIFO_STATUS 寄存器中 fth_fifo 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_fifo_fth_flag_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_fifo_status_t fifo_status;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_FIFO_STATUS,
                         (uint8_t *)&fifo_status, 1);
  *val = fifo_status.fth_fifo;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LPS22HB_串行接口
  * @brief       本节将所有与串行接口管理相关的函数分组
  * @{
  *
  */

/**
  * @brief  SPI 串行接口模式选择。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG1 寄存器中 sim 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_spi_mode_set(const stmdev_ctx_t *ctx, lps22hb_sim_t val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  if (ret == 0)
  {
    ctrl_reg1.sim = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);
  }

  return ret;
}

/**
  * @brief  SPI 串行接口模式选择。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG1 寄存器中 sim 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_spi_mode_get(const stmdev_ctx_t *ctx, lps22hb_sim_t *val)
{
  lps22hb_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG1, (uint8_t *)&ctrl_reg1, 1);

  switch (ctrl_reg1.sim)
  {
    case LPS22HB_SPI_4_WIRE:
      *val = LPS22HB_SPI_4_WIRE;
      break;

    case LPS22HB_SPI_3_WIRE:
      *val = LPS22HB_SPI_3_WIRE;
      break;

    default:
      *val = LPS22HB_SPI_4_WIRE;
      break;
  }

  return ret;
}

/**
  * @brief  禁用 I2C 接口。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 i2c_dis 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_i2c_interface_set(const stmdev_ctx_t *ctx,
                                  lps22hb_i2c_dis_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.i2c_dis = (uint8_t)val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  禁用 I2C 接口。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    获取 CTRL_REG2 寄存器中 i2c_dis 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_i2c_interface_get(const stmdev_ctx_t *ctx,
                                  lps22hb_i2c_dis_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  switch (ctrl_reg2.i2c_dis)
  {
    case LPS22HB_I2C_ENABLE:
      *val = LPS22HB_I2C_ENABLE;
      break;

    case LPS22HB_I2C_DISABLE:
      *val = LPS22HB_I2C_DISABLE;
      break;

    default:
      *val = LPS22HB_I2C_ENABLE;
      break;
  }

  return ret;
}

/**
  * @brief  在使用串行接口 (I2C 或 SPI) 进行多字节访问期间，
  *         寄存器地址自动递增。[设置]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 if_add_inc 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_auto_add_inc_set(const stmdev_ctx_t *ctx, uint8_t val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);

  if (ret == 0)
  {
    ctrl_reg2.if_add_inc = val;
    ret = lps22hb_write_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  }

  return ret;
}

/**
  * @brief  在使用串行接口 (I2C 或 SPI) 进行多字节访问期间，
  *         寄存器地址自动递增。[获取]
  *
  * @param  ctx    读/写接口定义
  * @param  val    改变 CTRL_REG2 寄存器中 if_add_inc 的值
  * @retval        接口状态 (必须返回 0 -> 无错误)。
  *
  */
int32_t lps22hb_auto_add_inc_get(const stmdev_ctx_t *ctx, uint8_t *val)
{
  lps22hb_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lps22hb_read_reg(ctx, LPS22HB_CTRL_REG2, (uint8_t *)&ctrl_reg2, 1);
  *val = ctrl_reg2.if_add_inc;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @}
  *
  */