/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2020 Rockchip Electronics Co. Ltd.
 * Author: Elaine Zhang <zhangqing@rock-chips.com>
 */

#ifndef _ASM_ARCH_CRU_RK3568_H
#define _ASM_ARCH_CRU_RK3568_H

#define MHz		1000000
#define KHz		1000
#define OSC_HZ		(24 * MHz)

#define APLL_HZ		(816 * MHz)
#define GPLL_HZ		(1188 * MHz)
#define CPLL_HZ		(1000 * MHz)
#define PPLL_HZ		(200 * MHz)

/* RK3568 pll id */
enum rk3568_pll_id {
	APLL,
	DPLL,
	CPLL,
	GPLL,
	NPLL,
	VPLL,
	PPLL,
	HPLL,
	PLL_COUNT,
};

struct rk3568_clk_info {
	unsigned long id;
	char *name;
	bool is_cru;
};

/* Private data for the clock driver - used by rockchip_get_cru() */
struct rk3568_pmuclk_priv {
	struct rk3568_pmucru *pmucru;
	ulong ppll_hz;
	ulong hpll_hz;
};

struct rk3568_clk_priv {
	struct rk3568_cru *cru;
	struct rk3568_grf *grf;
	ulong ppll_hz;
	ulong hpll_hz;
	ulong gpll_hz;
	ulong cpll_hz;
	ulong npll_hz;
	ulong vpll_hz;
	ulong armclk_hz;
	ulong armclk_enter_hz;
	ulong armclk_init_hz;
	bool sync_kernel;
	bool set_armclk_rate;
};

struct rk3568_pll {
	unsigned int con0;
	unsigned int con1;
	unsigned int con2;
	unsigned int con3;
	unsigned int con4;
	unsigned int reserved0[3];
};

struct rk3568_pmucru {
	struct rk3568_pll pll[2];/* Address Offset: 0x0000 */
	unsigned int reserved0[16];/* Address Offset: 0x0040 */
	unsigned int mode_con00;/* Address Offset: 0x0080 */
	unsigned int reserved1[31];/* Address Offset: 0x0084 */
	unsigned int pmu_clksel_con[10];/* Address Offset: 0x0100 */
	unsigned int reserved2[22];/* Address Offset: 0x0128 */
	unsigned int pmu_clkgate_con[3];/* Address Offset: 0x0180 */
	unsigned int reserved3[29];/* Address Offset: 0x018C */
	unsigned int pmu_softrst_con[1];/* Address Offset: 0x0200 */
};

check_member(rk3568_pmucru, mode_con00, 0x80);
check_member(rk3568_pmucru, pmu_softrst_con[0], 0x200);

struct rk3568_cru {
	struct rk3568_pll pll[6];
	unsigned int mode_con00;/* Address Offset: 0x00C0 */
	unsigned int misc_con[3];/* Address Offset: 0x00C4 */
	unsigned int glb_cnt_th;/* Address Offset: 0x00D0 */
	unsigned int glb_srst_fst;/* Address Offset: 0x00D4 */
	unsigned int glb_srsr_snd; /* Address Offset: 0x00D8 */
	unsigned int glb_rst_con;/* Address Offset: 0x00DC */
	unsigned int glb_rst_st;/* Address Offset: 0x00E0 */
	unsigned int reserved0[7];/* Address Offset: 0x00E4 */
	unsigned int clksel_con[85]; /* Address Offset: 0x0100 */
	unsigned int reserved1[43];/* Address Offset: 0x0254 */
	unsigned int clkgate_con[36];/* Address Offset: 0x0300 */
	unsigned int reserved2[28]; /* Address Offset: 0x0390 */
	unsigned int softrst_con[30];/* Address Offset: 0x0400 */
	unsigned int reserved3[2];/* Address Offset: 0x0478 */
	unsigned int ssgtbl[32];/* Address Offset: 0x0480 */
	unsigned int reserved4[32];/* Address Offset: 0x0500 */
	unsigned int sdmmc0_con[2];/* Address Offset: 0x0580 */
	unsigned int sdmmc1_con[2];/* Address Offset: 0x058C */
	unsigned int sdmmc2_con[2];/* Address Offset: 0x0590 */
	unsigned int emmc_con[2];/* Address Offset: 0x0598 */
};

