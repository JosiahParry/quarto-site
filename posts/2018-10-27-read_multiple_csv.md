---
title: "Reading Multiple csvs as 1 data frame"
subtitle: "Reading chunked csv files"
date: '2018-10-27'
slug: read-chunked-csv
categories:
  - R
  - tutorial
tags:
  - R 
  - tidyverse
  - tutorial
  - data manipulation
rmd_hash: ce7159ac3e9a2cc6

---

In an earlier [posting](josiahparry.com/post/write-chunked-csv) I wrote about having to break a single csv into multiple csvs. In other scenarios one data set maybe provided as multiple a csvs.

Thankfully `purrr` has a beautiful function called [`map_df()`](https://purrr.tidyverse.org/reference/map.html) which will make this into a two liner. This process has essentially 3 steps.

1.  Create a vector of all `.csv` files that should be merged together.
2.  Read each file using [`readr::read_csv()`](https://readr.tidyverse.org/reference/read_delim.html)
3.  Combine each dataframe into one.

[`map_df()`](https://purrr.tidyverse.org/reference/map.html) maps (applys) a function to each value of an object and produces a dataframe of all outputs.

For this example I will use the csvs I created in a previous tutorial utilizing a dataset from the Quantitative Social Science book.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># Get all csv file names </span>
<span class='nv'>file_names</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/list.files.html'>list.files</a></span><span class='o'>(</span><span class='s'>"../../static/data/chunk_data"</span>, pattern <span class='o'>=</span> <span class='s'>"\\.csv"</span>, full.names <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span>
<span class='nv'>file_names</span>
<span class='c'>#&gt;  [1] "../../static/data/chunk_data/social_chunked_1.csv" </span>
<span class='c'>#&gt;  [2] "../../static/data/chunk_data/social_chunked_10.csv"</span>
<span class='c'>#&gt;  [3] "../../static/data/chunk_data/social_chunked_11.csv"</span>
<span class='c'>#&gt;  [4] "../../static/data/chunk_data/social_chunked_12.csv"</span>
<span class='c'>#&gt;  [5] "../../static/data/chunk_data/social_chunked_13.csv"</span>
<span class='c'>#&gt;  [6] "../../static/data/chunk_data/social_chunked_2.csv" </span>
<span class='c'>#&gt;  [7] "../../static/data/chunk_data/social_chunked_3.csv" </span>
<span class='c'>#&gt;  [8] "../../static/data/chunk_data/social_chunked_4.csv" </span>
<span class='c'>#&gt;  [9] "../../static/data/chunk_data/social_chunked_5.csv" </span>
<span class='c'>#&gt; [10] "../../static/data/chunk_data/social_chunked_6.csv" </span>
<span class='c'>#&gt; [11] "../../static/data/chunk_data/social_chunked_7.csv" </span>
<span class='c'>#&gt; [12] "../../static/data/chunk_data/social_chunked_8.csv" </span>
<span class='c'>#&gt; [13] "../../static/data/chunk_data/social_chunked_9.csv"</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span>
<span class='c'># apply </span>
<span class='nv'>all_csvs</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map_df</a></span><span class='o'>(</span><span class='nv'>file_names</span>, <span class='nv'>read_csv</span><span class='o'>)</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>5866</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Rows: </span><span style='color: #0000BB;'>25000</span> <span style='font-weight: bold;'>Columns: </span><span style='color: #0000BB;'>6</span></span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>──</span> <span style='font-weight: bold;'>Column specification</span> <span style='color: #00BBBB;'>────────────────────────────────────────────────────────────────</span></span>
<span class='c'>#&gt; <span style='font-weight: bold;'>Delimiter:</span> ","</span>
<span class='c'>#&gt; <span style='color: #BB0000;'>chr</span> (2): sex, messages</span>
<span class='c'>#&gt; <span style='color: #00BB00;'>dbl</span> (4): yearofbirth, primary2004, primary2006, hhsize</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Use `spec()` to retrieve the full column specification for this data.</span>
<span class='c'>#&gt; <span style='color: #00BBBB;'>ℹ</span> Specify the column types or set `show_col_types = FALSE` to quiet this message.</span>

<span class='c'># preview the data</span>
<span class='nf'><a href='https://rdrr.io/r/utils/head.html'>head</a></span><span class='o'>(</span><span class='nv'>all_csvs</span><span class='o'>)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 6 × 6</span></span>
<span class='c'>#&gt;   sex    yearofbirth primary2004 messages   primary2006 hhsize</span>
<span class='c'>#&gt;   <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>        <span style='color: #555555; font-style: italic;'>&lt;dbl&gt;</span>       <span style='color: #555555; font-style: italic;'>&lt;dbl&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>            <span style='color: #555555; font-style: italic;'>&lt;dbl&gt;</span>  <span style='color: #555555; font-style: italic;'>&lt;dbl&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span> male          <span style='text-decoration: underline;'>1</span>941           0 Civic Duty           0      2</span>
<span class='c'>#&gt; <span style='color: #555555;'>2</span> female        <span style='text-decoration: underline;'>1</span>947           0 Civic Duty           0      2</span>
<span class='c'>#&gt; <span style='color: #555555;'>3</span> male          <span style='text-decoration: underline;'>1</span>951           0 Hawthorne            1      3</span>
<span class='c'>#&gt; <span style='color: #555555;'>4</span> female        <span style='text-decoration: underline;'>1</span>950           0 Hawthorne            1      3</span>
<span class='c'>#&gt; <span style='color: #555555;'>5</span> female        <span style='text-decoration: underline;'>1</span>982           0 Hawthorne            1      3</span>
<span class='c'>#&gt; <span style='color: #555555;'>6</span> male          <span style='text-decoration: underline;'>1</span>981           0 Control              0      3</span></code></pre>

</div>

