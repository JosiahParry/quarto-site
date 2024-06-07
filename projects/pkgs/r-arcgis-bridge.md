---
categories: [rust, r, package, spatial]
---

# R-ArcGIS Bridge

The R-ArcGIS Bridge is a suite of R packages designed to integrate the ArcGIS into the R ecosystem. I have developed a (growing) number of R packages to integrate ArcGIS location and data services to the R community. They are written in Rust and in R. 

See the [R-ArcGIS Bridge](https://r.esri.com/r-bridge-site/) website for more detailed documentation. 

I have built 

- [`{arcgisutils}`](https://github.com/R-ArcGIS/arcgisutils/): provides authentication, custom json serialization (written in Rust) and parsing, and other developer oriented utilities.
- [`{arcgislayers}`](https://github.com/R-ArcGIS/arcgislayers/): read and write data in ArcGIS data services such as ArcGIS Online, Enterprise, Platform, and Hub.
- [`{arcgisgeocode}`](https://github.com/R-ArcGIS/arcgisgeocode): Rust based R package that interfaces to the ArcGIS World Geocoding service. Possibly the fastest geocoding client in the R ecosystem. 
- [`{arcgisplaces}`](https://github.com/r-arcgis/arcgisplaces): Rust based R package built upon [serde_esri](https://github.com/JosiahParry/serde_esri) to interact with the [ArcGIS Places Service](https://developers.arcgis.com/documentation/mapping-apis-and-services/places/)
- [`{arcpbf}`](https://github.com/R-ArcGIS/arcpbf/): Rust based R package to process protocol buffers from ArcGIS Feature Services. This results in a dramatically faster parsing time than processing JSON. 

