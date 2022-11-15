---
title: Medium Data and Production API Pipeline
subtitle: "Compress your JSON with gzip to increase API speed and reduce memory utilization in R"

date: '2020-09-05'
slug: gzip-api
categories: [production, api]
tags: [production, api, httr]

rmd_hash: 841d4d252d1bd34f

---

"\[P\]arsing huge json strings is difficult and inefficient."[^1] If you have an API that needs to receive a large amount of json, sending that over will be slow.

Q: How can we improve that? A: Compression.

## Background

An API is an application programming interface. APIs are how machines talk to other machines. APIs are useful because they are language agnostic meaning that the same API request from Python, or R, or JavaScript will work and return the same results. To send data to an API we use a POST request. The data that we send is usually required to be in json format.

## Context

**Problem**: With large data API POST requests can become extremely slow and take up a lot of storage space. This can cause a bottleneck.

**Solution**: Compress your data and send a file instead of sending plain text json.

## Standard approach

Interacting with an API from R is usually done with the `{httr}` package. Imagine you want to send a dataframe to an API as json. We can do that by using the [`httr::POST()`](https://httr.r-lib.org/reference/POST.html), providing a dataframe to the body, and encoding it to json by setting `encode = "json"`.

First let's load our libraries:

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://httr.r-lib.org/'>httr</a></span><span class='o'>)</span>          <span class='c'># interacts with apis</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://arxiv.org/abs/1403.2805'>jsonlite</a></span><span class='o'>)</span>      <span class='c'># works with json (for later)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://github.com/hadley/nycflights13'>nycflights13</a></span><span class='o'>)</span>  <span class='c'># data for posting </span></code></pre>

</div>

Next, let's create a sample [`POST()`](https://httr.r-lib.org/reference/POST.html) request to illustrate how posting a dataframe as json works.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'>
<span class='nv'>b_url</span> <span class='o'>&lt;-</span> <span class='s'>"http://httpbin.org/post"</span> <span class='c'># an easy to work with sample API POST endpoint</span>

<span class='nf'><a href='https://httr.r-lib.org/reference/POST.html'>POST</a></span><span class='o'>(</span>url <span class='o'>=</span> <span class='nv'>b_url</span>, 
     body <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>x <span class='o'>=</span> <span class='nv'>cars</span><span class='o'>)</span>,
     encode <span class='o'>=</span> <span class='s'>"json"</span><span class='o'>)</span>
<span class='c'>#&gt; Response [http://httpbin.org/post]</span>
<span class='c'>#&gt;   Date: 2022-11-14 22:14</span>
<span class='c'>#&gt;   Status: 200</span>
<span class='c'>#&gt;   Content-Type: application/json</span>
<span class='c'>#&gt;   Size: 4.81 kB</span>
<span class='c'>#&gt; &#123;</span>
<span class='c'>#&gt;   "args": &#123;&#125;, </span>
<span class='c'>#&gt;   "data": "&#123;\"x\":[&#123;\"speed\":4,\"dist\":2&#125;,&#123;\"speed\":4,\"dist\":10&#125;,&#123;\"speed\":7,\"...</span>
<span class='c'>#&gt;   "files": &#123;&#125;, </span>
<span class='c'>#&gt;   "form": &#123;&#125;, </span>
<span class='c'>#&gt;   "headers": &#123;</span>
<span class='c'>#&gt;     "Accept": "application/json, text/xml, application/xml, */*", </span>
<span class='c'>#&gt;     "Accept-Encoding": "deflate, gzip", </span>
<span class='c'>#&gt;     "Content-Length": "1150", </span>
<span class='c'>#&gt;     "Content-Type": "application/json", </span>
<span class='c'>#&gt; ...</span></code></pre>

</div>

## Alternative approach

An alternative approach would be to write our dataframe as json to a compressed gzip file. The process will be to:

1.  Create a temporary file which will store our compressed json.
2.  Create a gzip file connection to write the temporary file as a gzip.
3.  Upload the temporary file to the API.
4.  Remove the temporary file.

Writing to a temporary gzipped file looks like:

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># create the tempfile </span>
<span class='nv'>tmp</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/tempfile.html'>tempfile</a></span><span class='o'>(</span><span class='o'>)</span>

<span class='c'># create a gzfile connection (to enable writing gz)</span>
<span class='nv'>gz_tmp</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/connections.html'>gzfile</a></span><span class='o'>(</span><span class='nv'>tmp</span><span class='o'>)</span>

