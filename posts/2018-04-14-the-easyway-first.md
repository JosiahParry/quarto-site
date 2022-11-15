---
title: "Coursera R-Programming: Week 2 Problems"
subtitle: "The Tidy Approach"

date: '2018-04-14'
slug: tidy-coursera-r-programming
categories:
  - R
  - tutorial
tags:
  - R
  - tidyverse
  - coursera
  - tutorial
rmd_hash: 01d3399395026cb3

---

Over the past several weeks I have been helping students, career professionals, and people of other backgrounds learn R. During this time one this has become apparent, people are teaching the old paradigm of R and avoiding the tidyverse all together.

I recently had a student reach out to me in need of help with the first programming assignment from the [Coursera R-Programming](https://www.coursera.org/learn/r-programming) course (part of the Johns Hopkins Data Science Specialization). This particular student was struggling with combining the her new knowledge of R data types, conditional statements, looping, control statements, scoping, and functions to solve the assignment problem set. I provided her with a walk through of each question in base R, the style of the course. I couldn't help but empathize with her as I too learned the long way first. However I thought that she shouldn't be learning the hard way first (see David Robinson's [blog post](http://varianceexplained.org/r/teach-hard-way/), *"Don't teach students the hard way first"*), she should be learning the effective way.

In my written response to her, I gave her solutions to her problems in base R and using the tidyverse. Here, I will go over the problems and adress them from a tidy perspective. This will not serve as a full introduction to the tidyverse. For an introduction and a reason why the tidyverse is superior to base R, I leave you with [**Stat 545**: *Introduction to dplyr*](http://stat545.com/block009_dplyr-intro.html)

The assignment utilizes a directory of data called `specdata` which can be downloaded [**here**](https://d396qusza40orc.cloudfront.net/rprog%2Fdata%2Fspecdata.zip), and describes it:

> The zip file contains 332 comma-separated-value (CSV) files containing pollution monitoring data for fine particulate matter (PM) air pollution at 332 locations in the United States. Each file contains data from a single monitor and the ID number for each monitor is contained in the file name. For example, data for monitor 200 is contained in the file "200.csv". Each file contains three variables:

> -   **Date**: the date of the observation in `YYYY-MM-DD` format (year-month-day)
> -   **sulfate**: the level of sulfate PM in the air on that date (measured in micrograms per cubic meter)
> -   **nitrate**: the level of nitrate PM in the air on that date (measured in micrograms per cubic meter)

> For this programming assignment you will need to unzip this file and create the directory 'specdata'. Once you have unzipped the zip file, do not make any modifications to the files in the 'specdata' directory. In each file you'll notice that there are many days where either sulfate or nitrate (or both) are missing (coded as NA). This is common with air pollution monitoring data in the United States.

------------------------------------------------------------------------

# Part I

Problem 1:

> Write a function named 'pollutantmean' that calculates the mean of a pollutant (sulfate or nitrate) across a specified list of monitors. The function 'pollutantmean' takes three arguments: 'directory', 'pollutant', and 'id'. Given a vector monitor ID numbers, 'pollutantmean' reads that monitors' particulate matter data from the directory specified in the 'directory' argument and returns the mean of the pollutant across all of the monitors, ignoring any missing values coded as NA. A prototype of the function is as follows

     pollutantmean <- function(directory, pollutant, id = 1:332){
        ## 'directory' is a character vector of length 1 indicating
        ## the location of the CSV files
        
        ## 'pollutant' is a character vector of length 1 indicating
        ## the name of the pollutant for which we will calculate the 
        ## mean; either "sulfate" or "nitrate"

        ## 'id' is an integer vector indicating the monitor ID numbers
        ## to be used

        ## Return the mean of the pollutant across all monitors list
        ## in the 'id' vector (ignoring NA values)
        ## NOTE: Do not round the result!
    }

Before we tackle the function, I believe the best approach is to first solve the problem in a regular script. This problem has four clear steps:

1.  Identify files in the directory
2.  Subset files based on provided ID
3.  Read the files
4.  Calculate and return the mean on the desired column

This problem gives us a directory of files from which we need to read in the data based on the provided IDs. For the sake of this walk through we will randomly sample 10 values within the range designated in the problem statement (332).

We will first generate random IDs, then identify all of the files within the specified directory and obtain their file paths using the [`list.files()`](https://rdrr.io/r/base/list.files.html) function. After this we will subset our file list based on the IDs, then iterate over our file list and read in each file as a csv using `purrr:map_df()` combined with [`readr::read_csv()`](https://readr.tidyverse.org/reference/read_delim.html). Fortunately [`map_df()`](https://purrr.tidyverse.org/reference/map.html) returns a nice and pretty data frame which lets us avoid having to explicitly bind each unique data frame.

#### Identify Files

Here we create 10 random IDs and store them in the `ids` variable. Next we use [`list.files()`](https://rdrr.io/r/base/list.files.html) to look within the `specdata` folder that we downloaded above. Everyone's path will most likely be different. Be sure to obtain the correct file path---help for [Mac](http://osxdaily.com/2013/06/19/copy-file-folder-path-mac-os-x/).

Next we identify the files we need based on the sampled `ids` and store the subset in the `files_filtered` variable. We use the values of the `ids` to locate the file paths positionally. For example, ID number 1 is the first file, number 10 is the tenth, etc.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># Load our handy dandy functions</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span>

<span class='c'># 10 random IDs in ID range</span>
<span class='nv'>ids</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/sample.html'>sample</a></span><span class='o'>(</span><span class='m'>1</span><span class='o'>:</span><span class='m'>332</span>, <span class='m'>10</span><span class='o'>)</span>

<span class='c'># Identify all files within the directory</span>
<span class='nv'>files</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/list.files.html'>list.files</a></span><span class='o'>(</span><span class='s'>"../../data/specdata"</span>, full.names <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span>

<span class='c'># Subset the data</span>
<span class='nv'>files_filtered</span> <span class='o'>&lt;-</span> <span class='nv'>files</span><span class='o'>[</span><span class='nv'>ids</span><span class='o'>]</span>

<span class='c'># View the files to verify</span>
<span class='nf'><a href='https://rdrr.io/r/base/paste.html'>paste</a></span><span class='o'>(</span><span class='nv'>ids</span>, <span class='nv'>files_filtered</span><span class='o'>)</span></code></pre>

</div>

#### Reading the Files

Now that we have identified the files that we are going to read in, we can use `purrr:map_df()` to apply the [`readr::read_csv()`](https://readr.tidyverse.org/reference/read_delim.html) function to each value of `files_filtered` and return a data frame (hence the `_df()` suffix). We supply additional arguments to [`read_csv()`](https://readr.tidyverse.org/reference/read_delim.html) to ensure that every column is read in properly.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># Read in the subset of the data. Notice the brackets after files[]</span>
<span class='nv'>specdata</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map_df</a></span><span class='o'>(</span><span class='nv'>files_filtered</span>, <span class='nv'>read_csv</span>, 
                   col_types <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>
                     <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_datetime.html'>col_date</a></span><span class='o'>(</span><span class='o'>)</span>,
                     <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_double</a></span><span class='o'>(</span><span class='o'>)</span>,
                     <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_double</a></span><span class='o'>(</span><span class='o'>)</span>,
                     <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_integer</a></span><span class='o'>(</span><span class='o'>)</span>
                   <span class='o'>)</span><span class='o'>)</span>

<span class='nf'><a href='https://pillar.r-lib.org/reference/glimpse.html'>glimpse</a></span><span class='o'>(</span><span class='nv'>specdata</span><span class='o'>)</span></code></pre>

</div>

Next, we get to utilize some `dplyr` magic. Here we take the `specdata` object we created from reading in our files, deselct the `Date` column, then utilize [`summarise_if()`](https://dplyr.tidyverse.org/reference/summarise_all.html) to apply the [`mean()`](https://rdrr.io/r/base/mean.html) function to our data. [`summarise_if()`](https://dplyr.tidyverse.org/reference/summarise_all.html) requires that we provide a logical statement as the first argument. *If* (hence the `_if()` suffix) the logical statement evaluates to `TRUE` on a column then it will apply a list of functions to those columns where the statement evaluated to `TRUE`. We can also specify additional arguments to the functions. Here we specify `na.rm = TRUE` for handling missing values.

In this case, we are checking to see if our columns are of the data type `double` using the [`is.double()`](https://rdrr.io/r/base/double.html) function. If you're wondering why we didn't use [`is.numeric()`](https://rdrr.io/r/base/numeric.html), it's because the `ID` column is an integer which is considered numeric.

If we wanted to take the underlying vector of one of the columns, we can also, use `dplyr::pull(col_name)`. This will be helpful later when we want to obtain the mean of just one column.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># Obtain mean nitrate</span>
<span class='nv'>specdata</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>Date</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/summarise_all.html'>summarise_if</a></span><span class='o'>(</span><span class='nv'>is.double</span>, <span class='nv'>mean</span>, na.rm <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='c'># Pull just the sulfate column</span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/pull.html'>pull</a></span><span class='o'>(</span><span class='nv'>sulfate</span><span class='o'>)</span>

<span class='nv'>specdata</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>Date</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/summarise_all.html'>summarise_if</a></span><span class='o'>(</span><span class='nv'>is.double</span>, <span class='nv'>mean</span>, na.rm <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='c'># Pull just the nitrate column</span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/pull.html'>pull</a></span><span class='o'>(</span><span class='nv'>nitrate</span><span class='o'>)</span>
</code></pre>

</div>

Now that we have all of the tools, we can put this together into a single function, which I will call `pollutant_mean()` to somewhat adhere---functions should take the name of verbs---to the tidyverse style guide.

Here we have three arguments:

-   `directory`: Where to look for the files
-   `pollutant`: Which pollutant (nitrate or sulfate) to evaluate
    -   This needs to be a character value unless we want to get into [tidyeval](https://dplyr.tidyverse.org/articles/programming.html), which frankly I will leave to the professionals. But I will provide an alternative solution at the end that doesn't require quoted `pollutant` names.
-   `id`: Which monitoring stations we should look at

Within the function we take everything we did in the above steps but generalize it to a function. We identify the files in the directory provided (`specdata`), subset the files positionally based on the provided `id` vector, and then iterate over the file names and read them in with [`map_df()`](https://purrr.tidyverse.org/reference/map.html) and [`read_csv()`](https://readr.tidyverse.org/reference/read_delim.html).

Next we take our data and calculate the mean on both `sulfate` and `nitrate` columns. We then [`pull()`](https://dplyr.tidyverse.org/reference/pull.html) the specified column from the `pollutant` argument and then [`return()`](https://rdrr.io/r/base/function.html) that value.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'>
<span class='nv'>pollutant_mean</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>directory</span>, <span class='nv'>pollutant</span>, <span class='nv'>id</span> <span class='o'>=</span> <span class='m'>1</span><span class='o'>:</span><span class='m'>332</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='nv'>files</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/list.files.html'>list.files</a></span><span class='o'>(</span><span class='nv'>directory</span>, full.names <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span>
  <span class='nv'>files_filtered</span> <span class='o'>&lt;-</span> <span class='nv'>files</span><span class='o'>[</span><span class='nv'>id</span><span class='o'>]</span>
  <span class='nv'>specdata</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map_df</a></span><span class='o'>(</span><span class='nv'>files_filtered</span>, <span class='nv'>read_csv</span>, 
                     col_types <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_datetime.html'>col_date</a></span><span class='o'>(</span><span class='o'>)</span>,
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_double</a></span><span class='o'>(</span><span class='o'>)</span>,
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_double</a></span><span class='o'>(</span><span class='o'>)</span>,
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_integer</a></span><span class='o'>(</span><span class='o'>)</span>
                     <span class='o'>)</span><span class='o'>)</span>
  
  <span class='nv'>specdata</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>Date</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/summarise_all.html'>summarise_if</a></span><span class='o'>(</span><span class='nv'>is.double</span>, <span class='nv'>mean</span>, na.rm <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/pull.html'>pull</a></span><span class='o'>(</span><span class='nv'>pollutant</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='o'>)</span>

<span class='o'>&#125;</span></code></pre>

</div>

Here we can test out the function with both types of pollutants and different `id` values.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>pollutant_mean</span><span class='o'>(</span>directory <span class='o'>=</span> <span class='s'>"../../data/specdata"</span>, pollutant <span class='o'>=</span> <span class='s'>"sulfate"</span>, id <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/sample.html'>sample</a></span><span class='o'>(</span><span class='m'>1</span><span class='o'>:</span><span class='m'>332</span>, <span class='m'>20</span><span class='o'>)</span><span class='o'>)</span>
<span class='c'>#pollutant_mean("../../data/specdata", "nitrate", 2)</span></code></pre>

</div>

------------------------------------------------------------------------

# Part II:

Let us continue to the second problem in the problem set.

Problem 2:

> Write a function that reads a directory full of files and reports the number of completely observed cases in each data file. The function should return a data frame where the first column is the name of the file and the second column is the number of complete cases.

The assignment provides an example function format, but I think it to be a bit misleading. So I will go about this in the way I think is best. We will work on creating a function called `complete_spec_cases()` which will take only two arguments, `directory`, and `id`. `directory` and `id` will be used in the the same way as the previous problem.

For this problem our goal is to identify how many complete cases there are by provided ID. This should be exceptionally simple. We will have to identify our files, subset them, and read them in the same way as before. Next we can identify complete cases by piping our `specdata` object to [`na.omit()`](https://rdrr.io/r/stats/na.fail.html) which will remove any row with a missing value. Next, we have to group by the `ID` column and pipe our grouped data frame to [`count()`](https://dplyr.tidyverse.org/reference/count.html) which will count how many observations there are by group. We will then return this data frame to the user.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>complete_spec_cases</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>directory</span>, <span class='nv'>id</span> <span class='o'>=</span> <span class='m'>1</span><span class='o'>:</span><span class='m'>332</span><span class='o'>)</span> <span class='o'>&#123;</span>

  <span class='nv'>files</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/list.files.html'>list.files</a></span><span class='o'>(</span><span class='nv'>directory</span>, full.names <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span>
  
  <span class='nv'>specdata</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map_df</a></span><span class='o'>(</span><span class='nv'>files</span><span class='o'>[</span><span class='nv'>id</span><span class='o'>]</span>, <span class='nv'>read_csv</span>,
                     col_types <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_datetime.html'>col_date</a></span><span class='o'>(</span><span class='o'>)</span>,
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_double</a></span><span class='o'>(</span><span class='o'>)</span>,
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_double</a></span><span class='o'>(</span><span class='o'>)</span>,
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_integer</a></span><span class='o'>(</span><span class='o'>)</span>
                     <span class='o'>)</span><span class='o'>)</span>
  
  <span class='nv'>complete_specdata</span> <span class='o'>&lt;-</span> <span class='nv'>specdata</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://rdrr.io/r/stats/na.fail.html'>na.omit</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/group_by.html'>group_by</a></span><span class='o'>(</span><span class='nv'>ID</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/summarise.html'>summarise</a></span><span class='o'>(</span>nobs <span class='o'>=</span> <span class='nf'><a href='https://dplyr.tidyverse.org/reference/context.html'>n</a></span><span class='o'>(</span><span class='o'>)</span><span class='o'>)</span>
  
  <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nv'>complete_specdata</span><span class='o'>)</span>
<span class='o'>&#125;</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>complete_spec_cases</span><span class='o'>(</span><span class='s'>"../../data/specdata"</span>, id <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/sample.html'>sample</a></span><span class='o'>(</span><span class='m'>1</span><span class='o'>:</span><span class='m'>332</span>, <span class='m'>20</span><span class='o'>)</span><span class='o'>)</span></code></pre>

</div>

------------------------------------------------------------------------

# Part III:

This final problem is probably the most complicated, but with the method we just used above and with a bit more help from the `purrr` and `dplyr` packages, we can do this no problem.

Problem 3:

> Write a function that takes a directory of data files and a threshold for complete cases and calculates the correlation between sulfate and nitrate for monitor locations where the *number of completely observed cases* (on all variables) is greater than the threshold. The function should return a vector of correlations for the monitors that meet the threshold requirement. If no monitors meet the threshold requirement, then the function should return a numeric vector of length 0. A prototype of this function follows:

    Correct <- function(directory, threshold = 0){
        ## 'directory' is a character vector of length 1 indicating
        ## the location of the CSV files

        ## 'threshold' is a numeric vector of length 1 indicating the
        ## number of completely observed observations (on all
        ## variables) required to compute the correlation between
        ## nitrate and sulfate; the default is 0

        ## Return a numeric vector of correlations
        ## NOTE: Do not round the result!
    }

Let keep this simple. The above statement essentially is asking that we find the correlation between `nitrate` and `sulfate` for each monitoring station (ID). But there is a catch! Each ID must meet a specified threshold of complete cases, and if none of the monitors meet the requirement the function must return a `numeric(0)`.

The way we will structure this function will be to first read in the data---as we have done twice now, except this time there will be no subsetting of IDs. Then we need to identify the number of complete cases by ID---as we did in problem 2---and identify the stations that meet the threshold requirement. At this point we will use an `if` statement to check if we have at least 1 monitoring station that meets our threshold, if we do not, we return the `numeric(0)`---there is most likely a more tidy way to do this, but I am not aware. If we have at least 1 monitoring station that meets the specified threshold we will use an [`inner_join()`](https://dplyr.tidyverse.org/reference/mutate-joins.html) to make sure that `specdata` contains only those IDs that meet the requirement.

For the sake of this example, we will continue to use the `specdata` object we created in previous examples, and we will set our threshold to `100.` Once we identify the stations with the proper number of counts (`> 100`), we will store that data frame in an object called `id_counts`

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'>
<span class='nv'>id_counts</span> <span class='o'>&lt;-</span> <span class='nv'>specdata</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://rdrr.io/r/stats/na.fail.html'>na.omit</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/group_by.html'>group_by</a></span><span class='o'>(</span><span class='nv'>ID</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/count.html'>count</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/filter.html'>filter</a></span><span class='o'>(</span><span class='nv'>n</span> <span class='o'>&gt;</span> <span class='m'>100</span><span class='o'>)</span> 
  
  <span class='kr'>if</span> <span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/nrow.html'>nrow</a></span><span class='o'>(</span><span class='nv'>id_counts</span><span class='o'>)</span> <span class='o'>&lt;</span> <span class='m'>1</span><span class='o'>)</span> <span class='o'>&#123;</span>
    <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/numeric.html'>numeric</a></span><span class='o'>(</span><span class='m'>0</span><span class='o'>)</span><span class='o'>)</span>
  <span class='o'>&#125;</span> <span class='kr'>else</span> <span class='o'>&#123;</span>
    <span class='nf'><a href='https://rdrr.io/r/base/print.html'>print</a></span><span class='o'>(</span><span class='s'>"All is well."</span><span class='o'>)</span>
  <span class='o'>&#125;</span>

  <span class='nv'>specdata</span> <span class='o'>&lt;-</span> <span class='nv'>id_counts</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate-joins.html'>inner_join</a></span><span class='o'>(</span><span class='nv'>specdata</span>, by <span class='o'>=</span> <span class='s'>"ID"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
    <span class='nf'><a href='https://rdrr.io/r/stats/na.fail.html'>na.omit</a></span><span class='o'>(</span><span class='o'>)</span> 
  
  <span class='nv'>specdata</span></code></pre>

</div>

This is where it gets kind of funky. Once we have filtered down our data set, we need to calculate the correlations for each ID. The way that we do this is by nesting our data frame on the `ID` column. Calling `nest(-ID)` allows us to, for each value of ID, create a data frame for just those rows where the `ID` is the same. We will then have a new list type column where each value is actually a data frame. Let's check out what this looks like before we hop into the function.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>specdata</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://tidyr.tidyverse.org/reference/nest.html'>nest</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>ID</span><span class='o'>)</span></code></pre>

</div>

Now that we know how to nest our data, we need to calculate the correlations for each row (ID value). We will do this by combining [`mutate()`](https://dplyr.tidyverse.org/reference/mutate.html) and [`map()`](https://purrr.tidyverse.org/reference/map.html). Here `.x` references the data that is within each nested tibble. To learn more about `purrr` I recommend the [chapter](http://r4ds.had.co.nz/iteration.html) on iteration from *R For Data Science*.

After we have done our calculations we undo our nesting using [`unnest()`](https://tidyr.tidyverse.org/reference/nest.html) on the new column we created, and deselect the `data` column.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>specdata</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://rdrr.io/r/stats/na.fail.html'>na.omit</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://tidyr.tidyverse.org/reference/nest.html'>nest</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>ID</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate.html'>mutate</a></span><span class='o'>(</span>correlation <span class='o'>=</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>data</span>, <span class='o'>~</span><span class='nf'><a href='https://rdrr.io/r/stats/cor.html'>cor</a></span><span class='o'>(</span><span class='nv'>.x</span><span class='o'>$</span><span class='nv'>sulfate</span>, <span class='nv'>.x</span><span class='o'>$</span><span class='nv'>nitrate</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://tidyr.tidyverse.org/reference/nest.html'>unnest</a></span><span class='o'>(</span><span class='nv'>correlation</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>data</span><span class='o'>)</span></code></pre>

</div>

We can now place these above examples within a new function called `pollutant_cor()`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>pollutant_cor</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>directory</span>, <span class='nv'>threshold</span> <span class='o'>=</span> <span class='m'>0</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='nv'>files</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/list.files.html'>list.files</a></span><span class='o'>(</span><span class='nv'>directory</span>, full.names <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span>
  <span class='nv'>specdata</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map_df</a></span><span class='o'>(</span><span class='nv'>files</span>, <span class='nv'>read_csv</span>, 
                     col_types <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_datetime.html'>col_date</a></span><span class='o'>(</span><span class='o'>)</span>,
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_double</a></span><span class='o'>(</span><span class='o'>)</span>,
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_double</a></span><span class='o'>(</span><span class='o'>)</span>,
                       <span class='nf'><a href='https://readr.tidyverse.org/reference/parse_atomic.html'>col_integer</a></span><span class='o'>(</span><span class='o'>)</span>
                     <span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> <span class='nf'><a href='https://rdrr.io/r/stats/na.fail.html'>na.omit</a></span><span class='o'>(</span><span class='o'>)</span>
  
  <span class='nv'>id_counts</span> <span class='o'>&lt;-</span> <span class='nv'>specdata</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/group_by.html'>group_by</a></span><span class='o'>(</span><span class='nv'>ID</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/count.html'>count</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/filter.html'>filter</a></span><span class='o'>(</span><span class='nv'>n</span> <span class='o'>&gt;</span> <span class='nv'>threshold</span><span class='o'>)</span> 
  
  <span class='kr'>if</span> <span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/nrow.html'>nrow</a></span><span class='o'>(</span><span class='nv'>id_counts</span><span class='o'>)</span> <span class='o'>&lt;</span> <span class='m'>1</span><span class='o'>)</span> <span class='o'>&#123;</span>
    <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/numeric.html'>numeric</a></span><span class='o'>(</span><span class='m'>0</span><span class='o'>)</span><span class='o'>)</span>
  <span class='o'>&#125;</span>
  
  <span class='nv'>correlations</span> <span class='o'>&lt;-</span> <span class='nv'>id_counts</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate-joins.html'>inner_join</a></span><span class='o'>(</span><span class='nv'>specdata</span>, by <span class='o'>=</span> <span class='s'>"ID"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://tidyr.tidyverse.org/reference/nest.html'>nest</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>ID</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate.html'>mutate</a></span><span class='o'>(</span>correlation <span class='o'>=</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>data</span>, <span class='o'>~</span><span class='nf'><a href='https://rdrr.io/r/stats/cor.html'>cor</a></span><span class='o'>(</span><span class='nv'>.x</span><span class='o'>$</span><span class='nv'>sulfate</span>, <span class='nv'>.x</span><span class='o'>$</span><span class='nv'>nitrate</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://tidyr.tidyverse.org/reference/nest.html'>unnest</a></span><span class='o'>(</span><span class='nv'>correlation</span><span class='o'>)</span>
  
  <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nv'>correlations</span><span class='o'>)</span>

  
<span class='o'>&#125;</span></code></pre>

</div>

We can now test our function against two different thresholds to see how it reacts.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>pollutant_cor</span><span class='o'>(</span><span class='s'>"../../data/specdata"</span>, <span class='m'>100</span><span class='o'>)</span></code></pre>

</div>

If we set the threshold to 100,000, we should expect a `numeric(0)`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>pollutant_cor</span><span class='o'>(</span><span class='s'>"../../data/specdata"</span>, <span class='m'>100000</span><span class='o'>)</span></code></pre>

</div>

It all works!

