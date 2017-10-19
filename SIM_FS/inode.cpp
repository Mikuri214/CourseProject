#include "sim.h"
#include <memory.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/highuid.h>
#include <linux/vfs.h>


static void sim_read_inode ( struct inode * inode );
static int sim_write_inode ( struct inode * inode, int wait );
static int sim_statfs ( struct super_block *sb, struct kstatfs *buf );
static int sim_remount ( struct super_block * sb, int * flags, char * data );
static void sim_delete_inode ( struct inode *inode )
{
	inode->i_size = 0;
	sim_truncate ( inode );//清空文件
	sim_free_inode ( inode );//释放索引节点
}

static kmem_cache_t * sim_inode_cachep;

static void init_once ( void * foo, kmem_cache_t * cachep, unsigned long flags )
{
	struct sim_inode_info *ei = ( struct sim_inode_info * ) foo;

	if ( ( flags & ( SLAB_CTOR_VERIFY | SLAB_CTOR_CONSTRUCTOR ) ) ==
		SLAB_CTOR_CONSTRUCTOR )
		inode_init_once ( &ei->vfs_inode );
}
static int init_inodecache ( void )
{
	sim_inode_cachep = kmem_cache_create ( "sim_inode_cache",
		sizeof ( struct sim_inode_info ),
		0, SLAB_RECLAIM_ACCOUNT,
		init_once, NULL );
	if ( sim_inode_cachep == NULL )
		return -ENOMEM;
	return 0;
}
static void destroy_inodecache ( void )
{
	if ( kmem_cache_destroy ( sim_inode_cachep ) )
		printk ( KERN_INFO "sim_inode_cache: not all structures were freed\n" );
}

//super_operations
static struct inode *sim_alloc_inode ( struct super_block *sb )
{
	struct sim_inode_info *ei;
	ei = ( struct sim_inode_info * )kmem_cache_alloc ( sim_inode_cachep, SLAB_KERNEL );
	//在缓冲池分配一个SIMFS的内存索引节点
	if ( !ei )
		return NULL;
	return &ei->vfs_inode;
}
static void sim_destroy_inode ( struct inode *inode )
{
	kmem_cache_free ( sim_inode_cachep, sim_i ( inode ) );
}
static void sim_read_inode ( struct inode * inode )
{
	struct buffer_head * bh;
	struct sim_inode * raw_inode;
	struct sim_inode_info *sim_inode = sim_i ( inode );
	int i;

	raw_inode = sim_raw_inode ( inode->i_sb, inode->i_ino, &bh );
	if ( !raw_inode ) {
		make_bad_inode ( inode );
		return;
	}
	inode->i_mode = raw_inode->i_mode;
	inode->i_gid = ( gid_t ) raw_inode->i_gid;
	inode->i_nlink = raw_inode->i_nlinks;
	inode->i_size = raw_inode->i_size;
	inode->i_mtime.tv_sec = raw_inode->i_mtime;
	inode->i_atime.tv_sec = raw_inode->i_atime;
	inode->i_ctime.tv_sec = raw_inode->i_ctime;
	inode->i_mtime.tv_nsec = 0;
	inode->i_atime.tv_nsec = 0;
	inode->i_ctime.tv_nsec = 0;
	inode->i_blocks = inode->i_blksize = 0;
	for ( i = 0; i < 10; i++ )
		sim_inode->i_data[i] = raw_inode->i_zone[i];
	sim_set_inode ( inode, old_decode_dev ( raw_inode->i_zone[0] ) );
	brelse ( bh );
}
static struct buffer_head * sim_update_inode ( struct inode * inode )
{
	struct buffer_head * bh;
	struct sim_inode * raw_inode;
	struct sim_inode_info *sim_inode = sim_i ( inode );
	int i;

