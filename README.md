
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
test_query <- wk::as_rct(grd::grd(buildings, nx = 10, ny = 10))
```

``` r
system.time(tree_geos <- rtree_geos(buildings))
#>    user  system elapsed 
#>   1.857   0.464   2.605
system.time(result_geos <- rtree_geos_query(tree_geos, test_query))
#>    user  system elapsed 
#>   0.845   0.020   0.865
```

``` r
system.time(tree_fgb <- rtree_flatgeobuf(buildings))
#>    user  system elapsed 
#>   0.346   0.058   0.459
system.time(result_fgb <- rtree_flatgeobuf_query(tree_fgb, test_query))
#>    user  system elapsed 
#>   0.162   0.011   0.173
```
