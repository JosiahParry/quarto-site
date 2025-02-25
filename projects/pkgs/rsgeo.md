---
categories: [rust, spatial, r]
---

# rsgeo (deprecated)

`rsgeo` is an interface to the Rust libraries `geo-types` and `geo`.
`geo-types` implements pure rust geometry primitives. The `geo` library
adds additional algorithm functionalities on top of `geo-types`. This
package lets you harness the speed, safety, and memory efficiency of
these libraries. `geo-types` does not support Z or M dimensions. There
is no support for CRS at this moment.

[GitHub Repo](https://github.com/JosiahParry/rsgeo/)

----

Note that this has been deprecated in favor of development towards geoarrow-rust integration [`{geoarrowrs}`](https://github.com/josiahParry/geoarrowrs) which also uses `geo` under the hood for some calculations. 