check_member(rk3568_cru, mode_con00, 0xc0);
check_member(rk3568_cru, softrst_con[0], 0x400);

struct pll_rate_table {
	unsigned long rate;
	unsigned int fbdiv;
	unsigned int postdiv1;
	unsigned int refdiv;
	unsigned int postdiv2;
	unsigned int dsmpd;
	unsigned int frac;
};

#define RK3568_PMU_MODE			0x80
#define RK3568_PMU_PLL_CON(x)		((x) * 0x4)
#define RK3568_PLL_CON(x)		((x) * 0x4)
#define RK3568_MODE_CON			0xc0

enum {
	/* CRU_PMU_CLK_SEL0_CON */
	RTC32K_SEL_SHIFT		= 6,
	RTC32K_SEL_MASK			= 0x3 << RTC32K_SEL_SHIFT,
	RTC32K_SEL_PMUPVTM		= 0,
	RTC32K_SEL_OSC1_32K,
	RTC32K_SEL_OSC0_DIV32K,

	/* CRU_PMU_CLK_SEL1_CON */
	RTC32K_FRAC_NUMERATOR_SHIFT	= 16,
	RTC32K_FRAC_NUMERATOR_MASK	= 0xffff << 16,
	RTC32K_FRAC_DENOMINATOR_SHIFT	= 0,
	RTC32K_FRAC_DENOMINATOR_MASK	= 0xffff,

	/* CRU_PMU_CLK_SEL2_CON */
	PCLK_PDPMU_SEL_SHIFT		= 15,
	PCLK_PDPMU_SEL_MASK		= 1 << PCLK_PDPMU_SEL_SHIFT,
	PCLK_PDPMU_SEL_PPLL		= 0,
	PCLK_PDPMU_SEL_GPLL,
	PCLK_PDPMU_DIV_SHIFT		= 0,
	PCLK_PDPMU_DIV_MASK		= 0x1f,

	/* CRU_PMU_CLK_SEL3_CON */
	CLK_I2C0_DIV_SHIFT		= 0,
	CLK_I2C0_DIV_MASK		= 0x7f,

	/* CRU_PMU_CLK_SEL6_CON */
	CLK_PWM0_SEL_SHIFT		= 7,
	CLK_PWM0_SEL_MASK		= 1 << CLK_PWM0_SEL_SHIFT,
	CLK_PWM0_SEL_XIN24M		= 0,
	CLK_PWM0_SEL_PPLL,
	CLK_PWM0_DIV_SHIFT		= 0,
	CLK_PWM0_DIV_MASK		= 0x7f,

	/* CRU_CLK_SEL0_CON */
	CLK_CORE_PRE_SEL_SHIFT		= 7,
	CLK_CORE_PRE_SEL_MASK		= 1 << CLK_CORE_PRE_SEL_SHIFT,
	CLK_CORE_PRE_SEL_SRC		= 0,
	CLK_CORE_PRE_SEL_APLL,

	/* CRU_CLK_SEL2_CON */
	SCLK_CORE_PRE_SEL_SHIFT		= 15,
	SCLK_CORE_PRE_SEL_MASK		= 1 << SCLK_CORE_PRE_SEL_SHIFT,
	SCLK_CORE_PRE_SEL_SRC		= 0,
	SCLK_CORE_PRE_SEL_NPLL,
	SCLK_CORE_SRC_SEL_SHIFT		= 8,
	SCLK_CORE_SRC_SEL_MASK		= 3 << SCLK_CORE_SRC_SEL_SHIFT,
	SCLK_CORE_SRC_SEL_APLL		= 0,
	SCLK_CORE_SRC_SEL_GPLL,
	SCLK_CORE_SRC_SEL_NPLL,
	SCLK_CORE_SRC_DIV_SHIFT		= 0,
	SCLK_CORE_SRC_DIV_MASK		= 0x1f << SCLK_CORE_SRC_DIV_SHIFT,

