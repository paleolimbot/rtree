
<!-- README.md is generated from README.Rmd. Please edit that file -->

# rtree

<!-- badges: start -->
<!-- badges: end -->

The goal of rtree is to experiment with a few different types of spatial
indexes at a pretty low level.

## Installation

You can install the development version of rtree from
[GitHub](https://github.com/) with:

``` r
# install.packages("remotes")
remotes::install_github("paleolimbot/rtree")
```

## Example

This is a basic example which shows you how to solve a common problem:

``` r
library(geoarrow)
library(rtree)

buildings <- read_geoparquet("../geoarrow/data-raw/denmark_osm/osm_buildings_a-geoarrow.parquet")
test_query <- wk::as_rct(grd::grd(buildings, nx = 20, ny = 20))
```

``` r
bench::mark(rtree_geos(buildings))
#> Warning: Some expressions had a GC in every iteration; so filtering is disabled.
#> # A tibble: 1 × 6
#>   expression                 min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>            <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 rtree_geos(buildings)    1.03s    1.03s     0.968     108MB    0.968
tree_geos <- rtree_geos(buildings)

bench::mark(rtree_geos_query(tree_geos, test_query))
#> # A tibble: 1 × 6
#>   expression                                   min   median `itr/sec` mem_alloc
#>   <bch:expr>                              <bch:tm> <bch:tm>     <dbl> <bch:byt>
#> 1 rtree_geos_query(tree_geos, test_query)   38.9ms   39.7ms      25.2      64MB
#> # … with 1 more variable: gc/sec <dbl>
```

``` r
bench::mark(rtree_flatgeobuf(buildings))
#> # A tibble: 1 × 6
#>   expression                       min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>                  <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 rtree_flatgeobuf(buildings)    368ms    368ms      2.72     108MB     2.72
tree_fgb <- rtree_flatgeobuf(buildings)

bench::mark(rtree_flatgeobuf_query(tree_fgb, test_query))
#> # A tibble: 1 × 6
#>   expression                                      min median `itr/sec` mem_alloc
#>   <bch:expr>                                   <bch:> <bch:>     <dbl> <bch:byt>
#> 1 rtree_flatgeobuf_query(tree_fgb, test_query)  351ms  354ms      2.82      64MB
#> # … with 1 more variable: gc/sec <dbl>
```
