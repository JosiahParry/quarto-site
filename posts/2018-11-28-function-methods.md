---
title: "[Not so] generic functions"
subtitle: "A simple explanation because I struggled to understand any documentation"
slug: function-methods
date: '2018-11-28'
output:
  blogdown::html_page:
    toc: true
categories:
  - R
  - tutorial
tags:
  - R
  - tutorial
  - packages
  - functions
  - methods
  - generics
  - s3
rmd_hash: e6a8a96c09cdebb7

---

Lately I have been doing more of my spatial analysis work in R with the help of the `sf` package. One shapefile I was working with had some horrendously named columns, and naturally, I tried to clean them using the [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) function from the `janitor` package. But lo, an egregious error occurred. To this end, I officially filed my complaint as an [issue](https://github.com/sfirke/janitor/pull/249). The solution presented was to *simply* create a method for `sf` objects.

Yeah, methods, how tough can those be? Apparently the process isn't at all difficult. But figuring out the process? That was difficult. This post will explain how I went about the process for converting the [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) function into a *generic* (I'll explain this in a second), and creating a *method* for `sf` and `tbl_graph` objects.

### The Jargon

Okay, I want to address the jargon. What the hell is a *generic* function, and what is a *method*? But first, I want to give a quick tl;dr on what a function is. I define as function as bit of code that takes an input, changes it in some way, and produces an output. Even simpler, a function takes an input and creates an output.

#### Generic Functions

Now, what is a ***generic*** function? My favorite definition that I've seen so far comes from *LispWorks Ltd* (their website is a historic landmark, I recommend you give it a look for a reminder of what the internet used to be). They define a [generic function](http://clhs.lisp.se/Body/07_fa.htm) as

> a function whose behavior depends on the classes or identities of the arguments supplied to it.

