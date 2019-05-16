/*******************************************************************************************
*	Author: Nicolas Leo                     											   *
*	Class: CS 0449, 8/3/18                    											   *
*	Project 4: Dice Driver                												   *
*	Description: This program builds a character device that simulates rolling 6-sided die.*
*	Driver must be compiled in 32-bit mode.	(make ARCH=i386)						   	   *
*******************************************************************************************/

// Required headers for all modules
#include <linux/init.h>
#include <linux/module.h>

// Driver specific headers
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/random.h>
#include <asm/uaccess.h>

/*
 	Helper function to get a random byte.
 */
unsigned char get_random_byte(int max) 
{
	unsigned char c; //Stores a random byte
	get_random_bytes(&c, 1);
	return c%max;
}


/*
 	dice_read is the function called when a process calls read() on
 	/dev/dice. It creates a random number between [0, 5].
*/
static ssize_t dice_read(struct file * file, char * buf, size_t count, loff_t *ppos)
{
	char * temp = kmalloc(count*sizeof(char), GFP_KERNEL); // Array of random bytes
	int i; 		      // Counter for loop

	// Count must be at least one to be valid
	if (count < 1)
		return -EINVAL;

	for (i = 0; i < count; ++i)
		temp[i] = (char)get_random_byte(6) + 1;

	/*
	 	Besides copying the roll to the user provided buffer,
	 	this function also checks that the user has permission to
	 	write to the buffer, that it is mapped, etc.
	*/
	if (copy_to_user(buf, temp, count * sizeof(char)))
		return -EINVAL;

	*ppos += count;

	kfree(temp);
	// Tell the user how many rolls were written.
	return count;
}

/*
	This device only implements the read operation
*/
static const struct file_operations dice_fops = 
{
	.owner		= THIS_MODULE,
	.read		= dice_read,
};

static struct miscdevice dice_driver = 
{
	/*
	 	We don't care what minor number we end up with, so tell the
	 	kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	// 	Name ourselves /dev/dice_driver.
	"dice_driver",
	/*
		What functions to call when a program performs file
	 	operations on the device.
	 */
	&dice_fops //Gives the minor number a dynamic value and creates /dev/dice
};

static int __init dice_init(void)
{
	// Create the "dice_driver" device in the /sys/class/misc directory.
	int ret = misc_register(&dice_driver);

	if (ret)
		printk(KERN_ERR "Unable to register \"dice_driver\" as a misc device\n");

	return ret;
}

/* 
	dice_exit unregisters the device.
*/
static void __exit dice_exit(void)
{
    misc_deregister(&dice_driver);
}

module_init(dice_init);
module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nicolas Leo <nll21@pitt.edu");
MODULE_DESCRIPTION("Virtual 6-sided dice throw module");