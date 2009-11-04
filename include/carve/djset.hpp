// Begin License:
// Copyright (C) 2006-2008 Tobias Sargeant (tobias.sargeant@gmail.com).
// All rights reserved.
//
// This file is part of the Carve CSG Library (http://carve-csg.com/)
//
// This file may be used under the terms of the GNU General Public
// License version 2.0 as published by the Free Software Foundation
// and appearing in the file LICENSE.GPL2 included in the packaging of
// this file.
//
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
// End:


#pragma once

#include <carve/carve.hpp>
#include <vector>



namespace carve {
namespace djset {



  class djset {

  protected:
    struct elem {
      size_t parent, rank;
      elem(size_t p, size_t r) : parent(p), rank(r) {}
      elem() {}
    };
    std::vector<elem> set;
    size_t n_sets;

  public:
    djset(size_t N) {
      n_sets = N;
      set.reserve(N);
      for (size_t i = 0; i < N; ++i) {
        set.push_back(elem(i,0));
      }
    }

    size_t count() const {
      return n_sets;
    }

    size_t find_set_head(size_t a) {
      if (a == set[a].parent) return a;
    
      size_t a_head = a;
      while (set[a_head].parent != a_head) a_head = set[a_head].parent;
      set[a].parent = a_head;
      return a_head;
    }

    bool same_set(size_t a, size_t b) {
      return find_set_head(a) == find_set_head(b);
    }

    void merge_sets(size_t a, size_t b) {
      a = find_set_head(a);
      b = find_set_head(b);
      if (a != b) {
        n_sets--;
        if (set[a].rank < set[b].rank) {
          set[a].parent = b;
        } else if (set[b].rank < set[a].rank) {
          set[b].parent = a;
        } else {
          set[a].rank++;
          set[b].parent = a;
        }
      }
    }

    template<typename in_iter_t, typename out_collection_t>
    void collate(in_iter_t in, out_collection_t &out) {
      std::vector<size_t> set_id(set.size(), n_sets);
      out.clear();
      out.resize(n_sets);
      size_t c = 0;
      for (size_t i = 0; i < set.size(); ++i) {
        size_t s = find_set_head(i);
        if (set_id[s] == n_sets) set_id[s] = c++;
        s = set_id[s];
        std::insert_iterator<typename out_collection_t::value_type> j(out[s], out[s].end());
        *j = *in++;
      }
    }
  };



}
}
