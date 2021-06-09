#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <asm/io.h>

#define FILENAME "queue_file"
#define MAX_BUF_SIZE 200 * PAGE_SIZE

#define KBD_DATA_REG 0x60
#define KBD_SCANCODE_MASK 0x7f
#define KBD_STATUS_MASK 0x80

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kononenko Sergey");

static char *ascii_map[] = {
    "[ESC]", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "bs", "[Tab]", "Q",
    "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "[Enter]", "[CTRL]", "A", "S", "D",
    "F", "G", "H", "J", "K", "L", ";", "\'", "`", "[LShift]", "\\", "Z", "X", "C", "V", "B", "N", "M",
    ",", ".", "/", "[RShift]", "[PrtSc]", "[Alt]", " ", "[Caps]", "F1", "F2", "F3", "F4", "F5",
    "F6", "F7", "F8", "F9", "F10", "[Num]", "[Scroll]", "[Home(7)]", "[Up(8)]", "[PgUp(9)]", "-",
    "[Left(4)]", "[Center(5)]", "[Right(6)]", "+", "[End(1)]", "[Down(2)]", "[PgDn(3)]", "[Ins]", "[Del]"};

static struct workqueue_struct *queue;
static struct work_struct *work1, *work2;
static struct proc_dir_entry *queue_file = NULL;

static char tmp[MAX_BUF_SIZE] = {0};
static int cur_pos = 0;

static int seq_show(struct seq_file *m, void *v)
{
    seq_printf(m, "%s", tmp);
    tmp[0] = '\0';
    cur_pos = 0;
    printk(KERN_INFO "+: show is called\n");

    return 0;
}

static int open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "+: open is called\n");

    return single_open(file, seq_show, NULL);
}

static int release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "+: release is called\n");

    return single_release(inode, file);
}

static struct proc_ops pops = {
    .proc_read = seq_read,
    .proc_open = open,
    .proc_release = release,
};

int define_irq = 1;

static void work1_handler(struct work_struct *work)
{
    char scancode;
    char *key;

    scancode = inb(KBD_DATA_REG);
    key = ascii_map[(scancode & KBD_SCANCODE_MASK) - 1];
    if ((scancode & KBD_STATUS_MASK) == 0)
    {
        printk("%s is pressed\n", key);
    }
}

static void work2_handler(struct work_struct *work)
{
    // int len;
    printk("Work2 data: %ld, current cpu: %u\n", atomic_long_read(&work->data), smp_processor_id());
    // len = snprintf(tmp + cur_pos, MAX_BUF_SIZE - cur_pos - 1, "!WQ! nr_drainers: %d, saved_max_active: %d\n",
    //                queue->nr_drainers, queue->saved_max_active);
    // cur_pos += len;
    // tmp[cur_pos] = '\0';
}

static irqreturn_t irq_handler(int irq, void *dev)
{
    if (irq == define_irq)
    {
        int ret;
        unsigned int cpu;

        if (queue)
        {
            work1 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
            if (work1)
            {
                INIT_WORK(work1, work1_handler);
                ret = queue_work(queue, (struct work_struct *)work1);
            }

            work2 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
            if (work2)
            {
                cpu = smp_processor_id();
                atomic_long_set(&work2->data, (long)cpu);
                INIT_WORK(work2, work2_handler);
                ret = queue_work(queue, (struct work_struct *)work2);
            }
        }

        return IRQ_HANDLED;
    }
    else
    {
        return IRQ_NONE;
    }
}

static void freemem(void)
{
    if (queue_file)
    {
        remove_proc_entry(FILENAME, NULL);
    }
}

int init_module(void)
{
    if (!(queue_file = proc_create(FILENAME, 0666, NULL, &pops)))
    {
        freemem();
        printk(KERN_ERR "+: error during file creation\n");

        return -ENOMEM;
    }

    queue = create_workqueue("my_queue");

    return request_irq(define_irq, (irq_handler_t)irq_handler,
                       IRQF_SHARED, "test_irq_handler", (void *)(irq_handler));
}

void cleanup_module(void)
{
    freemem();
    flush_workqueue(queue);
    free_irq(define_irq, (void *)(irq_handler));
}