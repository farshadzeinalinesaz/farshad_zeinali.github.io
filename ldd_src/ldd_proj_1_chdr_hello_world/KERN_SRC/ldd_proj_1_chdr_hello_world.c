/*
===============================================================================
Driver Name		:		ldd_proj_1_chdr_hello_world
Author			:		FARSHAD
License			:		GPL
Description		:		LINUX CHARACTER DEVICE DRIVER PROJECT
===============================================================================
*/

#include"ldd_proj_1_chdr_hello_world.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FARSHAD");

static int __init ldd_proj_1_chdr_hello_world_init(void)
{
	printk(KERN_ALERT"%s has invoked",__FUNCTION__);

	return 0;
}

static void __exit ldd_proj_1_chdr_hello_world_exit(void)
{	
	printk(KERN_ALERT"%s has invoked",__FUNCTION__);
}

module_init(ldd_proj_1_chdr_hello_world_init);
module_exit(ldd_proj_1_chdr_hello_world_exit);

