---
title: "R Security Concerns and 2019 CRAN downloads"

date: '2020-02-04'
slug: cran-2019
categories:
  - R
tags:
  - R
  - R admins
  - tutorial
  - security
rmd_hash: 50e02934257d6020

---

The number of downloads that a package has can tell us a number of things. For example, how much other packages rely on it. A great case of this is [`rlang`](https://github.com/r-lib/rlang). Not many of use `rlang`, know what it does, or even bother to dive into it. But in reality it is the backbone of so many packages that we know and love and use every day.

The other thing that we can infer from the download numbers is how trusted a package may be. Packages that have been downloaded time and time and again have *likely* been scoped out thoroughly for any bugs. This can be rather comforting for security conscious groups.

With a little [`rvest`](https://github.com/tidyverse/rvest)-web-scraping-magic we can obtain the name of every package published to [CRAN](https://cran.r-project.org/).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://rvest.tidyverse.org/'>rvest</a></span><span class='o'>)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span>
<span class='nv'>url</span> <span class='o'>&lt;-</span> <span class='s'>"https://cran.r-project.org/web/packages/available_packages_by_name.html"</span>
<span class='nv'>cran_packages</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rvest.tidyverse.org/reference/rename.html'>html_session</a></span><span class='o'>(</span><span class='nv'>url</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://rvest.tidyverse.org/reference/rename.html'>html_nodes</a></span><span class='o'>(</span><span class='s'>"table a"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://rvest.tidyverse.org/reference/html_text.html'>html_text</a></span><span class='o'>(</span><span class='o'>)</span></code></pre>

</div>

RStudio keeps daily logs of packages downloaded from their CRAN mirror. The package [`cranlogs`](https://r-hub.github.io/cranlogs) makes the data available via and API and an R package. Below is a lightweight function which sums the total number of downloads for the entire year. `furrr` was used to speed up the computation a bit. This takes \~40 minutes to run, as such use the exported data in `data/cran-2019-total-downloads.csv`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>get_year_downloads</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>pkg</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='nf'>cranlogs</span><span class='nf'>::</span><span class='nf'>cran_downloads</span><span class='o'>(</span><span class='nv'>pkg</span>, 
                           from <span class='o'>=</span> <span class='s'>"2019-01-01"</span>,
                           to <span class='o'>=</span> <span class='s'>"2019-12-31"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/group_by.html'>group_by</a></span><span class='o'>(</span><span class='nv'>package</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/summarise.html'>summarise</a></span><span class='o'>(</span>total_downloads <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/sum.html'>sum</a></span><span class='o'>(</span><span class='nv'>count</span><span class='o'>)</span><span class='o'>)</span>
<span class='o'>&#125;</span>
<span class='nv'>total_cran_downloads</span> <span class='o'>&lt;-</span> <span class='nf'>furrr</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/furrr/man/future_map.html'>future_map_dfr</a></span><span class='o'>(</span>
  <span class='nv'>cran_packages</span>, 
  .f <span class='o'>=</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/safely.html'>possibly</a></span><span class='o'>(</span><span class='nv'>get_year_downloads</span>, otherwise <span class='o'>=</span> <span class='nf'><a href='https://tibble.tidyverse.org/reference/tibble.html'>tibble</a></span><span class='o'>(</span><span class='o'>)</span><span class='o'>)</span>
  <span class='o'>)</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>total_cran_downloads</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://readr.tidyverse.org/reference/read_delim.html'>read_csv</a></span><span class='o'>(</span><span class='s'>"data/cran-2019-total-downloads.csv"</span><span class='o'>)</span>
<span class='nf'><a href='https://pillar.r-lib.org/reference/glimpse.html'>glimpse</a></span><span class='o'>(</span><span class='nv'>total_cran_downloads</span><span class='o'>)</span></code></pre>

</div>

While this is helpful, these packges also have dependencies, and those dependencies have dependencies. The R core team have built out the `tools` package which contains, yes, wonderful tools. The function [`tools::package_dependencies()`](https://rdrr.io/r/tools/package_dependencies.html) provides us with the dependencies.

The below code identifies the top 500 downloaded packages and their dependencies.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>top_500_cran</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://dplyr.tidyverse.org/reference/top_n.html'>top_n</a></span><span class='o'>(</span><span class='nv'>total_cran_downloads</span>, <span class='m'>500</span>, <span class='nv'>total_downloads</span><span class='o'>)</span>
<span class='c'>#pull the package dependencies</span>
<span class='nv'>pkg_deps</span> <span class='o'>&lt;-</span> <span class='nv'>top_500_cran</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/pull.html'>pull</a></span><span class='o'>(</span><span class='nv'>package</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
  <span class='nf'>tools</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/r/tools/package_dependencies.html'>package_dependencies</a></span><span class='o'>(</span>recursive <span class='o'>=</span> <span class='kc'>TRUE</span><span class='o'>)</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/utils/head.html'>head</a></span><span class='o'>(</span><span class='nv'>pkg_deps</span><span class='o'>)</span></code></pre>

</div>

## Package Checks

Each package on CRAN goes through a rigorous checking process. The r cmd check is ran on each package for twelve different flavors from a combination of Linux and Windows. If you trust the checks that the R Core team do, I wouldn't reinvent the wheel.

The data are not provided directly from CRAN though an individual has provided these data via an API. I recommend using the API as a check against packages on ingest. I'd also do this process for every time you're syncing. Again, this doesn't mean that there are no vulnerabilities. But if there are functions that will literally break the machine, then the checks in general shouldn't work.

The biggest risk is really in the development and publication of applications. The greatest risk you are likely to face are going to be internal or accidental. For example using base R and Shiny, a developer can make an app unintentionally malicious---i.e. permitting system calls or creating a SQL injection. Though this would be rather difficult to build into an app, it is possible. The process here would be to institute a peer review process for the apps developed. Also, you're going to want to sandbox the applications---which Connect does and will improve with launcher in the future.

## Instituting Checks

We can use the [`cchecks`](https://github.com/ropenscilabs/cchecks) packge to interact with R-Hub's CRAN check API. They have done a wonderful job aggregating package check data. The data it returns, however, is in a rather deeply nested list. Below is a function defintion which can tidy up some of the important information produced from the API query.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'>#devtools::install_github("ropenscilabs/cchecks")</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'>cchecks</span><span class='o'>)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span>
<span class='nv'>tidy_checks</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>checks</span><span class='o'>)</span> <span class='o'>&#123;</span>
  
  <span class='nv'>check_res</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>checks</span>, <span class='nv'>pluck</span>, <span class='s'>"data"</span>, <span class='s'>"checks"</span><span class='o'>)</span>
  <span class='nv'>check_pkg</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>checks</span>, <span class='nv'>pluck</span>, <span class='s'>"data"</span>, <span class='s'>"package"</span><span class='o'>)</span>
  <span class='nv'>check_deets</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>checks</span>, <span class='nv'>pluck</span>, <span class='s'>"data"</span>, <span class='s'>"check_details"</span><span class='o'>)</span>
  <span class='nv'>check_summ</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>checks</span>, <span class='nv'>pluck</span>, <span class='s'>"data"</span>, <span class='s'>"summary"</span><span class='o'>)</span>
  
  <span class='nf'><a href='https://tibble.tidyverse.org/reference/tibble.html'>tibble</a></span><span class='o'>(</span>pkg <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/unlist.html'>unlist</a></span><span class='o'>(</span><span class='nv'>check_pkg</span><span class='o'>)</span>,
         check_results <span class='o'>=</span> <span class='nv'>check_res</span>,
         check_details <span class='o'>=</span> <span class='nv'>check_deets</span>,
         check_summary <span class='o'>=</span> <span class='nv'>check_summ</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'><a href='https://tidyr.tidyverse.org/reference/hoist.html'>unnest_wider</a></span><span class='o'>(</span><span class='nv'>check_summary</span><span class='o'>)</span>
  
<span class='o'>&#125;</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># query the API for packages</span>
<span class='nv'>checks</span>  <span class='o'>&lt;-</span> <span class='nf'>cch_pkgs</span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='s'>"spotifyr"</span>, <span class='s'>"genius"</span><span class='o'>)</span><span class='o'>)</span>
<span class='c'># tidy up the checks</span>
<span class='nv'>clean_checks</span> <span class='o'>&lt;-</span> <span class='nf'>tidy_checks</span><span class='o'>(</span><span class='nv'>checks</span><span class='o'>)</span>
<span class='c'># get check results</span>
<span class='nv'>clean_checks</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://tidyr.tidyverse.org/reference/nest.html'>unnest</a></span><span class='o'>(</span><span class='nv'>check_results</span><span class='o'>)</span>
<span class='c'># get check details</span>
<span class='nv'>clean_checks</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://tidyr.tidyverse.org/reference/hoist.html'>unnest_wider</a></span><span class='o'>(</span><span class='nv'>check_details</span><span class='o'>)</span></code></pre>

</div>

You can pull these checks for the top 500 packages and their dependencies in a rather straightforward manner now. You can iterate through these all. Note that this is an API and you may see some lag time. So go make some tea.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>top_checks</span> <span class='o'>&lt;-</span> <span class='nf'>cch_pkgs</span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/utils/head.html'>head</a></span><span class='o'>(</span><span class='nv'>top_500_cran</span><span class='o'>$</span><span class='nv'>package</span><span class='o'>)</span><span class='o'>)</span>
<span class='nf'>tidy_checks</span><span class='o'>(</span><span class='nv'>top_checks</span><span class='o'>)</span></code></pre>

</div>

### Resources

-   <https://environments.rstudio.com/validation.html>
-   <https://www.r-bloggers.com/overview-of-the-cran-checks-api/amp>
-   <https://blog.r-hub.io/2019/04/25/r-devel-linux-x86-64-debian-clang/>