	/* CRU_CLK_SEL3_CON */
	GICCLK_CORE_DIV_SHIFT		= 8,
	GICCLK_CORE_DIV_MASK		= 0x1f << GICCLK_CORE_DIV_SHIFT,
	ATCLK_CORE_DIV_SHIFT		= 0,
	ATCLK_CORE_DIV_MASK		= 0x1f << ATCLK_CORE_DIV_SHIFT,

	/* CRU_CLK_SEL4_CON */
	PERIPHCLK_CORE_PRE_DIV_SHIFT	= 8,
	PERIPHCLK_CORE_PRE_DIV_MASK	= 0x1f << PERIPHCLK_CORE_PRE_DIV_SHIFT,
	PCLK_CORE_PRE_DIV_SHIFT		= 0,
	PCLK_CORE_PRE_DIV_MASK		= 0x1f << PCLK_CORE_PRE_DIV_SHIFT,

	/* CRU_CLK_SEL5_CON */
	ACLK_CORE_NIU2BUS_SEL_SHIFT	= 14,
	ACLK_CORE_NIU2BUS_SEL_MASK	= 0x3 << ACLK_CORE_NIU2BUS_SEL_SHIFT,
	ACLK_CORE_NDFT_DIV_SHIFT	= 8,
	ACLK_CORE_NDFT_DIV_MASK		= 0x1f << ACLK_CORE_NDFT_DIV_SHIFT,

	/* CRU_CLK_SEL10_CON */
	HCLK_PERIMID_SEL_SHIFT		= 6,
	HCLK_PERIMID_SEL_MASK		= 3 << HCLK_PERIMID_SEL_SHIFT,
	HCLK_PERIMID_SEL_150M		= 0,
	HCLK_PERIMID_SEL_100M,
	HCLK_PERIMID_SEL_75M,
	HCLK_PERIMID_SEL_24M,
	ACLK_PERIMID_SEL_SHIFT		= 4,
	ACLK_PERIMID_SEL_MASK		= 3 << ACLK_PERIMID_SEL_SHIFT,
	ACLK_PERIMID_SEL_300M		= 0,
	ACLK_PERIMID_SEL_200M,
	ACLK_PERIMID_SEL_100M,
	ACLK_PERIMID_SEL_24M,

	/* CRU_CLK_SEL21_CON */
	I2S3_MCLKOUT_TX_SEL_SHIFT	= 15,
	I2S3_MCLKOUT_TX_SEL_MASK	= 1 << I2S3_MCLKOUT_TX_SEL_SHIFT,
	I2S3_MCLKOUT_TX_SEL_MCLK	= 0,
	I2S3_MCLKOUT_TX_SEL_12M,
	CLK_I2S3_SEL_SHIFT		= 10,
	CLK_I2S3_SEL_MASK		= 0x3 << CLK_I2S3_SEL_SHIFT,
	CLK_I2S3_SEL_SRC		= 0,
	CLK_I2S3_SEL_FRAC,
	CLK_I2S3_SEL_CLKIN,
	CLK_I2S3_SEL_XIN12M,
	CLK_I2S3_SRC_SEL_SHIFT		= 8,
	CLK_I2S3_SRC_SEL_MASK		= 0x3 << CLK_I2S3_SRC_SEL_SHIFT,
	CLK_I2S3_SRC_SEL_GPLL		= 0,
	CLK_I2S3_SRC_SEL_CPLL,
	CLK_I2S3_SRC_SEL_NPLL,
	CLK_I2S3_SRC_DIV_SHIFT		= 0,
	CLK_I2S3_SRC_DIV_MASK		= 0x7f << CLK_I2S3_SRC_DIV_SHIFT,

	/* CRU_CLK_SEL22_CON */
	CLK_I2S3_FRAC_NUMERATOR_SHIFT	= 16,
	CLK_I2S3_FRAC_NUMERATOR_MASK	= 0xffff << 16,
	CLK_I2S3_FRAC_DENOMINATOR_SHIFT	= 0,
	CLK_I2S3_FRAC_DENOMINATOR_MASK	= 0xffff,

