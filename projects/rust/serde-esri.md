---
categories: [rust, spatial]
---

# serde_esri 

[`serde_esri`](https://github.com/JosiahParry/serde_esri) provides struct definitions to represent Esri JSON objects with [`serde::Deserialize`](https://docs.rs/serde/1.0.192/serde/de/trait.Deserialize.html) and [`serde::Serialize`](https://docs.rs/serde/1.0.192/serde/de/trait.Serialize.html) trait implementations.

`serde_esri` has additional features:

- [`geo`](https://docs.rs/geo) implements `From` for the Esri JSON objects.
- [`geoarrow`](https://docs.rs/geoarrow) provides compatibility with arrow and geoarrow by implementing geoarrow geometry traits as well as providing a utility function `featureset_to_geoarrow()` which converts a `FeatureSet` to an arrow `GeoTable`.
- `places-client` provides an API client for the Places Service REST API. 
    - built ontop of [`reqwest`](https://docs.rs/reqwest/latest/reqwest/)

