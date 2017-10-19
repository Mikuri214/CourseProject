#include <linux/fs.h>
#include <linux/pagemap.h>
#include <stdlib.h>
#include "sim_fs.h"

//  inode data 
struct sim_inode_info {
	/* 文件所占用的盘上逻辑块号数组，其中0-6是直接块号，7是一次间接块，8是二次间接块，9是三次间接块 */
	__u32 i_data[16];  
	/* 内嵌的VFS节点 */
	struct inode vfs_inode;
};

//  super-block data 
struct sim_sb_info {
	/* i节点数 */
	unsigned long s_ninodes;
	/* 逻辑块数 */
	unsigned long s_nzones;
	/* i节点位图所占块数 */
	unsigned long s_imap_blocks;
	/* 逻辑块位图所占块数 */
	unsigned long s_zmap_blocks;
	/* 数据区中第一个逻辑块号 */
	unsigned long s_firstdatazone;
	/* log2(磁盘块数/逻辑块) */
	unsigned long s_log_zone_size;
	/* 最大的文件长度，以字节为单位 */
	unsigned long s_max_size;
	/* 目录项的长度 */
	int s_dirsize;
	/* 目录项中文件名的长度 */
	int s_namelen;
	/* 最大的链接数 */
	int s_link_max;
	/* i节点位图缓冲区 */
	struct buffer_head ** s_imap;
	/* 逻辑块位图缓冲区 */
	struct buffer_head ** s_zmap;
	/* 超级块缓冲区 */
	struct buffer_head * s_sbh;
	/* 指向磁盘上超级块的指针 */
	struct sim_super_block * s_ms;
	unsigned short s_mount_state;
};

//translation
static inline struct sim_sb_info *sim_sb ( struct super_block *sb )
{
	return sb->s_fs_info;
}
static inline struct sim_inode_info *sim_i ( struct inode *inode )
{
	return list_entry ( inode, struct sim_inode_info, vfs_inode );
}


/*
*	inode && super block  operations        
*/
//allocate&&release operations  implementing with bitmap     bipmap.cpp
extern struct sim_inode * sim_raw_inode ( struct super_block *, ino_t, struct buffer_head ** );
extern struct inode * sim_new_inode ( const struct inode * dir, int * error );
extern void sim_free_inode ( struct inode * inode );
extern unsigned long sim_count_free_inodes ( struct sim_sb_info *sbi );
extern int sim_new_block ( struct inode * inode );
extern void sim_free_block ( struct inode * inode, int block );
extern unsigned long sim_count_free_blocks ( struct sim_sb_info *sbi );


extern void truncate ( struct inode * );
extern void sim_truncate ( struct inode * );
extern int sim_sync_inode ( struct inode * );
extern void sim_set_inode ( struct inode *, dev_t );
extern int get_block ( struct inode *, long, struct buffer_head *, int );
extern unsigned sim_blocks ( loff_t );

/*
*	directory operations                   dir.cpp
*/
extern  int sim_readdir ( struct file *, void *, filldir_t );
extern int sim_getattr ( struct vfsmount *, struct dentry *, struct kstat * );
extern struct sim_dir_entry *sim_find_entry ( struct dentry*, struct page** );
extern int sim_add_link ( struct dentry*, struct inode* );
extern int sim_delete_entry ( struct sim_dir_entry*, struct page* );
extern int sim_make_empty ( struct inode*, struct inode* );
extern int sim_empty_dir ( struct inode* );
extern void sim_set_link ( struct sim_dir_entry*, struct page*, struct inode* );
extern struct sim_dir_entry *sim_dotdot ( struct inode*, struct page** );
extern ino_t sim_inode_by_name ( struct dentry* );

/*
*	file operations                            file.cpp
*/
extern int sim_sync_file ( struct file *, struct dentry *, int );



extern struct inode_operations sim_file_inode_operations;
extern struct inode_operations sim_dir_inode_operations;
extern struct file_operations sim_file_operations;
extern struct file_operations sim_dir_operations;
extern struct dentry_operations sim_dentry_operations;
