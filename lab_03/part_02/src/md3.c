#include <linux/init.h>
#include <linux/module.h>

#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kononenko Sergey");

static int __init md_init(void)
{
    printk("> module md3 loaded\n");
    printk("> data exported from md1        : %s\n", md1_data);
    printk("> data returned from md1_proc() : %s\n", md1_proc());

    return -1;
}

module_init(md_init);
