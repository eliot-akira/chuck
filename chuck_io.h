/*----------------------------------------------------------------------------
 ChucK Concurrent, On-the-fly Audio Programming Language
 Compiler and Virtual Machine
 
 Copyright (c) 2004 Ge Wang and Perry R. Cook.  All rights reserved.
 http://chuck.cs.princeton.edu/
 http://soundlab.cs.princeton.edu/
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 U.S.A.
 -----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// name: chuck_io.h
// desc: chuck i/o
//
// authors: Spencer Salazar (spencer@ccrma.stanford.edu)
//    date: Summer 2012
//-----------------------------------------------------------------------------

#ifndef __CHUCK_IO_H__
#define __CHUCK_IO_H__

#include "chuck_oo.h"
#include "util_thread.h"
#include "util_buffers.h"
#include <list>


struct Chuck_IO_Serial : public Chuck_IO
{
public:
    Chuck_IO_Serial();
    virtual ~Chuck_IO_Serial();
    
    
public:
    // meta
    virtual t_CKBOOL open( const t_CKUINT i, t_CKINT flags, t_CKUINT baud = BAUD_9600 );
    virtual t_CKBOOL open( const std::string & path, t_CKINT flags );
    
    virtual t_CKBOOL good();
    virtual void close();
    virtual void flush();
    virtual t_CKINT mode();
    virtual void mode( t_CKINT flag );
    
    // reading
    virtual Chuck_String * readLine();
    virtual t_CKINT readInt( t_CKINT flags );
    virtual t_CKFLOAT readFloat();
    virtual t_CKBOOL readString( std::string & str );
    virtual t_CKBOOL eof();
    
    // writing
    virtual void write( const std::string & val );
    virtual void write( t_CKINT val );
    virtual void write( t_CKFLOAT val );

    // serial stuff
    virtual t_CKBOOL setBaudRate( t_CKUINT rate );
    virtual t_CKUINT getBaudRate();
    
    // async reading
    virtual void readAsync( t_CKUINT type, t_CKUINT num );
    virtual Chuck_String * getLine();
    virtual t_CKINT getByte();
    virtual Chuck_Array * getBytes();
    virtual Chuck_Array * getInts();
    virtual Chuck_Array * getFloats();
    virtual Chuck_String * getString();
    
    static const t_CKUINT TYPE_BYTE = 0;
    static const t_CKUINT TYPE_WORD = 1;
    static const t_CKUINT TYPE_INT = 2;
    static const t_CKUINT TYPE_FLOAT = 3;
    static const t_CKUINT TYPE_STRING = 4;
    static const t_CKUINT TYPE_LINE = 5;
    
    struct Read
    {
        t_CKUINT m_type; // type
        t_CKUINT m_num; // how many
        t_CKUINT m_status;
        t_CKUINT m_val; // ISSUE: 64-bit
        
        enum Status
        {
            STATUS_PENDING,
            STATUS_SUCCESS,
            STATUS_FAILURE,
            STATUS_EOF,
        };
    };
    
    virtual t_CKBOOL ready();
    
    // available baud rates
    const static t_CKUINT BAUD_2400;
    const static t_CKUINT BAUD_4800;
    const static t_CKUINT BAUD_9600;
    const static t_CKUINT BAUD_19200;
    const static t_CKUINT BAUD_38400;
    const static t_CKUINT BAUD_7200;
    const static t_CKUINT BAUD_14400;
    const static t_CKUINT BAUD_28800;
    const static t_CKUINT BAUD_57600;
    const static t_CKUINT BAUD_76800;
    const static t_CKUINT BAUD_115200;
    const static t_CKUINT BAUD_230400;

protected:
    
    XThread * m_read_thread;
    static void *shell_read_cb(void *);
    void read_cb();
    bool m_do_read_thread;
    
    CircularBuffer<Read> m_asyncReadRequests;
    CircularBuffer<Read> m_asyncReadResponses;
    
    CBufferSimple * m_event_buffer;
    
    int m_fd;
    FILE * m_cfd;
    
    char * m_buf;
    t_CKUINT m_buf_size;
    t_CKUINT m_buf_begin, m_buf_end;
    t_CKINT m_flags;
};

t_CKBOOL init_class_serialio( Chuck_Env * env ); // added 1.3.1

#endif // __CHUCK_IO_H__

