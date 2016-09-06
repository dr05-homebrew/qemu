/*
 * Blackfin Serial Peripheral Interface (SPI) model.
 *
 * Copyright 2016 Jonathan Neuschäfer
 *
 * Licensed under the Lesser GPL 2 or later.
 */

#ifndef BFIN_SPI_H
#define BFIN_SPI_H

#define SPI_CTL_TIMOD_SHIFT 0
#define SPI_CTL_TIMOD_MASK  (3 <<  0)
#define SPI_CTL_TIMOD_RINT  (0 <<  0)
#define SPI_CTL_TIMOD_TINT  (1 <<  0)
#define SPI_CTL_TIMOD_RDMA  (2 <<  0)
#define SPI_CTL_TIMOD_TDMA  (3 <<  0)
#define SPI_CTL_SZ          (1 <<  2)
#define SPI_CTL_GM          (1 <<  3)
#define SPI_CTL_PSSE        (1 <<  4)
#define SPI_CTL_EMISO       (1 <<  5)
#define SPI_CTL_SIZE        (1 <<  8)
#define SPI_CTL_LSBF        (1 <<  9)
#define SPI_CTL_CPHA        (1 << 10)
#define SPI_CTL_CPOL        (1 << 11)
#define SPI_CTL_MSTR        (1 << 12)
#define SPI_CTL_WOM         (1 << 13)
#define SPI_CTL_SPE         (1 << 14)
#define SPI_CTL_VALID_MASK  0x7f3f

#define SPI_FLG_FLS1        (1 << 1)
#define SPI_FLG_FLS2        (1 << 2)
#define SPI_FLG_FLS3        (1 << 3)
#define SPI_FLG_FLS4        (1 << 4)
#define SPI_FLG_FLS5        (1 << 5)
#define SPI_FLG_FLS6        (1 << 6)
#define SPI_FLG_FLS7        (1 << 7)
#define SPI_FLG_FLG1        (1 << 1)
#define SPI_FLG_FLG2        (1 << 2)
#define SPI_FLG_FLG3        (1 << 3)
#define SPI_FLG_FLG4        (1 << 4)
#define SPI_FLG_FLG5        (1 << 5)
#define SPI_FLG_FLG6        (1 << 6)
#define SPI_FLG_FLG7        (1 << 7)
#define SPI_FLG_VALID_MASK  0xefef
#define SPI_FLG_ON_MASK     0x0100

#define SPI_STAT_SPIF       (1 << 0)
#define SPI_STAT_MODF       (1 << 1)
#define SPI_STAT_TXE        (1 << 2)
#define SPI_STAT_TXS        (1 << 3)
#define SPI_STAT_RBSY       (1 << 4)
#define SPI_STAT_RXS        (1 << 5)
#define SPI_STAT_TXCOL      (1 << 6)
#define SPI_STAT_W1C_MASK   0x56

#endif
