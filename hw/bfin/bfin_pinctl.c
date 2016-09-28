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

#define TYPE_BFIN_PINCTL "bfin_pinctl"
#define BFIN_PINCTL(obj) OBJECT_CHECK(BfinPinctlState, (obj), TYPE_BFIN_PINCTL)

typedef struct BfinPinctlState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;

    /* Order after here is important -- matches hardware MMR layout.  */
    bu16 BFIN_MMR_16(portf_fer);
    bu16 BFIN_MMR_16(portg_fer);
    bu16 BFIN_MMR_16(porth_fer);
    bu16 BFIN_MMR_16(portf_mux);
    bu16 BFIN_MMR_16(portg_mux);
    bu16 BFIN_MMR_16(porth_mux);
    bu32 _dummy_1c;
    bu32 _dummy_20, _dummy_24, _dummy_28, _dummy_2c;
    bu32 _dummy_30, _dummy_34, _dummy_38, _dummy_3c;
    bu16 BFIN_MMR_16(portf_hysteresis);
    bu16 BFIN_MMR_16(portg_hysteresis);
    bu16 BFIN_MMR_16(porth_hysteresis);
    bu32 _dummy_4c;
    bu32 _dummy_50, _dummy_54, _dummy_58, _dummy_5c;
    bu32 _dummy_60, _dummy_64, _dummy_68, _dummy_6c;
    bu32 _dummy_70, _dummy_74, _dummy_78, _dummy_7c;
    bu16 BFIN_MMR_16(nongpio_drive);
    bu32 _dummy_84;
    bu16 BFIN_MMR_16(nongpio_hysteresis);
} BfinPinctlState;
#define BfinMMRState BfinPinctlState
#define mmr_base() offsetof(BfinMMRState, portf_fer)

static const char * const mmr_names[] =
{
    "PORTF_FER",
    "PORTG_FER",
    "PORTH_FER",
    "PORTF_MUX",
    "PORTG_MUX",
    "PORTH_MUX",
    "<INV>",
    "<INV>", "<INV>", "<INV>", "<INV>",
    "<INV>", "<INV>", "<INV>", "<INV>",
    "PORTF_HYSTERESIS",
    "PORTG_HYSTERESIS",
    "PORTH_HYSTERESIS",
    "<INV>",
    "<INV>", "<INV>", "<INV>", "<INV>",
    "<INV>", "<INV>", "<INV>", "<INV>",
    "<INV>", "<INV>", "<INV>", "<INV>",
    "NONGPIO_DRIVE",
    "<INV>",
    "NONGPIO_HYSTERESIS",
};

static void bfin_pinctl_io_write(void *opaque, hwaddr addr,
                              uint64_t value, unsigned size)
{
    //BfinPinctlState *s = opaque;

    HW_TRACE_WRITE();
}

static uint64_t bfin_pinctl_io_read(void *opaque, hwaddr addr, unsigned size)
{
    //BfinPinctlState *s = opaque;

    HW_TRACE_READ();

    return 0;
}

static const MemoryRegionOps bfin_pinctl_io_ops = {
    .read = bfin_pinctl_io_read,
    .write = bfin_pinctl_io_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .valid = {
        .min_access_size = 2,
        .max_access_size = 4,
    },
};

static void bfin_pinctl_reset(DeviceState *d)
{
    BfinPinctlState *s = BFIN_PINCTL(d);

    s->portf_fer = 0;
    /* TODO */
}

static int bfin_pinctl_init(SysBusDevice *sbd)
{
    DeviceState *dev = DEVICE(sbd);
    BfinPinctlState *s = BFIN_PINCTL(dev);

    memory_region_init_io(&s->iomem, OBJECT(s), &bfin_pinctl_io_ops, s, "pinctl", mmr_size());
    sysbus_init_mmio(sbd, &s->iomem);

    return 0;
}

static void bfin_pinctl_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    SysBusDeviceClass *k = SYS_BUS_DEVICE_CLASS(klass);

    k->init = bfin_pinctl_init;

    dc->reset = bfin_pinctl_reset;
    dc->desc = "Blackfin Pin Configuration Controller";
}

static TypeInfo bfin_pinctl_info = {
    .name          = TYPE_BFIN_PINCTL,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(BfinPinctlState),
    .class_init    = bfin_pinctl_class_init,
};

static void bfin_pinctl_register_types(void)
{
    type_register_static(&bfin_pinctl_info);
}

type_init(bfin_pinctl_register_types)
