#include "driver/tty.h"
#include "linux/printk.h"
#include "linux/mm.h"
#include "linux/idt.h"
#include "linux/apic.h"
#include "linux/cpu.h"
#include "linux/gdt64.h"
#include "linux/syscall.h"
#include "libs/unistd.h"
#include "libs/stdio.h"

void x64_user_main()
{
    printf("///\n\ // _ooOoo_ //\n\ // o8888888o //\n\ // 88\" . \"88 //\n\ // (| ^_^ |) //\n\ // O\\ = /O //\n\ // ____/`---'\\____ //\n\ // .' \\\\| |// `. //\n\ // / \\\\||| : |||// \\ //\n\ // / _||||| -:- |||||- \\ //\n\ // | | \\\\\\ - /// | | //\n\ // | \\_| ''\\---/'' | | //\n\ // \\ .-\\__ `-` ___/-. / //\n\ // ___`. .' /--.--\\ `. . ___ //\n\ // ."" '< `.___\\_<|>_/___.' >'"". //\n\ // | | : `- \\`.;`\\ _ /`;.`/ - ` : | | //\n\ // \\ \\ `-. \\_ __\\ /__ _/ .-` / / //\n\ // ========`-.____`-.___\\_____/___.-`____.-'======== //\n\ // `=---=' //\n\ // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ //\n\ // 手写x64os暂时告一段落 谢谢佛祖保佑 //\n\ ///\n");
    while(1);
}

void x64_ap_main(void)
{
    init_ap_idt(); //和bsp共享idt
    syscall_init(); //初始化syscall
    ap_local_apic_init(); //使能本地apic。
    lapic_timer_cycle_start(INTER_ID_LAPIC_TIMER, 5000000);

    kpcr_create();
    init_tss_current_core(); //ap核初始化tss

    kpcr_swapgs();
    int cpuid = kpcr_get_offset(0);
    uint64_t stack = kpcr_get_offset(24);
    asm volatile(
        "mov rsp, rax;" 
        :: "a"(stack)
    );
    kpcr_swapgs();

    *((uint32_t *)0xb000) = 0;
    
    __asm volatile("sti;"); //必须设置好栈再开中断，不然广播调度消息时，多ap有几率共用一个栈，导致异常


    printk("ap kpcr %d stack %x init suceess..\n", cpuid, stack);

    

    //初始化专属数据区
    
    while(1) {
        asm volatile("hlt;");
        // kpcr_swapgs();
        // log("cores %d wait up from hlt....\n", kpcr_get_offset(0));
        // kpcr_swapgs();
    }
}

void _delay_ms()
{
    for(int i = 0; i < 100; ++i) {
        for(int j = 0; j < 10000;) {
            ++j;
        }
    }
}
void delay_s(int ms)
{
    ms = ms*10;
    while(ms-- >0) {
        _delay_ms();
    }
}

int x64_kernel_main()
{
    console_init();
    init_serial();
    mm_init();
    init_idt();
    apic_init();
    init_tss_current_core(); //bsp核初始化tss
    syscall_init(); //初始化syscall
    ap_init();
    task_init();

    delay_s(1);
    lapic_timer_cycle_start(INTER_ID_LAPIC_TIMER, 5000000);
    // apic_broadcast_message_interrupt(INTER_ID_SCHED_BROADCAST);

    while(1) {
        asm volatile("hlt;");
        // kpcr_swapgs();
        // log("cores %d wait up from hlt....\n", kpcr_get_offset(0));
        // kpcr_swapgs();
    }

    return 0;
}

