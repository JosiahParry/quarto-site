---
title: Installing Python on my M1 in under 10 minutes
author: Josiah Parry
date: '2022-02-12'
slug: []
categories: [python, r, reticulate, spatial]
tags: []

rmd_hash: 4252cad9c6e044b7

---

Installing python has never been an easy task for me. I remember back in 2016 I wanted to learn how to use pyspark and thus python, I couldn't figure out how to install python so I gave up. In graduate school I couldn't install python so I used a docker container my professor created and never changed a thing. When working at RStudio I used the Jupyter Lab instance in RStudio Workbench when I couldn't install it locally.

Now, I want to compare pysal results to some functionality I've written in R. To do that, I need a python installation. I've heard extra horror stories about installing Python on the new Mac M1 chip---which I have.

Prior to installing, I took to twitter for suggestions. I received the phenomenal tweet below encouraging me to install with [`{reticulate}`](https://rstudio.github.io/reticulate/)[^1] which was absolutely phenomenal advice.

<blockquote class="twitter-tweet">
<p lang="es" dir="ltr">
reticulate::install_miniconda() 😉
</p>
--- Kevin Ushey (@kevin_ushey) <a href="https://twitter.com/kevin_ushey/status/1491188923292581888?ref_src=twsrc%5Etfw">February 8, 2022</a>
</blockquote>
<script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>

## Installing Python

The steps to install python, at least for me, was *very* simple.

1.  Install reticulate
2.  Install miniconda

``` r
install.packages("reticulate")
reticulate::install_miniconda()
```

That's it. That's all it took.

## Creating my first conda environment

After installing python, I restarted R, and began building my first conda environment. I created a conda environment called `geo` for my geospatial work. I installed `libpysal`, `geopandas`, and `esda`. These installed every other dependency I needed--e.g. `pandas`, and `numpy`.

``` r
reticulate::conda_create("geo")
reticulate::use_condaenv("geo")
reticulate::conda_install("geo", c("libpysal", "geopandas", "esda"))
```

## Using my conda environment

To begin using my new conda environment, I opened up a fresh R session and a fresh R Markdown document. In my first code chunk I told reticulate which conda environment to use. Then my following code chunks were python which opened up the python repl. Make sure that you start your code chunk with ```` ```{python} ````

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>reticulate</span><span class='nf'>::</span><span class='nf'><a href='https://rstudio.github.io/reticulate/reference/use_python.html'>use_condaenv</a></span><span class='o'>(</span><span class='s'>"geo"</span><span class='o'>)</span></code></pre>

</div>

In the following example I utilize `esda` to calculate a local join count.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'>import libpysal
import geopandas as gpd
from esda.join_counts_local import Join_Counts_Local

fp = libpysal.examples.root + "/guerry/" + "Guerry.shp" 

guerry_ds = gpd.read_file(fp)
guerry_ds['SELECTED'] = 0
guerry_ds.loc[(guerry_ds['Donatns'] > 10997), 'SELECTED'] = 1

w = libpysal.weights.Queen.from_dataframe(guerry_ds)

LJC_uni = Join_Counts_Local(connectivity=w).fit(guerry_ds['SELECTED'])

LJC_uni.p_sim</code></pre>

</div>

``` python
## array([  nan,   nan,   nan,   nan,   nan,   nan,   nan,   nan,   nan,
##          nan,   nan,   nan, 0.435,   nan, 0.025, 0.025,   nan, 0.328,
##          nan,   nan,   nan,   nan,   nan,   nan, 0.342,   nan, 0.334,
##          nan,   nan,   nan,   nan,   nan,   nan, 0.329,   nan,   nan,
##          nan,   nan,   nan,   nan,   nan,   nan,   nan,   nan,   nan,
##          nan,   nan,   nan,   nan,   nan,   nan,   nan,   nan, 0.481,
##          nan,   nan,   nan,   nan,   nan,   nan,   nan,   nan,   nan,
##          nan,   nan,   nan,   nan,   nan,   nan,   nan,   nan,   nan,
##          nan,   nan, 0.02 ,   nan,   nan,   nan,   nan,   nan, 0.125,
##          nan, 0.043,   nan,   nan])
```

[^1]: is a package that lets you call python from R.

