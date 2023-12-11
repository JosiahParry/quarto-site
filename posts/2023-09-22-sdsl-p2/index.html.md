---
title: "SDSL P2 - on community driven development"
draft: true
keep-md: true
---





The starkest differences between the three language communities—R, Python, and Julia—was the way in which packages are tested and distributed. R

-   R is the least developer friendly langauge there is

The culture of R prides itself in being backwards compatible so much so that much of base R's syntax is still compatible with S (or is it S+?).

There are two groups that impact R's development: R-core and CRAN. R-core is the team responsible for building the R language itself. CRAN, the comprehensive R archive network is the package distribution network (you also can download R from them. What specifically, then, is CRAN? lol)

- I have gripes with R core but this is focused on package distribution and building

## CRAN overview
CRAN provides many niceties for the R community.
- distribute packages
- not only source, but binaries for windows and mac
  - current version of R, old versions, and the development version
- 

## How the others do it

-   Both other langauges have visibility into testing and compilation through Github CI

### Python

- python has a community driven package management approach in conda-forge
- devs are responsible for building binaries for PyPI
    -   this would be honestly a nice option for R users


### Julia

yygrdasil 


### What do they have in common?

- visibility visibility visibility
- the ability to contribute to the process
  - moreso for julia (its quite impressive frankly)
  

  
## CRAN pain points

### Tone

- be nice ffs. CRAN are volunteers, but so are all of the developers. 
- isochrone incident

### System Dependencies  

- sf is in a special situation. big package lots of people rely on it. their voice matters to CRAN
- R is hindered by the restrictions that CRAN places on R package developers
    -   most importantly: the requirement to vendor dependencies
        -   sf is spoiled in that its widely used so CRAN will actually communicate with them to make the process better
        -   CRAN provides binaries for them
- Other languages have to vendor them

:::{.callout-note}
Vendoring is the inclusion of the dependency itself in the source code 
:::

- vendoring dependencies can be tough and awkward
- rpolars is hit hard by this problem. It becomes a very big package to install which far exceeds CRAN (tiny) limit of 5mb
- extendr community has struggled with this issue quite a bit.
- we're extremeley limited in what we can build because of this requirement
- we're disincentivized to innovate and incorporate other tools
- even _though_ R is framed as an interface language the amount of interfacing we can do is limited not by the language but by arcane standards imposed by it?

## If you don't like CRAN don't use it? 

Declaration Quote

Are you not allowed to want the best for _everyone_?
CRANs policies are not good for the growth of the language. They are bad for the community. VERY talented developers have switched languages or stopped building R packages because of CRAN. That is not good. 

Kara Woo's quote that I liked

I remember when the change-cran.org movement happened a few years ago. I was much more of a user than I am developer now. Holy shit does that change your perspective. 


## How do we fix it?

Much, if not all, of my frustration stems from the fact that CRAN does not provide thorough automated testing. We can test in github actions, 

for example i ran into a problem that was tested manually on a fedora linux machine. I have never used fedora, none of the GHA are on fedora. I built a fedora docker container from scratch and installed the dependencies in the way that I _thought_ they might have been for the CRAN maintainer. 

If CRAN tests are not reproducible how are we to be able to prevent wasting their time and ours? 

CRAN needs to make all of their tests automated. They should provide infrastructure to test our packages our selves. 

The fact that `R CMD check` isn't sufficient is ridiculous. 


### Automate it

### Improve visibility

### Expand CRAN's team

- make it possible to expand the CRAN team beyond the clique 
- Jeroen, for example, 


## On retirement

-   We talked about Roger's retirement
    -   not stated out loud, but i think the quiet assumption was that what is being done is very generous if not overly generous. The time frame provided has been massive. The amount of work to help others is massive. In other ecosystems package will just be abandonded and thats not necessarily bad. But this amount of committment to the community is impressive.
        -   i find it to be a really nice precedent to set.
        -   i also think people will not listen without a little bit of pain themselves sometimes, so its okay to rip packages off of cran without them being fixed in advance. A fire under an ass is a great motivator
-   What about retirement of CRAN staff? Who is going to replace them? What plans do they have to handle retirement? How do people ascend into CRAN? Right now its a niche clique.







## Potential Standardization Topics:

-   storing of CRS in memory:
    -   sf is strings
    -   geopandas is a proj pointer
    -   geoarrow requires projjson

-   Julia can contribute to the build process in Github



## Esri specific

-   R, Python, and Julia do not know how to perform spherical buffers. I think this is something that the geoprocessing team already has solved
-   arrow tables can be provided as input into a GP tool!!
    -   interestingly OIDs must be i64
    -   geomerty encodings are wkb, wkt, geojson, or EsriJSON
        -   when GeoArrow spec 1.0.0 comes out we should consider supportin that structure
            -   it would enable, in theory, R data as input to ArcPy if its an arrow table
-   Geodesic buffering is not widely available in SDSL but been available in ArcGIS since 9.3
    -   https://www.esri.com/news/arcuser/0111/geodesic.html

https://twitter.com/medriscoll/status/1704563111653687502

-   TODO: GitHub discussion about enumarating AGR interactions and which are "ok" and which are not? What are the assumptions that we're violating.
    -   This obviously shouldn't error, but a warning, or way to report on the assumptions made or violated throughout geometric operations. This requires propagating attributes forward in analyses which might be easier for R devs than others
