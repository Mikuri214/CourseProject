#include "sim.h"
#include<string>
#include <linux/pagemap.h>

static inline void inc_count ( struct inode *inode )
{
	inode->i_nlink++;
	mark_inode_dirty ( inode );
}

static inline void dec_count ( struct inode *inode )
{
	inode->i_nlink--;
	mark_inode_dirty ( inode );
}

static int add_nondir ( struct dentry *dentry, struct inode *inode )
{
	int err = sim_add_link ( dentry, inode );
	if ( !err ) {
		d_instantiate ( dentry, inode );
		return 0;
	}
	dec_count ( inode );
	iput ( inode );
	return err;
}

static int sim_hash ( struct dentry *dentry, struct qstr *qstr )
{
	unsigned long hash;
	int i;
	const unsigned char *name;

	i = sim_sb ( dentry->d_inode->i_sb )->s_namelen;
	if ( i >= qstr->len )
		return 0;
	/* Truncate the name in place, avoids having to define a compare
	function. */
	qstr->len = i;
	name = qstr->name;
	hash = init_name_hash ();
	while ( i-- )
		hash = partial_name_hash ( *name++, hash );
	qstr->hash = end_name_hash ( hash );
	return 0;
}

struct dentry_operations sim_dentry_operations = {
	.d_hash = sim_hash,
};

static struct dentry *sim_lookup ( struct inode * dir, struct dentry *dentry, struct nameidata *nd )
{
	struct inode * inode = NULL;
	ino_t ino;

	dentry->d_op = dir->i_sb->s_root->d_op;

	if ( dentry->d_name.len > sim_sb ( dir->i_sb )->s_namelen )
		return ERR_PTR ( -ENAMETOOLONG );

	ino = sim_inode_by_name ( dentry );
	if ( ino ) {
		inode = iget ( dir->i_sb, ino );

		if ( !inode )
			return ERR_PTR ( -EACCES );
	}
	d_add ( dentry, inode );
	return NULL;
}

static int sim_mknod ( struct inode * dir, struct dentry *dentry, int mode, dev_t rdev )
{
	int error;
	struct inode *inode;

	if ( !old_valid_dev ( rdev ) )
		return -EINVAL;

	inode = sim_new_inode ( dir, &error );//在dir代表的目录中新建一个索引节点

	if ( inode ) {
		inode->i_mode = mode;
		sim_set_inode ( inode, rdev ); //根据索引节点类型，设置不同的操作
		mark_inode_dirty ( inode );
		error = add_nondir ( dentry, inode );//将目录项和索引节点联系起来
	}
	return error;
}

static int sim_create ( struct inode * dir, struct dentry *dentry, int mode,
struct nameidata *nd )
{
	return sim_mknod ( dir, dentry, mode, 0 );
}

static int sim_symlink ( struct inode * dir, struct dentry *dentry,
	const char * symname )
{
	int err = -ENAMETOOLONG;
	int i = strlen ( symname ) + 1;
	struct inode * inode;

	if ( i > dir->i_sb->s_blocksize )
		return err;

	inode = sim_new_inode ( dir, &err );
	if ( !inode )
		return err;

	inode->i_mode = S_IFLNK | 0777;
	sim_set_inode ( inode, 0 );
	err = page_symlink ( inode, symname, i );
	if ( err )
	{
		dec_count ( inode );
		iput ( inode );
		return err;
	}
	err = add_nondir ( dentry, inode );
	return err;
}

static int sim_link ( struct dentry * old_dentry, struct inode * dir,
struct dentry *dentry )
{
	struct inode *inode = old_dentry->d_inode;

	if ( inode->i_nlink >= sim_sb ( inode->i_sb )->s_link_max )
		return -EMLINK;

	inode->i_ctime = CURRENT_TIME_SEC;
	inc_count ( inode );
	atomic_inc ( &inode->i_count );
	return add_nondir ( dentry, inode );
}

static int sim_unlink ( struct inode * dir, struct dentry *dentry )
{
	int err = -ENOENT;
	struct inode * inode = dentry->d_inode;
	struct page * page;
	struct sim_dir_entry * de;

	de = sim_find_entry ( dentry, &page );
	if ( !de )
		return err;

	err = sim_delete_entry ( de, page );
	if ( err )
		return err;

	inode->i_ctime = dir->i_ctime;
	dec_count ( inode );
	return err;
}

