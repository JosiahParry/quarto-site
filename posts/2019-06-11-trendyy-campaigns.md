---
title: "Google Trends for Campaigns"
subtile: Using search popularity to compare candidates
date: '2019-06-11'
slug: trendyy-4-campaigns
categories: R
tags:
- R
- tutorial
- politics
- youthvote
- campaigns
- trendyy
rmd_hash: 048a0dcf3a0265d9
---

Over the past few years we have seen Google Trends becoming quite ubiquitous in politics. Pundits have used Google seach trends as talking points. It is not uncommon to hear news about a candidates search trends the days following a town hall or significant rally. It seems that Google trends are becoming the go to proxy for a candidate's salience.

As a campaign, you are interested in the popularity of a candidate *relative* to another one. If candidate A has seen a gain from 50 to 70, that is all well and good. But how does that compare with candidates C and D? There are others potential use cases---that may be less fraught with media interruptions. For example, one can keep track of the popularity of possible policy issues---i.e. healthcare, gun safety, women's rights.

Though the usefulness of Google Trends search popularity is still unclear, it may be something that your campaign might like to track. In this chapter we will explore how to acquire and utilize trend data using R. This chapter will describe how one can utilize Google Trends data to compare candidate search popularity and view related search terms. This will be done with the tidyverse, and the package `trendyy` for accessing this data.

## Google Trends Data

### Relative Popularity

