#include <generated/autoconf.h>
#include <linux/mm.h>
#include <linux/major.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>  
#include <linux/fs.h>    
#include <linux/errno.h> 
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/smp_lock.h>
#include <linux/timer.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/irq.h>
#include <linux/mutex.h>

#include <mach/irqs.h>
#include <asm/io.h> 
#include <asm/uaccess.h>
#include <mach/gpio.h>
#include <plat/mux.h>
#include <mach/hardware.h>
#include <plat/omap34xx.h>
#include "omap_cir.h"


static unsigned int irq = 132;
unsigned int ir;
static DECLARE_WAIT_QUEUE_HEAD (wq);
static UINT32 currclk = 0, lastclk = 0;
static UINT32 flag = 0,codeflag=0,sendflag=0,int_flag=0,s_flag=0;;
static UINT32 data = 0,user_data=0,val,save=0,save_data=0;
UINT32 pulse_count = 0,p_count=0;
UINT32 timediff;
UINT32 bound_data;
struct timespec ts;
struct cir_event {
        struct timeval time;
        __u16 type;
        __u16 code;
        __s32 value;
}cir;

struct mutex    mutex;
void short_do_tasklet (unsigned long);
DECLARE_TASKLET (short_tasklet, short_do_tasklet, 0);

static irqreturn_t cir_handler (int irq, void *dev_id )
{       

	if (irq != 292)
		return IRQ_NONE;

        currclk = omap_readl(TIMER_32K_SYNCHRONIZED);
        timediff = (currclk - lastclk);
        lastclk = currclk;
        //printk("%d \n",timediff);
        if((timediff < (ONE + TOLERANCE) && timediff > (ONE - TOLERANCE)) || (timediff < (ZERO + TOLERANCE) && timediff > (ZERO - TOLERANCE)))
        {   
                 if(timediff < (ONE + TOLERANCE) && timediff > (ONE - TOLERANCE))
                {
                       data = data | (1<< (pulse_count));
                }
                pulse_count++;
        
                if(pulse_count==20)
               {
                      pulse_count = 0;
                      if(int_flag==2)
                     {
                            s_flag=1;
                            
                            tasklet_schedule(&short_tasklet);
                            //printk("hi\n");
                            if(user_data==302351)
                                       int_flag=0;
                     }
                     
                     int_flag++;
                     //printk("data %d\n",data);
                     user_data=data;
                      if(save_data==0)
                            {
                                save_data=user_data;
                            }
                     data = 0;
                     //sendflag=1;
                     codeflag=1;
               }
         }

         else 
         {
               pulse_count = 0;
              // printk("timediff= %d\n",timediff);
               if( timediff >2000 && timediff <8000 )
               {
                    val=0;
                    int_flag=0;
                    if(save_data==user_data && save_data !=0)
                    {
                                 save++;
                                 //printk("i m saving %d\n",save);
                                 if(save==2)
                                        save=1;
                                 tasklet_schedule(&short_tasklet);
                    }
                    
               }
               if( timediff >=8050)
               {
                    codeflag=0;
                    val=0;
                    int_flag=0;
                    save_data=0;
                 
               }
         }
        return IRQ_HANDLED;
}

void short_do_tasklet (unsigned long unused)
{
    int crclk=0,difclk=0;
     while(1)
    {
       crclk = omap_readl(TIMER_32K_SYNCHRONIZED);
        difclk = (crclk-lastclk);
       // printk("difclick : %d\n",difclk);
        if((difclk>8000 && save !=0) || (difclk>8000 && s_flag==1))
         {
                 flag = 1;   
                 wake_up_interruptible (&wq);
                 //printk("hi\n");
                break;
         }
        if(difclk>11000 )
         {
                break;
          }
      }
}


static INT32 cir_Open (struct inode *inode, struct file *file)
{
        lastclk = 0;
        return 0;
}


static INT32 cir_Close (struct inode *inode, struct file *file)
{
     return 0;
}