static int sim_mkdir ( struct inode * dir, struct dentry *dentry, int mode )
{
	struct inode * inode;
	int err = -EMLINK;

	if ( dir->i_nlink >= sim_sb ( dir->i_sb )->s_link_max )
		goto out;

	inc_count ( dir );

	inode = sim_new_inode ( dir, &err );
	if ( !inode )
		goto out_dir;

	inode->i_mode = S_IFDIR | mode;
	if ( dir->i_mode & S_ISGID )
		inode->i_mode |= S_ISGID;
	sim_set_inode ( inode, 0 );

	inc_count ( inode );

	err = sim_make_empty ( inode, dir );
	if ( err )
		goto out_fail;

	err = sim_add_link ( dentry, inode );
	if ( err )
		goto out_fail;

	d_instantiate ( dentry, inode );
out:
	return err;

out_fail:
	//需要减少两次，因为在alloc_inode中已经设置过一次inode->i_nlink=1
	dec_count ( inode );
	dec_count ( inode );
	iput ( inode );
out_dir:
	dec_count ( dir );
	goto out;
}

static int sim_rmdir ( struct inode * dir, struct dentry *dentry )
{
	struct inode * inode = dentry->d_inode;
	int err = -ENOTEMPTY;

	if ( sim_empty_dir ( inode ) ) {
		err = sim_unlink ( dir, dentry );
		if ( !err ) {
			dec_count ( dir );
			dec_count ( inode );
		}
	}
	return err;
}

static int sim_rename ( struct inode * old_dir, struct dentry *old_dentry,
struct inode * new_dir, struct dentry *new_dentry )
{
	struct sim_sb_info * info = sim_sb ( old_dir->i_sb );
	struct inode * old_inode = old_dentry->d_inode;
	struct inode * new_inode = new_dentry->d_inode;
	struct page * dir_page = NULL;
	struct sim_dir_entry * dir_de = NULL;
	struct page * old_page;
	struct sim_dir_entry * old_de;
	int err = -ENOENT;

	old_de = sim_find_entry ( old_dentry, &old_page );
	if ( !old_de )
		goto out;

	if ( S_ISDIR ( old_inode->i_mode ) ) {
		err = -EIO;
		dir_de = sim_dotdot ( old_inode, &dir_page );
		if ( !dir_de )
			goto out_old;
	}

	if ( new_inode ) {
		struct page * new_page;
		struct sim_dir_entry * new_de;

		err = -ENOTEMPTY;
		if ( dir_de && !sim_empty_dir ( new_inode ) )
			goto out_dir;

		err = -ENOENT;
		new_de = sim_find_entry ( new_dentry, &new_page );
		if ( !new_de )
			goto out_dir;
		inc_count ( old_inode );
		sim_set_link ( new_de, new_page, old_inode );
		new_inode->i_ctime = CURRENT_TIME_SEC;
		if ( dir_de )
			new_inode->i_nlink--;
		dec_count ( new_inode );
	}
	else {
		if ( dir_de ) {
			err = -EMLINK;
			if ( new_dir->i_nlink >= info->s_link_max )
				goto out_dir;
		}
		inc_count ( old_inode );
		err = sim_add_link ( new_dentry, old_inode );
		if ( err ) {
			dec_count ( old_inode );
			goto out_dir;
		}
		if ( dir_de )
			inc_count ( new_dir );
	}

	sim_delete_entry ( old_de, old_page );
	dec_count ( old_inode );

	if ( dir_de ) {
		sim_set_link ( dir_de, dir_page, new_dir );
		dec_count ( old_dir );
	}
	return 0;

out_dir:
	if ( dir_de ) {
		kunmap ( dir_page );
		page_cache_release ( dir_page );
	}
out_old:
	kunmap ( old_page );
	page_cache_release ( old_page );
out:
	return err;
}

/*
* directories can handle most operations...
*/
struct inode_operations sim_dir_inode_operations = {
	.create = sim_create,
	.lookup = sim_lookup,
	.link = sim_link,
	.unlink = sim_unlink,
	.symlink = sim_symlink,
	.mkdir = sim_mkdir,
	.rmdir = sim_rmdir,
	.mknod = sim_mknod,
	.rename = sim_rename,
	.getattr = sim_getattr,
};