	/* CRU_CLK_SEL27_CON */
	CLK_CRYPTO_PKA_SEL_SHIFT	= 6,
	CLK_CRYPTO_PKA_SEL_MASK		= 3 << CLK_CRYPTO_PKA_SEL_SHIFT,
	CLK_CRYPTO_PKA_SEL_300M		= 0,
	CLK_CRYPTO_PKA_SEL_200M,
	CLK_CRYPTO_PKA_SEL_100M,
	CLK_CRYPTO_CORE_SEL_SHIFT	= 4,
	CLK_CRYPTO_CORE_SEL_MASK	= 3 << CLK_CRYPTO_CORE_SEL_SHIFT,
	CLK_CRYPTO_CORE_SEL_200M	= 0,
	CLK_CRYPTO_CORE_SEL_150M,
	CLK_CRYPTO_CORE_SEL_100M,
	HCLK_SECURE_FLASH_SEL_SHIFT	= 2,
	HCLK_SECURE_FLASH_SEL_MASK	= 3 << HCLK_SECURE_FLASH_SEL_SHIFT,
	HCLK_SECURE_FLASH_SEL_150M	= 0,
	HCLK_SECURE_FLASH_SEL_100M,
	HCLK_SECURE_FLASH_SEL_75M,
	HCLK_SECURE_FLASH_SEL_24M,
	ACLK_SECURE_FLASH_SEL_SHIFT	= 0,
	ACLK_SECURE_FLASH_SEL_MASK	= 3 << ACLK_SECURE_FLASH_SEL_SHIFT,
	ACLK_SECURE_FLASH_SEL_200M	= 0,
	ACLK_SECURE_FLASH_SEL_150M,
	ACLK_SECURE_FLASH_SEL_100M,
	ACLK_SECURE_FLASH_SEL_24M,

	/* CRU_CLK_SEL28_CON */
	CCLK_EMMC_SEL_SHIFT		= 12,
	CCLK_EMMC_SEL_MASK		= 7 << CCLK_EMMC_SEL_SHIFT,
	CCLK_EMMC_SEL_24M		= 0,
	CCLK_EMMC_SEL_200M,
	CCLK_EMMC_SEL_150M,
	CCLK_EMMC_SEL_100M,
	CCLK_EMMC_SEL_50M,
	CCLK_EMMC_SEL_375K,
	BCLK_EMMC_SEL_SHIFT		= 8,
	BCLK_EMMC_SEL_MASK		= 3 << BCLK_EMMC_SEL_SHIFT,
	BCLK_EMMC_SEL_200M		= 0,
	BCLK_EMMC_SEL_150M,
	BCLK_EMMC_SEL_125M,
	SCLK_SFC_SEL_SHIFT		= 4,
	SCLK_SFC_SEL_MASK		= 7 << SCLK_SFC_SEL_SHIFT,
	SCLK_SFC_SEL_24M		= 0,
	SCLK_SFC_SEL_50M,
	SCLK_SFC_SEL_75M,
	SCLK_SFC_SEL_100M,
	SCLK_SFC_SEL_125M,
	SCLK_SFC_SEL_150M,
	NCLK_NANDC_SEL_SHIFT		= 0,
	NCLK_NANDC_SEL_MASK		= 3 << NCLK_NANDC_SEL_SHIFT,
	NCLK_NANDC_SEL_200M		= 0,
	NCLK_NANDC_SEL_150M,
	NCLK_NANDC_SEL_100M,
	NCLK_NANDC_SEL_24M,

	/* CRU_CLK_SEL30_CON */
	CLK_SDMMC1_SEL_SHIFT		= 12,
	CLK_SDMMC1_SEL_MASK		= 7 << CLK_SDMMC1_SEL_SHIFT,
	CLK_SDMMC0_SEL_SHIFT		= 8,
	CLK_SDMMC0_SEL_MASK		= 7 << CLK_SDMMC0_SEL_SHIFT,
	CLK_SDMMC_SEL_24M		= 0,
	CLK_SDMMC_SEL_400M,
	CLK_SDMMC_SEL_300M,
	CLK_SDMMC_SEL_100M,
	CLK_SDMMC_SEL_50M,
	CLK_SDMMC_SEL_750K,

