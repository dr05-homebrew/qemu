/*
 * Blackfin SPI controller model.
 *
 * Copyright 2007-2016 Mike Frysinger
 * Copyright 2007-2011 Analog Devices, Inc.
 * Copyright 2016      Jonathan Neuschäfer
 *
 * Licensed under the Lesser GPL 2 or later.
 */

#include "qemu/osdep.h"
#include "bfin_devices.h"
#include "bfin_spi.h"
#include "hw/hw.h"
#include "hw/ssi/ssi.h"
#include "hw/sysbus.h"
#include "trace.h"

#define TYPE_BFIN_SPI "bfin_spi"
#define BFIN_SPI(obj) OBJECT_CHECK(BfinSPIState, (obj), TYPE_BFIN_SPI)

typedef struct BfinSPIState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    SSIBus *bus;

    /* Order after here is important -- matches hardware MMR layout.  */
    bu16 BFIN_MMR_16(spi_ctl);
    bu16 BFIN_MMR_16(spi_flg);
    bu16 BFIN_MMR_16(spi_stat);
    bu16 BFIN_MMR_16(spi_tdbr);
    bu16 BFIN_MMR_16(spi_rdbr);
    bu16 BFIN_MMR_16(spi_baud);
    bu16 BFIN_MMR_16(spi_shadow);
} BfinSPIState;
#define BfinMMRState BfinSPIState
#define mmr_base() offsetof(BfinMMRState, spi_ctl)

/* TODO: Print SPI0 or SPI1 instead of just SPI */
static const char * const mmr_names[] =
{
    "SPI_CTL",
    "SPI_FLG",
    "SPI_STAT",
    "SPI_TDBR",
    "SPI_RDBR",
    "SPI_BAUD",
    "SPI_SHADOW",
};

/* Perform a single SPI transfer */
static void bfin_spi_do_transfer(BfinSPIState *s)
{
    uint8_t byte;

    /* 16-bit mode isn't supported for now. */
    assert((s->spi_ctl & SPI_CTL_SIZE) == 0);

    byte = s->spi_tdbr;

    byte = ssi_transfer(s->bus, byte);

    s->spi_rdbr = byte;
    s->spi_stat |= SPI_STAT_RXS;
    s->spi_stat |= SPI_STAT_SPIF;
}

static void bfin_spi_io_write(void *opaque, hwaddr addr,
                              uint64_t value, unsigned size)
{
    BfinSPIState *s = opaque;
    bu16 *valuep = (void *)((uintptr_t)s + mmr_base() + addr);

    HW_TRACE_WRITE();

    switch (addr) {
    case mmr_offset(spi_rdbr):
    case mmr_offset(spi_shadow):
        /* Discard writes.  */
        break;
    case mmr_offset(spi_ctl):
        s->spi_ctl = value & SPI_CTL_VALID_MASK;
        break;
    case mmr_offset(spi_flg):
        s->spi_flg = (value & SPI_FLG_VALID_MASK) | SPI_FLG_ON_MASK;
        break;
    case mmr_offset(spi_stat):
        /* Only clear W1C (write 1 to clear) bits */
        s->spi_stat &= ~(value & SPI_STAT_W1C_MASK);
        break;
    case mmr_offset(spi_tdbr):
        s->spi_tdbr = value;
        if ((s->spi_ctl & SPI_CTL_MSTR) && (s->spi_ctl & SPI_CTL_SPE) &&
            (s->spi_ctl & SPI_CTL_TIMOD_MASK) == SPI_CTL_TIMOD_TINT) {
            bfin_spi_do_transfer(s);
        }
        break;
    default:
        /* TODO: bounds checking? */
        *valuep = value;
        break;
    }
}

static uint64_t bfin_spi_io_read(void *opaque, hwaddr addr, unsigned size)
{
    BfinSPIState *s = opaque;
    bu16 *valuep = (void *)((uintptr_t)s + mmr_base() + addr);

    HW_TRACE_READ();

    switch (addr) {
    case mmr_offset(spi_rdbr): {
        uint16_t value = s->spi_rdbr;
        s->spi_stat &= ~SPI_STAT_RXS;

        if ((s->spi_ctl & SPI_CTL_MSTR) && (s->spi_ctl & SPI_CTL_SPE) &&
            (s->spi_ctl & SPI_CTL_TIMOD_MASK) == SPI_CTL_TIMOD_RINT) {
            bfin_spi_do_transfer(s);
        }

        return value;
    }

    default:
        /* TODO: bounds checking? */
        return *valuep;
    }
}

static const MemoryRegionOps bfin_spi_io_ops = {
    .read = bfin_spi_io_read,
    .write = bfin_spi_io_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .valid = {
        .min_access_size = 2,
        .max_access_size = 4,
    },
};

static void bfin_spi_reset(DeviceState *d)
{
    BfinSPIState *s = BFIN_SPI(d);

    s->spi_ctl = 0x0400;
    s->spi_flg = 0xFF00;
    s->spi_stat = 0x0001;
    s->spi_tdbr = 0x0000;
    s->spi_rdbr = 0x0000;
    s->spi_baud = 0x0000;
    s->spi_shadow = 0x0000;
}

static int bfin_spi_init(SysBusDevice *sbd)
{
    DeviceState *dev = DEVICE(sbd);
    BfinSPIState *s = BFIN_SPI(dev);

    s->bus = ssi_create_bus(dev, "spi");
    memory_region_init_io(&s->iomem, OBJECT(s), &bfin_spi_io_ops, s, "spi", mmr_size());
    sysbus_init_mmio(sbd, &s->iomem);

    return 0;
}

static void bfin_spi_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    SysBusDeviceClass *k = SYS_BUS_DEVICE_CLASS(klass);

    k->init = bfin_spi_init;

    //dc->realize = bfin_spi_realize;
    //dc->vmsd = &vmstate_imx_spi;
    dc->reset = bfin_spi_reset;
    dc->desc = "Blackfin SPI Controller";
}

static TypeInfo bfin_spi_info = {
    .name          = TYPE_BFIN_SPI,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(BfinSPIState),
    .class_init    = bfin_spi_class_init,
};

static void bfin_spi_register_types(void)
{
    type_register_static(&bfin_spi_info);
}

type_init(bfin_spi_register_types)
