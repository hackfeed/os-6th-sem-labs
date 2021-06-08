#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define FILENAME "tasklet_file"
#define MAX_BUF_SIZE 200 * PAGE_SIZE

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kononenko Sergey");

static struct tasklet_struct tasklet;
static struct proc_dir_entry *tasklet_file = NULL;

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

char tasklet_data[] = "Key pressed";
int define_irq = 1;

static void tasklet_handler(unsigned long data)
{
    int len;
    printk("%s\n", (char *)data);
    len = snprintf(tmp + cur_pos, MAX_BUF_SIZE - cur_pos - 1, "State: %ld, Count: %d, Data: %s\n",
                   tasklet.state, atomic_read(&tasklet.count), (char *)tasklet.data);
    cur_pos += len;
    tmp[cur_pos] = '\0';
}

static irqreturn_t irq_handler(int irq, void *dev, struct pt_regs *regs)
{
    if (irq == define_irq)
    {
        int len = snprintf(tmp + cur_pos, MAX_BUF_SIZE - cur_pos - 1, "State: %ld, Count: %d, Data: %s\n",
                           tasklet.state, atomic_read(&tasklet.count), (char *)tasklet.data);
        cur_pos += len;
        tasklet_schedule(&tasklet);
        len = snprintf(tmp + cur_pos, MAX_BUF_SIZE - cur_pos - 1, "State: %ld, Count: %d, Data: %s\n",
                       tasklet.state, atomic_read(&tasklet.count), (char *)tasklet.data);
        cur_pos += len;
        tmp[cur_pos] = '\0';

        return IRQ_HANDLED;
    }
    else
    {
        return IRQ_NONE;
    }
}

static void freemem(void)
{
    if (tasklet_file)
    {
        remove_proc_entry(FILENAME, NULL);
    }
}

int init_module(void)
{
    if (!(tasklet_file = proc_create(FILENAME, 0666, NULL, &pops)))
    {
        freemem();
        printk(KERN_ERR "+: error during file creation\n");

        return -ENOMEM;
    }

    tasklet_init(&tasklet, &tasklet_handler, (unsigned long)tasklet_data);

    return request_irq(define_irq, (irq_handler_t)irq_handler,
                       IRQF_SHARED, "test_irq_handler", (void *)(irq_handler));
}

void cleanup_module(void)
{
    freemem();
    tasklet_kill(&tasklet);
    free_irq(define_irq, (void *)(irq_handler));
}