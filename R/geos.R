
#' GEOS Tree
#'
#' @param x A vector of geometries for which [wk::wk_envelope()] works
#' @param tree A tree index object
#' @param node_capacity Node size
#'
#' @export
#'
#' @examples
#' points <- wk::xy(runif(10), runif(10))
#' tree <- rtree_geos(points)
#' rtree_geos_query(tree, wk::rct(0.25, 0.25, 0.75, 0.75))
#' rtree_geos_query_once(tree, wk::rct(0.25, 0.25, 0.75, 0.75))
#' rtree_geos_size(tree)
#'
rtree_geos <- function(x, node_capacity = 16) {
  tree <- structure(
    geos_strtree_create(node_capacity),
    class = "rtree_geos_strtree"
  )

  rtree_geos_insert(tree, x)
  tree
}

#' @rdname rtree_geos
#' @export
rtree_geos_insert <- function(tree, x) {
  stopifnot(inherits(tree, "rtree_geos_strtree"))
  geos_strtree_insert(tree, wk::wk_envelope(x))
}

#' @rdname rtree_geos
#' @export
rtree_geos_query <- function(tree, x) {
  stopifnot(inherits(tree, "rtree_geos_strtree"))
  new_data_frame(geos_strtree_query(tree, wk::wk_envelope(x), FALSE))
}

#' @rdname rtree_geos
#' @export
rtree_geos_query_once <- function(tree, x) {
  stopifnot(inherits(tree, "rtree_geos_strtree"))
  geos_strtree_query(tree, wk::wk_envelope(x), TRUE)[[2]]
}

#' @rdname rtree_geos
#' @export
rtree_geos_size <- function(tree) {
  stopifnot(inherits(tree, "rtree_geos_strtree"))
  geos_strtree_size(tree)
}