<span class='c'># write json to the gz file connection</span>
<span class='nf'><a href='https://rdrr.io/pkg/jsonlite/man/read_json.html'>write_json</a></span><span class='o'>(</span><span class='nv'>cars</span>, <span class='nv'>gz_tmp</span><span class='o'>)</span>

<span class='c'># close the gz file connection</span>
<span class='nf'><a href='https://rdrr.io/r/base/connections.html'>close</a></span><span class='o'>(</span><span class='nv'>gz_tmp</span><span class='o'>)</span></code></pre>

</div>

Let's read the temp file to see what it contains.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># read the temp file for illustration </span>
<span class='nf'><a href='https://rdrr.io/r/base/readLines.html'>readLines</a></span><span class='o'>(</span><span class='nv'>tmp</span><span class='o'>)</span>
<span class='c'>#&gt; [1] "[&#123;\"speed\":4,\"dist\":2&#125;,&#123;\"speed\":4,\"dist\":10&#125;,&#123;\"speed\":7,\"dist\":4&#125;,&#123;\"speed\":7,\"dist\":22&#125;,&#123;\"speed\":8,\"dist\":16&#125;,&#123;\"speed\":9,\"dist\":10&#125;,&#123;\"speed\":10,\"dist\":18&#125;,&#123;\"speed\":10,\"dist\":26&#125;,&#123;\"speed\":10,\"dist\":34&#125;,&#123;\"speed\":11,\"dist\":17&#125;,&#123;\"speed\":11,\"dist\":28&#125;,&#123;\"speed\":12,\"dist\":14&#125;,&#123;\"speed\":12,\"dist\":20&#125;,&#123;\"speed\":12,\"dist\":24&#125;,&#123;\"speed\":12,\"dist\":28&#125;,&#123;\"speed\":13,\"dist\":26&#125;,&#123;\"speed\":13,\"dist\":34&#125;,&#123;\"speed\":13,\"dist\":34&#125;,&#123;\"speed\":13,\"dist\":46&#125;,&#123;\"speed\":14,\"dist\":26&#125;,&#123;\"speed\":14,\"dist\":36&#125;,&#123;\"speed\":14,\"dist\":60&#125;,&#123;\"speed\":14,\"dist\":80&#125;,&#123;\"speed\":15,\"dist\":20&#125;,&#123;\"speed\":15,\"dist\":26&#125;,&#123;\"speed\":15,\"dist\":54&#125;,&#123;\"speed\":16,\"dist\":32&#125;,&#123;\"speed\":16,\"dist\":40&#125;,&#123;\"speed\":17,\"dist\":32&#125;,&#123;\"speed\":17,\"dist\":40&#125;,&#123;\"speed\":17,\"dist\":50&#125;,&#123;\"speed\":18,\"dist\":42&#125;,&#123;\"speed\":18,\"dist\":56&#125;,&#123;\"speed\":18,\"dist\":76&#125;,&#123;\"speed\":18,\"dist\":84&#125;,&#123;\"speed\":19,\"dist\":36&#125;,&#123;\"speed\":19,\"dist\":46&#125;,&#123;\"speed\":19,\"dist\":68&#125;,&#123;\"speed\":20,\"dist\":32&#125;,&#123;\"speed\":20,\"dist\":48&#125;,&#123;\"speed\":20,\"dist\":52&#125;,&#123;\"speed\":20,\"dist\":56&#125;,&#123;\"speed\":20,\"dist\":64&#125;,&#123;\"speed\":22,\"dist\":66&#125;,&#123;\"speed\":23,\"dist\":54&#125;,&#123;\"speed\":24,\"dist\":70&#125;,&#123;\"speed\":24,\"dist\":92&#125;,&#123;\"speed\":24,\"dist\":93&#125;,&#123;\"speed\":24,\"dist\":120&#125;,&#123;\"speed\":25,\"dist\":85&#125;]"</span></code></pre>

</div>

### POSTing a file