void check_data(void)
{
	if (user_data == 302357)
	{
		bound_data=458826;
		user_data=home;
	}
        else if (user_data==302459)
        {
                bound_data=458832;
                user_data=left;
        }
        else  if (user_data==302460)
        {
                bound_data=458831;
                user_data=right;
        }
        else if (user_data==302457)
        {
                bound_data=458834;
                user_data=up;
         }
        else if (user_data==302458)
        {
                bound_data=458833;
                user_data=down;
        }
        else if (user_data==302347)
        {
                bound_data=458792;
                user_data=enter;
         }
         else if (user_data==302362)
         {
                bound_data=458793;
                user_data=esc;
         }
         else if (user_data==302363)
         {
                bound_data=458983;
                user_data=window;
         }
         else if(user_data==302420)
         {
                bound_data=458827;
                user_data=vol_up;
         }
         else if (user_data==302350)
         {
                bound_data=458830;
                user_data=vol_down;
         }

          else if(user_data==302336)
         {
                bound_data=458782;
                user_data=key1;
         }
          else if(user_data==302337)
         {
                bound_data=458783;
                user_data=key2;
         }
         else if(user_data==302338)
         {
                bound_data=458784;
                user_data=key3;
         }
          else if(user_data==302339)
         {
                bound_data=458785;
                user_data=key4;
         }        
         else if(user_data==302340)
         {
                bound_data=458786;
                user_data=key5;
         }
         else if(user_data==302341)
         {
                bound_data=458787;
                user_data=key6;
         }
         else if(user_data==302342)
         {
                bound_data=458788;
                user_data=key7;
         }
         else if(user_data==302343)
         {
                bound_data=458789;
                user_data=key8;
         }
         else if(user_data==302344)
         {
                bound_data=458790;
                user_data=key9;
         }
         else if(user_data==302345)
         {
                bound_data=458791;
                user_data=key0;
         }
         else if(user_data==302436)
         {
                bound_data=458807;
                user_data=keydot;
         }
         else if(user_data==302435)
         {
                bound_data=458807;
                user_data=keyspace;
         }
          else if(user_data==302437)
         {
                bound_data=458977;
                user_data=keyshift;
         }
         else if(user_data==302384)
         {
                bound_data=458756;
                user_data=keyA;
                if(save==1)
                 {
                        bound_data=458757;
                        user_data=keyB;
                 }
          }
          else if(user_data==302428)
         {
                bound_data=458758;
                user_data=keyC;
                if(save==1)
                 {
                        bound_data=458759;
                        user_data=keyD;
                 }
          }
          else if(user_data==404756)
         {
                bound_data=458760;
                user_data=keyE;
                if(save==1)
                 {
                        bound_data=458761;
                        user_data=keyF;
                 }
          }
          else if(user_data==302385)
         {
                bound_data=458762;
                user_data=keyG;
                if(save==1)
                 {
                        bound_data=458763;
                        user_data=keyH;
                 }
          }
          else if(user_data==302370)
         {
                bound_data=458764;
                user_data=keyI;
                if(save==1)
                 {
                        bound_data=458765;
                        user_data=keyJ;
                 }
          }
          else if(user_data==302386)
         {
                bound_data=458766;
                user_data=keyK;
                if(save==1)
                 {
                        bound_data=458767;
                        user_data=keyL;
                 }
          }
          else if(user_data==302371)
         {
                bound_data=458768;
                user_data=keyM;
                if(save==1)
                 {
                        bound_data=458769;
                        user_data=keyN;
                 }
          }
          else if(user_data==404857)
         {
                bound_data=458770;
                user_data=keyO;
                if(save==1)
                 {
                        bound_data=458771;
                        user_data=keyP;
                 }
          }
          else if(user_data==302393)
         {
                bound_data=458772;
                user_data=keyQ;
                if(save==1)
                 {
                        bound_data=458773;
                        user_data=keyR;
                 }
          }
          else if(user_data==302392)
         {
                bound_data=458774;
                user_data=keyS;
                if(save==1)
                 {
                        bound_data=458775;
                        user_data=keyT;
                 }
          }
          else if(user_data==404794)
         {
                bound_data=458776;
                user_data=keyU;
                if(save==1)
                 {
                        bound_data=458777;
                        user_data=keyV;
                 }
          }
          else if(user_data==404795)
         {
                bound_data=458778;
                user_data=keyW;
                if(save==1)
                 {
                        bound_data=458779;
                        user_data=keyX;
                 }
          }
          else if(user_data==302426)
         {
                bound_data=458780;
                user_data=keyY;
                if(save==1)
                 {
                        bound_data=458781;
                        user_data=keyZ;
                 }
          }
         /*if(user_data==128)
         {
                bound_data=458782;
                user_data=key1;
                if(save==1)
                 {
                        bound_data=458807;
                        user_data=keydot;
                 }
          }
          if(user_data==129)
         {
                bound_data=458783;
                user_data=key2;
                if(save==1)
                 {
                        bound_data=458756;
                        user_data=keyA;
                 }
                 if(save==2)
                 {
                        bound_data=458757;
                        user_data=keyB;
                 }
                 if(save==3)
                 {
                        bound_data=458758;
                        user_data=keyC;
                 }
          }
          if(user_data==130)
         {
                bound_data=458784;
                user_data=key3;
                if(save==1)
                 {
                        bound_data=458759;
                        user_data=keyD;
                 }
                 if(save==2)
                 {
                        bound_data=458760;
                        user_data=keyE;
                 }
                 if(save==3)
                 {
                        bound_data=458761;
                        user_data=keyF;
                 }
          }
          if(user_data==131)
         {
                bound_data=458785;
                user_data=key4;
                if(save==1)
                 {
                        bound_data=458762;
                        user_data=keyG;
                 }
                 if(save==2)
                 {
                        bound_data=458763;
                        user_data=keyH;
                 }
                 if(save==3)
                 {
                        bound_data=458764;
                        user_data=keyI;
                 }
          }
          if(user_data==132)
         {
                bound_data=458786;
                user_data=key5;
                if(save==1)
                 {
                        bound_data=458765;
                        user_data=keyJ;
                 }
                 if(save==2)
                 {
                        bound_data=458766;
                        user_data=keyK;
                 }
                 if(save==3)
                 {
                        bound_data=458767;
                        user_data=keyL;
                 }
          }
          if(user_data==133)
         {
                bound_data=458787;
                user_data=key6;
                if(save==1)
                 {
                        bound_data=458768;
                        user_data=keyM;
                 }
                 if(save==2)
                 {
                        bound_data=458769;
                        user_data=keyN;
                 }
                 if(save==3)
                 {
                        bound_data=458770;
                        user_data=keyO;
                 }
          }
          if(user_data==134)
         {
                bound_data=458788;
                user_data=key7;
                if(save==1)
                 {
                        bound_data=458771;
                        user_data=keyP;
                 }
                 if(save==2)
                 {
                        bound_data=458772;
                        user_data=keyQ;
                 }
                 if(save==3)
                 {
                        bound_data=458773;
                        user_data=keyR;
                 }
                 if(save==4)
                 {
                        bound_data=458774;
                        user_data=keyS;
                 }
          }
          if(user_data==135)
         {
                bound_data=458789;
                user_data=key8;
                if(save==1)
                 {
                        bound_data=458775;
                        user_data=keyT;
                 }
                 if(save==2)
                 {
                        bound_data=458776;
                        user_data=keyU;
                 }
                 if(save==3)
                 {
                        bound_data=458777;
                        user_data=keyV;
                 }
          }
          if(user_data==136)
         {
                bound_data=458790;
                user_data=key9;
                if(save==1)
                 {
                        bound_data=458778;
                        user_data=keyW;
                 }
                 if(save==2)
                 {
                        bound_data=458779;
                        user_data=keyX;
                 }
                 if(save==3)
                 {
                        bound_data=458780;
                        user_data=keyY;
                 }
                 if(save==4)
                 {
                        bound_data=458781;
                        user_data=keyZ;
                 }
                 
          }
          if(user_data==137)
         {
                bound_data=458791;
                user_data=key0;
                if(save==1)
                 {
                        bound_data=458807;
                        user_data=keyspace;
                 }
          }*/
          else if(user_data==302351)
         {
                bound_data=458794;
                user_data=keyback;
          }
          else
          {    
          }
}

