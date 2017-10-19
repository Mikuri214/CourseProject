#include <linux/fs.h>
#include <linux/pagemap.h>
#include <stdlib.h>
#include "sim_fs.h"

//  inode data 
struct sim_inode_info {
	/* �ļ���ռ�õ������߼�������飬����0-6��ֱ�ӿ�ţ�7��һ�μ�ӿ飬8�Ƕ��μ�ӿ飬9�����μ�ӿ� */
	__u32 i_data[16];  
	/* ��Ƕ��VFS�ڵ� */
	struct inode vfs_inode;
};

//  super-block data 
struct sim_sb_info {
	/* i�ڵ��� */
	unsigned long s_ninodes;
	/* �߼����� */
	unsigned long s_nzones;
	/* i�ڵ�λͼ��ռ���� */
	unsigned long s_imap_blocks;
	/* �߼���λͼ��ռ���� */
	unsigned long s_zmap_blocks;
	/* �������е�һ���߼���� */
	unsigned long s_firstdatazone;
	/* log2(���̿���/�߼���) */
	unsigned long s_log_zone_size;
	/* �����ļ����ȣ����ֽ�Ϊ��λ */
	unsigned long s_max_size;
	/* Ŀ¼��ĳ��� */
	int s_dirsize;
	/* Ŀ¼�����ļ����ĳ��� */
	int s_namelen;
	/* ���������� */
	int s_link_max;
	/* i�ڵ�λͼ������ */
	struct buffer_head ** s_imap;
	/* �߼���λͼ������ */
	struct buffer_head ** s_zmap;
	/* �����黺���� */
	struct buffer_head * s_sbh;
	/* ָ������ϳ������ָ�� */
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
