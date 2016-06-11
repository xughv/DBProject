#ifndef _BLOCK_FILE_H_
#define _BLOCK_FILE_H_

#include <cstdio>
#include "def.h"

// -----------------------------------------------------------------------------
//  NOTE: The author of the implementation of class BlockFile is
//        Yufei Tao and Qiang Huang.
//  Modified by Guanghui Xu
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  BlockFile: structure of reading and writing file for B-tree
// -----------------------------------------------------------------------------

class BlockFile {
public:
    FILE* fp_;                      // file pointer
    char* file_name_;               // file name
    bool  new_flag_;                // specifies if this is a new file
    
    int block_length_;              // length of a block
    int act_block_;                 // block num of fp position
    int num_blocks_;                // total num of blocks

    // -------------------------------------------------------------------------
    // constructor
    BlockFile(char* file_name, int block_length = 0);

    // destructor
    ~BlockFile();                   

    // -------------------------------------------------------------------------
    // write <bytes> of length <num>
    void PutBytes(char* bytes, int num) { fwrite(bytes, num, 1, fp_); }

    // read <bytes> of length <num>
    void GetBytes(char* bytes, int num) { fread(bytes, num, 1, fp_); }

    // move <fp_> to the right with <bnum>
    void SeekBlock(int bnum)
    { fseek(fp_, (bnum - act_block_) * block_length_, SEEK_CUR); }

    // -------------------------------------------------------------------------
    // write a value <num> (type int)
    void FwriteNumber(int num);

    // read a value (type int)
    int FreadNumber();

    // -------------------------------------------------------------------------
    // fetches info in the first block
    // <header>: excluding the header of blk file
    void ReadHeader(char* header);       

    // writes the info in the first block
    // <header>: excluding the header of blk file
    void SetHeader(char* header);

    // -------------------------------------------------------------------------
    // read a block <b> in the <pos>
    bool ReadBlock(Block block, int index);

    // write a block <b> in the <pos>
    bool WriteBlock(Block block, int index);

    // append a block at the end of file
    int AppendBlock(Block block);

    // delete last <num> blocks
    bool DeleteLastBlocks(int num);

    // -------------------------------------------------------------------------
    // whether this block is modified?
    bool IsNewFile() { return new_flag_; }

    // get block length
    int block_length() { return block_length_; }

    // get number of blocks
    int num_blocks() { return num_blocks_; }

};

#endif // _BLOCK_FILE_H_