static ssize_t cir_Read (struct file *file, char __user *buff, size_t count, loff_t * loff)
{
        wait_event_interruptible (wq, flag != 0 );
        ktime_get_ts(&ts);     
 
        //if(sendflag==1)
        //{
               cir.time.tv_sec = ts.tv_sec;
               cir.time.tv_usec = ts.tv_nsec / NSEC_PER_USEC;
               cir.value = (__s32) val;
               count=sizeof(struct cir_event);
               p_count++;
               if(p_count==6)
              {
                      sendflag=0;
                      p_count=0;
                      s_flag=0;
                      flag=0;
                      save=0;
              }
              if(p_count==1)
             {
                     check_data();
             }
             switch(codeflag)
             {
                     case 1:
                                   cir.type = (__u16)4;
                                   cir.code = (__u16)4;
                                   cir.value = (__s32) bound_data;
                                   codeflag=2;
                                   break;
                     case 2:
                                   cir.type = (__u16)1;
                                   cir.code = (__u16)user_data;
                                   cir.value = (__s32)1;
                                   codeflag=3;
                                   break;
                     case 3:
                                   cir.type = (__u16)0;
                                   cir.code = (__u16)0;
                                   cir.value = (__s32)0;
                                   codeflag=4;
                                   break;
                     case 4:
                                   cir.type = (__u16)4;
                                   cir.code = (__u16)4;
                                   cir.value = (__s32) bound_data;
                                   codeflag=5;
                                   break;
                     case 5:
                                   cir.type = (__u16)1;
                                   cir.code = (__u16)user_data;
                                   cir.value = (__s32)0;
                                   codeflag=6;
                                   break;
                     default:
                                   cir.type = (__u16)0;
                                   cir.code = (__u16)0;
                                   cir.value = (__s32)0;
                                   codeflag=7;
              }

              //printk("kernel got: t0=%d, t1=%d, type=%d, code=%d, v=%d\n",(int) cir.time.tv_sec, (int) cir.time.tv_usec, cir.type, cir.code, cir.value); 
              if (copy_to_user ((struct cir_event *)buff, &cir, count))
             {
                     printk ("\nCannot copy to user\n");
                     return -EFAULT;
             }
      // }
        data =0;
        pulse_count=0;
        return count;
}


