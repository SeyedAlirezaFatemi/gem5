/*
 * Copyright (c) 1999-2008 Mark D. Hill and David A. Wood
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * flitBuffer.C
 *
 * Niket Agarwal, Princeton University
 *
 * */

#include "flitBuffer.hh"

flitBuffer::flitBuffer()
{
        max_size = INFINITE_;
}

flitBuffer::flitBuffer(int maximum_size)
{
        max_size = maximum_size;
}

bool flitBuffer::isEmpty()
{
        return (m_buffer.size() == 0);
}

bool flitBuffer::isReady()
{
        if(m_buffer.size() != 0 )
        {
                flit *t_flit = m_buffer.peekMin();
                if(t_flit->get_time() <= g_eventQueue_ptr->getTime())
                        return true;
        }
        return false;
}

bool flitBuffer::isReadyForNext()
{
        if(m_buffer.size() != 0 )
        {
                flit *t_flit = m_buffer.peekMin();
                if(t_flit->get_time() <= (g_eventQueue_ptr->getTime() + 1))
                        return true;
        }
        return false;
}

bool flitBuffer::isFull()
{
        return (m_buffer.size() >= max_size);
}

void flitBuffer::setMaxSize(int maximum)
{
        max_size = maximum;
}

flit* flitBuffer:: getTopFlit()
{
        return m_buffer.extractMin();
}

flit* flitBuffer::peekTopFlit()
{
        return m_buffer.peekMin();
}

void flitBuffer::insert(flit *flt)
{
        m_buffer.insert(flt);
}

void flitBuffer::print(ostream& out) const
{
        out << "[flitBuffer: ";
        out << m_buffer.size() << "] " << endl;
}