	raw_inode = sim_raw_inode ( inode->i_sb, inode->i_ino, &bh );
	if ( !raw_inode )
		return NULL;
	//更新
	raw_inode->i_mode = inode->i_mode;
	raw_inode->i_gid = fs_high2lowgid ( inode->i_gid );
	raw_inode->i_nlinks = inode->i_nlink;
	raw_inode->i_size = inode->i_size;
	raw_inode->i_mtime = inode->i_mtime.tv_sec;
	raw_inode->i_atime = inode->i_atime.tv_sec;
	raw_inode->i_ctime = inode->i_ctime.tv_sec;
	if ( S_ISCHR ( inode->i_mode ) || S_ISBLK ( inode->i_mode ) )
		raw_inode->i_zone[0] = old_encode_dev ( inode->i_rdev );
	else for ( i = 0; i < 10; i++ )
		raw_inode->i_zone[i] = sim_inode->i_data[i];
	mark_buffer_dirty ( bh );//将磁盘索引节点所在的缓冲块标记为脏
	return bh;
}
static int sim_write_inode ( struct inode * inode, int wait )
{
	brelse ( sim_update_inode ( inode ) );
	return 0;
}
int sim_sync_inode ( struct inode * inode )
{
	int err = 0;
	struct buffer_head *bh;

	bh = sim_update_inode ( inode ); // 获取到磁盘索引节点所在的缓冲区
	if ( bh && buffer_dirty ( bh ) )//如果为脏，则同步
	{
		sync_dirty_buffer ( bh );
		if ( buffer_req ( bh ) && !buffer_uptodate ( bh ) )
		{
			printk ( "IO error syncing sim inode [%s:%08lx]\n",
				inode->i_sb->s_id, inode->i_ino );
			err = -1;
		}
	}
	else if ( !bh )
		err = -1;
	brelse ( bh );
	return err;
}
int sim_getattr ( struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat )
{
	generic_fillattr ( dentry->d_inode, stat );

	stat->blocks = ( BLOCK_SIZE / 512 ) * sim_blocks ( stat->size );
	stat->blksize = BLOCK_SIZE;
	return 0;
}
static int sim_remount ( struct super_block * sb, int * flags, char * data )
{
	struct sim_sb_info * sbi = sim_sb ( sb );
	struct sim_super_block * ms;

	ms = sbi->s_ms;
	if ( ( *flags & MS_RDONLY ) == ( sb->s_flags & MS_RDONLY ) )
		return 0;
	if ( *flags & MS_RDONLY ) {
		if ( ms->s_state & SIM_VALID_FS ||
			!( sbi->s_mount_state & SIM_VALID_FS ) )
			return 0;
		/* Mounting a rw partition read-only. */
		ms->s_state = sbi->s_mount_state;
		mark_buffer_dirty ( sbi->s_sbh );
	}
	else {
		/* Mount a partition which is read-only, read-write. */
		sbi->s_mount_state = ms->s_state;
		ms->s_state &= ~SIM_VALID_FS;
		mark_buffer_dirty ( sbi->s_sbh );

		if ( !( sbi->s_mount_state & SIM_VALID_FS ) )
			printk ( "SIM-fs warning: remounting unchecked fs, "
			"running fsck is recommended.\n" );
		else if ( ( sbi->s_mount_state & SIM_ERROR_FS ) )
			printk ( "SIM-fs warning: remounting fs with errors, "
			"running fsck is recommended.\n" );
	}
	return 0;
}
static int sim_statfs ( struct super_block *sb, struct kstatfs *buf )
{
	struct sim_sb_info *sbi = sim_sb ( sb );
	buf->f_type = sb->s_magic;
	buf->f_bsize = sb->s_blocksize;
	buf->f_blocks = ( sbi->s_nzones - sbi->s_firstdatazone ) << sbi->s_log_zone_size;
	buf->f_bfree = sim_count_free_blocks ( sbi );
	buf->f_bavail = buf->f_bfree;
	buf->f_files = sbi->s_ninodes;
	buf->f_ffree = sim_count_free_inodes ( sbi );
	buf->f_namelen = sbi->s_namelen;
	return 0;
}
static void sim_put_super ( struct super_block *sb )
{
	int i;
	struct sim_sb_info *sbi = sim_sb ( sb );

	if ( !( sb->s_flags & MS_RDONLY ) ) {
		sbi->s_ms->s_state = sbi->s_mount_state;
		mark_buffer_dirty ( sbi->s_sbh );
	}
	for ( i = 0; i < sbi->s_imap_blocks; i++ )
		brelse ( sbi->s_imap[i] );
	for ( i = 0; i < sbi->s_zmap_blocks; i++ )
		brelse ( sbi->s_zmap[i] );
	brelse ( sbi->s_sbh );
	kfree ( sbi->s_imap );
	sb->s_fs_info = NULL;
	kfree ( sbi );

	return;
}
void sim_set_inode ( struct inode *inode, dev_t rdev )//根据索引节点的属性，设置其对应的操作方法，从而将VFS的操作与SIMFS的操作联系起来