static struct file_operations cir_fops = {
        .owner    = THIS_MODULE,            
    .open    = cir_Open,
     .read    = cir_Read,
     .release = cir_Close
};


static int __init cir_init (void)
{
   int result;

     if (register_chrdev (CIR_MAJOR, CIR_DEVICE, &cir_fops) < 0)
        {
        
              printk (KERN_ERR CIR_DEVICE ": register_chrdev failed!\n");
            return -EIO;
       }
        omap_cfg_reg(irq);
 
        if (gpio_request(irq,"CIR GPIO") < 0) 
        {
                        printk(KERN_ERR "GPIO %d failed\n",irq);
                        return -EINVAL;
        }

        gpio_direction_output(irq,0);

        result = request_irq(gpio_to_irq(irq),cir_handler,IRQF_TRIGGER_RISING,CIR_DEVICE,NULL);
        mutex_init(&mutex);
        switch (result)
        {
          
          case -EBUSY:
               printk (KERN_ERR CIR_DEVICE ": IRQ %d busy\n", irq);
               return -EBUSY;
          case -EINVAL:
               printk (KERN_ERR CIR_DEVICE ": Bad irq number or handler\n");
               return -EINVAL;
       }
   return 0;
}


static void __exit cir_exit (void)
{
   
        gpio_free(irq);   
        free_irq (OMAP_GPIO_IRQ(irq), NULL);
    unregister_chrdev (CIR_MAJOR, CIR_DEVICE);
     
}

module_init(cir_init);
module_exit(cir_exit);
MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Arindam");
