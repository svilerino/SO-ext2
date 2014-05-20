#include <iostream>

#include "hdd.h"
#include "mbr.h"
#include "pentry.h"
#include <cstdlib>
#include <cstring>
#include "ext2fs.h"

using namespace std;

int main(int argc, char ** argv)
{
	HDD hdd(argv[1]);

	unsigned char buffer[SECTOR_SIZE];
	hdd.read(0, buffer);
	MBR mbr(buffer);
	const PartitionEntry & pentry = mbr[1];

	cout << pentry << endl << endl;

	Ext2FS * fs = new Ext2FS(hdd, 1);

	cout << *(fs->superblock()) << endl;

	unsigned int block_groups = fs->block_groups();
	for(unsigned int i = 0; i < block_groups; i++)
	{
		cout << *(fs->block_group(i)) << endl;
	}

	struct Ext2FSInode * inode = fs->inode_for_path("/grupos/g5");
	cout << *inode << endl;

	unsigned char bufferTest[18];
	//memset(bufferTest, '\0', 18);
	fd_t file = fs->open("/grupos/g5/nota.txt", "r");
	fs->seek(file, 13999);
	fs->read(file, bufferTest, 17);
	bufferTest[17] = '\0';
	cout << bufferTest << endl;
	return 0;
}