{
	if ( S_ISREG ( inode->i_mode ) ) {
		inode->i_op = &sim_file_inode_operations;
		inode->i_fop = &sim_file_operations;
		inode->i_mapping->a_ops = &sim_aops;
	}
	else if ( S_ISDIR ( inode->i_mode ) ) {
		inode->i_op = &sim_dir_inode_operations;
		inode->i_fop = &sim_dir_operations;
		inode->i_mapping->a_ops = &sim_aops;
	}
	else if ( S_ISLNK ( inode->i_mode ) ) {
		inode->i_op = &sim_symlink_inode_operations;
		inode->i_mapping->a_ops = &sim_aops;
	}
	else
		init_special_inode ( inode, inode->i_mode, rdev );
}

/* 超级块加载函数 */
static int sim_fill_super ( struct super_block *s, void *data, int silent )
{
	struct buffer_head *bh;
	struct buffer_head **map;
	struct sim_super_block *ms;
	int i, block;
	struct inode *root_inode; //设备的根目录
	struct sim_sb_info *sbi;

	/* 分配文件系统私有数据结构 */
	sbi = kmalloc ( sizeof ( struct sim_sb_info ), GFP_KERNEL );
	if ( !sbi )
		return -ENOMEM;
	//将VFS超级块和SIMFS的超级块联系起来 将其保存到s_fs_info中 
	s->s_fs_info = sbi;
	memset ( sbi, 0, sizeof ( struct sim_sb_info ) );

	if ( 64 != sizeof ( struct sim_inode ) )
		panic ( "bad  i-node size" );
	//设置VFS超级块的块大小
	if ( !sb_set_blocksize ( s, BLOCK_SIZE ) )
		goto out_bad_hblock;

	// 同步读取超级块数据，其块号为1   0为启动块
	if ( !( bh = sb_bread ( s, 1 ) ) )
		goto out_bad_sb;

	/* 将超级块数据强制转为sim_super_block结构，并从中读取数据 */
	ms = ( struct sim_super_block * ) bh->b_data;
	sbi->s_ms = ms;
	sbi->s_sbh = bh;//指向从磁盘读入的超级块所在的缓冲区
	sbi->s_mount_state = ms->s_state;
	sbi->s_ninodes = ms->s_ninodes;
	sbi->s_nzones = ms->s_nzones;
	sbi->s_imap_blocks = ms->s_imap_blocks;
	sbi->s_zmap_blocks = ms->s_zmap_blocks;
	sbi->s_firstdatazone = ms->s_firstdatazone;
	sbi->s_log_zone_size = ms->s_log_zone_size;
	sbi->s_nzones = ms->s_zones;
	sbi->s_dirsize = 32;
	sbi->s_namelen = 30;
	sbi->s_link_max = SIM_LINK_MAX;
	sbi->s_max_size = ms->s_max_size;
	s->s_magic = ms->s_magic;
		
     if ( s->s_magic != SIM_SUPER_MAGIC )
		goto out_no_fs;

	/* 分配结构保存i节点和逻辑块位图缓冲区 */
	i = ( sbi->s_imap_blocks + sbi->s_zmap_blocks ) * sizeof ( bh );
	map = kmalloc ( i, GFP_KERNEL );
	if ( !map )
		goto out_no_map;
	memset ( map, 0, i );
	sbi->s_imap = &map[0];
	sbi->s_zmap = &map[sbi->s_imap_blocks];

	block = 2;
	/* 读取i节点位图 */
	for ( i = 0; i < sbi->s_imap_blocks; i++ ) {
		if ( !( sbi->s_imap[i] = sb_bread ( s, block ) ) )
			goto out_no_bitmap;
		block++;
	}
	/* 读取逻辑块位图 */
	for ( i = 0; i < sbi->s_zmap_blocks; i++ ) {
		if ( !( sbi->s_zmap[i] = sb_bread ( s, block ) ) )
			goto out_no_bitmap;
		block++;
	}

	set_bit ( 0, sbi->s_imap[0]->b_data );
	set_bit ( 0, sbi->s_zmap[0]->b_data );

	/* 在读取inode前设置超级块回调 */
	s->s_op = &sim_sops;
	/* 读取根节点 */
	root_inode = iget ( s, SIM_ROOT_INO );
	if ( !root_inode || is_bad_inode ( root_inode ) )
		goto out_no_root;

	// 分配根inode  即根目录
	s->s_root = d_alloc_root ( root_inode );
	if ( !s->s_root )
		goto out_iput;


	if ( !( s->s_flags & MS_RDONLY ) ) {
		ms->s_state &= ~SIM_VALID_FS;
		mark_buffer_dirty ( bh );
	}
	if ( !( sbi->s_mount_state & SIM_VALID_FS ) )
		printk ( "SIM-fs: mounting unchecked file system, "
		"running fsck is recommended.\n" );
	else if ( sbi->s_mount_state & SIM_ERROR_FS )
		printk ( "SIM-fs: mounting file system with errors, "
		"running fsck is recommended.\n" );
	return 0;

out_iput:
	iput ( root_inode );
	goto out_freemap;

out_no_root:
	if ( !silent )
		printk ( "SIM-fs: get root inode failed\n" );
	goto out_freemap;

out_no_bitmap:
	printk ( "SIM-fs: bad superblock or unable to read bitmaps\n" );
out_freemap:
	for ( i = 0; i < sbi->s_imap_blocks; i++ )
		brelse ( sbi->s_imap[i] );
	for ( i = 0; i < sbi->s_zmap_blocks; i++ )
		brelse ( sbi->s_zmap[i] );
	kfree ( sbi->s_imap );
	goto out_release;

out_no_map:
	if ( !silent )
		printk ( "SIM-fs: can't allocate map\n" );
	goto out_release;

out_no_fs:
	if ( !silent )
		printk ( "VFS: Can't find a  sim filesystem on device "
		"%s.\n", s->s_id );
out_release:
	brelse ( bh );
	goto out;

out_bad_hblock:
	printk ( "SIM-fs: blocksize too small for device.\n" );
	goto out;

out_bad_sb:
	printk ( "SIM-fs: unable to read superblock\n" );
out:
	s->s_fs_info = NULL;
	kfree ( sbi );
	return -EINVAL;
}


