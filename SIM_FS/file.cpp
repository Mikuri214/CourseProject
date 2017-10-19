#include <linux/buffer_head.h>		/* for fsync_inode_buffers() */
#include "sim.h"


int sim_sync_file ( struct file * file, struct dentry *dentry, int datasync )
{
	struct inode *inode = dentry->d_inode;
	int err;

	err = sync_mapping_buffers ( inode->i_mapping );
	if ( !( inode->i_state & I_DIRTY ) )
		return err;
	if ( datasync && !( inode->i_state & I_DIRTY_DATASYNC ) )
		return err;

	err |= sim_sync_inode ( inode );
	return err ? -EIO : 0;
}

struct file_operations sim_file_operations = {
	.llseek = generic_file_llseek,
	.read = generic_file_read,
	.write = generic_file_write,
	.mmap = generic_file_mmap,
	.fsync = sim_sync_file,
	.sendfile = generic_file_sendfile,
};

struct inode_operations sim_file_inode_operations = {
	.truncate = sim_truncate,
	.getattr = sim_getattr,
};