	/* CRU_CLK_SEL31_CON */
	CLK_MAC0_OUT_SEL_SHIFT		= 14,
	CLK_MAC0_OUT_SEL_MASK		= 3 << CLK_MAC0_OUT_SEL_SHIFT,
	CLK_MAC0_OUT_SEL_125M		= 0,
	CLK_MAC0_OUT_SEL_50M,
	CLK_MAC0_OUT_SEL_25M,
	CLK_MAC0_OUT_SEL_24M,
	CLK_GMAC0_PTP_REF_SEL_SHIFT	= 12,
	CLK_GMAC0_PTP_REF_SEL_MASK	= 3 << CLK_GMAC0_PTP_REF_SEL_SHIFT,
	CLK_GMAC0_PTP_REF_SEL_62_5M	= 0,
	CLK_GMAC0_PTP_REF_SEL_100M,
	CLK_GMAC0_PTP_REF_SEL_50M,
	CLK_GMAC0_PTP_REF_SEL_24M,
	CLK_MAC0_2TOP_SEL_SHIFT		= 8,
	CLK_MAC0_2TOP_SEL_MASK		= 3 << CLK_MAC0_2TOP_SEL_SHIFT,
	CLK_MAC0_2TOP_SEL_125M		= 0,
	CLK_MAC0_2TOP_SEL_50M,
	CLK_MAC0_2TOP_SEL_25M,
	CLK_MAC0_2TOP_SEL_PPLL,
	RGMII0_CLK_SEL_SHIFT		= 4,
	RGMII0_CLK_SEL_MASK		= 3 << RGMII0_CLK_SEL_SHIFT,
	RGMII0_CLK_SEL_125M		= 0,
	RGMII0_CLK_SEL_125M_1,
	RGMII0_CLK_SEL_2_5M,
	RGMII0_CLK_SEL_25M,
	RMII0_CLK_SEL_SHIFT		= 3,
	RMII0_CLK_SEL_MASK		= 1 << RMII0_CLK_SEL_SHIFT,
	RMII0_CLK_SEL_2_5M		= 0,
	RMII0_CLK_SEL_25M,
	RMII0_EXTCLK_SEL_SHIFT		= 2,
	RMII0_EXTCLK_SEL_MASK		= 1 << RMII0_EXTCLK_SEL_SHIFT,
	RMII0_EXTCLK_SEL_MAC0_TOP	= 0,
	RMII0_EXTCLK_SEL_IO,
	RMII0_MODE_SHIFT		= 0,
	RMII0_MODE_MASK			= 3 << RMII0_MODE_SHIFT,
	RMII0_MODE_SEL_RGMII		= 0,
	RMII0_MODE_SEL_RMII,
	RMII0_MODE_SEL_GMII,

	/* CRU_CLK_SEL32_CON */
	CLK_SDMMC2_SEL_SHIFT		= 8,
	CLK_SDMMC2_SEL_MASK		= 7 << CLK_SDMMC2_SEL_SHIFT,

	/* CRU_CLK_SEL38_CON */
	ACLK_VOP_PRE_SEL_SHIFT		= 6,
	ACLK_VOP_PRE_SEL_MASK		= 3 << ACLK_VOP_PRE_SEL_SHIFT,
	ACLK_VOP_PRE_SEL_CPLL		= 0,
	ACLK_VOP_PRE_SEL_GPLL,
	ACLK_VOP_PRE_SEL_HPLL,
	ACLK_VOP_PRE_SEL_VPLL,
	ACLK_VOP_PRE_DIV_SHIFT		= 0,
	ACLK_VOP_PRE_DIV_MASK		= 0x1f << ACLK_VOP_PRE_DIV_SHIFT,

