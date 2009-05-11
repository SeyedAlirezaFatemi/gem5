
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
 * $Id$
 *
 */

#include "Histogram.hh"

Histogram::Histogram(int binsize, int bins)
{
  m_binsize = binsize;
  m_bins = bins;
  clear();
}

Histogram::~Histogram()
{
}

void Histogram::clear(int binsize, int bins)
{
  m_binsize = binsize;
  clear(bins);
}

void Histogram::clear(int bins)
{
  m_bins = bins;
  m_largest_bin = 0;
  m_max = 0;
  m_data.setSize(m_bins);
  for (int i = 0; i < m_bins; i++) {
    m_data[i] = 0;
  }
  m_count = 0;
  m_max = 0;

  m_sumSamples = 0;
  m_sumSquaredSamples = 0;
}


void Histogram::add(int64 value)
{
  assert(value >= 0);
  m_max = max(m_max, value);
  m_count++;

  m_sumSamples += value;
  m_sumSquaredSamples += (value*value);

  int index;
  if (m_binsize == -1) {
    // This is a log base 2 histogram
    if (value == 0) {
      index = 0;
    } else {
      index = int(log(double(value))/log(2.0))+1;
      if (index >= m_data.size()) {
        index = m_data.size()-1;
      }
    }
  } else {
    // This is a linear histogram
    while (m_max >= (m_bins * m_binsize)) {
      for (int i = 0; i < m_bins/2; i++) {
        m_data[i] = m_data[i*2] + m_data[i*2 + 1];
      }
      for (int i = m_bins/2; i < m_bins; i++) {
        m_data[i] = 0;
      }
      m_binsize *= 2;
    }
    index = value/m_binsize;
  }
  assert(index >= 0);
  m_data[index]++;
  m_largest_bin = max(m_largest_bin, index);
}

void Histogram::add(const Histogram& hist)
{
  assert(hist.getBins() == m_bins);
  assert(hist.getBinSize() == -1);  // assume log histogram
  assert(m_binsize == -1);

  for (int j = 0; j < hist.getData(0); j++) {
    add(0);
  }

  for (int i = 1; i < m_bins; i++) {
    for (int j = 0; j < hist.getData(i); j++) {
      add(1<<(i-1));  // account for the + 1 index
    }
  }

}

// Computation of standard deviation of samples a1, a2, ... aN
// variance = [SUM {ai^2} - (SUM {ai})^2/N]/(N-1)
// std deviation equals square root of variance
double Histogram::getStandardDeviation() const
{
  double variance;
  if(m_count > 1){
    variance = (double)(m_sumSquaredSamples - m_sumSamples*m_sumSamples/m_count)/(m_count - 1);
  } else {
    return 0;
  }
  return sqrt(variance);
}

void Histogram::print(ostream& out) const
{
  printWithMultiplier(out, 1.0);
}

void Histogram::printPercent(ostream& out) const
{
  if (m_count == 0) {
    printWithMultiplier(out, 0.0);
  } else {
    printWithMultiplier(out, 100.0/double(m_count));
  }
}

void Histogram::printWithMultiplier(ostream& out, double multiplier) const
{
  if (m_binsize == -1) {
    out << "[binsize: log2 ";
  } else {
    out << "[binsize: " << m_binsize << " ";
  }
  out << "max: " << m_max << " ";
  out << "count: " << m_count << " ";
  //  out << "total: " <<  m_sumSamples << " ";
  if (m_count == 0) {
    out << "average: NaN |";
    out << "standard deviation: NaN |";
  } else {
    out << "average: " << setw(5) << ((double) m_sumSamples)/m_count << " | ";
    out << "standard deviation: " << getStandardDeviation() << " |";
  }
  for (int i = 0; i < m_bins && i <= m_largest_bin; i++) {
    if (multiplier == 1.0) {
      out << " " << m_data[i];
    } else {
      out << " " << double(m_data[i]) * multiplier;
    }
  }
  out << " ]";
}

bool node_less_then_eq(const Histogram* n1, const Histogram* n2)
{
  return (n1->size() > n2->size());
}
