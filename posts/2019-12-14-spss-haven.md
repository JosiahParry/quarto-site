---
title: "Finding an SPSS {haven}"
subtitle: "How to deal with `.sav` files from SPSS"
date: "2019-12-14"
rmd_hash: c3b76a832dfeb21d

---

> Note (2022-11-14): the dataset that was used can no longer be access from the url I provided in this blog post. I have found it at a zip file in a blog post at <https://blog.faradars.org/wp-content/uploads/2020/07/noisedata.zip> if you want to try downloading it from there.

My education as a social scientist---undergratuate studies in sociology and anthropology---was largely focused on theory and the application of theory to social problems. For the most part, I taught myself how to apply those methods through R. I was fortunate enough to have avoided ever using SPSS. Perhaps that is good. Perhaps it is not. The use of R in the social sciences is increasing and I will go as far as to say that that is great news. However, there are still holdouts.

Very recently I came across data exported from SPSS in the wild. In the work that I get to engage in at the Boston Area Research Initiative ([BARI](https://www.northeastern.edu/csshresearch/bostonarearesearchinitiative/)) we receive data from various sources, whether these be municipal organizations, the police department, or non-profits, and as Tolstoy said:

<blockquote class="twitter-tweet">
<p lang="en" dir="ltr">
"All clean datasets are alike; every messy dataset is messy in its own way.", Leo Tolstoy. <a href="https://t.co/Z70eXgKRaK">https://t.co/Z70eXgKRaK</a>
</p>
--- Maria Khalusova (@mariaKhalusova) <a href="https://twitter.com/mariaKhalusova/status/1204853611743842304?ref_src=twsrc%5Etfw">December 11, 2019</a>
</blockquote>
<script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>

In this post I want to illustrate two of the main pain points I encountered while working with data from SPSS.I also go rather deep into the weeds about R data structures and how we can manipulate them. While this is "about" SPSS data, folks looking for a better understanding of R object may benefit from this as well (specifically Gripe 2).

## {haven}

I am not sure where the inspiration for the name "haven" came from. But I am sure that its name does indeed speak for itself. haven enables R programmers that are SPSS---Stata and SAS as well---illiterate to work with data that is not naturally intended for R use. There are two key behaviors of SPSS data that I was unaware of and that plagued me. I break these down into three gripes below.

## Gripes

I maintain three gripes about SPSS data.

1.  Factor values are represented numerically and the values that they represent are stored in metadata.
2.  Column names are vague. The values they represent are stored as a label (metadata).
3.  Missing values can be represented innumerably. Each column can have user defined missing values, i.e. `9999`.

Now, I must say that I have found it best practice to try and combat my own gripes.

In regards to the former two gripes, this is not unheard of behavior nor is it rare. Associating numeric values with *related* values---oh, I don't know...think of a user ID and an email address---is in essence the core of relational database systems (RDBMS). One may even have the gall to argue that RDBMS power many of the tools I use and take for granted. I would most likely be willing to concede that point.

The third gripe can be quite easily countered if I am to be frank. Missing data is in of itself data. An `NA` is an `NA` is an `NA` may not be a generalizable statement. Providing values such as `9999` in place of a missing value in some cases may be a relic of antiquity where missingness could not be handled by software. Or, perhaps we can frame missingness other ways. Let's imagine we are conducting a study and we want to keep track of why there was missing data. This could have been from a non-response, or a withdrawal, or erroneously entered data, or any other number of reasons. Keeping a record of that missing data may useful.

### Gripe 1: numbers representing characters (or labels)

Sometimes I really would like `stringsAsFactors = TRUE`. Working with survey data tends to be one of those times. R has a robust method of creating, handling, and manipulating factors[^1] and because of this, we aren't required to numerically encode our data. This may be a personal preference, but I really like to be reminded of *what* I am working with and seeing the factor levels clearly spelled out for me is quite nice.

Since the data I am working with at BARI is confidential, I've found some SPSS data hosted by the [University of Bath](https://www.bath.ac.uk/)[^2] to illustrate this with.

Reading in data is rather straightforward. SPSS data come with a `.sav` file extension. Following standard [`readr`](https://readr.tidyverse.org/) convention we can read a .sav file with [`haven::read_sav()`](https://haven.tidyverse.org/reference/read_spss.html).

*Note: the syntax above is used for referencing an exported function from a namespace (package name). The syntax is `pkgname::function()`.*

The below line reads in the sav file from the University of Bath.

*Note: by wrapping an object assignment expression, such as the below, in parentheses the object is then printed (I just recently figured this out).*

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://haven.tidyverse.org'>haven</a></span><span class='o'>)</span>

<span class='c'># read in sav file</span>
<span class='o'>(</span><span class='nv'>noise</span> <span class='o'>&lt;-</span> <span class='nf'>haven</span><span class='nf'>::</span><span class='nf'><a href='https://haven.tidyverse.org/reference/read_spss.html'>read_sav</a></span><span class='o'>(</span><span class='s'>"http://staff.bath.ac.uk/pssiw/stats2/noisedata.sav"</span><span class='o'>)</span><span class='o'>)</span></code></pre>

</div>

The above shows `GENDER` codes as a numeric value. But if you print out the tibble to your console you will see labels. So in this case, where there is a `1` under `GENDER`, the printed console shows `[male]`, and the same is true for `2` and `[female]`. We can get a sense of this by viewing the structure of the tibble.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/utils/str.html'>str</a></span><span class='o'>(</span><span class='nv'>noise</span><span class='o'>)</span></code></pre>

</div>

Above we can see that `GENDER` has an attribute `labels` which is a named numeric vector. The unique values are 1 and 2 representing "male" and "female" respectively. I struggle to keep this mental association. I'd prefer to have this shown explicitly. Fortunately, haven provides the function [`haven::as_factor()`](https://haven.tidyverse.org/reference/as_factor.html) which will convert these pesky integer columns to their respective factor values. We just need to pass the data frame as the only argument---sure, there are other arguments if you want to get fancy.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># selecting just 2 columns and 2 rows for simlicity.</span>
<span class='nv'>small_lil_df</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://dplyr.tidyverse.org/reference/slice.html'>slice</a></span><span class='o'>(</span><span class='nv'>noise</span>, <span class='m'>1</span>, <span class='m'>20</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='m'>1</span>, <span class='m'>2</span><span class='o'>)</span>

<span class='c'># convert coded response to response text</span>
<span class='nf'>haven</span><span class='nf'>::</span><span class='nf'><a href='https://haven.tidyverse.org/reference/as_factor.html'>as_factor</a></span><span class='o'>(</span><span class='nv'>small_lil_df</span><span class='o'>)</span></code></pre>

</div>

And now we can just forget those integers ever existed in the firstplace!

### Gripe 2: uninformative column names

There are three key rules to [tidy data](https://vita.had.co.nz/papers/tidy-data.pdf).

1.  Each variable forms a column.
2.  Each observation forms a row.
3.  Each type of observational unit forms a table.

Each variable forms a column. Got it. So this means that any thing that can help describe our observation should be a column. Say we have a table of survey respondents. In this case each row should be a respondent and each column should be a variable (or feature, or predictor, or x, or whatever) associate with that respondent. This could be something like age, birth date, or the respondents response to a survey question.

In the [tidyverse style guide](https://style.tidyverse.org/index.html) Hadley Wickham writes

> Generally, variable names should be nouns and function names should be verbs. Strive for names that are concise and meaningful (this is not easy!).

I personaly try to extend this to column names as well. Feature names are important so I, as a researcher, can remember what is what. From my encounters with SPSS data, I've found that feature names can be rather uninformative e.g. "Q12." Much like factors, columns may have associated information hidden somewhere within them.

We read in the personality data set from the Universit of Bath below.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># read in sav file with column labels</span>
<span class='o'>(</span><span class='nv'>personality</span> <span class='o'>&lt;-</span> <span class='nf'>haven</span><span class='nf'>::</span><span class='nf'><a href='https://haven.tidyverse.org/reference/read_spss.html'>read_sav</a></span><span class='o'>(</span><span class='s'>"http://staff.bath.ac.uk/pssiw/stats2/personality.sav"</span><span class='o'>)</span><span class='o'>)</span></code></pre>

</div>

The first thing that I notice is rather apalling: each *column* represents a *person*. oof, untidy. But this issue isn't what brought me to these data. If you print the data to the console, you see something similar as what is above. If you *view* (`View(df)`) the data, the story is different. There is associated information underneath each column header.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/utils/str.html'>str</a></span><span class='o'>(</span><span class='nv'>personality</span><span class='o'>[</span>,<span class='m'>1</span><span class='o'>:</span><span class='m'>5</span><span class='o'>]</span><span class='o'>)</span></code></pre>

</div>

Yikes. These labels seem to be character judgements! Whatever the labels represent, I want them, and I want them as the column headers.

From looking at the structure of the data frame we can glean that each column has a label.

**Warning**: I'm going to walk through a fair bit of the theory and under the hood work to make these labels column names. Scroll to the bottom of this section to find the function definition.

#### Aside: R theory:

-   Each column of a data frame is actually just a vector. Each vector can have it's own attributes (as above).
-   A data frame is actually just a list of equal length vectors (same number of observations).[^3]
-   "All objects can have arbitrary additional attributes, used to store metadata about the object."[^4]
-   We can fetch list elements using `[[` notation, e.g. `my_list[[1]]`
    -   [`purrr::pluck()`](https://purrr.tidyverse.org/reference/pluck.html) is an alternative to using `[[` for grabbing the underlying elements inside of a data structure. This means we can use `pluck(my_list, 1)` in place of `my_list[[1]]`

Okay, but *how* does one actually get the label from the vector? The first step is to actually grab the vector. Below I use [`purrr::pluck()`](https://purrr.tidyverse.org/reference/pluck.html)to pull the first column. Note that [`slice()`](https://dplyr.tidyverse.org/reference/slice.html) is used for grabing specific row indexes. The below code is equivalent to `personality[1:10,][[1]]`. I prefer using the purrr functions because they are more legible.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>col_1</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://dplyr.tidyverse.org/reference/slice.html'>slice</a></span><span class='o'>(</span><span class='nv'>personality</span>, <span class='m'>1</span><span class='o'>:</span><span class='m'>10</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://purrr.tidyverse.org/reference/pluck.html'>pluck</a></span><span class='o'>(</span><span class='m'>1</span><span class='o'>)</span></code></pre>

</div>

We can access all of the vectors attributes with, you guessed it, the [`attributes()`](https://rdrr.io/r/base/attributes.html) function.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/base/attributes.html'>attributes</a></span><span class='o'>(</span><span class='nv'>col_1</span><span class='o'>)</span></code></pre>

</div>

This returns a named list. We can access (or set) specific attributes using the [`attr()`](https://rdrr.io/r/base/attr.html) function. The two arguments we must supply are `x`, the object, and `which`, which attribute we seek. In this case the values are `col_1` and `label` respectively.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/base/attr.html'>attr</a></span><span class='o'>(</span><span class='nv'>col_1</span>, <span class='s'>"label"</span><span class='o'>)</span></code></pre>

</div>

purrr yet again makes working with list objects easy. purrr exports a function factory[^5] called [`purrr::attr_getter()`](https://purrr.tidyverse.org/reference/attr_getter.html). This function generates a function which accesses specific attributes. We can create a function `get_label()` using [`attr_getter()`](https://purrr.tidyverse.org/reference/attr_getter.html) all we have to do is tell it *which* attribute we would like.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># generate a get_label f(x) via purr</span>
<span class='nv'>get_label</span> <span class='o'>&lt;-</span> <span class='nf'>purrr</span><span class='nf'>::</span><span class='nf'><a href='https://purrr.tidyverse.org/reference/attr_getter.html'>attr_getter</a></span><span class='o'>(</span><span class='s'>"label"</span><span class='o'>)</span>

<span class='nf'>get_label</span><span class='o'>(</span><span class='nv'>col_1</span><span class='o'>)</span></code></pre>

</div>

Well, lovely. Let's just use this on our `personality` data frame.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>get_label</span><span class='o'>(</span><span class='nv'>personality</span><span class='o'>)</span></code></pre>

</div>

Ope. Welp. That didn't work. We should just give up 🤷.

![](https://media.giphy.com/media/O5xChSjqUIxsk/giphy.gif)

The reason this didn't work is because we tried to use `get_label()` on a tibble which didn't have the "label" attribute. We can verify this by looking at the list names of the attributes of `personality`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/attributes.html'>attributes</a></span><span class='o'>(</span><span class='nv'>personality</span><span class='o'>)</span><span class='o'>)</span></code></pre>

</div>

But what about the attribtues of each column? We can iterate over each column using [`map()`](https://purrr.tidyverse.org/reference/map.html) to look at the attributes. Below I iterate over the first five columns. More on [`map()`](https://purrr.tidyverse.org/reference/map.html)[^6].

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='nv'>personality</span>, <span class='m'>1</span><span class='o'>:</span><span class='m'>5</span><span class='o'>)</span>, <span class='o'>~</span><span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/attributes.html'>attributes</a></span><span class='o'>(</span><span class='nv'>.</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span></code></pre>

</div>

Well, now that we've iterated over the columns and illustrated that the attributes live there, why not iterate over the columns and use `get_label()`?

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># use get_label to retrieve column labels</span>
<span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map_chr</a></span><span class='o'>(</span><span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='nv'>personality</span>, <span class='m'>1</span><span class='o'>:</span><span class='m'>5</span><span class='o'>)</span>, <span class='nv'>get_label</span><span class='o'>)</span></code></pre>

</div>

Again, yikes @ the labels. Let's store these results into a vector so we can rename the original columns.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>pers_labels</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map_chr</a></span><span class='o'>(</span><span class='nv'>personality</span>, <span class='nv'>get_label</span><span class='o'>)</span></code></pre>

</div>

We can now change the names using [`setNames()`](https://rdrr.io/r/stats/setNames.html) from base R. We will then make the column headers tidy (personal definition of tidy column names) using [`janitor::clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/stats/setNames.html'>setNames</a></span><span class='o'>(</span><span class='nv'>personality</span>, <span class='nv'>pers_labels</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'>janitor</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/janitor/man/clean_names.html'>clean_names</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='m'>1</span><span class='o'>:</span><span class='m'>5</span><span class='o'>)</span></code></pre>

</div>

In the case that a column doesn't have a label, `get_label()` will return `NULL` and then [`setNames()`](https://rdrr.io/r/stats/setNames.html) will fail. To work around this, you can use the name of the column rather than the label value. Below is a function definition which handles this for you and, optionally, lets you specify which columns to rename based on a regex pattern. I think we've done enough list manipulation for the day. If you have questions about the function definition I'd be happy to work through it one on one with you via twitter DMs.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>label_to_colname</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>df</span>, <span class='nv'>pattern</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='nv'>get_label</span> <span class='o'>&lt;-</span> <span class='nf'>purrr</span><span class='nf'>::</span><span class='nf'><a href='https://purrr.tidyverse.org/reference/attr_getter.html'>attr_getter</a></span><span class='o'>(</span><span class='s'>"label"</span><span class='o'>)</span>
  <span class='nv'>col_labels</span> <span class='o'>&lt;-</span> <span class='nf'>purrr</span><span class='nf'>::</span><span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>df</span>, <span class='nv'>get_label</span><span class='o'>)</span>

  <span class='nv'>col_labels</span><span class='o'>[</span><span class='nf'><a href='https://rdrr.io/r/base/unlist.html'>unlist</a></span><span class='o'>(</span><span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>col_labels</span>, <span class='nv'>is.null</span><span class='o'>)</span><span class='o'>)</span><span class='o'>]</span>  <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nv'>col_labels</span><span class='o'>[</span><span class='nf'><a href='https://rdrr.io/r/base/unlist.html'>unlist</a></span><span class='o'>(</span><span class='nf'>purrr</span><span class='nf'>::</span><span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>col_labels</span>, <span class='nv'>is.null</span><span class='o'>)</span><span class='o'>)</span><span class='o'>]</span><span class='o'>)</span>

  <span class='kr'>if</span> <span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/missing.html'>missing</a></span><span class='o'>(</span><span class='nv'>pattern</span><span class='o'>)</span><span class='o'>)</span> <span class='o'>&#123;</span>
    <span class='nv'>names_to_replace</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/rep.html'>rep</a></span><span class='o'>(</span><span class='kc'>TRUE</span>, <span class='nf'><a href='https://rdrr.io/r/base/nrow.html'>ncol</a></span><span class='o'>(</span><span class='nv'>df</span><span class='o'>)</span><span class='o'>)</span>
  <span class='o'>&#125;</span> <span class='kr'>else</span> <span class='o'>&#123;</span>
    <span class='nv'>names_to_replace</span> <span class='o'>&lt;-</span> <span class='nf'>stringr</span><span class='nf'>::</span><span class='nf'><a href='https://stringr.tidyverse.org/reference/str_detect.html'>str_detect</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nv'>col_labels</span><span class='o'>)</span>, <span class='nv'>pattern</span><span class='o'>)</span>
  <span class='o'>&#125;</span>
  
  <span class='nf'><a href='https://rdrr.io/r/base/colnames.html'>colnames</a></span><span class='o'>(</span><span class='nv'>df</span><span class='o'>)</span><span class='o'>[</span><span class='nv'>names_to_replace</span><span class='o'>]</span> <span class='o'>&lt;-</span> <span class='nf'>janitor</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/janitor/man/make_clean_names.html'>make_clean_names</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/unlist.html'>unlist</a></span><span class='o'>(</span><span class='nv'>col_labels</span><span class='o'>[</span><span class='nv'>names_to_replace</span><span class='o'>]</span><span class='o'>)</span><span class='o'>)</span>

  <span class='nf'>haven</span><span class='nf'>::</span><span class='nf'><a href='https://haven.tidyverse.org/reference/zap_label.html'>zap_label</a></span><span class='o'>(</span><span class='nv'>df</span><span class='o'>)</span>
<span class='o'>&#125;</span>

<span class='nf'>label_to_colname</span><span class='o'>(</span><span class='nv'>personality</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='m'>1</span><span class='o'>:</span><span class='m'>5</span><span class='o'>)</span>

<span class='c'># heh.</span>
<span class='nf'>label_to_colname</span><span class='o'>(</span><span class='nv'>personality</span>, <span class='s'>"PERS37"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='m'>37</span><span class='o'>)</span></code></pre>

</div>

### Gripe 3: user defined missing values

I'll keep this one short. If there are user defined missing values in a `.sav` file, you can encode these as `NA` by setting the `user_na` arugment to `TRUE`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># there aren't any missing values but you get the idea</span>
<span class='nv'>noise</span> <span class='o'>&lt;-</span> <span class='nf'>haven</span><span class='nf'>::</span><span class='nf'><a href='https://haven.tidyverse.org/reference/read_spss.html'>read_sav</a></span><span class='o'>(</span><span class='s'>"http://staff.bath.ac.uk/pssiw/stats2/noisedata.sav"</span>,
                         user_na <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span></code></pre>

</div>

And if for any reason that did not suffice, you can replace missing values with [`replace_na()`](https://tidyr.tidyverse.org/reference/replace_na.html)[^7].

## Take aways

-   All data is messy in it's own way.
-   [`haven::read_sav()`](https://haven.tidyverse.org/reference/read_spss.html) will read SPSS data.
-   [`haven::as_factor()`](https://haven.tidyverse.org/reference/as_factor.html) will apply column labels in place of the numeric values (if present).  
-   Replace user defined NA values by setting `user_na = TRUE` i.e. `haven::read_sav("filepath.sav", user_na = TRUE)`
-   All R objects can have attributes.
-   You can access attributes using [`attributes()`](https://rdrr.io/r/base/attributes.html) or [`attr()`](https://rdrr.io/r/base/attr.html).
-   Data frames are made of vectors.
-   Data frames are actually just lists masquerading as rectangles.

[^1]: Check out [forcats](https://forcats.tidyverse.org/) for working with factors.

[^2]: Anthony Horowitz wrote a rather fun murder mystery novel titled [Magpie Murders](https://www.amazon.com/Magpie-Murders-Novel-Anthony-Horowitz/dp/0062645226) which takes place in a small town outside of bath. I recommend it.

[^3]: <http://adv-r.had.co.nz/Data-structures.html#data-frames>

[^4]: <http://adv-r.had.co.nz/Data-structures.html#attributes>

[^5]: A function factory is an object which creates other function objects.

[^6]: <https://adv-r.hadley.nz/functionals.html#map>

[^7]: <https://tidyr.tidyverse.org/reference/replace_na.html>