	/* CRU_CLK_SEL39_CON */
	DCLK0_VOP_SEL_SHIFT		= 10,
	DCLK0_VOP_SEL_MASK		= 3 << DCLK0_VOP_SEL_SHIFT,
	DCLK_VOP_SEL_HPLL		= 0,
	DCLK_VOP_SEL_VPLL,
	DCLK_VOP_SEL_GPLL,
	DCLK_VOP_SEL_CPLL,
	DCLK0_VOP_DIV_SHIFT		= 0,
	DCLK0_VOP_DIV_MASK		= 0xff << DCLK0_VOP_DIV_SHIFT,

	/* CRU_CLK_SEL40_CON */
	DCLK1_VOP_SEL_SHIFT		= 10,
	DCLK1_VOP_SEL_MASK		= 3 << DCLK1_VOP_SEL_SHIFT,
	DCLK1_VOP_DIV_SHIFT		= 0,
	DCLK1_VOP_DIV_MASK		= 0xff << DCLK1_VOP_DIV_SHIFT,

	/* CRU_CLK_SEL41_CON */
	DCLK2_VOP_SEL_SHIFT		= 10,
	DCLK2_VOP_SEL_MASK		= 3 << DCLK2_VOP_SEL_SHIFT,
	DCLK2_VOP_DIV_SHIFT		= 0,
	DCLK2_VOP_DIV_MASK		= 0xff << DCLK2_VOP_DIV_SHIFT,

	/* CRU_CLK_SEL43_CON */
	DCLK_EBC_SEL_SHIFT		= 6,
	DCLK_EBC_SEL_MASK		= 3 << DCLK_EBC_SEL_SHIFT,
	DCLK_EBC_SEL_GPLL_400M		= 0,
	DCLK_EBC_SEL_CPLL_333M,
	DCLK_EBC_SEL_GPLL_200M,

	/* CRU_CLK_SEL47_CON */
	ACLK_RKVDEC_SEL_SHIFT		= 7,
	ACLK_RKVDEC_SEL_MASK		= 1 << ACLK_RKVDEC_SEL_SHIFT,
	ACLK_RKVDEC_SEL_GPLL		= 0,
	ACLK_RKVDEC_SEL_CPLL,
	ACLK_RKVDEC_DIV_SHIFT		= 0,
	ACLK_RKVDEC_DIV_MASK		= 0x1f << ACLK_RKVDEC_DIV_SHIFT,

	/* CRU_CLK_SEL49_CON */
	CLK_RKVDEC_CORE_SEL_SHIFT	= 14,
	CLK_RKVDEC_CORE_SEL_MASK	= 0x3 << CLK_RKVDEC_CORE_SEL_SHIFT,
	CLK_RKVDEC_CORE_SEL_GPLL	= 0,
	CLK_RKVDEC_CORE_SEL_CPLL,
	CLK_RKVDEC_CORE_SEL_NPLL,
	CLK_RKVDEC_CORE_SEL_VPLL,
	CLK_RKVDEC_CORE_DIV_SHIFT	= 8,
	CLK_RKVDEC_CORE_DIV_MASK	= 0x1f << CLK_RKVDEC_CORE_DIV_SHIFT,

	/* CRU_CLK_SEL50_CON */
	PCLK_BUS_SEL_SHIFT		= 4,
	PCLK_BUS_SEL_MASK		= 3 << PCLK_BUS_SEL_SHIFT,
	PCLK_BUS_SEL_100M		= 0,
	PCLK_BUS_SEL_75M,
	PCLK_BUS_SEL_50M,
	PCLK_BUS_SEL_24M,
	ACLK_BUS_SEL_SHIFT		= 0,
	ACLK_BUS_SEL_MASK		= 3 << ACLK_BUS_SEL_SHIFT,
	ACLK_BUS_SEL_200M		= 0,
	ACLK_BUS_SEL_150M,
	ACLK_BUS_SEL_100M,
	ACLK_BUS_SEL_24M,

