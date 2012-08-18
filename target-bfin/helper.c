/*
 * Blackfin helpers
 *
 * Copyright 2007-2012 Mike Frysinger
 * Copyright 2007-2011 Analog Devices, Inc.
 *
 * Licensed under the Lesser GPL 2 or later.
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <signal.h>
#include <assert.h>

#include "config.h"
#include "cpu.h"
#include "exec-all.h"
#include "host-utils.h"

#if defined(CONFIG_USER_ONLY)

void do_interrupt(CPUArchState *env)
{
    env->exception_index = -1;
}

int cpu_handle_mmu_fault(CPUArchState *env, target_ulong address, int rw,
                         int mmu_idx)
{
    env->exception_index = EXCP_DCPLB_VIOLATE;
    return 1;
}

#else

void do_interrupt(CPUArchState *env)
{
    qemu_log_mask(CPU_LOG_INT,
        "exception at pc=%x type=%x\n", env->pc, env->exception_index);

    switch (env->exception_index) {
    default:
        cpu_abort(env, "unhandled exception type=%d\n",
                  env->exception_index);
        break;
    }
}

target_phys_addr_t cpu_get_phys_page_debug(CPUArchState *env, target_ulong addr)
{
    return addr & TARGET_PAGE_MASK;
}

int cpu_handle_mmu_fault(CPUArchState *env, target_ulong address, int rw,
                         int mmu_idx)
{
    static const char * const rw_map[] = { "read", "write", "exec", };
    int prot;

    /* XXX: walk the CPLB tables here */
    printf("%s: %5s @ " TARGET_FMT_lx " (mmu_idx=%i)\n",
        __func__, rw_map[rw], address, mmu_idx);

    address &= TARGET_PAGE_MASK;
    prot = PAGE_BITS;
    tlb_set_page(env, address, address, prot, mmu_idx, TARGET_PAGE_SIZE);

    return 0;
}

#endif

typedef struct CPUListState {
    fprintf_function cpu_fprintf;
    FILE *file;
} BfinCPUListState;

/* Sort alphabetically by type name, except for "any". */
static gint cpu_list_compare(gconstpointer a, gconstpointer b)
{
    ObjectClass *class_a = (ObjectClass *)a;
    ObjectClass *class_b = (ObjectClass *)b;
    const char *name_a, *name_b;

    name_a = object_class_get_name(class_a);
    name_b = object_class_get_name(class_b);
    if (strcmp(name_a, "any") == 0) {
        return 1;
    } else if (strcmp(name_b, "any") == 0) {
        return -1;
    } else {
        return strcmp(name_a, name_b);
    }
}

static void cpu_list_entry(gpointer data, gpointer user_data)
{
    ObjectClass *oc = data;
    BfinCPUListState *s = user_data;

    (*s->cpu_fprintf)(s->file, "  %s\n",
                      object_class_get_name(oc));
}

void cpu_bfin_list(FILE *f, fprintf_function cpu_fprintf)
{
    BfinCPUListState s = {
        .file = f,
        .cpu_fprintf = cpu_fprintf,
    };
    GSList *list;

    list = object_class_get_list(TYPE_BFIN_CPU, false);
    list = g_slist_sort(list, cpu_list_compare);
    (*cpu_fprintf)(f, "Available CPUs:\n");
    g_slist_foreach(list, cpu_list_entry, &s);
    g_slist_free(list);
}
