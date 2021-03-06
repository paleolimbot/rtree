# Generated by cpp11: do not edit by hand

packed_rtree_create <- function(rct, node_size) {
  .Call(`_rtree_packed_rtree_create`, rct, node_size)
}

packed_rtree_query <- function(tree_sexp, rct) {
  .Call(`_rtree_packed_rtree_query`, tree_sexp, rct)
}

rtree_libgeos_init_api <- function() {
  invisible(.Call(`_rtree_rtree_libgeos_init_api`))
}

geos_strtree_create <- function(node_size) {
  .Call(`_rtree_geos_strtree_create`, node_size)
}

geos_strtree_size <- function(tree_sexp) {
  .Call(`_rtree_geos_strtree_size`, tree_sexp)
}

geos_strtree_insert <- function(tree_sexp, rct) {
  .Call(`_rtree_geos_strtree_insert`, tree_sexp, rct)
}

geos_strtree_query <- function(tree_sexp, rct, query_once) {
  .Call(`_rtree_geos_strtree_query`, tree_sexp, rct, query_once)
}
