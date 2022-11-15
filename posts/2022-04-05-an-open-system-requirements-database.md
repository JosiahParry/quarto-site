---
title: Actually identifying R package System Requirements 
author: Josiah Parry
date: '2022-04-05'
slug: []
categories: []
tags: [RSPM, admin, R, ]
rmd_hash: 7b33d50138f67c39

---

During my approximately three years at RStudio there were two things that stumped system admins more than anything: proxied authentication and system dependencies for R package (god help anyone trying to install rgdal on RHEL 7). When RStudio Package Manager (RSPM) [v1.0.8](https://docs.rstudio.com/rspm/news/#rstudio-package-manager-108) was released there was finally an answer. RSPM can help you identify system requirements via the GUI. Also, there's a restful API that isn't fully supported but can provide these system requirements programatically if needed. As such, I think it is still a little used feature for most RSPM users and admins.

`{pak}` did a great job of providing an interface to the public installation of RSPM. Back in May 2021 [I suggested](https://github.com/r-lib/pak/issues/299) that the API calls to RSPM be made publicly available. Since then [`pak::pkg_system_requirements()`](http://pak.r-lib.org/reference/local_system_requirements.html) has become an exported function. It is *exceptionally* useful. I use it in my current work to create a bash script which installs system requirements into a fresh Databricks compute cluster and then install R packages from RSPM.

One of my qualms about the RSPM API and thus the output of pak is that it *always* includes the installation commands for the provided OS--e.g. `apt-get install -y` which I suppose could be easily [`stringr::str_remove()`](https://stringr.tidyverse.org/reference/str_remove.html)d.

The second qualm has been that this relies on RSPM. The logic has been semi-hidden behind a closed-source tool. However, RStudio maintains a repository [`r-system-requirements`](https://github.com/rstudio/r-system-requirements/) which is used by RSPM to identify system requirements from a packages `DESCRIPTION` file.

All of the logic for RSPM is in that repository. And that's why I made `https://r-sysreqs.josiahparry.com`. It's a way to provide the REST API functionality from RSPM without having to rely strictly on the public installation.

Users can use the functionality from [`{sysreqs}`](https://github.com/JosiahParry/r-sysreqs) to make this api available on their own machines.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>sysreqs</span><span class='nf'>::</span><span class='nf'><a href='https://r-sysreqs.josiahparry.com/reference/get_pkgs_sysreqs.html'>get_pkgs_sysreqs</a></span><span class='o'>(</span><span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='s'>"rgdal"</span>, <span class='s'>"igraph"</span><span class='o'>)</span>,
                          <span class='s'>"ubuntu"</span>, <span class='s'>"18.04"</span><span class='o'>)</span> |&gt; 
  <span class='nf'>tidyr</span><span class='nf'>::</span><span class='nf'><a href='https://tidyr.tidyverse.org/reference/nest.html'>unnest</a></span><span class='o'>(</span><span class='nv'>dependencies</span><span class='o'>)</span>
<span class='c'>#&gt; <span style='color: #555555;'># A tibble: 6 × 5</span></span>
<span class='c'>#&gt;   pkg    name    dependencies pre_installs post_installs</span>
<span class='c'>#&gt;   <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>  <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>   <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>        <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>        <span style='color: #555555; font-style: italic;'>&lt;chr&gt;</span>        </span>
<span class='c'>#&gt; <span style='color: #555555;'>1</span> rgdal  gdal    libgdal-dev  <span style='color: #BB0000;'>NA</span>           <span style='color: #BB0000;'>NA</span>           </span>
<span class='c'>#&gt; <span style='color: #555555;'>2</span> rgdal  gdal    gdal-bin     <span style='color: #BB0000;'>NA</span>           <span style='color: #BB0000;'>NA</span>           </span>
<span class='c'>#&gt; <span style='color: #555555;'>3</span> rgdal  proj    libproj-dev  <span style='color: #BB0000;'>NA</span>           <span style='color: #BB0000;'>NA</span>           </span>
<span class='c'>#&gt; <span style='color: #555555;'>4</span> igraph glpk    libglpk-dev  <span style='color: #BB0000;'>NA</span>           <span style='color: #BB0000;'>NA</span>           </span>
<span class='c'>#&gt; <span style='color: #555555;'>5</span> igraph gmp     libgmp3-dev  <span style='color: #BB0000;'>NA</span>           <span style='color: #BB0000;'>NA</span>           </span>
<span class='c'>#&gt; <span style='color: #555555;'>6</span> igraph libxml2 libxml2-dev  <span style='color: #BB0000;'>NA</span>           <span style='color: #BB0000;'>NA</span></span></code></pre>

</div>

