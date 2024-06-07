---
categories: [rust, r, spatial]
---

# sfconversions

A minimal Rust library to convert geometry objects from the R package [{sf}](https://r-spatial.github.io/sf/) into [geo-types](https://docs.rs/geo-types/latest/geo_types/) geometry primitives using [extendr](https://extendr.github.io/extendr/extendr_api/).

Provides simple conversions between sfg, sfc objects from sf, and geometry primitives from geo_types that can be used with other georust libraries powered by [extendr](https://extendr.github.io/extendr/extendr_api/).

Due to the [orphan rule](https://github.com/Ixrec/rust-orphan-rules) conversion directly from extendr `Lists` to geo_types is not possible. For that reason a simple struct `Geom` is implemented with a single field `geom` which contains a geo_types `Geometry` enum. 

[`sfconversions`](https://github.com/JosiahParry/sfconversions)