To post a file we use the function [`httr::upload_file()`](https://httr.r-lib.org/reference/upload_file.html). The argument we provide is the path, in this case the file path is stored in the `tmp` object.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://httr.r-lib.org/reference/POST.html'>POST</a></span><span class='o'>(</span><span class='nv'>b_url</span>, body <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>x <span class='o'>=</span> <span class='nf'><a href='https://httr.r-lib.org/reference/upload_file.html'>upload_file</a></span><span class='o'>(</span><span class='nv'>tmp</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span>
<span class='c'>#&gt; Response [http://httpbin.org/post]</span>
<span class='c'>#&gt;   Date: 2022-11-14 22:14</span>
<span class='c'>#&gt;   Status: 200</span>
<span class='c'>#&gt;   Content-Type: application/json</span>
<span class='c'>#&gt;   Size: 874 B</span>
<span class='c'>#&gt; &#123;</span>
<span class='c'>#&gt;   "args": &#123;&#125;, </span>
<span class='c'>#&gt;   "data": "", </span>
<span class='c'>#&gt;   "files": &#123;</span>
<span class='c'>#&gt;     "x": "data:text/plain;base64,H4sIAAAAAAAAA4XSPQ6DMAwF4L3HyMyQ+C8JV6m6wdCtEt0q7t6p...</span>
<span class='c'>#&gt;   &#125;, </span>
<span class='c'>#&gt;   "form": &#123;&#125;, </span>
<span class='c'>#&gt;   "headers": &#123;</span>
<span class='c'>#&gt;     "Accept": "application/json, text/xml, application/xml, */*", </span>
<span class='c'>#&gt;     "Accept-Encoding": "deflate, gzip", </span>
<span class='c'>#&gt; ...</span></code></pre>

</div>

### Comparing R object to gzip

Now, you may be asking, is this really that big of a difference? It actually is. If you'll notice from the first response where we POSTed the `cars` dataframe the response size was 4.81kB. This response with the compressed file was only 870B. Thats a whole lot smaller.

We can compare the object size to the file size for another look. The below is in bytes.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/base/cat.html'>cat</a></span><span class='o'>(</span><span class='s'>" cars: "</span>, <span class='nf'><a href='https://rdrr.io/r/utils/object.size.html'>object.size</a></span><span class='o'>(</span><span class='nv'>cars</span><span class='o'>)</span>, <span class='s'>"\n"</span>,
    <span class='s'>"compressed cars: "</span>, <span class='nf'><a href='https://rdrr.io/r/base/file.info.html'>file.size</a></span><span class='o'>(</span><span class='nv'>tmp</span><span class='o'>)</span><span class='o'>)</span>
<span class='c'>#&gt;  cars:  1648 </span>
<span class='c'>#&gt;  compressed cars:  210</span></code></pre>

</div>

## Benchmarking

Let's extend this example to some larger datasets as well as benchmark the results. We'll use data from `nycflights13`. In this example we'll send two dataset to an API as the parameters `metadata` and `data`. Generally metadata is smaller than the data. So for this example we'll send 1,000 rows as the metadata and 10,000 rows as the data. We'll call on the `weather` and `flights` datasets from `nycflights13`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>small_weather</span> <span class='o'>&lt;-</span> <span class='nf'>dplyr</span><span class='nf'>::</span><span class='nf'><a href='https://dplyr.tidyverse.org/reference/sample_n.html'>sample_n</a></span><span class='o'>(</span><span class='nv'>weather</span>, <span class='m'>1000</span><span class='o'>)</span>
<span class='nv'>small_flights</span> <span class='o'>&lt;-</span> <span class='nf'>dplyr</span><span class='nf'>::</span><span class='nf'><a href='https://dplyr.tidyverse.org/reference/sample_n.html'>sample_n</a></span><span class='o'>(</span><span class='nv'>flights</span>, <span class='m'>10000</span><span class='o'>)</span></code></pre>

</div>

### Making it functional

As always, I recommend making your repetitive tasks into functions. Here we will create two functions. One for posting the data as gzip files and the second as pure json. These will be labeled `post_gz()` and `post_json()` respectively.

These functions will take two parameters: `metadata` and `data`.