The key metric that Google Trends provides is the *relative* popularity of a search term by a given geography. Relative search popularity is scaled from 0 to 100. This number is scaled based on population and geography size (for more information go [here](https://support.google.com/trends/answer/4365533?hl=en)). This number may be useful on it's own, but the strength of Google Trends is it's ability to compare multiple terms. Using Google Trends we can compare up to 5 search terms---presumably candidates.

### Related Queries

In addition to popularity, Google Trends provides you with related queries. This can help your media team understand in what context their candidate is being associated online.

## `trendyy`

Now that we have an intuition of how Google Trends may be utilized, we will look at how actually acquire these data in R. To get started install the package using `install.packages("trendyy")`.

Once the package is installed, load the `tidyverse` and `trendyy`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://github.com/josiahparry/trendyy'>trendyy</a></span><span class='o'>)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span></code></pre>

</div>

In this example we will look at the top five polling candidates as of today (6/10/2019). These are, in no particular order, Joe Biden, Kamala Harris, Beto O'Rourke, Bernie Sanders, and Elizabeth Warren. Create a vector with the search terms that you will use (in this case the above candidates).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>candidates</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='s'>"Joe Biden"</span>, <span class='s'>"Kamala Harris"</span>, <span class='s'>"Beto O'Rourke"</span>, <span class='s'>"Bernie Sanders"</span>, <span class='s'>"Elizabeth Warren"</span><span class='o'>)</span></code></pre>

</div>

Next we will use the `trendyy` package to get search popularity. The function [`trendy()`](https://rdrr.io/pkg/trendyy/man/trendy.html) has three main arguments: `search_terms`, `from`, and `to` (in the form of `"yyyy-mm-dd"`). The first argument is the only mandatory one. Provide a vector of length 5 or less as the first argument. Here we will use the `candidates` vector and look at data from the past two weeks. I will create two variables for the beginning and end dates. This will be to demonstrate how functions can be used to programatically search date ranges.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># to today</span>
<span class='nv'>end</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/Sys.time.html'>Sys.Date</a></span><span class='o'>(</span><span class='o'>)</span>
<span class='c'># from 2 weeks ago</span>
<span class='nv'>begin</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/Sys.time.html'>Sys.Date</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>-</span> <span class='m'>14</span></code></pre>

</div>

Pass these arguments to [`trendy()`](https://rdrr.io/pkg/trendyy/man/trendy.html) and save them to a variable.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>candidate_trends</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/pkg/trendyy/man/trendy.html'>trendy</a></span><span class='o'>(</span>search_terms <span class='o'>=</span> <span class='nv'>candidates</span>, from <span class='o'>=</span> <span class='nv'>begin</span>, to <span class='o'>=</span> <span class='nv'>end</span><span class='o'>)</span>

<span class='nv'>candidate_trends</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>~Trendy results~</span></span>
<span class='c'><span style='font-weight: bold;'>#&gt; </span></span>
<span class='c'>#&gt; Search Terms: Joe Biden, Kamala Harris, Beto O'Rourke, Bernie Sanders, Elizabeth Warren</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; (&gt;^.^)&gt; ~~~~~~~~~~~~~~~~~~~~ summary ~~~~~~~~~~~~~~~~~~~~ &lt;(^.^&lt;)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 5 × 5</span></span>
<span class='c'>#&gt;   keyword          max_hits min_hits from       to        </span>
<span class='c'>#&gt;   <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>               <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;date&gt;</span>     <span style='color: #555555; font-style: italic;'>&lt;date&gt;</span>    </span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span> Bernie Sanders         12        4 2022-10-31 2022-11-10</span>
<span class='c'>#&gt; <span style='color: #555555;'>2</span> Beto O'Rourke           6        1 2022-10-31 2022-11-10</span>
<span class='c'>#&gt; <span style='color: #555555;'>3</span> Elizabeth Warren        3        1 2022-10-31 2022-11-10</span>
<span class='c'>#&gt; <span style='color: #555555;'>4</span> Joe Biden             100       42 2022-10-31 2022-11-10</span>
<span class='c'>#&gt; <span style='color: #555555;'>5</span> Kamala Harris          10        5 2022-10-31 2022-11-10</span></code></pre>

</div>

Trendy creates an object of class `trendy` see `class(candidate_trends)` trendy. There are a number of accessor functions. We will use [`get_interest()`](https://rdrr.io/pkg/trendyy/man/get_interest.html) and [`get_related_queries()`](https://rdrr.io/pkg/trendyy/man/get_related_queries.html). See the documentation of the others.

To access to relative popularity, we will use `get_interest(trendy)`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>popularity</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/pkg/trendyy/man/get_interest.html'>get_interest</a></span><span class='o'>(</span><span class='nv'>candidate_trends</span><span class='o'>)</span>

<span class='nv'>popularity</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 55 × 7</span></span>
<span class='c'>#&gt;    date                 hits keyword   geo   time                  gprop category      </span>
<span class='c'>#&gt;    <span style='color: #555555; font-style: italic;'>&lt;dttm&gt;</span>              <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>     <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>                 <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>         </span>
<span class='c'>#&gt; <span style='color: #555555;'> 1</span> 2022-10-31 <span style='color: #555555;'>00:00:00</span>    48 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 2</span> 2022-11-01 <span style='color: #555555;'>00:00:00</span>    42 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 3</span> 2022-11-02 <span style='color: #555555;'>00:00:00</span>    51 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 4</span> 2022-11-03 <span style='color: #555555;'>00:00:00</span>    52 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 5</span> 2022-11-04 <span style='color: #555555;'>00:00:00</span>    47 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 6</span> 2022-11-05 <span style='color: #555555;'>00:00:00</span>    45 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 7</span> 2022-11-06 <span style='color: #555555;'>00:00:00</span>    46 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 8</span> 2022-11-07 <span style='color: #555555;'>00:00:00</span>    52 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 9</span> 2022-11-08 <span style='color: #555555;'>00:00:00</span>    67 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'>10</span> 2022-11-09 <span style='color: #555555;'>00:00:00</span>   100 Joe Biden world 2022-10-31 2022-11-14 web   All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 45 more rows</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># ℹ Use `print(n = ...)` to see more rows</span></span></code></pre>

</div>

For related queries we will use `get_related_queries(trendy)`. Note that you can either pipe the object or pass it directly.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>candidate_trends</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rdrr.io/pkg/trendyy/man/get_related_queries.html'>get_related_queries</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='c'># picking queries for a random candidate</span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/filter.html'>filter</a></span><span class='o'>(</span><span class='nv'>keyword</span> <span class='o'>==</span> <span class='nf'><a href='https://rdrr.io/r/base/sample.html'>sample</a></span><span class='o'>(</span><span class='nv'>candidates</span>, <span class='m'>1</span><span class='o'>)</span><span class='o'>)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 42 × 5</span></span>
<span class='c'>#&gt;    subject related_queries value                    keyword        category      </span>
<span class='c'>#&gt;    <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>           <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>                    <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>          <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>         </span>
<span class='c'>#&gt; <span style='color: #555555;'> 1</span> 100     top             election                 Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 2</span> 68      top             big mouth bernie sanders Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 3</span> 61      top             biden                    Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 4</span> 56      top             election results         Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 5</span> 47      top             bernie sanders meme      Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 6</span> 37      top             bernie sanders party     Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 7</span> 33      top             bernie sanders age       Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 8</span> 33      top             joe biden                Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'> 9</span> 33      top             trump                    Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'>10</span> 30      top             donald trump             Bernie Sanders All categories</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 32 more rows</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># ℹ Use `print(n = ...)` to see more rows</span></span></code></pre>

</div>

## Visualizing Trends

I'm guessing your director enjoys charts---so do I. To make the data more accessible, use the `popularity` tibble to create a time series chart of popularity over the past two weeks. We will use `ggplot2`. Remember that time should be displayed on the x axis. We want to have a line for each candidate, so map the color aesthetic to the keyword.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggplot.html'>ggplot</a></span><span class='o'>(</span><span class='nv'>popularity</span>, 
       <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/aes.html'>aes</a></span><span class='o'>(</span>x <span class='o'>=</span> <span class='nv'>date</span>, y <span class='o'>=</span> <span class='nv'>hits</span>, color <span class='o'>=</span> <span class='nv'>keyword</span><span class='o'>)</span><span class='o'>)</span> <span class='o'>+</span> 
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/geom_path.html'>geom_line</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/labs.html'>labs</a></span><span class='o'>(</span>x <span class='o'>=</span> <span class='s'>""</span>, y <span class='o'>=</span> <span class='s'>"Search Popularity"</span>, 
       title <span class='o'>=</span> <span class='s'>"Google popularity of top 5 polling candidates"</span><span class='o'>)</span> <span class='o'>+</span> 
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggtheme.html'>theme_minimal</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/theme.html'>theme</a></span><span class='o'>(</span>legend.position <span class='o'>=</span> <span class='s'>"bottom"</span>, 
        legend.title <span class='o'>=</span> <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/element.html'>element_blank</a></span><span class='o'>(</span><span class='o'>)</span><span class='o'>)</span>
</code></pre>
<img src="figs/unnamed-chunk-7-1.png" width="700px" style="display: block; margin: auto;" />

</div>

