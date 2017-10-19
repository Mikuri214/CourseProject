#ifndef _LINUX_SIM_FS_H
#define _LINUX_SIM_FS_H

#include <linux/types.h>
#include <linux/magic.h>

#define SIM_ROOT_INO 1

#define SIM_LINK_MAX	250

#define MINIX_I_MAP_SLOTS	8        
#define MINIX_Z_MAP_SLOTS	64       

//register return code
#define SIM_VALID_FS		0x0001		/* Clean fs. */
#define SIM_ERROR_FS		0x0002		/* fs has errors. */

#define SIM_INODES_PER_BLOCK ((BLOCK_SIZE)/(sizeof (struct sim_inode)))

struct sim_inode {
	__u16 i_mode;
	__u32 i_size;
	__u32 i_time;
	__u8  i_gid;
	__u8  i_nlinks;
	__u16 i_zone[9];
};

struct sim_super_block {
	__u32 s_ninodes;
	__u16 s_pad0;
	__u16 s_imap_blocks;
	__u16 s_zmap_blocks;
	__u16 s_firstdatazone;
	__u16 s_log_zone_size;
	__u16 s_pad1;
	__u32 s_max_size;
	__u32 s_zones;
	__u16 s_magic;
	__u16 s_pad2;
	__u16 s_blocksize;
};

struct sim_dir_entry {
	__u32 inode;
	char name[0];
};

#endif