Define `post_gz()`

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>post_gz</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>metadata</span>, <span class='nv'>data</span><span class='o'>)</span> <span class='o'>&#123;</span>
  
  <span class='c'># write metadata to temp file</span>
  <span class='nv'>tmp_meta</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/tempfile.html'>tempfile</a></span><span class='o'>(</span><span class='s'>"metadata"</span><span class='o'>)</span>
  <span class='nv'>gz_temp_meta</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/connections.html'>gzfile</a></span><span class='o'>(</span><span class='nv'>tmp_meta</span><span class='o'>)</span>
  <span class='nf'><a href='https://rdrr.io/pkg/jsonlite/man/read_json.html'>write_json</a></span><span class='o'>(</span><span class='nv'>metadata</span>, <span class='nv'>gz_temp_meta</span><span class='o'>)</span>
  <span class='nf'><a href='https://rdrr.io/r/base/connections.html'>close</a></span><span class='o'>(</span><span class='nv'>gz_temp_meta</span><span class='o'>)</span>
  
  <span class='c'># write data to temp file</span>
  <span class='nv'>tmp_data</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/tempfile.html'>tempfile</a></span><span class='o'>(</span><span class='s'>"data"</span><span class='o'>)</span>
  <span class='nv'>gz_temp_data</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/connections.html'>gzfile</a></span><span class='o'>(</span><span class='nv'>tmp_data</span><span class='o'>)</span>
  <span class='nf'><a href='https://rdrr.io/pkg/jsonlite/man/read_json.html'>write_json</a></span><span class='o'>(</span><span class='nv'>data</span>, <span class='nv'>gz_temp_data</span><span class='o'>)</span>
  <span class='nf'><a href='https://rdrr.io/r/base/connections.html'>close</a></span><span class='o'>(</span><span class='nv'>gz_temp_data</span><span class='o'>)</span>
  
  <span class='c'># post </span>
  <span class='nv'>q</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://httr.r-lib.org/reference/POST.html'>POST</a></span><span class='o'>(</span><span class='nv'>b_url</span>, 
       body <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>
         metadata <span class='o'>=</span> <span class='nf'><a href='https://httr.r-lib.org/reference/upload_file.html'>upload_file</a></span><span class='o'>(</span><span class='nv'>tmp_meta</span><span class='o'>)</span>,
         data <span class='o'>=</span> <span class='nf'><a href='https://httr.r-lib.org/reference/upload_file.html'>upload_file</a></span><span class='o'>(</span><span class='nv'>tmp_data</span><span class='o'>)</span>
       <span class='o'>)</span><span class='o'>)</span>
  
  <span class='c'># remove temp files</span>
  <span class='nf'><a href='https://rdrr.io/r/base/unlink.html'>unlink</a></span><span class='o'>(</span><span class='nv'>tmp_meta</span><span class='o'>)</span>
  <span class='nf'><a href='https://rdrr.io/r/base/unlink.html'>unlink</a></span><span class='o'>(</span><span class='nv'>tmp_data</span><span class='o'>)</span>
  
  <span class='c'># return a character for purposes of bench marking</span>
  <span class='s'>"Posted..."</span>
<span class='o'>&#125;</span></code></pre>

</div>

Define `post_json()`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>post_json</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>metadata</span>, <span class='nv'>data</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='nv'>q</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://httr.r-lib.org/reference/POST.html'>POST</a></span><span class='o'>(</span><span class='nv'>b_url</span>, 
       body <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>
         metadata <span class='o'>=</span> <span class='nv'>metadata</span>,
         data <span class='o'>=</span> <span class='nv'>data</span>
       <span class='o'>)</span>,
       encode <span class='o'>=</span> <span class='s'>"json"</span><span class='o'>)</span> 
  
  <span class='s'>"Posted..."</span>
<span class='o'>&#125;</span></code></pre>

</div>

Now that these functions have been defined, let's compare their performance using the package `bench`. We'll run each function 50 times to get a good understanding of their respective performance.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>bm</span> <span class='o'>&lt;-</span> <span class='nf'>bench</span><span class='nf'>::</span><span class='nf'><a href='http://bench.r-lib.org/reference/mark.html'>mark</a></span><span class='o'>(</span>
  <span class='nf'>post_gz</span><span class='o'>(</span><span class='nv'>small_weather</span>, <span class='nv'>small_flights</span><span class='o'>)</span>,
  <span class='nf'>post_json</span><span class='o'>(</span><span class='nv'>small_weather</span>, <span class='nv'>small_flights</span><span class='o'>)</span>,
  iterations <span class='o'>=</span> <span class='m'>5</span>
  <span class='o'>)</span>

<span class='nv'>bm</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>ggplot2</span><span class='nf'>::</span><span class='nf'><a href='https://ggplot2.tidyverse.org/reference/autoplot.html'>autoplot</a></span><span class='o'>(</span><span class='nv'>bm</span><span class='o'>)</span></code></pre>

</div>

[^1]: <https://www.opencpu.org/posts/jsonlite-streaming/>

