/// Tasks to be performed at init. Callable via the CLI.
///
/// (c) Koheron 

#ifndef __MISC_INIT_HPP__
#define __MISC_INIT_HPP__

#include "../drivers/core/dev_mem.hpp"

class InitTasks
{
  public:
    InitTasks(Klib::DevMem& dev_mem_);
    
    // Display IP address last number onto the RedPitaya LEDs
    void show_ip_on_leds();
    
  private:
    Klib::DevMem& dev_mem;
};

#endif // __MISC_INIT_HPP__