This means that we have to create a function that looks at the class of an object and perform an operation based on the object class. That means if there is `"numeric"` or `"list"` object, they will be treated differently. These are called `methods`. Note: you can find the class of an object by using the [`class()`](https://rdrr.io/r/base/class.html) function on any object.

#### Methods

To steal from *LispWorks Ltd* again, a [method](http://clhs.lisp.se/Body/26_glo_m.htm#method) is

> part of a generic function which provides information about how that generic function should behave \[for\] certain classes.

This means that a method is part of a generic function and has to be defined separately. Imagine we have a generic function called `f` with methods for `list` and `numeric` objects. The way that we would denote these methods is by putting a period after the function name and indicating the type of object the function is to be used on. These would look like `f.list` and `f.numeric` respectively.

But to save time you can always create a `default` method which will be dispatched (used) on any object that it hasn't been explicitly told how to operate on (by a specific method).

Now that the intuition of what generic functions and methods R, we can begin the work of actually creating them. This tutorial will walk through the steps I took in changing the [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) from a standard function into a generic function with methods for `sf` objects and `tbl_graph` objects from the [`sf`](https://github.com/r-spatial/sf) and [`tidygraph`](https://github.com/thomasp85/tidygraph) packages respectively.

A brief overview of the process:

1.  Define the generic function
2.  Create a default method
3.  Create additional methods

A quick note: The code that follows is not identical to that of the package. I will be changing it up to make it simpler to read and understand what is happening.

### The Generic Method

The first step, as described above, is to create a generic function. Generic functions are made by creating a new function with the body containing only a call to the [`UseMethod()`](https://rdrr.io/r/base/UseMethod.html) function. The only argument to this is the name of your generic function---this should be the same as the name of the function you are making. This tells R that you are creating a generic function. Additionally, you should add any arguments that will be necessary for your function. Here, there are two arguments: `dat` and `case`. These indicate the data to be cleaned and the preferred style for them to be cleaned according to.

I am not setting any default values for `dat` to make it required, whereas I am setting `case` to `"snake"`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>clean_names</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>dat</span>, <span class='nv'>case</span> <span class='o'>=</span> <span class='s'>"snake"</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='kr'><a href='https://rdrr.io/r/base/UseMethod.html'>UseMethod</a></span><span class='o'>(</span><span class='s'>"clean_names"</span><span class='o'>)</span>
<span class='o'>&#125;</span></code></pre>

</div>

Now we have created a generic function. But this function doesn't know how to run on any given object types. In other words, there are no methods associated with it. To illustrate this try using the [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) function we just defined on objects of different types.

    clean_names(1) # numeric 
    clean_names("test") # character 
    clean_names(TRUE) # logical 

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'>#&gt; [1] "no applicable method for 'clean_names' applied to an object of class \"c('double', 'numeric')\""</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'>#&gt; [1] "no applicable method for 'clean_names' applied to an object of class \"character\""</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'>#&gt; [1] "no applicable method for 'clean_names' applied to an object of class \"logical\""</span></code></pre>

</div>

The output of these calls say `no applicable method for 'x' applied to an object of [class]`. In order to prevent this from happening, we can create a default method. A default method will always be used if the function doesn't have a method for the provided object type.

### The Default Method

Remember that methods are indicated by writing `function.method`. It is also important to note that the `method` should indicate an object class. To figure out what class an object is you can use the [`class()`](https://rdrr.io/r/base/class.html) function. For example `class(1)` tells you that the number `1` is "numeric".

In this next step I want to create a default method that will be used on every object that there isn't a method explicitly for. To do this I will create a function called `clean_names.default`.

As background, the [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) function takes a data frame and changes column headers to fit a given style. [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) in the [development version](https://github.com/sfirke/janitor) is based on the function [`make_clean_names()`](https://rdrr.io/pkg/janitor/man/make_clean_names.html) which takes a character vector and makes each value match a given style (the default is snake, and you should only use snake case because everything else is wrong \* sarcasm \* ).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://github.com/sfirke/janitor'>janitor</a></span><span class='o'>)</span></code></pre>

</div>

Now let's see how this function works. For this we will use the ugliest character vector I have ever seen from the [tests](https://github.com/sfirke/janitor/blob/master/tests/testthat/test-clean-names.R) for [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) (h/t @sfirke for making this).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>ugly_names</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span>
  <span class='s'>"sp ace"</span>, <span class='s'>"repeated"</span>, <span class='s'>"a**^@"</span>, <span class='s'>"%"</span>, <span class='s'>"*"</span>, <span class='s'>"!"</span>,
  <span class='s'>"d(!)9"</span>, <span class='s'>"REPEATED"</span>, <span class='s'>"can\"'t"</span>, <span class='s'>"hi_`there`"</span>, <span class='s'>"  leading spaces"</span>,
  <span class='s'>"€"</span>, <span class='s'>"ação"</span>, <span class='s'>"Farœ"</span>, <span class='s'>"a b c d e f"</span>, <span class='s'>"testCamelCase"</span>, <span class='s'>"!leadingpunct"</span>,
  <span class='s'>"average # of days"</span>, <span class='s'>"jan2009sales"</span>, <span class='s'>"jan 2009 sales"</span>
<span class='o'>)</span>

<span class='nv'>ugly_names</span>
<span class='c'>#&gt;  [1] "sp ace"            "repeated"          "a**^@"             "%"                </span>
<span class='c'>#&gt;  [5] "*"                 "!"                 "d(!)9"             "REPEATED"         </span>
<span class='c'>#&gt;  [9] "can\"'t"           "hi_`there`"        "  leading spaces"  "€"                </span>
<span class='c'>#&gt; [13] "ação"              "Farœ"              "a b c d e f"       "testCamelCase"    </span>
<span class='c'>#&gt; [17] "!leadingpunct"     "average # of days" "jan2009sales"      "jan 2009 sales"</span></code></pre>

</div>

Now to see how this function works:

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/pkg/janitor/man/make_clean_names.html'>make_clean_names</a></span><span class='o'>(</span><span class='nv'>ugly_names</span><span class='o'>)</span>
<span class='c'>#&gt;  [1] "sp_ace"                 "repeated"               "a"                     </span>
<span class='c'>#&gt;  [4] "percent"                "x"                      "x_2"                   </span>
<span class='c'>#&gt;  [7] "d_9"                    "repeated_2"             "cant"                  </span>
<span class='c'>#&gt; [10] "hi_there"               "leading_spaces"         "x_3"                   </span>
<span class='c'>#&gt; [13] "acao"                   "faroe"                  "a_b_c_d_e_f"           </span>
<span class='c'>#&gt; [16] "test_camel_case"        "leadingpunct"           "average_number_of_days"</span>
<span class='c'>#&gt; [19] "jan2009sales"           "jan_2009_sales"</span></code></pre>

</div>

Très magnifique!

The body of the default method will take column names from a dataframe, clean them, and reassign them. Before we can do this, a dataframe is needed!

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># create a data frame with 20 columns</span>
<span class='nv'>test_df</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://tibble.tidyverse.org/reference/as_tibble.html'>as_tibble</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/matrix.html'>matrix</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/sample.html'>sample</a></span><span class='o'>(</span><span class='m'>100</span>, <span class='m'>20</span><span class='o'>)</span>, ncol <span class='o'>=</span> <span class='m'>20</span><span class='o'>)</span><span class='o'>)</span>

<span class='c'># makes the column names the `ugly_names` vector</span>
<span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nv'>test_df</span><span class='o'>)</span> <span class='o'>&lt;-</span> <span class='nv'>ugly_names</span>

<span class='c'># print the data frame.</span>
<span class='nv'>test_df</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 1 × 20</span></span>
<span class='c'>#&gt;   `sp ace` repeated `a**^@`   `%`   `*`   `!` `d(!)9` REPEATED `can"'t` hi_\th…¹   lea…²</span>
<span class='c'>#&gt;      <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span>       78       18      99    28    26    38      43       73       51       33      23</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 9 more variables: `€` &lt;int&gt;, ação &lt;int&gt;, Farœ &lt;int&gt;, `a b c d e f` &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   testCamelCase &lt;int&gt;, `!leadingpunct` &lt;int&gt;, `average # of days` &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   jan2009sales &lt;int&gt;, `jan 2009 sales` &lt;int&gt;, and abbreviated variable names</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   ¹​`hi_\`there\``, ²​`  leading spaces`</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># ℹ Use `colnames()` to see all variable names</span></span></code></pre>

</div>

The process for writing this function is:

-   take a dataframe
-   take the old column names and clean them
-   reassign the column names as the new clean names
-   return the object

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>clean_names.default</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>dat</span>, <span class='nv'>case</span> <span class='o'>=</span> <span class='s'>"snake"</span><span class='o'>)</span> <span class='o'>&#123;</span> 
  <span class='c'># retrieve the old names</span>
  <span class='nv'>old_names</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nv'>dat</span><span class='o'>)</span>
  <span class='c'># clean the old names</span>
  <span class='nv'>new_names</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/pkg/janitor/man/make_clean_names.html'>make_clean_names</a></span><span class='o'>(</span><span class='nv'>old_names</span>, case <span class='o'>=</span> <span class='nv'>case</span><span class='o'>)</span>
  <span class='c'># assign the column names as the clean names vector</span>
  <span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nv'>dat</span><span class='o'>)</span> <span class='o'>&lt;-</span> <span class='nv'>new_names</span>
  <span class='c'># return the data</span>
  <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nv'>dat</span><span class='o'>)</span>
  <span class='o'>&#125;</span></code></pre>

</div>

Now that the default method has been defined. Try running the function on our test dataframe!

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/pkg/janitor/man/clean_names.html'>clean_names</a></span><span class='o'>(</span><span class='nv'>test_df</span><span class='o'>)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 1 × 20</span></span>
<span class='c'>#&gt;   sp_ace repeated     a percent     x   x_2   d_9 repeated_2  cant hi_th…¹ leadi…²   x_3</span>
<span class='c'>#&gt;    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>      <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span>     78       18    99      28    26    38    43         73    51      33      23    27</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 8 more variables: acao &lt;int&gt;, faroe &lt;int&gt;, a_b_c_d_e_f &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   test_camel_case &lt;int&gt;, leadingpunct &lt;int&gt;, average_number_of_days &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   jan2009sales &lt;int&gt;, jan_2009_sales &lt;int&gt;, and abbreviated variable names ¹​hi_there,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   ²​leading_spaces</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># ℹ Use `colnames()` to see all variable names</span></span></code></pre>

</div>

Oh, my gorsh. Look at that! We can try replicating this with a named vector to see how the default method dispatched on unknown objects!

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># create a vector with 20 elements</span>
<span class='nv'>test_vect</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='m'>1</span><span class='o'>:</span><span class='m'>20</span><span class='o'>)</span>

<span class='c'># name each element with the ugly_names vector </span>
<span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nv'>test_vect</span><span class='o'>)</span> <span class='o'>&lt;-</span> <span class='nv'>ugly_names</span>

<span class='c'># try cleaning!</span>
<span class='nf'><a href='https://rdrr.io/pkg/janitor/man/clean_names.html'>clean_names</a></span><span class='o'>(</span><span class='nv'>test_vect</span><span class='o'>)</span>
<span class='c'>#&gt;                 sp_ace               repeated                      a </span>
<span class='c'>#&gt;                      1                      2                      3 </span>
<span class='c'>#&gt;                percent                      x                    x_2 </span>
<span class='c'>#&gt;                      4                      5                      6 </span>
<span class='c'>#&gt;                    d_9             repeated_2                   cant </span>
<span class='c'>#&gt;                      7                      8                      9 </span>
<span class='c'>#&gt;               hi_there         leading_spaces                    x_3 </span>
<span class='c'>#&gt;                     10                     11                     12 </span>
<span class='c'>#&gt;                   acao                  faroe            a_b_c_d_e_f </span>
<span class='c'>#&gt;                     13                     14                     15 </span>
<span class='c'>#&gt;        test_camel_case           leadingpunct average_number_of_days </span>
<span class='c'>#&gt;                     16                     17                     18 </span>
<span class='c'>#&gt;           jan2009sales         jan_2009_sales </span>
<span class='c'>#&gt;                     19                     20</span></code></pre>

</div>

It looks like this default function works super well with named objects! Now, we will broach the problem I started with, `sf` objects.

### `sf` method

This section will go over the process for creating the `sf` method. If you have not ever used the `sf` package, I suggest you give it a try! It makes dataframe objects with spatial data associated with it. This allows you to perform many of the functions from the `tidyverse` to spatial data.

Before getting into it, I want to create a test object to work with. I will take the `test_df` column, create longitude and latitude columns, and then convert it into an `sf` object. The details of `sf` objects is out of the scope of this post.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://r-spatial.github.io/sf/'>sf</a></span><span class='o'>)</span>

<span class='nv'>test_sf</span> <span class='o'>&lt;-</span> <span class='nv'>test_df</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
  <span class='c'># create xy columns</span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate.html'>mutate</a></span><span class='o'>(</span>long <span class='o'>=</span> <span class='o'>-</span><span class='m'>80</span>, 
         lat <span class='o'>=</span> <span class='m'>40</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='c'># convert to sf object </span>
  <span class='nf'><a href='https://r-spatial.github.io/sf/reference/st_as_sf.html'>st_as_sf</a></span><span class='o'>(</span>coords <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='s'>"long"</span>, <span class='s'>"lat"</span><span class='o'>)</span><span class='o'>)</span>

<span class='c'># converting geometry column name to poor style</span>
<span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nv'>test_sf</span><span class='o'>)</span><span class='o'>[</span><span class='m'>21</span><span class='o'>]</span> <span class='o'>&lt;-</span> <span class='s'>"Geometry"</span>

<span class='c'># telling sf which column is now the geometry</span>
<span class='nf'><a href='https://r-spatial.github.io/sf/reference/st_geometry.html'>st_geometry</a></span><span class='o'>(</span><span class='nv'>test_sf</span><span class='o'>)</span> <span class='o'>&lt;-</span> <span class='s'>"Geometry"</span>

<span class='nv'>test_sf</span>
<span class='c'>#&gt; Simple feature collection with 1 feature and 20 fields</span>
<span class='c'>#&gt; Geometry type: POINT</span>
<span class='c'>#&gt; Dimension:     XY</span>
<span class='c'>#&gt; Bounding box:  xmin: -80 ymin: 40 xmax: -80 ymax: 40</span>
<span class='c'>#&gt; CRS:           NA</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 1 × 21</span></span>
<span class='c'>#&gt;   `sp ace` repeated `a**^@`   `%`   `*`   `!` `d(!)9` REPEATED `can"'t` hi_\th…¹   lea…²</span>
<span class='c'>#&gt;      <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span>       78       18      99    28    26    38      43       73       51       33      23</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 10 more variables: `€` &lt;int&gt;, ação &lt;int&gt;, Farœ &lt;int&gt;, `a b c d e f` &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   testCamelCase &lt;int&gt;, `!leadingpunct` &lt;int&gt;, `average # of days` &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   jan2009sales &lt;int&gt;, `jan 2009 sales` &lt;int&gt;, Geometry &lt;POINT&gt;, and abbreviated</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   variable names ¹​`hi_\`there\``, ²​`  leading spaces`</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># ℹ Use `colnames()` to see all variable names</span></span></code></pre>

</div>

The `sf` object has been created. But now how does our default method of the [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) function work on this object? There is only one way to know, try it.

    clean_names(test_sf)

    Error in st_geometry.sf(x) : attr(obj, "sf_column") does not point to a geometry column. Did you rename it, without setting st_geometry(obj) <- "newname"?

Notice how it fails. `sf` noticed that I changed the name of the geometry column without explicitly telling it I did so. Since the geometry column is *almost* always the last column of an sf object, we can use the [`make_clean_names()`](https://rdrr.io/pkg/janitor/man/make_clean_names.html) function on every column but the last one! To do this we will use the [`rename_at()`](https://dplyr.tidyverse.org/reference/select_all.html) function from `dplyr`. This function allows you rename columns based on their name or position, and a function that renames it (in this case, [`make_clean_names()`](https://rdrr.io/pkg/janitor/man/make_clean_names.html)).

For this example dataset, say I wanted to clean the first column. How would I do that? Note that the first column is called `sp ace`. How this works can be seen in a simple example. In the below function call we are using the [`rename_at()`](https://dplyr.tidyverse.org/reference/select_all.html) function (for more, go [here](https://dplyr.tidyverse.org/reference/select.html)), selecting the first column name, and renaming it using the [`make_clean_names()`](https://rdrr.io/pkg/janitor/man/make_clean_names.html) function.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://dplyr.tidyverse.org/reference/select_all.html'>rename_at</a></span><span class='o'>(</span><span class='nv'>test_df</span>, .vars <span class='o'>=</span> <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/vars.html'>vars</a></span><span class='o'>(</span><span class='m'>1</span><span class='o'>)</span>, .funs <span class='o'>=</span> <span class='nv'>make_clean_names</span><span class='o'>)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 1 × 20</span></span>
<span class='c'>#&gt;   sp_ace repea…¹ `a**^@`   `%`   `*`   `!` `d(!)9` REPEA…² can"'…³ hi_\t…⁴   lea…⁵   `€`</span>
<span class='c'>#&gt;    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span>     78      18      99    28    26    38      43      73      51      33      23    27</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 8 more variables: ação &lt;int&gt;, Farœ &lt;int&gt;, `a b c d e f` &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   testCamelCase &lt;int&gt;, `!leadingpunct` &lt;int&gt;, `average # of days` &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   jan2009sales &lt;int&gt;, `jan 2009 sales` &lt;int&gt;, and abbreviated variable names</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   ¹​repeated, ²​REPEATED, ³​`can"'t`, ⁴​`hi_\`there\``, ⁵​`  leading spaces`</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># ℹ Use `colnames()` to see all variable names</span></span></code></pre>

</div>

Notice how only the first column has been cleaned. It went from `sp ace` to `sp_ace`. The goal is to replicate this for all columns *except* the last one.

To write the sf method, the above line of code can be adapted to select columns 1 through the number of columns *minus* 1 (so geometry isn't selected). In order to make this work, we need to identify the second to last column---this will be supplied as the ending value of our selected variables.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>clean_names.sf</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>dat</span>, <span class='nv'>case</span> <span class='o'>=</span> <span class='s'>"snake"</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='c'># identify last column that is not geometry</span>
  <span class='nv'>last_col_to_clean</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/nrow.html'>ncol</a></span><span class='o'>(</span><span class='nv'>dat</span><span class='o'>)</span> <span class='o'>-</span> <span class='m'>1</span>
  <span class='c'># create a new dat object</span>
  <span class='nv'>dat</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select_all.html'>rename_at</a></span><span class='o'>(</span><span class='nv'>dat</span>, 
                   <span class='c'># rename the first up until the second to last</span>
                   .vars <span class='o'>=</span> <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/vars.html'>vars</a></span><span class='o'>(</span><span class='m'>1</span><span class='o'>:</span><span class='nv'>last_col_to_clean</span><span class='o'>)</span>, 
                   <span class='c'># clean using the make_clean_names</span>
                   .funs <span class='o'>=</span> <span class='nv'>make_clean_names</span><span class='o'>)</span>
  <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nv'>dat</span><span class='o'>)</span>
<span class='o'>&#125;</span></code></pre>

</div>

Voilà! Our first non-default method has been created. This means that when an `sf` object is supplied to our generic function [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) it looks at the class of the object---`class(sf_object)`---notices it's an `sf` object, then dispatches (uses) the [`clean_names.sf()`](https://rdrr.io/pkg/janitor/man/clean_names.html) method instead of the default.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/pkg/janitor/man/clean_names.html'>clean_names</a></span><span class='o'>(</span><span class='nv'>test_sf</span><span class='o'>)</span>
<span class='c'>#&gt; Simple feature collection with 1 feature and 20 fields</span>
<span class='c'>#&gt; Geometry type: POINT</span>
<span class='c'>#&gt; Dimension:     XY</span>
<span class='c'>#&gt; Bounding box:  xmin: -80 ymin: 40 xmax: -80 ymax: 40</span>
<span class='c'>#&gt; CRS:           NA</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 1 × 21</span></span>
<span class='c'>#&gt;   sp_ace repeated     a percent     x   x_2   d_9 repeated_2  cant hi_th…¹ leadi…²   x_3</span>
<span class='c'>#&gt;    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>      <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span>     78       18    99      28    26    38    43         73    51      33      23    27</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 9 more variables: acao &lt;int&gt;, faroe &lt;int&gt;, a_b_c_d_e_f &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   test_camel_case &lt;int&gt;, leadingpunct &lt;int&gt;, average_number_of_days &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   jan2009sales &lt;int&gt;, jan_2009_sales &lt;int&gt;, Geometry &lt;POINT&gt;, and abbreviated</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   variable names ¹​hi_there, ²​leading_spaces</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># ℹ Use `colnames()` to see all variable names</span></span></code></pre>

</div>

Here we see that it worked exactly as we hoped. Every column but the last has been altered. This allows `sf` to name it's geometry columns whatever it would like without disrupting it.

Shortly after this addition was added to the package I became aware of another type of object that had problems using [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html). This is the `tbl_graph` object from the `tidygraph` package from Thomas Lin Pederson.

### `tbl_graph` method

In issue [#252](https://github.com/sfirke/janitor/issues/252) [@gvdr](https://github.com/gvdr) noted that calling [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) on a `tbl_graph` doesn't execute. Thankfully [@Tazinho](https://github.com/Tazinho) noted that you could easily clean the column headers by using the [`rename_all()`](https://dplyr.tidyverse.org/reference/select_all.html) function from `dplyr`.

Here the solution was even easier than above. As a reminder, in order to make the `tbl_graph` method, we need to specify the name of the generic followed by the object class.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>clean_names.tbl_graph</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>dat</span>, <span class='nv'>case</span> <span class='o'>=</span> <span class='s'>"snake"</span><span class='o'>)</span> <span class='o'>&#123;</span> 
  <span class='c'># rename all columns</span>
  <span class='nv'>dat</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select_all.html'>rename_all</a></span><span class='o'>(</span><span class='nv'>dat</span>, <span class='nv'>make_clean_names</span><span class='o'>)</span>
  <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nv'>dat</span><span class='o'>)</span>
  <span class='o'>&#125;</span></code></pre>

</div>

In order to test the function, we will need a graph to test it on. This example draws on the example used in the issue.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidygraph.data-imaginist.com'>tidygraph</a></span><span class='o'>)</span>
<span class='c'># create test graph to test clean_names</span>
<span class='nv'>test_graph</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://tidygraph.data-imaginist.com/reference/sampling_games.html'>play_erdos_renyi</a></span><span class='o'>(</span><span class='m'>0</span>, <span class='m'>0.5</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='c'># attach test_df as columns </span>
  <span class='nf'><a href='https://tidygraph.data-imaginist.com/reference/bind_graphs.html'>bind_nodes</a></span><span class='o'>(</span><span class='nv'>test_df</span><span class='o'>)</span>

<span class='nv'>test_graph</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tbl_graph: 1 nodes and 0 edges</span></span>
<span class='c'><span style='color: #555555;'>#&gt; #</span></span>
<span class='c'><span style='color: #555555;'>#&gt; # A rooted tree</span></span>
<span class='c'><span style='color: #555555;'>#&gt; #</span></span>
<span class='c'><span style='color: #555555;'>#&gt; # Node Data: 1 × 20 (active)</span></span>
<span class='c'>#&gt;   sp ace… repeat… `a**^@`   `%`   `*`   `!` `d(!)9` REPEAT… can"'t… hi_\th…   lead…</span>
<span class='c'>#&gt;     <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span>      78      18      99    28    26    38      43      73      51      33      23</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 9 more variables: `€` &lt;int&gt;, ação &lt;int&gt;, Farœ &lt;int&gt;, `a b c d e f` &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   testCamelCase &lt;int&gt;, `!leadingpunct` &lt;int&gt;, `average # of days` &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   jan2009sales &lt;int&gt;, `jan 2009 sales` &lt;int&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#</span></span>
<span class='c'><span style='color: #555555;'>#&gt; # Edge Data: 0 × 2</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 2 variables: from &lt;int&gt;, to &lt;int&gt;</span></span></code></pre>

</div>

Here we see that there is a graph with only 1 node and 0 edges (relations) with bad column headers (for more, visit the [GitHub page](https://github.com/thomasp85/tidygraph/)). Now we can test this as well.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/pkg/janitor/man/clean_names.html'>clean_names</a></span><span class='o'>(</span><span class='nv'>test_graph</span><span class='o'>)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tbl_graph: 1 nodes and 0 edges</span></span>
<span class='c'><span style='color: #555555;'>#&gt; #</span></span>
<span class='c'><span style='color: #555555;'>#&gt; # A rooted tree</span></span>
<span class='c'><span style='color: #555555;'>#&gt; #</span></span>
<span class='c'><span style='color: #555555;'>#&gt; # Node Data: 1 × 20 (active)</span></span>
<span class='c'>#&gt;   sp_ace repeat…     a percent     x   x_2   d_9 repeat…  cant hi_the… leadin…   x_3</span>
<span class='c'>#&gt;    <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span> <span style='color: #555555; font-style: italic;'>&lt;int&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span>     78      18    99      28    26    38    43      73    51      33      23    27</span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 8 more variables: acao &lt;int&gt;, faroe &lt;int&gt;, a_b_c_d_e_f &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   test_camel_case &lt;int&gt;, leadingpunct &lt;int&gt;, average_number_of_days &lt;int&gt;,</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#   jan2009sales &lt;int&gt;, jan_2009_sales &lt;int&gt;</span></span>
<span class='c'>#&gt; <span style='color: #555555;'>#</span></span>
<span class='c'><span style='color: #555555;'>#&gt; # Edge Data: 0 × 2</span></span>
<span class='c'>#&gt; <span style='color: #555555;'># … with 2 variables: from &lt;int&gt;, to &lt;int&gt;</span></span></code></pre>

</div>

It worked as anticipated!

### Review (tl;dr)

In the preceding sections we learned what generic functions and methods are. How to create a generic function, a default method, and methods for objects of different classes.

-   **generic function**: "A generic function is a function whose behavior depends on the classes or identities of the arguments supplied to it"
-   **generic function method**: "part of a generic function and which provides information about how that generic function should behave \[for\] certain classes"

The process to create a function with a method is to:

1.  Create a generic function with:
    -   `f_x <- function() { UseMethod("f_x") }`
2.  Define the default method with:
    -   `f_x.default <- function() { do something }`
3.  Define object class specific methods with:
    -   `f_x.class <- function() { do something else}`

#### Notes

If you have not yet encountered the `janitor` package it will help you tremendously with various data cleaning processes. Clearly, [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) is my favorite function as it helps me enforce my preferred style (and the only). If you are not aware of *"proper"* R style, I suggest you read the [style guide](http://adv-r.had.co.nz/Style.html) in [Advanced R](http://adv-r.had.co.nz/).

While on the subject of *Advanced R*, I suggest you read the "Creating new methods and generics" [section](http://adv-r.had.co.nz/OO-essentials.html) of it. I struggled comprehending it at first because I didn't even know what a method was. However, if after reading this you feel like you want more, that's the place to go.

I'd like to thank [@sfirke](https://github.com/sfirke) for being exceptionally helpful in guiding my contributions to the `janitor` package.

