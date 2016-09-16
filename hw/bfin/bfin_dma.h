/*
 * Blackfin DMA engine model
 *
 * Copyright 2016 Jonathan Neuschäfer
 *
 * Licensed under the Lesser GPL 2 or later.
 */

#ifndef BFIN_DMA_H
#define BFIN_DMA_H

#define DMA_CONFIG_DMAEN                (1 << 0)
#define DMA_CONFIG_WNR                  (1 << 1)
#define DMA_CONFIG_WDSIZE_SHIFT         2
#define DMA_CONFIG_WDSIZE_MASK          (3 << 2)
#define DMA_CONFIG_WDSIZE_8BIT          (0 << 2)
#define DMA_CONFIG_WDSIZE_16BIT         (1 << 2)
#define DMA_CONFIG_WDSIZE_32BIT         (2 << 2)
#define DMA_CONFIG_DMA2D                (1 << 4)
#define DMA_CONFIG_SYNC                 (1 << 5)
#define DMA_CONFIG_DI_SEL               (1 << 6)
#define DMA_CONFIG_DI_EN                (1 << 7)
#define DMA_CONFIG_NDSIZE_SHIFT         8
#define DMA_CONFIG_NDSIZE_MASK          (15 << 8)
#define DMA_CONFIG_FLOW_SHIFT           12
#define DMA_CONFIG_FLOW_MASK            (7 << 12)
#define DMA_CONFIG_FLOW_STOP            (0 << 12)
#define DMA_CONFIG_FLOW_AUTOBUF         (1 << 12)
#define DMA_CONFIG_FLOW_DESC_ARR        (4 << 12)
#define DMA_CONFIG_FLOW_DESC_LIST_S     (6 << 12)
#define DMA_CONFIG_FLOW_DESC_LIST_L     (7 << 12)

#define DMA_IRQ_STATUS_DMA_DONE         (1 << 0)
#define DMA_IRQ_STATUS_DMA_ERR          (1 << 1)
#define DMA_IRQ_STATUS_DFETCH           (1 << 2)
#define DMA_IRQ_STATUS_DMA_RUN          (1 << 3)
#define DMA_IRQ_STATUS_W1C_MASK         0x3

#define HMDMA_CONTROL_HMDMAEN           (1 << 0)
#define HMDMA_CONTROL_REP               (1 << 1)
#define HMDMA_CONTROL_UTE               (1 << 3)
#define HMDMA_CONTROL_OIE               (1 << 4)
#define HMDMA_CONTROL_BDIE              (1 << 5)
#define HMDMA_CONTROL_MBDI              (1 << 6)
#define HMDMA_CONTROL_DRQ_SHIFT         8
#define HMDMA_CONTROL_DRQ_MASK          (3 << 8)
#define HMDMA_CONTROL_DRQ_NONE          (0 << 8)
#define HMDMA_CONTROL_DRQ_SINGLE        (1 << 8)
#define HMDMA_CONTROL_DRQ_MULTI         (2 << 8)
#define HMDMA_CONTROL_DRQ_MULTI_URG     (3 << 8)
#define HMDMA_CONTROL_RBC               (1 << 12)
#define HMDMA_CONTROL_PS                (1 << 13)
#define HMDMA_CONTROL_IO                (1 << 14)
#define HMDMA_CONTROL_BDI               (1 << 15)
#define HMDMA_CONTROL_RW_MASK           0x027b
#define HMDMA_CONTROL_W1C_MASK          0xc000

#define DMA_PERIPHERAL_MAP_PMAP_SHIFT   12
#define DMA_PERIPHERAL_MAP_PMAP_MASK    (15 << 12)
#define DMA_PERIPHERAL_MAP_CTYPE        (1 << 6)

#endif
