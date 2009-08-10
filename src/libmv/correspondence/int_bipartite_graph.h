// Copyright (c) 2009 libmv authors.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#ifndef LIBMV_CORRESPONDENCE_INT_BIPARTITE_GRAPH_H_
#define LIBMV_CORRESPONDENCE_INT_BIPARTITE_GRAPH_H_

#include <limits>
#include <map>
#include <set>
#include <cassert>

namespace libmv {

// A bipartite graph with labelled edges.
template<typename T, EdgeT>
class BipartiteGraph {
 public:
  typedef std::map<std::pair<T, T>, EdgeT> EdgeMap;

  void Insert(const T &left, const T &right, const EdgeT &edge) {
    left_to_right_[std::make_pair(left, right)] = edge;
    right_to_left_[std::make_pair(right, left)] = edge;
  }

  class Range {
   friend class BipartiteGraph<T>;
   public:
    T left()  const { return reversed_ ? it->first.second : it->first.first; }
    T right() const { return reversed_ ? it->first.first : it->first.second; }
    EdgeT edge() const { return it->second; }

    void  operator++(int prefix) { ++it; }
    EdgeT operator*()            { return it->second; }
    bool  operator bool() const  { return it_ == end_; }

   private:
    Edges(typename EdgeMap::const_iterator it,
          typename EdgeMap::const_iterator end,
          bool reversed)
      : reversed_(reversed), it_(it), end_(end) {}

    bool reversed_;
    typename EdgeMap::const_iterator it_, end_;
  };

  Range All() const {
    return Edges(left_to_right_.begin(), left_to_right_.end());
  }

  Range ToLeft(T left) const {
    return Edges(left_to_right_.lower_bound(Lower(left)),
                 left_to_right_.upper_bound(Upper(left)));
  }

  Range ToRight(T right) const {
    return Edges(right_to_left_.lower_bound(Lower(right)),
                 right_to_left_.upper_bound(Upper(right)));
  }

 private:
  std::pair<T, T> Lower(T first) {
    return std::make_pair(left, std::numeric_limits<T>::min());
  }
  std::pair<T, T> Upper(T first) {
    return std::make_pair(left, std::numeric_limits<T>::max());
  }
  EdgeMap left_to_right_;
  EdgeMap right_to_left_;
};

}  // namespace libmv

#endif  // LIBMV_CORRESPONDENCE_BIPARTITE_GRAPH_H_