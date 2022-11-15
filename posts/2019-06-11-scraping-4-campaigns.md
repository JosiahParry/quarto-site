---
title: "Web-scraping for Campaigns"
subtile: "Scraping polling data for campaign purposes"
date: "2019-06-11"
slug: "scraping-4-campaigns"
categories:
  - R
tags:
  - R
  - tutorial
  - politics
  - youthvote
  - campaigns
rmd_hash: 7ab1f4ed2ecd2de1

---

Note: 2022-11-14 I'm migrating my website and this can no longer be reproduced. This blog post is based on the short guide I wrote back in 2019. [Please see the old bookdown here](https://r4campaigns.netlify.app/).

------------------------------------------------------------------------

As the primaries approach, I am experiencing a mix of angst, FOMO, and excitement. One of my largest concerns is that progressive campaigns are stuck in a sort of antiquated but nonetheless entrenched workflow. Google Sheets reign in metric reporting. Here I want to present one use case (of a few more to come) where R can be leveraged by your data team.

In this post I show you how to scrape the most recent polling data from FiveThirtyEight. FiveThirtyEight aggregates this data in an available way. This can allow you as a Data Manager to provide a useful report to your Media Manager.

As always, please feel free to contact me on Twitter [@josiahparry](https://twitter.com/josiahparry) if you have any questions or want to discuss this further.

------------------------------------------------------------------------

# Polling use case

A very important metric to keep track of is how your candidate is polling. Are they gaining a lead in the polls or falling behind? This data is often reported via traditional news organizations or some other mediums. The supposed demi-God and mythical pollster Nate Silver's organization FiveThirtyEight does a wonderful job aggregating polls. Their page [National 2020 Democratic Presidential Primary Polls](https://projects.fivethirtyeight.com/2020-primaries/democratic/national/) has a table of the most recent polls from many different pollsters.

In this use case we will acquire this data by web scraping using `rvest`. We will also go over ways to programatically save polls results to a text file. Saving polling results can allow you present a long term view of your candidate's growth during the quarter.

## Understanding `rvest`

This use case will provide a cursory overview of the package `rvest`. To learn more go [here](http://rvest.tidyverse.org/).

Web scraping is the process of extracting data from a website. Websites are written in HTML and CSS. There are a few aspects of these languages that are used in web scraping that is important to know. HTML is written in a series of what are call tags. A tag is a set of characters wrapped in angle brackets---i.e. `<img>`.

With CSS (cascading style sheets), web developers can give unique identifiers to a tag. Classes can also be assigned to a tag. Think of these as group. With web scraping we can specify a particular part of a website by it's HTML tag and perhaps it's class or ID. `rvest` provides a large set of functions to make this simpler.

## Example

For this example we will be scraping FiveThirtyEight's aggregated poll table. The table can be found at <https://projects.fivethirtyeight.com/2020-primaries/democratic/national/>.

Before we begin, we must always prepare our workspace. Mise en place.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://rvest.tidyverse.org/'>rvest</a></span><span class='o'>)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span></code></pre>

</div>

The first thing we will have to do is specify what page we will be scraping from. [`html_session()`](https://rvest.tidyverse.org/reference/rename.html) will simulate a session in an html browser. By providing a URL to [`html_session()`](https://rvest.tidyverse.org/reference/rename.html) we will then be able to access the underlying code of that page. Create an object called `session` by providing the FiveThirtyEight URL to [`html_session()`](https://rvest.tidyverse.org/reference/rename.html).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>session</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rvest.tidyverse.org/reference/rename.html'>html_session</a></span><span class='o'>(</span><span class='s'>"https://projects.fivethirtyeight.com/2020-primaries/democratic/national/"</span><span class='o'>)</span></code></pre>

</div>

The next and most important step is to identify which piece of HTML code contains the table. The easiest way to do this is to open up the webpage in Chrome and open up the Inspect Elements view (on Mac - ⌘ + Shift + C). Now that this is open, click the select element button at the top left corner of the inspection pane. Now hover over the table.

You will see that the HTML element is highlighted. We can see that it is a `table` tag. Additionally we see that there are two different classes `polls-table` and `tracker`. To specify a class we put a preceding `.` to the class name---i.e. `.class-name`. If there are multiple classes we just append the second class name to it---i.e. `.first-class.second-class`. Be aware that these selectors can be quite finicky and be a bit difficult to figure out. You might need to do some googling or playing around with the selector.

To actually access the content of this HTML element, we must specify the element using the proper selector. [`html_node()`](https://rvest.tidyverse.org/reference/rename.html) will be used to do this. Provide the html session and the CSS selector to [`html_node()`](https://rvest.tidyverse.org/reference/rename.html) to extract the HTML element.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>session</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rvest.tidyverse.org/reference/rename.html'>html_node</a></span><span class='o'>(</span><span class='s'>".polls-table.tracker"</span><span class='o'>)</span></code></pre>

</div>

Here we see that this returns on object of class `xml_node`. This object returns some HTML code but it is still not entirely workable. Since this is an HTML table we want to extract we can use the handy [`html_table()`](https://rvest.tidyverse.org/reference/html_table.html). Note that if this wasn't a table but rather text, you can use [`html_text()`](https://rvest.tidyverse.org/reference/html_text.html).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>session</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rvest.tidyverse.org/reference/rename.html'>html_node</a></span><span class='o'>(</span><span class='s'>".polls-table.tracker"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rvest.tidyverse.org/reference/html_table.html'>html_table</a></span><span class='o'>(</span><span class='o'>)</span></code></pre>

</div>

Take note of the extremely informative error. It appears we might have to deal with mismatching columns.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>session</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rvest.tidyverse.org/reference/rename.html'>html_node</a></span><span class='o'>(</span><span class='s'>".polls-table.tracker"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rvest.tidyverse.org/reference/html_table.html'>html_table</a></span><span class='o'>(</span>fill <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rdrr.io/r/utils/head.html'>head</a></span><span class='o'>(</span><span class='o'>)</span></code></pre>

</div>

This is much better! But based on visual inspection the column headers are not properly matched. There are a few things that need to be sorted out: there are two date columns, there are commas and percents where numeric columns should be, the column headers are a little messy, and the table isn't a tibble (this is just personal preference).

We will handle the final two issues first as they are easiest to deal with. The function [`clean_names()`](https://rdrr.io/pkg/janitor/man/clean_names.html) from `janitor` will handle the column headers, and [`as_tibble()`](https://tibble.tidyverse.org/reference/as_tibble.html) will coerce the data.frame into a proper tibble. Save this semi-clean tibble into an object called `polls`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>polls</span> <span class='o'>&lt;-</span> <span class='nv'>session</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rvest.tidyverse.org/reference/rename.html'>html_node</a></span><span class='o'>(</span><span class='s'>".polls-table.tracker"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rvest.tidyverse.org/reference/html_table.html'>html_table</a></span><span class='o'>(</span>fill <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'>janitor</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/janitor/man/clean_names.html'>clean_names</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://tibble.tidyverse.org/reference/as_tibble.html'>as_tibble</a></span><span class='o'>(</span><span class='o'>)</span>

<span class='nv'>polls</span></code></pre>

</div>

We want to shift over the column names to the right just once. Unfortunately there is no elegant way to do this (that I am aware of). We can see that the first column is completely useless so that can be removed. Once that column is removed we can reset the names this way they will be well aligned.

We will start by creating a vector of the original column names.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>col_names</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/names.html'>names</a></span><span class='o'>(</span><span class='nv'>polls</span><span class='o'>)</span>
<span class='nv'>col_names</span></code></pre>

</div>

Unfortunately this also presents another issue. Once a column is deselected, there will be one more column name than column. So we will need to select all but the last element of the original names. We will create a vector called `new_names`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># identify the integer number of the last column</span>
<span class='nv'>last_col</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/length.html'>length</a></span><span class='o'>(</span><span class='nv'>col_names</span><span class='o'>)</span> <span class='o'>-</span> <span class='m'>1</span>

<span class='c'># create a vector which will be used for the new names</span>
<span class='nv'>new_names</span> <span class='o'>&lt;-</span> <span class='nv'>col_names</span><span class='o'>[</span><span class='m'>1</span><span class='o'>:</span><span class='nv'>last_col</span><span class='o'>]</span></code></pre>

</div>

Now we can try implementing the hacky solution. Here we will deselect the first column and reset the names using [`setNames()`](https://rdrr.io/r/stats/setNames.html). Following, we will use the [`mutate_at()`](https://dplyr.tidyverse.org/reference/mutate_all.html) variant to remove the percent sign from every candidate column and coerce them into integer columns. Here we will specify which variables to *not* mutate at within [`vars()`](https://ggplot2.tidyverse.org/reference/vars.html).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>polls</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='m'>1</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>  
  <span class='nf'><a href='https://rdrr.io/r/stats/setNames.html'>setNames</a></span><span class='o'>(</span><span class='nv'>new_names</span><span class='o'>)</span><span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='m'>1</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate_all.html'>mutate_at</a></span><span class='o'>(</span><span class='nf'><a href='https://ggplot2.tidyverse.org/reference/vars.html'>vars</a></span><span class='o'>(</span><span class='o'>-</span><span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='s'>"dates"</span>, <span class='s'>"pollster"</span>, <span class='s'>"sample"</span>, <span class='s'>"sample_2"</span><span class='o'>)</span><span class='o'>)</span>, 
            <span class='o'>~</span><span class='nf'><a href='https://rdrr.io/r/base/integer.html'>as.integer</a></span><span class='o'>(</span><span class='nf'><a href='https://stringr.tidyverse.org/reference/str_remove.html'>str_remove</a></span><span class='o'>(</span><span class='nv'>.</span>, <span class='s'>"%"</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span>

</code></pre>

</div>

Now we must tidy the data. We will use [`tidyr::gather()`](https://tidyr.tidyverse.org/reference/gather.html) to transform the data from wide to long. In short, gather takes the column headers (the `key` argument) and creates a new variable from the values of the columns (the `value` argument). In this case, we will create a new column called `candidate` from the column headers and a second column called `points` which are a candidates polling percentage. Next we deselect any columns that we do not want to be gathered.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>polls</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='m'>1</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rdrr.io/r/stats/setNames.html'>setNames</a></span><span class='o'>(</span><span class='nv'>new_names</span><span class='o'>)</span><span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='m'>1</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate_all.html'>mutate_at</a></span><span class='o'>(</span><span class='nf'><a href='https://ggplot2.tidyverse.org/reference/vars.html'>vars</a></span><span class='o'>(</span><span class='o'>-</span><span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='s'>"dates"</span>, <span class='s'>"pollster"</span>, <span class='s'>"sample"</span>, <span class='s'>"sample_2"</span><span class='o'>)</span><span class='o'>)</span>,
            <span class='o'>~</span><span class='nf'><a href='https://rdrr.io/r/base/integer.html'>as.integer</a></span><span class='o'>(</span><span class='nf'><a href='https://stringr.tidyverse.org/reference/str_remove.html'>str_remove</a></span><span class='o'>(</span><span class='nv'>.</span>, <span class='s'>"%"</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://tidyr.tidyverse.org/reference/gather.html'>gather</a></span><span class='o'>(</span><span class='nv'>candidate</span>, <span class='nv'>points</span>, <span class='o'>-</span><span class='nv'>dates</span>, <span class='o'>-</span><span class='nv'>pollster</span>, <span class='o'>-</span><span class='nv'>sample</span>, <span class='o'>-</span><span class='nv'>sample_2</span><span class='o'>)</span></code></pre>

</div>

There are a few more house-keeping things that need to be done to improve this data set. `sample_2` is rather uninformative. On the FiveThirtyEight website there is a key which describes what these values represent (`A = ADULTS, RV = REGISTERED VOTERS, V = VOTERS, LV = LIKELY VOTERS`). This should be specified in our data set. In addition the `sample` column ought to be cast into an integer column. And finally, those messy dates will need to be cleaned. My approach to this requires creating a function to handle this cleaning. First, the simple stuff.

To do the first two above steps, we will continue our function chain and save it to a new variable `polls_tidy`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>polls_tidy</span> <span class='o'>&lt;-</span> <span class='nv'>polls</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='m'>1</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rdrr.io/r/stats/setNames.html'>setNames</a></span><span class='o'>(</span><span class='nv'>new_names</span><span class='o'>)</span><span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='m'>1</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate_all.html'>mutate_at</a></span><span class='o'>(</span><span class='nf'><a href='https://ggplot2.tidyverse.org/reference/vars.html'>vars</a></span><span class='o'>(</span><span class='o'>-</span><span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='s'>"dates"</span>, <span class='s'>"pollster"</span>, <span class='s'>"sample"</span>, <span class='s'>"sample_2"</span><span class='o'>)</span><span class='o'>)</span>, 
            <span class='o'>~</span><span class='nf'><a href='https://rdrr.io/r/base/integer.html'>as.integer</a></span><span class='o'>(</span><span class='nf'><a href='https://stringr.tidyverse.org/reference/str_remove.html'>str_remove</a></span><span class='o'>(</span><span class='nv'>.</span>, <span class='s'>"%"</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://tidyr.tidyverse.org/reference/gather.html'>gather</a></span><span class='o'>(</span><span class='nv'>candidate</span>, <span class='nv'>points</span>, <span class='o'>-</span><span class='nv'>dates</span>, <span class='o'>-</span><span class='nv'>pollster</span>, <span class='o'>-</span><span class='nv'>sample</span>, <span class='o'>-</span><span class='nv'>sample_2</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate.html'>mutate</a></span><span class='o'>(</span>sample_2 <span class='o'>=</span> <span class='nf'><a href='https://dplyr.tidyverse.org/reference/case_when.html'>case_when</a></span><span class='o'>(</span>
    <span class='nv'>sample_2</span> <span class='o'>==</span> <span class='s'>"RV"</span> <span class='o'>~</span> <span class='s'>"Registered Voters"</span>,
    <span class='nv'>sample_2</span> <span class='o'>==</span> <span class='s'>"LV"</span> <span class='o'>~</span> <span class='s'>"Likely Voters"</span>,
    <span class='nv'>sample_2</span> <span class='o'>==</span> <span class='s'>"A"</span> <span class='o'>~</span> <span class='s'>"Adults"</span>,
    <span class='nv'>sample_2</span> <span class='o'>==</span> <span class='s'>"V"</span> <span class='o'>~</span> <span class='s'>"Voters"</span>
  <span class='o'>)</span>,
  sample <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/integer.html'>as.integer</a></span><span class='o'>(</span><span class='nf'><a href='https://stringr.tidyverse.org/reference/str_remove.html'>str_remove</a></span><span class='o'>(</span><span class='nv'>sample</span>, <span class='s'>","</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span>

<span class='nv'>polls_tidy</span></code></pre>

</div>

### Date cleaning

Next we must work to clean the date field. I find that when working with a messy column, creating a single function which handles the cleaning is one of the most effective approaches. Here we will create a function which takes a value provided from the `dates` field and return a cleaned date. There are two unique cases I identified. There are poll dates which occurred during a single month, or a poll that spanned two months. The dates are separated by a single hyphen `-`. If we split the date at `-` we will either receive two elements with a month indicated or one month with a day and a day number. In the latter case we will have to carry over the month. Then the year can be appended to it and parsed as a date using the `lubridate` package. For more on `lubridate` visit [here](https://lubridate.tidyverse.org/).

The function will only return one date at a time. The two arguments will be `date` and `.return` to indicate whether the first or second date should be provided. The internals of this function rely heavily on the `stringr` package (see R for Data Science [Chapter 14](https://r4ds.had.co.nz/strings.html)). [`switch()`](https://rdrr.io/r/base/switch.html) at the end of the function determines which date should be returned (see Advanced R [Chapter 5](https://adv-r.hadley.nz/control-flow.html)).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>clean_date</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>date</span>, <span class='nv'>.return</span> <span class='o'>=</span> <span class='s'>"first"</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='c'># take date and split at the comma to get the year and the month-day combo</span>
  <span class='nv'>date_split</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://stringr.tidyverse.org/reference/str_split.html'>str_split</a></span><span class='o'>(</span><span class='nv'>date</span>, <span class='s'>","</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='c'># remove from list / coerce to vector</span>
    <span class='nf'><a href='https://rdrr.io/r/base/unlist.html'>unlist</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='c'># remove extra white space</span>
    <span class='nf'><a href='https://stringr.tidyverse.org/reference/str_trim.html'>str_trim</a></span><span class='o'>(</span><span class='o'>)</span>
  
  <span class='c'># extract the year</span>
  <span class='nv'>date_year</span> <span class='o'>&lt;-</span> <span class='nv'>date_split</span><span class='o'>[</span><span class='m'>2</span><span class='o'>]</span>
  
  <span class='c'># split the month day portion and coerce to vector</span>
  <span class='nv'>dates</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/unlist.html'>unlist</a></span><span class='o'>(</span><span class='nf'><a href='https://stringr.tidyverse.org/reference/str_split.html'>str_split</a></span><span class='o'>(</span><span class='nv'>date_split</span><span class='o'>[</span><span class='m'>1</span><span class='o'>]</span>,  <span class='s'>"-"</span><span class='o'>)</span><span class='o'>)</span>
  
  <span class='c'># paste the month day and year together then parse as date using `mdy()`</span>
  <span class='nv'>first_date</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/paste.html'>paste</a></span><span class='o'>(</span><span class='nv'>dates</span><span class='o'>[</span><span class='m'>1</span><span class='o'>]</span>, <span class='nv'>date_year</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'>lubridate</span><span class='nf'>::</span><span class='nf'><a href='https://lubridate.tidyverse.org/reference/ymd.html'>mdy</a></span><span class='o'>(</span><span class='o'>)</span>
  
  <span class='nv'>second_date</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/ifelse.html'>ifelse</a></span><span class='o'>(</span><span class='o'>!</span><span class='nf'><a href='https://stringr.tidyverse.org/reference/str_detect.html'>str_detect</a></span><span class='o'>(</span><span class='nv'>dates</span><span class='o'>[</span><span class='m'>2</span><span class='o'>]</span>, <span class='s'>"[A-z]+"</span><span class='o'>)</span>,
                        yes <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/paste.html'>paste</a></span><span class='o'>(</span><span class='nf'><a href='https://stringr.tidyverse.org/reference/str_extract.html'>str_extract</a></span><span class='o'>(</span><span class='nv'>dates</span><span class='o'>[</span><span class='m'>1</span><span class='o'>]</span>, <span class='s'>"[A-z]+"</span><span class='o'>)</span>, 
                              <span class='nv'>dates</span><span class='o'>[</span><span class='m'>2</span><span class='o'>]</span>, 
                              <span class='nv'>date_year</span><span class='o'>)</span>, 
                        no <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/paste.html'>paste</a></span><span class='o'>(</span><span class='nv'>dates</span><span class='o'>[</span><span class='m'>2</span><span class='o'>]</span>, <span class='nv'>date_year</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'>lubridate</span><span class='nf'>::</span><span class='nf'><a href='https://lubridate.tidyverse.org/reference/ymd.html'>mdy</a></span><span class='o'>(</span><span class='o'>)</span>
  
  <span class='kr'><a href='https://rdrr.io/r/base/switch.html'>switch</a></span><span class='o'>(</span><span class='nv'>.return</span>, 
         first <span class='o'>=</span> <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nv'>first_date</span><span class='o'>)</span>,
         second <span class='o'>=</span> <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nv'>second_date</span><span class='o'>)</span>
         <span class='o'>)</span>
  
<span class='o'>&#125;</span>

<span class='c'># test on a date</span>
<span class='nf'>clean_date</span><span class='o'>(</span><span class='nv'>polls_tidy</span><span class='o'>$</span><span class='nv'>dates</span><span class='o'>[</span><span class='m'>10</span><span class='o'>]</span>, .return <span class='o'>=</span> <span class='s'>"first"</span><span class='o'>)</span>
<span class='nf'>clean_date</span><span class='o'>(</span><span class='nv'>polls_tidy</span><span class='o'>$</span><span class='nv'>dates</span><span class='o'>[</span><span class='m'>10</span><span class='o'>]</span>, .return <span class='o'>=</span> <span class='s'>"second"</span><span class='o'>)</span></code></pre>

</div>

We can use this new function to create two new columns `poll_start` and `poll_end` using [`mutate()`](https://dplyr.tidyverse.org/reference/mutate.html). Following this we can deselect the original `dates` column, remove any observations missing a `points` value, remove duplicates using [`distinct()`](https://dplyr.tidyverse.org/reference/distinct.html), and save this to `polls_clean`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>polls_clean</span> <span class='o'>&lt;-</span> <span class='nv'>polls_tidy</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate.html'>mutate</a></span><span class='o'>(</span>poll_start <span class='o'>=</span> <span class='nf'>clean_date</span><span class='o'>(</span><span class='nv'>dates</span>, <span class='s'>"first"</span><span class='o'>)</span>,
         poll_end <span class='o'>=</span> <span class='nf'>clean_date</span><span class='o'>(</span><span class='nv'>dates</span>, <span class='s'>"second"</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>dates</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/filter.html'>filter</a></span><span class='o'>(</span><span class='o'>!</span><span class='nf'><a href='https://rdrr.io/r/base/NA.html'>is.na</a></span><span class='o'>(</span><span class='nv'>points</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/distinct.html'>distinct</a></span><span class='o'>(</span><span class='o'>)</span>

<span class='nv'>polls_clean</span></code></pre>

</div>

### Visualization

The cleaned data can be aggregated and visualized.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>avg_polls</span> <span class='o'>&lt;-</span> <span class='nv'>polls_clean</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/group_by.html'>group_by</a></span><span class='o'>(</span><span class='nv'>candidate</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/summarise.html'>summarise</a></span><span class='o'>(</span>avg_points <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/mean.html'>mean</a></span><span class='o'>(</span><span class='nv'>points</span>, na.rm <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span>,
            min_points <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/Extremes.html'>min</a></span><span class='o'>(</span><span class='nv'>points</span>, na.rm <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span>,
            max_points <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/Extremes.html'>max</a></span><span class='o'>(</span><span class='nv'>points</span>, na.rm <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span>,
            n_polls <span class='o'>=</span> <span class='nf'><a href='https://dplyr.tidyverse.org/reference/context.html'>n</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>-</span> <span class='nf'><a href='https://rdrr.io/r/base/sum.html'>sum</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/NA.html'>is.na</a></span><span class='o'>(</span><span class='nv'>points</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> <span class='c'># identify how many polls candidate is in</span>
  <span class='c'># remove candidates who appear in 50 or fewer polls: i.e. HRC</span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/filter.html'>filter</a></span><span class='o'>(</span><span class='nv'>n_polls</span> <span class='o'>&gt;</span> <span class='m'>50</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/arrange.html'>arrange</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>avg_points</span><span class='o'>)</span>

<span class='nv'>avg_polls</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>avg_polls</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate.html'>mutate</a></span><span class='o'>(</span>candidate <span class='o'>=</span> <span class='nf'><a href='https://forcats.tidyverse.org/reference/fct_reorder.html'>fct_reorder</a></span><span class='o'>(</span><span class='nv'>candidate</span>, <span class='nv'>avg_points</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggplot.html'>ggplot</a></span><span class='o'>(</span><span class='nf'><a href='https://ggplot2.tidyverse.org/reference/aes.html'>aes</a></span><span class='o'>(</span><span class='nv'>candidate</span>, <span class='nv'>avg_points</span><span class='o'>)</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/geom_bar.html'>geom_col</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>+</span> 
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggtheme.html'>theme_minimal</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/coord_flip.html'>coord_flip</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/labs.html'>labs</a></span><span class='o'>(</span>title <span class='o'>=</span> <span class='s'>"Polls Standings"</span>, x <span class='o'>=</span> <span class='s'>""</span>, y <span class='o'>=</span> <span class='s'>"%"</span><span class='o'>)</span></code></pre>

</div>

## Creating historic polling data

It may become useful to have a running history of how candidates have been polling. We can use R to write a csv file of the data from FiveThirtyEight. However, what happens when the polls update? How we can we keep the previous data *and* the new data? We will work through an example using a combination of [`bind_rows()`](https://dplyr.tidyverse.org/reference/bind.html) and [`distinct()`](https://dplyr.tidyverse.org/reference/distinct.html). I want to emphasize that this is not a good practice if you need to scale to hundreds of thousand of rows. This works in this case as the data are inherently small.

To start, I have created a sample dataset which contains 80% of these polls (maybe less by the time you do this!). Note that is probably best to version control this or have multiple copies as a failsafe.

The approach we will take is to read in the historic polls data set and bind rows with the `polls_clean` data we have scraped. Next we remove duplicate rows using [`distinct()`](https://dplyr.tidyverse.org/reference/distinct.html).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>old_polls</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://readr.tidyverse.org/reference/read_delim.html'>read_csv</a></span><span class='o'>(</span><span class='s'>"https://raw.githubusercontent.com/JosiahParry/r-4-campaigns/master/data/polls.csv"</span><span class='o'>)</span>

<span class='nv'>old_polls</span>

<span class='nv'>updated_polls</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://dplyr.tidyverse.org/reference/bind.html'>bind_rows</a></span><span class='o'>(</span><span class='nv'>old_polls</span>, <span class='nv'>polls_clean</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/distinct.html'>distinct</a></span><span class='o'>(</span><span class='o'>)</span>

<span class='nv'>updated_polls</span></code></pre>

</div>

Now you have a cleaned data set which has been integrated with the recently scraped data. Write this to a csv using [`write_csv()`](https://readr.tidyverse.org/reference/write_delim.html) for later use.

