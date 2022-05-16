#include <cpp11.hpp>
using namespace cpp11;

#include "libgeos.h"
#include "libgeos.c"

static GEOSContextHandle_t context;

class GEOSSTRTreeWrapper {
public:
  GEOSSTRtree* ptr;
  uintptr_t size;

  GEOSSTRTreeWrapper(int node_size):
      ptr(GEOSSTRtree_create_r(context, node_size)), size(0) {
    if (ptr == nullptr) {
      stop("error creating GEOSSTRTree");
    }
  }

  ~GEOSSTRTreeWrapper() {
    GEOSSTRtree_destroy_r(context, ptr);
  }
};

class GEOSRectWrapper {
public:
  GEOSGeometry* ptr;

  GEOSRectWrapper(double xmin, double ymin, double xmax, double ymax):
    ptr(nullptr) {
    double xs[] = {xmin, xmax};
    double ys[] = {ymin, ymax};

    GEOSCoordSequence* seq = GEOSCoordSeq_copyFromArrays_r(
      context,
      xs,
      ys,
      nullptr,
      nullptr,
      2
    );

    if (seq == nullptr) {
      stop("error creating GEOSCoordSequence");
    }

    ptr = GEOSGeom_createLineString_r(context, seq);
    if (ptr == nullptr) {
      GEOSCoordSeq_destroy_r(context, seq);
      stop("error creating GEOSGeometry");
    }
  }

  ~GEOSRectWrapper() {
    if (ptr != nullptr) {
      GEOSGeom_destroy_r(context, ptr);
    }
  }

};

[[cpp11::register]]
void rtree_libgeos_init_api() {
  libgeos_init_api();
  context = GEOS_init_r();
}

[[cpp11::register]]
SEXP geos_strtree_create(int node_size) {
  return external_pointer<GEOSSTRTreeWrapper>(new GEOSSTRTreeWrapper(node_size));
}

[[cpp11::register]]
int geos_strtree_size(sexp tree_sexp) {
  external_pointer<GEOSSTRTreeWrapper> tree(tree_sexp);
  return tree->size;
}

[[cpp11::register]]
int geos_strtree_insert(sexp tree_sexp, list rct) {
  external_pointer<GEOSSTRTreeWrapper> tree(tree_sexp);

  doubles xmins = rct[0];
  doubles ymins = rct[1];
  doubles xmaxs = rct[2];
  doubles ymaxs = rct[3];

  R_xlen_t n = xmins.size();

  for (R_xlen_t i = 0; i < n; i++) {
    if ((i % 1000) == 0) {
      check_user_interrupt();
    }

    GEOSRectWrapper geom(xmins[i], ymins[i], xmaxs[i], ymaxs[i]);
    GEOSSTRtree_insert_r(context, tree->ptr, geom.ptr, reinterpret_cast<void*>(tree->size));
    tree->size++;
  }

  return tree->size;
}

struct GEOSSTRTreeQuery {
  int ix_;
  writable::integers* ix;
  writable::integers* itree;
  SEXP error;
  bool found_item;
};

void query_callback_once(void *item, void *userdata) {
  uintptr_t item_value = reinterpret_cast<uintptr_t>(item);
  auto query = reinterpret_cast<GEOSSTRTreeQuery*>(userdata);
  if (query->error != R_NilValue || query->found_item) {
    return;
  }

  try {
    query->itree->push_back(static_cast<int>(item_value + 1));
    query->found_item = true;
  } catch (unwind_exception& e) {
    query->error = e.token;
  }
}

void query_callback(void *item, void *userdata) {
  uintptr_t item_value = reinterpret_cast<uintptr_t>(item);
  auto query = reinterpret_cast<GEOSSTRTreeQuery*>(userdata);
  if (query->error != R_NilValue) {
    return;
  }

  try {
    query->ix->push_back(query->ix_ + 1);
    query->itree->push_back(static_cast<int>(item_value + 1));
  } catch (unwind_exception& e) {
    query->error = e.token;
  }
}

[[cpp11::register]]
list geos_strtree_query(sexp tree_sexp, list rct, bool query_once) {
  external_pointer<GEOSSTRTreeWrapper> tree(tree_sexp);

  doubles xmins = rct[0];
  doubles ymins = rct[1];
  doubles xmaxs = rct[2];
  doubles ymaxs = rct[3];

  R_xlen_t n = xmins.size();

  writable::integers ix;
  writable::integers itree;
  struct GEOSSTRTreeQuery query {0, &ix, &itree, R_NilValue, false};

  GEOSQueryCallback callback;
  if (query_once) {
    callback = &query_callback_once;
  } else {
    callback = &query_callback;
  }

  for (R_xlen_t i = 0; i < n; i++) {
    if ((i % 1000) == 0) {
      check_user_interrupt();
    }

    query.ix_ = i;
    query.found_item = false;
    GEOSRectWrapper geom(xmins[i], ymins[i], xmaxs[i], ymaxs[i]);
    GEOSSTRtree_query_r(context, tree->ptr, geom.ptr, callback, &query);

    if (query.error != R_NilValue) {
      throw unwind_exception(query.error);
    } else if (query_once && !query.found_item) {
      itree.push_back(NA_INTEGER);
    }
  }

  writable::list result = {ix, itree};
  result.names() = {"x", "tree"};
  return result;
}