	/* CRU_CLK_SEL51_CON */
	CLK_TSADC_DIV_SHIFT		= 8,
	CLK_TSADC_DIV_MASK		= 0x7f << CLK_TSADC_DIV_SHIFT,
	CLK_TSADC_TSEN_SEL_SHIFT	= 4,
	CLK_TSADC_TSEN_SEL_MASK		= 0x3 << CLK_TSADC_TSEN_SEL_SHIFT,
	CLK_TSADC_TSEN_SEL_24M		= 0,
	CLK_TSADC_TSEN_SEL_100M,
	CLK_TSADC_TSEN_SEL_CPLL_100M,
	CLK_TSADC_TSEN_DIV_SHIFT	= 0,
	CLK_TSADC_TSEN_DIV_MASK		= 0x7 << CLK_TSADC_TSEN_DIV_SHIFT,

	/* CRU_CLK_SEL52_CON */
	CLK_UART_SEL_SHIFT		= 12,
	CLK_UART_SEL_MASK		= 0x3 << CLK_UART_SEL_SHIFT,
	CLK_UART_SEL_SRC		= 0,
	CLK_UART_SEL_FRAC,
	CLK_UART_SEL_XIN24M,
	CLK_UART_SRC_SEL_SHIFT		= 8,
	CLK_UART_SRC_SEL_MASK		= 0x3 << CLK_UART_SRC_SEL_SHIFT,
	CLK_UART_SRC_SEL_GPLL		= 0,
	CLK_UART_SRC_SEL_CPLL,
	CLK_UART_SRC_SEL_480M,
	CLK_UART_SRC_DIV_SHIFT		= 0,
	CLK_UART_SRC_DIV_MASK		= 0x3f << CLK_UART_SRC_DIV_SHIFT,

	/* CRU_CLK_SEL53_CON */
	CLK_UART_FRAC_NUMERATOR_SHIFT	= 16,
	CLK_UART_FRAC_NUMERATOR_MASK	= 0xffff << 16,
	CLK_UART_FRAC_DENOMINATOR_SHIFT	= 0,
	CLK_UART_FRAC_DENOMINATOR_MASK	= 0xffff,

	/* CRU_CLK_SEL71_CON */
	CLK_I2C_SEL_SHIFT		= 8,
	CLK_I2C_SEL_MASK		= 3 << CLK_I2C_SEL_SHIFT,
	CLK_I2C_SEL_200M		= 0,
	CLK_I2C_SEL_100M,
	CLK_I2C_SEL_24M,
	CLK_I2C_SEL_CPLL_100M,

	/* CRU_CLK_SEL72_CON */
	CLK_PWM3_SEL_SHIFT		= 12,
	CLK_PWM3_SEL_MASK		= 3 << CLK_PWM3_SEL_SHIFT,
	CLK_PWM2_SEL_SHIFT		= 10,
	CLK_PWM2_SEL_MASK		= 3 << CLK_PWM2_SEL_SHIFT,
	CLK_PWM1_SEL_SHIFT		= 8,
	CLK_PWM1_SEL_MASK		= 3 << CLK_PWM1_SEL_SHIFT,
	CLK_PWM_SEL_100M		= 0,
	CLK_PWM_SEL_24M,
	CLK_PWM_SEL_CPLL_100M,
	CLK_SPI3_SEL_SHIFT		= 6,
	CLK_SPI3_SEL_MASK		= 3 << CLK_SPI3_SEL_SHIFT,
	CLK_SPI2_SEL_SHIFT		= 4,
	CLK_SPI2_SEL_MASK		= 3 << CLK_SPI2_SEL_SHIFT,
	CLK_SPI1_SEL_SHIFT		= 2,
	CLK_SPI1_SEL_MASK		= 3 << CLK_SPI1_SEL_SHIFT,
	CLK_SPI0_SEL_SHIFT		= 0,
	CLK_SPI0_SEL_MASK		= 3 << CLK_SPI0_SEL_SHIFT,
	CLK_SPI_SEL_200M		= 0,
	CLK_SPI_SEL_24M,
	CLK_SPI_SEL_CPLL_100M,

