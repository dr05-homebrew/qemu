/*
 * Blackfin GPIO controller model.
 *
 * Copyright 2016      Jonathan Neuschäfer
 *
 * Licensed under the Lesser GPL 2 or later.
 */

#include "qemu/osdep.h"
#include "bfin_devices.h"
#include "hw/hw.h"
#include "hw/ssi/ssi.h"
#include "hw/sysbus.h"
#include "trace.h"

#define TYPE_BFIN_PORT "bfin_port"
#define BFIN_PORT(obj) OBJECT_CHECK(BfinPortState, (obj), TYPE_BFIN_PORT)

typedef struct BfinPortState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;

    /* Order after here is important -- matches hardware MMR layout.  */
    bu16 BFIN_MMR_16(port_io);
    bu16 BFIN_MMR_16(port_io_set);
    bu16 BFIN_MMR_16(port_io_clear);
    bu16 BFIN_MMR_16(port_io_maska);
    bu16 BFIN_MMR_16(port_io_maska_clear);
    bu16 BFIN_MMR_16(port_io_maska_set);
    bu16 BFIN_MMR_16(port_io_maska_toggle);
    bu16 BFIN_MMR_16(port_io_maskb);
    bu16 BFIN_MMR_16(port_io_maskb_clear);
    bu16 BFIN_MMR_16(port_io_maskb_set);
    bu16 BFIN_MMR_16(port_io_maskb_toggle);
    bu16 BFIN_MMR_16(port_io_dir);
    bu16 BFIN_MMR_16(port_io_polar);
    bu16 BFIN_MMR_16(port_io_edge);
    bu16 BFIN_MMR_16(port_io_both);
    bu16 BFIN_MMR_16(port_io_inen);

} BfinPortState;
#define BfinMMRState BfinPortState
#define mmr_base() offsetof(BfinMMRState, port_io)

/* TODO: Print PORTFIO instead of PORTxIO */
static const char * const mmr_names[] =
{
    "PORTxIO",
    "PORTxIO_SET",
    "PORTxIO_CLEAR",
    "PORTxIO_MASKA",
    "PORTxIO_MASKA_CLEAR",
    "PORTxIO_MASKA_SET",
    "PORTxIO_MASKA_TOGGLE",
    "PORTxIO_MASKB",
    "PORTxIO_MASKB_CLEAR",
    "PORTxIO_MASKB_SET",
    "PORTxIO_MASKB_TOGGLE",
    "PORTxIO_DIR",
    "PORTxIO_POLAR",
    "PORTxIO_EDGE",
    "PORTxIO_BOTH",
    "PORTxIO_INEN",
};

static void bfin_port_io_write(void *opaque, hwaddr addr,
                              uint64_t value, unsigned size)
{
    BfinPortState *s = opaque;
    bu16 *valuep = (void *)((uintptr_t)s + mmr_base() + addr);

    HW_TRACE_WRITE();

    switch (addr) {
    default:
        /* TODO: bounds checking? */
        *valuep = value;
        break;
    }
}

static uint64_t bfin_port_io_read(void *opaque, hwaddr addr, unsigned size)
{
    BfinPortState *s = opaque;
    bu16 *valuep = (void *)((uintptr_t)s + mmr_base() + addr);

    HW_TRACE_READ();

    switch (addr) {
    default:
        /* TODO: bounds checking? */
        return *valuep;
    }
}

static const MemoryRegionOps bfin_port_io_ops = {
    .read = bfin_port_io_read,
    .write = bfin_port_io_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .valid = {
        .min_access_size = 2,
        .max_access_size = 4,
    },
};

static void bfin_port_reset(DeviceState *d)
{
    BfinPortState *s = BFIN_PORT(d);

    s->port_io = 0;
    /* TODO */
}

static int bfin_port_init(SysBusDevice *sbd)
{
    DeviceState *dev = DEVICE(sbd);
    BfinPortState *s = BFIN_PORT(dev);

    memory_region_init_io(&s->iomem, OBJECT(s), &bfin_port_io_ops, s, "port", mmr_size());
    sysbus_init_mmio(sbd, &s->iomem);

    return 0;
}

static void bfin_port_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    SysBusDeviceClass *k = SYS_BUS_DEVICE_CLASS(klass);

    k->init = bfin_port_init;

    dc->reset = bfin_port_reset;
    dc->desc = "Blackfin Port Controller";
}

static TypeInfo bfin_port_info = {
    .name          = TYPE_BFIN_PORT,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(BfinPortState),
    .class_init    = bfin_port_class_init,
};

static void bfin_port_register_types(void)
{
    type_register_static(&bfin_port_info);
}

type_init(bfin_port_register_types)
