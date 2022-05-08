#include <cpp11.hpp>
using namespace cpp11;

#include <vector>
#include <algorithm>

#include "packedrtree.h"

[[cpp11::register]]
SEXP packed_rtree_create(list rct, int node_size) {
  doubles xmins = rct[0];
  doubles ymins = rct[1];
  doubles xmaxs = rct[2];
  doubles ymaxs = rct[3];

  R_xlen_t n = xmins.size();

  FlatGeobuf::NodeItem extent = FlatGeobuf::NodeItem::create();
  extent.minX = std::numeric_limits<double>::infinity();
  extent.minY = std::numeric_limits<double>::infinity();
  extent.maxX = -std::numeric_limits<double>::infinity();
  extent.maxY = -std::numeric_limits<double>::infinity();

  std::vector<FlatGeobuf::NodeItem> items(n);
  for (R_xlen_t i = 0; i < n; i++) {
    items[i].minX = xmins[i];
    items[i].minY = ymins[i];
    items[i].maxX = xmaxs[i];
    items[i].maxY = ymaxs[i];

    extent.minX = std::min<double>(extent.minX, items[i].minX);
    extent.minY = std::min<double>(extent.minY, items[i].minY);
    extent.maxX = std::min<double>(extent.maxX, items[i].maxX);
    extent.maxY = std::min<double>(extent.maxY, items[i].maxY);
  }

  external_pointer<FlatGeobuf::PackedRTree> result(
      new FlatGeobuf::PackedRTree(std::move(items), extent, node_size));
  return result;
}

[[cpp11::register]]
list packed_rtree_query(sexp tree_sexp, list rct) {
  external_pointer<FlatGeobuf::PackedRTree> tree(tree_sexp);

  doubles xmins = rct[0];
  doubles ymins = rct[1];
  doubles xmaxs = rct[2];
  doubles ymaxs = rct[3];

  writable::integers ix;
  writable::integers itree;

  R_xlen_t n = xmins.size();

  for (R_xlen_t i = 0; i < n; i++) {
    std::vector<FlatGeobuf::SearchResultItem> item_results = tree->search(
      xmins[i], ymins[i], xmaxs[i], ymaxs[i]
    );

    for (const auto& item_result: item_results) {
      ix.push_back(i + 1);
      itree.push_back(item_result.index + 1);
      if ((ix.size() % 1000) == 0) {
        check_user_interrupt();
      }
    }
  }

  writable::list result = {ix, itree};
  result.names() = {"x", "tree"};
  return result;
}