	/* CRU_CLK_SEL73_CON */
	PCLK_TOP_SEL_SHIFT		= 12,
	PCLK_TOP_SEL_MASK		= 3 << PCLK_TOP_SEL_SHIFT,
	PCLK_TOP_SEL_100M		= 0,
	PCLK_TOP_SEL_75M,
	PCLK_TOP_SEL_50M,
	PCLK_TOP_SEL_24M,
	HCLK_TOP_SEL_SHIFT		= 8,
	HCLK_TOP_SEL_MASK		= 3 << HCLK_TOP_SEL_SHIFT,
	HCLK_TOP_SEL_150M		= 0,
	HCLK_TOP_SEL_100M,
	HCLK_TOP_SEL_75M,
	HCLK_TOP_SEL_24M,
	ACLK_TOP_LOW_SEL_SHIFT		= 4,
	ACLK_TOP_LOW_SEL_MASK		= 3 << ACLK_TOP_LOW_SEL_SHIFT,
	ACLK_TOP_LOW_SEL_400M		= 0,
	ACLK_TOP_LOW_SEL_300M,
	ACLK_TOP_LOW_SEL_200M,
	ACLK_TOP_LOW_SEL_24M,
	ACLK_TOP_HIGH_SEL_SHIFT		= 0,
	ACLK_TOP_HIGH_SEL_MASK		= 3 << ACLK_TOP_HIGH_SEL_SHIFT,
	ACLK_TOP_HIGH_SEL_500M		= 0,
	ACLK_TOP_HIGH_SEL_400M,
	ACLK_TOP_HIGH_SEL_300M,
	ACLK_TOP_HIGH_SEL_24M,

	/* CRU_CLK_SEL78_CON */
	CPLL_500M_DIV_SHIFT		= 8,
	CPLL_500M_DIV_MASK		= 0x1f << CPLL_500M_DIV_SHIFT,

	/* CRU_CLK_SEL79_CON */
	CPLL_250M_DIV_SHIFT		= 8,
	CPLL_250M_DIV_MASK		= 0x1f << CPLL_250M_DIV_SHIFT,
	CPLL_333M_DIV_SHIFT		= 0,
	CPLL_333M_DIV_MASK		= 0x1f << CPLL_333M_DIV_SHIFT,

	/* CRU_CLK_SEL80_CON */
	CPLL_62P5M_DIV_SHIFT		= 8,
	CPLL_62P5M_DIV_MASK		= 0x1f << CPLL_62P5M_DIV_SHIFT,
	CPLL_125M_DIV_SHIFT		= 0,
	CPLL_125M_DIV_MASK		= 0x1f << CPLL_125M_DIV_SHIFT,

	/* CRU_CLK_SEL81_CON */
	CPLL_25M_DIV_SHIFT		= 8,
	CPLL_25M_DIV_MASK		= 0x3f << CPLL_25M_DIV_SHIFT,
	CPLL_50M_DIV_SHIFT		= 0,
	CPLL_50M_DIV_MASK		= 0x1f << CPLL_50M_DIV_SHIFT,

	/* CRU_CLK_SEL82_CON */
	CPLL_100M_DIV_SHIFT		= 0,
	CPLL_100M_DIV_MASK		= 0x1f << CPLL_100M_DIV_SHIFT,

	/* GRF_SOC_CON2 */
	I2S3_MCLKOUT_SEL_SHIFT		= 15,
	I2S3_MCLKOUT_SEL_MASK		= 0x1 << I2S3_MCLKOUT_SEL_SHIFT,
	I2S3_MCLKOUT_SEL_RX		= 0,
	I2S3_MCLKOUT_SEL_TX,
	I2S3_MCLK_IOE_SEL_SHIFT		= 3,
	I2S3_MCLK_IOE_SEL_MASK		= 0x1 << I2S3_MCLK_IOE_SEL_SHIFT,
	I2S3_MCLK_IOE_SEL_CLKIN		= 0,
	I2S3_MCLK_IOE_SEL_CLKOUT,

};
#endif
