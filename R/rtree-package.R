#' @keywords internal
"_PACKAGE"

## usethis namespace: start
#' @useDynLib rtree, .registration = TRUE
## usethis namespace: end
NULL

.onLoad <- function(...) {
  requireNamespace("libgeos", quietly = TRUE)
  rtree_libgeos_init_api()
}
