
#' FlatGeobuf PackedRTree
#'
#' @inheritParams rtree_geos
#'
#' @export
#'
#' @examples
#' points <- wk::xy(runif(10), runif(10))
#' tree <- rtree_flatgeobuf(points)
#' rtree_flatgeobuf_query(tree, wk::rct(0.25, 0.25, 0.75, 0.75))
#'
rtree_flatgeobuf <- function(x, node_capacity = 16) {
  structure(
    packed_rtree_create(wk::wk_envelope(x), node_capacity),
    class = "rtree_flatgeobuf"
  )
}

#' @rdname rtree_flatgeobuf
#' @export
rtree_flatgeobuf_query <- function(tree, x) {
  stopifnot(inherits(tree, "rtree_flatgeobuf"))
  new_data_frame(packed_rtree_query(tree, wk::wk_envelope(x)))
}