static int sim_get_block ( struct inode *inode, sector_t block,
struct buffer_head *bh_result, int create )
{
	return get_block ( inode, block, bh_result, create );
}

//address_space_operation functions
static int sim_writepage ( struct page *page, struct writeback_control *wbc )
{
	return block_write_full_page ( page, sim_get_block, wbc );
}
static int sim_readpage ( struct file *file, struct page *page )
{
	return block_read_full_page ( page, sim_get_block );
}
static int sim_prepare_write ( struct file *file, struct page *page, unsigned from, unsigned to )
{
	return block_prepare_write ( page, from, to, sim_get_block );
}
static sector_t sim_bmap ( struct address_space *mapping, sector_t block )
{
	return generic_block_bmap ( mapping, block, sim_get_block );
}





/*
* The function that is called for file truncation.
*/
void sim_truncate ( struct inode * inode )
{
	if ( !( S_ISREG ( inode->i_mode ) || S_ISDIR ( inode->i_mode ) || S_ISLNK ( inode->i_mode ) ) )
		return;

	truncate ( inode );
}

static struct super_block *sim_get_sb ( struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data )
{
	return get_sb_bdev ( fs_type, flags, dev_name, data, sim_fill_super );
}

static struct address_space_operations sim_aops = {
	.readpage = sim_readpage,
	.writepage = sim_writepage,
	.sync_page = block_sync_page,
	.prepare_write = sim_prepare_write,
	.commit_write = generic_commit_write,
	.bmap = sim_bmap
};

static struct inode_operations sim_symlink_inode_operations = {
	.readlink = generic_readlink,
	.follow_link = page_follow_link_light,
	.put_link = page_put_link,
	.getattr = sim_getattr,
};

static struct super_operations sim_sops = {
	.alloc_inode = sim_alloc_inode, //分配一个索引节点
	.destroy_inode = sim_destroy_inode,
	.read_inode = sim_read_inode,
	.write_inode = sim_write_inode, //将索引节点写入磁盘
	.delete_inode = sim_delete_inode,
	.put_super = sim_put_super, //释放超级块
	.statfs = sim_statfs, //获取文件系统状态
	.remount_fs = sim_remount,
};

static struct file_system_type sim_fs_type = {
	.owner = THIS_MODULE,
	.name = "sim",
	.get_sb = sim_get_sb,
	.kill_sb = kill_block_super,
	.fs_flags = FS_REQUIRES_DEV,
};



static int  __init  init_sim_fs ( void )
{
	int err = init_inodecache ();
	if ( err )
		return err;
	err = register_filesystem ( &sim_fs_type );//向内核注册SIM文件系统
	if ( err )
	{
		destroy_inodecache ();
		return err;
	}
	return 0;
}

static void __exit exit_sim_fs ( void )
{
	unregister_filesystem ( &sim_fs_type );//注销文件系统
	destroy_inodecache (); //销毁缓冲池
}
//文件系统在kernel中被当作一个模块实现
module_init ( init_sim_fs )
module_exit ( exit_sim_fs )
MODULE_LICENSE ( "GPL" );