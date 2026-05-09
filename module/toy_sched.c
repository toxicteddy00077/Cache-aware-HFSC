#include <asm-generic/errno-base.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aryamaan Singh");
MODULE_DESCRIPTION("Just a toy");

static struct timer_list toy_timer;
static u64 timer_interval_ms = 2000;

// ---------- Memory ----------
typedef struct {
  int tick_cnt;
  char name[16];
} toy_arena_mem;

static toy_arena_mem *arena_mem;

// ---------- Timer heartbeat --------
static void toy_tick(struct timer_list *timer){
  arena_mem -> tick_cnt++;

  pr_info("ToySched [Tick %d]: Interrupted task '%s' (PID: %d)\n", 
            arena_mem -> tick_cnt, current->comm, current->pid);

  mod_timer(timer, jiffies + msecs_to_jiffies(timer_interval_ms));
}

// ---------- Initialaization
static int __init toy_init(void){
  pr_info("Loading module \n");

  arena_mem = kmalloc(sizeof(toy_arena_mem), GFP_KERNEL);
  if(!arena_mem){
    pr_err("Fialed to allocate Mem");
    return  -ENOMEM;
  }

  snprintf(arena_mem -> name, 16, "chungus");
  pr_info("allcoated mem for %s \n", arena_mem -> name);

  timer_setup(&toy_timer, toy_tick, 0);
  mod_timer(&toy_timer, jiffies + msecs_to_jiffies(timer_interval_ms));
  
  return 0;
}

static void __exit toy_exit(void){
  pr_info("ToySched: Unloading module...\n");

    // Free the memory we allocated
    if (arena_mem) {
        kfree(arena_mem);
        pr_info("ToySched: Memory freed safely.\n");
    }
}

module_init(toy_init);
module_exit(toy_exit);
