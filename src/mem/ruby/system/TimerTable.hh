
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
 * TimerTable.h
 *
 * Description:
 *
 * $Id$
 *
 */

#ifndef TIMERTABLE_H
#define TIMERTABLE_H

#include "Global.hh"
#include "Map.hh"
#include "Address.hh"
class Consumer;
class Chip;

class TimerTable {
public:

  // Constructors
  TimerTable(Chip* chip_ptr);

  // Destructor
  //~TimerTable();

  // Class Methods
  static void printConfig(ostream& out) {}

  // Public Methods
  void setConsumer(Consumer* consumer_ptr) { ASSERT(m_consumer_ptr==NULL); m_consumer_ptr = consumer_ptr; }
  void setDescription(const string& name) { m_name = name; }

  bool isReady() const;
  const Address& readyAddress() const;
  bool isSet(const Address& address) const { return m_map.exist(address); }
  void set(const Address& address, Time relative_latency);
  void unset(const Address& address);
  void print(ostream& out) const;
private:
  // Private Methods
  void updateNext() const;

  // Private copy constructor and assignment operator
  TimerTable(const TimerTable& obj);
  TimerTable& operator=(const TimerTable& obj);

  // Data Members (m_prefix)
  Map<Address, Time> m_map;
  Chip* m_chip_ptr;
  mutable bool m_next_valid;
  mutable Time m_next_time; // Only valid if m_next_valid is true
  mutable Address m_next_address;  // Only valid if m_next_valid is true
  Consumer* m_consumer_ptr;  // Consumer to signal a wakeup()
  string m_name;
};

// ******************* Definitions *******************

// Output operator definition
extern inline
ostream& operator<<(ostream& out, const TimerTable& obj)
{
  obj.print(out);
  out << flush;
  return out;
}
#endif //TIMERTABLE_H
