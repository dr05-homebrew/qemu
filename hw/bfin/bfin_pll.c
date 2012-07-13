/*
 * Blackfin Phase Lock Loop (PLL) model.
 *
 * Copyright 2007-2014 Mike Frysinger
 * Copyright 2007-2011 Analog Devices, Inc.
 *
 * Licensed under the Lesser GPL 2 or later.
 */

#include "hw/hw.h"
#include "hw/sysbus.h"
#include "trace.h"
#include "bfin_devices.h"

#define TYPE_BFIN_PLL "bfin_pll"
#define BFIN_PLL(obj) OBJECT_CHECK(BfinPLLState, (obj), TYPE_BFIN_PLL)

typedef struct BfinPLLState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;

  /* Order after here is important -- matches hardware MMR layout.  */
  bu16 BFIN_MMR_16(pll_ctl);
  bu16 BFIN_MMR_16(pll_div);
  bu16 BFIN_MMR_16(vr_ctl);
  bu16 BFIN_MMR_16(pll_stat);
  bu16 BFIN_MMR_16(pll_lockcnt);

  /* XXX: Not really the best place for this ...  */
  bu32 chipid;
} BfinPLLState;
#define BfinMMRState BfinPLLState
#define mmr_base() offsetof(BfinMMRState, pll_ctl)

static const char * const mmr_names[] =
{
  "PLL_CTL", "PLL_DIV", "VR_CTL", "PLL_STAT", "PLL_LOCKCNT", "CHIPID",
};

static void bfin_pll_io_write(void *opaque, hwaddr addr,
                              uint64_t value, unsigned size)
{
    BfinPLLState *s = opaque;

    HW_TRACE_WRITE();
}

static uint64_t bfin_pll_io_read(void *opaque, hwaddr addr, unsigned size)
{
    BfinPLLState *s = opaque;

    HW_TRACE_READ();

    return 0;
}

static const MemoryRegionOps bfin_pll_io_ops = {
    .read = bfin_pll_io_read,
    .write = bfin_pll_io_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .valid = {
        .min_access_size = 2,
        .max_access_size = 4,
    },
};

static int bfin_pll_init(SysBusDevice *sbd)
{
    DeviceState *dev = DEVICE(sbd);
    BfinPLLState *s = BFIN_PLL(dev);

    memory_region_init_io(&s->iomem, OBJECT(s), &bfin_pll_io_ops, s, "pll", mmr_size());
    sysbus_init_mmio(sbd, &s->iomem);

    return 0;
}

static void bfin_pll_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    SysBusDeviceClass *k = SYS_BUS_DEVICE_CLASS(klass);

    k->init = bfin_pll_init;
}

static TypeInfo bfin_pll_info = {
    .name          = "bfin_pll",
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(BfinPLLState),
    .class_init    = bfin_pll_class_init,
};

static void bfin_pll_register_types(void)
{
    type_register_static(&bfin_pll_info);
}

type_init(bfin_pll_register_types)
