
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
#'
rtree_geos <- function(x, node_capacity = 16) {
  geos::geos_strtree(wk::wk_envelope(x), node_capacity = node_capacity)
}

#' @rdname rtree_geos
#' @export
rtree_geos_query <- function(tree, x) {
  stopifnot(inherits(tree, "geos_strtree"))
  result <- geos::geos_strtree_query(tree, wk::wk_envelope(x))
  new_data_frame(
    list(
      x = vctrs::vec_rep_each(seq_along(result), lengths(result, FALSE)),
      tree = unlist(result, use.names = FALSE)
    )
  )
}
