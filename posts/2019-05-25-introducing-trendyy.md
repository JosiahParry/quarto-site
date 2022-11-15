---
title: "Introducing trendyy"
subtitle: "A tidy wrapper for gtrendsR"
date: "2019-05-25"
categories:
  - R
tags:
  - trendyy
  - tutorial
rmd_hash: 0de72e1e68eeceec

---

`trendyy` is a package for querying Google Trends. It is build around [Philippe Massicotte](https://github.com/PMassicotte)'s package [`gtrendsR`](https://github.com/PMassicotte/gtrendsR) which accesses this data wonderfully.

The inspiration for this package was to provide a tidy interface to the trends data.

## Getting Started

### Installation

You can install `trendyy` from CRAN using `install.packages("trendyy")`.

### Usage

Use [`trendy()`](https://rdrr.io/pkg/trendyy/man/trendy.html) to search Google Trends. The only mandatory argument is `search_terms`. This is a character vector with the terms of interest. It is important to note that Google Trends is only capable of comparing up to five terms. Thus, if your `search_terms` vector is longer than 5, it will search each term individually. This will remove the direct comparative advantage that Google Trends gives you.

#### Additional arguments

![](https://imgs.xkcd.com/comics/iso_8601_2x.png)

-   `from`: The beginning date of the query in `"YYYY-MM-DD"` format.
-   `to`: The end date of the query in `"YYYY-MM-DD"` format.
-   `...` : any additional arguments that would be passed to [`gtrendsR::gtrends()`](https://rdrr.io/pkg/gtrendsR/man/gtrends.html). Note that it might be useful to indicate the geography of interest. See [`gtrendsR::countries`](https://rdrr.io/pkg/gtrendsR/man/countries.html) for list of possible geographies.

#### Accessor Functions

-   [`get_interest()`](https://rdrr.io/pkg/trendyy/man/get_interest.html): Retrieve interest over time
-   [`get_interest_city()`](https://rdrr.io/pkg/trendyy/man/get_interest_city.html): Retrieve interest by city
-   [`get_interest_country()`](https://rdrr.io/pkg/trendyy/man/get_interest_country.html): Retrieve interest by country
-   [`get_interest_dma()`](https://rdrr.io/pkg/trendyy/man/get_interest_dma.html): Retrieve interest by DMA
-   [`get_interest_region()`](https://rdrr.io/pkg/trendyy/man/get_interest_region.html): Retrieve interest by region
-   [`get_related_queries()`](https://rdrr.io/pkg/trendyy/man/get_related_queries.html): Retrieve related queries
-   [`get_related_topics()`](https://rdrr.io/pkg/trendyy/man/get_related_topics.html): Retrieve related topics

## Example

Seeing as I found an interest in this due to the relatively pervasive use of Google Trends in political analysis, I will compare the top five polling candidates in the 2020 Democratic Primary. As of May 22nd, they were Joe Biden, Kamala Harris, Beto O'Rourke, Bernie Sanders, and Elizabeth Warren.

First, I will create a vector of my desired search terms. Second, I will pass that vector to [`trendy()`](https://rdrr.io/pkg/trendyy/man/trendy.html) specifying my query date range from the first of 2019 until today (May 25th, 2019).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>candidates</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='s'>"Joe Biden"</span>, <span class='s'>"Kamala Harris"</span>, <span class='s'>"Beto O'Rourke"</span>, <span class='s'>"Bernie Sanders"</span>, <span class='s'>"Elizabeth Warren"</span><span class='o'>)</span>

<span class='nv'>candidate_trends</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/pkg/trendyy/man/trendy.html'>trendy</a></span><span class='o'>(</span><span class='nv'>candidates</span>, from <span class='o'>=</span> <span class='s'>"2019-01-01"</span>, to <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/Sys.time.html'>Sys.Date</a></span><span class='o'>(</span><span class='o'>)</span><span class='o'>)</span></code></pre>

</div>

Now that we have a trendy object, we can print it out to get a summary of the trends.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>candidate_trends</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>~Trendy results~</span></span>
<span class='c'><span style='font-weight: bold;'>#&gt; </span></span>
<span class='c'>#&gt; Search Terms: Joe Biden, Kamala Harris, Beto O'Rourke, Bernie Sanders, Elizabeth Warren</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; (&gt;^.^)&gt; ~~~~~~~~~~~~~~~~~~~~ summary ~~~~~~~~~~~~~~~~~~~~ &lt;(^.^&lt;)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 5 × 5</span></span>
<span class='c'>#&gt;   keyword          max_hits min_hits from       to        </span>
<span class='c'>#&gt;   <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>               <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;date&gt;</span>     <span style='color: #555555; font-style: italic;'>&lt;date&gt;</span>    </span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span> Bernie Sanders         21        1 2019-01-06 2022-11-06</span>
<span class='c'>#&gt; <span style='color: #555555;'>2</span> Beto O'Rourke           1        0 2019-01-06 2022-11-06</span>
<span class='c'>#&gt; <span style='color: #555555;'>3</span> Elizabeth Warren        8        1 2019-01-06 2022-11-06</span>
<span class='c'>#&gt; <span style='color: #555555;'>4</span> Joe Biden             100        1 2019-01-06 2022-11-06</span>
<span class='c'>#&gt; <span style='color: #555555;'>5</span> Kamala Harris          48        1 2019-01-06 2022-11-06</span></code></pre>

</div>

In order to retrieve the trend data, use [`get_interest()`](https://rdrr.io/pkg/trendyy/man/get_interest.html). Note, that this is dplyr friendly.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/pkg/trendyy/man/get_interest.html'>get_interest</a></span><span class='o'>(</span><span class='nv'>candidate_trends</span><span class='o'>)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 1,005 × 7</span></span>
<span class='c'>#&gt;    date                 hits keyword   geo   time                  gprop category      </span>
<span class='c'>#&gt;    <span style='color: #555555; font-style: italic;'>&lt;dttm&gt;</span>              <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>     <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>                 <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>         </span>
<span class='c'>#&gt; <span style='color: #555555;'> 1</span> 2019-01-06 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 2</span> 2019-01-13 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 3</span> 2019-01-20 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 4</span> 2019-01-27 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 5</span> 2019-02-03 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 6</span> 2019-02-10 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 7</span> 2019-02-17 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 8</span> 2019-02-24 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 9</span> 2019-03-03 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'>10</span> 2019-03-10 <span style='color: #555555;'>00:00:00</span>     1 Joe Biden world 2019-01-01 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 995 more rows</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># ℹ Use `print(n = ...)` to see more rows</span></span></code></pre>

</div>

### Plotting Interest

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>candidate_trends</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rdrr.io/pkg/trendyy/man/get_interest.html'>get_interest</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggplot.html'>ggplot</a></span><span class='o'>(</span><span class='nf'><a href='https://ggplot2.tidyverse.org/reference/aes.html'>aes</a></span><span class='o'>(</span><span class='nv'>date</span>, <span class='nv'>hits</span>, color <span class='o'>=</span> <span class='nv'>keyword</span><span class='o'>)</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/geom_path.html'>geom_line</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/geom_point.html'>geom_point</a></span><span class='o'>(</span>alpha <span class='o'>=</span> <span class='m'>.2</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggtheme.html'>theme_minimal</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/theme.html'>theme</a></span><span class='o'>(</span>legend.position <span class='o'>=</span> <span class='s'>"bottom"</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/labs.html'>labs</a></span><span class='o'>(</span>x <span class='o'>=</span> <span class='s'>""</span>, 
       y <span class='o'>=</span> <span class='s'>"Relative Search Popularity"</span>,
       title <span class='o'>=</span> <span class='s'>"Google Search Popularity"</span><span class='o'>)</span>
</code></pre>
<img src="figs/unnamed-chunk-4-1.png" width="700px" style="display: block; margin: auto;" />

</div>

It is also possible to view the related search queries for a given set of keywords using [`get_related_queries()`](https://rdrr.io/pkg/trendyy/man/get_related_queries.html).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>candidate_trends</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rdrr.io/pkg/trendyy/man/get_related_queries.html'>get_related_queries</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/group_by.html'>group_by</a></span><span class='o'>(</span><span class='nv'>keyword</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/sample_n.html'>sample_n</a></span><span class='o'>(</span><span class='m'>2</span><span class='o'>)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 10 × 5</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># Groups:   keyword [5]</span></span>
<span class='c'>#&gt;    subject  related_queries value                        keyword          category      </span>
<span class='c'>#&gt;    <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>           <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>                        <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>            <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>         </span>
<span class='c'>#&gt; <span style='color: #555555;'> 1</span> +3,450%  rising          klobuchar                    Bernie Sanders   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 2</span> 81       top             joe biden                    Bernie Sanders   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 3</span> 32       top             kamala harris                Beto ORourke     All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 4</span> Breakout rising          beto orourke announcement    Beto ORourke     All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 5</span> Breakout rising          elizabeth warren beer video  Elizabeth Warren All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 6</span> 40       top             elizabeth warren net worth   Elizabeth Warren All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 7</span> Breakout rising          joe biden stimulus           Joe Biden        All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 8</span> Breakout rising          joe biden senile             Joe Biden        All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 9</span> Breakout rising          kamala harris husbands       Kamala Harris    All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'>10</span> 30       top             vice president kamala harris Kamala Harris    All categories</span></code></pre>

</div>

### Useful Resources

-   [How Trends Data Is Adjusted](https://support.google.com/trends/answer/4365533?hl=en)
-   [Post by Google News Lab](https://medium.com/google-news-lab/what-is-google-trends-data-and-what-does-it-mean-b48f07342ee8)

