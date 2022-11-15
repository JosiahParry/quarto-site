---
title: "Excel in pRod"
date: "2020-03-01"
tags:
  - plumber
  - Excel
  - R in prod
  - API
  - tutorial
  
rmd_hash: 9ea263b524aa3f58
html_dependencies:
- <script src="htmlwidgets-1.5.4/htmlwidgets.js"></script>
- <script src="es6shim-0.35.6/es6shim.js"></script>
- <script src="es7shim-6.0.0/es7shim.js"></script>
- <script src="graphre-0.1.3/graphre.js"></script>
- <script src="nomnoml-1.4.0/nomnoml.js"></script>
- <script src="nomnoml-binding-0.2.5/nomnoml.js"></script>

---

## or *how to incorporate excel into a production API using plumber* or, *a micro-excel-micro-service*.

I recently had a conversation that touched on using [`plumber`](https://rplumber.io) to automate the parsing of Excel documents for administering data science assets. This brings up some very interesting points:

1.  Excel is sometimes unavoidable and we need to be okay with that.
2.  How can we incorporate Excel into production?

> Note that this is no time to 💩 on Excel. It serves very real business purposes and unfortunately not everyone can learn to program 😕. Here's a fun one for the h8rs: almost every presidential election campaign's data program is based on the back of Google Sheets.

In this post I set out to explore if and how one can incorporate Excel into productionized code. Please see the [GitHub repository](https://github.com/JosiahParry/plumber-excel) for the code used here.

What does it mean to productionize code---aka put it in prod[^1]? There is no one definition of what this mean and each and every organization will operationalize it differently.

> An *operationalized definition*, at least in the social science perspective, is how a thing is defined so as to have a shared understanding of said thing.

Greg Wilson has defined it as "code that *isn't* going to make the operations team cry" [^2] (emphasis his). This is my favorite definition because it is whimsical, sardonic, honest, and acknowledges that the code will have to leave the data science inner circle.

As I understand it, the current data science discourse emphasizes the use of RESTful APIs as the best, or at least the dominant, way of *productionizing* code.

An API is an application programming interface. When I was first learning what APIs were my uncle told me to think of them as "machines talking to other machines." That's not so far off!

A RESTful API is a special kind of API that does ***re**presentational **s**tate **t**ransfer*. Frankly, I do not know what that really means. As I understand it, REST is actually an opinionated way of architecting APIs. RESTful APIs use HTTP requests which makes them very easy to access. RESTful APIs are key in developing micro-services and micro-services are at the core of putting code in prod[^3]. Within the python ecosystem Flask is one of the leading libraries for making micro-services. Within the R space a package called `plumber` is taking on that role.

We can envisage a hypothetical scenario in which we receive Excel files via some data collection process. Once that Excel file is received it is used to kick off some other process---e.g. report writing or data processing. Often people may create Shiny applications to provide a UI for uploading and data input. This is really great when we want to develop a user-centric interface. But what about when we want to automate the process or at least make the processing available to other tools? This is when we can turn to plumber as a way to create a micro-service to handle this.

<div class="highlight">

<div id="htmlwidget-77f7966dca011158b958" style="width:700px;height:415.296px;" class="nomnoml html-widget"></div>
<script type="application/json" data-for="htmlwidget-77f7966dca011158b958">{"x":{"code":"\n#fill: #FEFEFF\n#lineWidth: 1\n#zoom: 4\n#direction: right\n\n[<frame> Plumber API |\n   [<abstract>Recieve Excel File]-:> [<abstract>1: upload]\n   [<abstract>Recieve Excel File] -:> [2: use data]\n  ]\n","svg":false,"png":null},"evals":[],"jsHooks":[]}</script>

</div>

The above graphic (made with [`nomnoml`](https://github.com/javierluraschi/nomnoml)) illustrates two different ways we can approach this. First, we will receive the Excel file. From there we may want to upload the file into a shared drive, a database, or both. Alternatively, we may not want to store the data, but rather use it immediately.

From an API development perspective, we can imagine each process as an API *endpoint*. An endpoint is essentially a url which says where each application interaction will happen. In this small example, we will create two endpoints: `/read_excel` and `/upload`. The first will, you guessed it, read an Excel file that is sent with a request. The second will upload said file.

Before we can approach creating the API, we need to first figure out how we can even send a file through an API. And before we can figure that out, we need to know what type of requests we can make to an API. Since the REST API will be an HTTP API, its imperative we know what type of requests we can make with an HTTP protocol. There are 7 HTTP request types.

-   GET
-   POST
-   PUT
-   HEAD
-   DELETE
-   PATCH
-   OPTION

Frankly, I do not remember what `HEAD`, `PATCH`, and `OPTIONS` do---if you don't use it you lose it, right? For super simple APIs all we need to know are `GET` and `POST` requests---catch me never `DELETE`ing anything, I can't be trusted.

`GET` is used to *get* data from a resource. You can pass key value pairs as parameters into the `GET` request. "`GET` requests are only used to request data (not modify)."[^4] You should never, ever, ever, ever, ever, **ever** send sensitive information through a `GET` request.

That brings us to the `POST` method. `POST` methods are used to send data to a server for the purpose of creating, modifying, or updating resources[^5]. When you have a lot of parameters to send, or if they're sensitive, or if you need to send a file via API use `POST`.

In approaching this API design I had three questions.

1.  How do you even send a file via HTTP request?
2.  Once we send it, how do we access the file and where does it go?
3.  How do we get the data from the API to R?

I don't speak Linux so bless `httr` for making this easy(ish). `httr` contains two functions that will be central to POSTing an excel file. There are [`POST()`](https://httr.r-lib.org/reference/POST.html) for making the POST request and [`upload_file()`](https://httr.r-lib.org/reference/upload_file.html) which will be used for uploading the file in the post request.

> Can we just take a moment to appreciate how perfectly named functions can be sometimes? The more self-explanatory the better.

If you don't have much experience crafting requests with `httr`, I recommend you start with the [quickstart vignette](https://cran.r-project.org/web/packages/httr/vignettes/quickstart.html).

The structure of our POST request will look like

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>POST</span><span class='o'>(</span><span class='nv'>end_point</span>, 
     body <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>param <span class='o'>=</span> <span class='nf'>upload_file</span><span class='o'>(</span><span class='s'>"file-path.ext"</span><span class='o'>)</span><span class='o'>)</span>
     <span class='o'>)</span></code></pre>

</div>

## Building the first endpoint

Now we get *how* the file will be sent. But the tough part is actually building the plumber API endpoint which will receive it. There is a fair amount of discourse on how files should be uploaded via plumber[^6]. Fortunately, [@krlmlr](https://github.com/krlmlr) pointed out [`mime::parse_multipart()`](https://rdrr.io/pkg/mime/man/parse_multipart.html) which can be used for handling files sent in requests[^7].

> Note: MIME is a standard way of sending files through the internet. I know nothing about MIME types and greatly appreciate the work that Yihui Xie, Jeffrey Horner, and Bian Beilei have done with the [`{mime}`](https://github.com/yihui/mime) package for abstracting all of this away.

`parse_multipart()` will take the incoming request and return a named list. Most importantly for us the returned object contains our POSTed file to a temporary location. Within the resultant list is a path to to the temporary file. In our plumber function definition, we parse the request, and pull out the `datapath`. That saved path is then fed to [`readxl::read_excel()`](https://readxl.tidyverse.org/reference/read_excel.html) which returns a tibble!

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'>#* Read excel file </span>
<span class='c'>#* @post /read_excel</span>

<span class='kr'>function</span><span class='o'>(</span><span class='nv'>req</span><span class='o'>)</span> <span class='o'>&#123;</span>
  
  <span class='nv'>multipart</span> <span class='o'>&lt;-</span> <span class='nf'>mime</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/mime/man/parse_multipart.html'>parse_multipart</a></span><span class='o'>(</span><span class='nv'>req</span><span class='o'>)</span>
  
  <span class='nv'>fp</span> <span class='o'>&lt;-</span> <span class='nf'>purrr</span><span class='nf'>::</span><span class='nf'><a href='https://purrr.tidyverse.org/reference/pluck.html'>pluck</a></span><span class='o'>(</span><span class='nv'>multipart</span>, <span class='m'>1</span>, <span class='s'>"datapath"</span>, <span class='m'>1</span><span class='o'>)</span>
  
  <span class='nf'>readxl</span><span class='nf'>::</span><span class='nf'><a href='https://readxl.tidyverse.org/reference/read_excel.html'>read_excel</a></span><span class='o'>(</span><span class='nv'>fp</span><span class='o'>)</span>
  
<span class='o'>&#125;</span></code></pre>

</div>

### A note on developing plumber APIs

Unfortunately it's not easy to illustrate the development of a plumber API in blog format. My secret for developing plumber APIs is a combination of the RStudio background jobs launcher and the `rstudioapi` package. To figure out the structure of the named list returned from `parse_multipart()` I returned the `multipart` object from the API.

In the session I was using to develop the API I had 3 scripts. The first, `plumber.R` contains the plumber endpoint definitions. The second, `activate.R` contains the following two lines of code:

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>pr</span> <span class='o'>&lt;-</span> <span class='nf'>plumber</span><span class='nf'>::</span><span class='nf'><a href='https://www.rplumber.io/reference/plumb.html'>plumb</a></span><span class='o'>(</span><span class='s'>"plumber.R"</span><span class='o'>)</span>
<span class='nv'>pr</span><span class='o'>$</span><span class='nf'>run</span><span class='o'>(</span>host <span class='o'>=</span> <span class='s'>"127.0.0.1"</span>, port <span class='o'>=</span> <span class='m'>5846</span><span class='o'>)</span></code></pre>

</div>

These two lines start the API defined in `plumber.R`. In my third script, where I was developing from, I had the following function call:

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># start a background job using the RStudio job launcher</span>
<span class='nf'>rstudioapi</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/rstudioapi/man/jobRunScript.html'>jobRunScript</a></span><span class='o'>(</span>path <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/file.path.html'>file.path</a></span><span class='o'>(</span><span class='nf'>here</span><span class='nf'>::</span><span class='nf'><a href='https://here.r-lib.org//reference/here.html'>here</a></span><span class='o'>(</span><span class='o'>)</span>, <span class='s'>"activate.R"</span><span class='o'>)</span>,
                         workingDir <span class='o'>=</span> <span class='nf'>here</span><span class='nf'>::</span><span class='nf'><a href='https://here.r-lib.org//reference/here.html'>here</a></span><span class='o'>(</span><span class='o'>)</span><span class='o'>)</span></code></pre>

</div>

This function call sources the `activate.R` in a background session. Having the API running in another session frees up the current session I work from to develop sample POST requests.This provides a rather fast paced iterative way of testing endpoint function definitions.

![](https://media.giphy.com/media/cbkBcxRLwjoys/giphy.gif)

<sub> I hope you're able to understand all that use of the word *session*</sub>

For example, to figure out where `datapath` was, my API definition was strictly

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'>#* Read excel file </span>
<span class='c'>#* @post /read_excel</span>

<span class='kr'>function</span><span class='o'>(</span><span class='nv'>req</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='nf'>mime</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/mime/man/parse_multipart.html'>parse_multipart</a></span><span class='o'>(</span><span class='nv'>req</span><span class='o'>)</span>
<span class='o'>&#125;</span></code></pre>

</div>

allowing me to work with the resultant object from an active R session.

## Uploading files

**Disclaimer**: Uploading files is inherently risky business. Every time you put a new file into your system you are creating an opportunity for vulnerabilities. I am not a security expert nor an API expert so take me with a grain of salt.

Defining an upload process is rather straight forward now that we are able to access the temporary file. We will use [`fs::file_copy()`](https://fs.r-lib.org/reference/copy.html) to copy the temporary file to a permanent location. To do this, we need to determine where it will be uploaded. For the sake of example I am hard coding the upload path to be at `./data`. You could feasibly create another parameter which determines where the file will be copied to, but I didn't 🤷🏻‍♂️.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'>
<span class='c'>#* Upload excel file</span>
<span class='c'>#* @post /upload</span>
<span class='kr'>function</span><span class='o'>(</span><span class='nv'>req</span><span class='o'>)</span> <span class='o'>&#123;</span>
  
  <span class='nv'>multipart</span> <span class='o'>&lt;-</span> <span class='nf'>mime</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/mime/man/parse_multipart.html'>parse_multipart</a></span><span class='o'>(</span><span class='nv'>req</span><span class='o'>)</span>
  
  <span class='nv'>fp</span> <span class='o'>&lt;-</span> <span class='nf'>purrr</span><span class='nf'>::</span><span class='nf'><a href='https://purrr.tidyverse.org/reference/pluck.html'>pluck</a></span><span class='o'>(</span><span class='nv'>multipart</span>, <span class='m'>1</span>, <span class='s'>"datapath"</span>, <span class='m'>1</span><span class='o'>)</span>
  
  <span class='nv'>f_name</span> <span class='o'>&lt;-</span> <span class='nf'>purrr</span><span class='nf'>::</span><span class='nf'><a href='https://purrr.tidyverse.org/reference/pluck.html'>pluck</a></span><span class='o'>(</span><span class='nv'>multipart</span>, <span class='m'>1</span>, <span class='s'>"name"</span><span class='o'>)</span>
  
  <span class='nv'>u_path</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/r/base/file.path.html'>file.path</a></span><span class='o'>(</span><span class='s'>"data"</span>, <span class='nv'>f_name</span><span class='o'>)</span>
  
  <span class='nf'>fs</span><span class='nf'>::</span><span class='nf'><a href='https://fs.r-lib.org/reference/copy.html'>file_copy</a></span><span class='o'>(</span><span class='nv'>fp</span>, <span class='nv'>u_path</span><span class='o'>)</span>
  
<span class='o'>&#125;</span></code></pre>

</div>

## Creating API wrappers

Creating API wrappers is one of my favorite activities because it's rather simple and feels super empowering 💪🏼. As mentioned earlier, all we will need to do to create the POST request is to specify where to make the request (the endpoint), and provide some parameters to it.

Spin up the API in the background.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># start a background job using the RStudio job launcher</span>
<span class='nf'>rstudioapi</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/rstudioapi/man/jobRunScript.html'>jobRunScript</a></span><span class='o'>(</span>path <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/file.path.html'>file.path</a></span><span class='o'>(</span><span class='nf'>here</span><span class='nf'>::</span><span class='nf'><a href='https://here.r-lib.org//reference/here.html'>here</a></span><span class='o'>(</span><span class='o'>)</span>, <span class='s'>"activate.R"</span><span class='o'>)</span>,
                         workingDir <span class='o'>=</span> <span class='nf'>here</span><span class='nf'>::</span><span class='nf'><a href='https://here.r-lib.org//reference/here.html'>here</a></span><span class='o'>(</span><span class='o'>)</span><span class='o'>)</span></code></pre>

</div>

We first define an object called `b_url` (base url) with our endpoint. Next we specify the path of the file we want to upload within the [`upload_file()`](https://httr.r-lib.org/reference/upload_file.html) command. In the repository I've included `test.xls` which contains information about top coded variables in the American Community Survey (social science, amirite?). Note that uploaded file is part of a named list within the `body` argument. Any parameters that need to be passed to your API need to be defined in the list provided to the `body`. I *think* the name of the uploaded file needs to match that of what is defined in the plumber API (`req`). I may be wrong, but for safe measures!

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://httr.r-lib.org/'>httr</a></span><span class='o'>)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span>

<span class='c'># define the url</span>
<span class='nv'>b_url</span> <span class='o'>&lt;-</span> <span class='s'>"http://127.0.0.1:5846/read_excel"</span>

<span class='c'># make the request!</span>
<span class='nv'>req</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://httr.r-lib.org/reference/POST.html'>POST</a></span><span class='o'>(</span><span class='nv'>b_url</span>, body <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>req <span class='o'>=</span> <span class='nf'><a href='https://httr.r-lib.org/reference/upload_file.html'>upload_file</a></span><span class='o'>(</span><span class='s'>"data/test.xls"</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span></code></pre>

</div>

We have now uploaded the file and made our request! Though the request is useless to us if we can't access the data 😮. We can get the content of the request using [`httr::content()`](https://httr.r-lib.org/reference/content.html). I set `type = "text/json"` because I find it easier to make json into a tibble than a named list.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># get json</span>
<span class='nv'>res_json</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://httr.r-lib.org/reference/content.html'>content</a></span><span class='o'>(</span><span class='nv'>req</span>, type <span class='o'>=</span> <span class='s'>"text/json"</span><span class='o'>)</span>

<span class='c'># show the first 50 characters of resultant json</span>
<span class='nf'><a href='https://rdrr.io/r/base/strtrim.html'>strtrim</a></span><span class='o'>(</span><span class='nv'>res_json</span>, <span class='m'>50</span><span class='o'>)</span></code></pre>

</div>

To get this json into a tibble will use [`jsonlite::fromJSON()`](https://rdrr.io/pkg/jsonlite/man/fromJSON.html) and [`tibble::as_tibble()`](https://tibble.tidyverse.org/reference/as_tibble.html).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>res</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://httr.r-lib.org/reference/content.html'>content</a></span><span class='o'>(</span><span class='nv'>req</span>, type <span class='o'>=</span> <span class='s'>"text/json"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'>jsonlite</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/jsonlite/man/fromJSON.html'>fromJSON</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://tibble.tidyverse.org/reference/as_tibble.html'>as_tibble</a></span><span class='o'>(</span><span class='o'>)</span> 

<span class='nf'><a href='https://pillar.r-lib.org/reference/glimpse.html'>glimpse</a></span><span class='o'>(</span><span class='nv'>res</span><span class='o'>)</span></code></pre>

</div>

Boom!!! It worked. Now time to make it a function. To make this generalizable we need to make it so that users can specify file paths for [`upload_file()`](https://httr.r-lib.org/reference/upload_file.html).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># define `post_excel()`</span>
<span class='nv'>post_excel</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>file</span><span class='o'>)</span> <span class='o'>&#123;</span>

  <span class='nv'>b_url</span> <span class='o'>&lt;-</span> <span class='s'>"http://127.0.0.1:5846/read_excel"</span>
  
  <span class='nv'>req</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://httr.r-lib.org/reference/POST.html'>POST</a></span><span class='o'>(</span><span class='nv'>b_url</span>, body <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>req <span class='o'>=</span> <span class='nf'><a href='https://httr.r-lib.org/reference/upload_file.html'>upload_file</a></span><span class='o'>(</span><span class='nv'>file</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span>
  
  <span class='nv'>res</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://httr.r-lib.org/reference/content.html'>content</a></span><span class='o'>(</span><span class='nv'>req</span>, type <span class='o'>=</span> <span class='s'>"text/json"</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'>jsonlite</span><span class='nf'>::</span><span class='nf'><a href='https://rdrr.io/pkg/jsonlite/man/fromJSON.html'>fromJSON</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
    <span class='nf'>tibble</span><span class='nf'>::</span><span class='nf'><a href='https://tibble.tidyverse.org/reference/as_tibble.html'>as_tibble</a></span><span class='o'>(</span><span class='o'>)</span>
  
  <span class='nv'>res</span>
  
<span class='o'>&#125;</span>
</code></pre>

</div>

You've created a wrapper to your API! Now you have a micro-service running and accessible via an R wrapper.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>post_excel</span><span class='o'>(</span><span class='s'>"data/test.xls"</span><span class='o'>)</span></code></pre>

</div>

We can create a similar function for the `/upload` endpoint.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>upload</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>file</span><span class='o'>)</span> <span class='o'>&#123;</span>
  <span class='nv'>b_url</span> <span class='o'>&lt;-</span> <span class='s'>"http://127.0.0.1:5846/upload"</span>
  
  <span class='nf'>usethis</span><span class='nf'>::</span><span class='nf'><a href='https://usethis.r-lib.org/reference/ui.html'>ui_info</a></span><span class='o'>(</span><span class='nf'>glue</span><span class='nf'>::</span><span class='nf'><a href='https://glue.tidyverse.org/reference/glue.html'>glue</a></span><span class='o'>(</span><span class='s'>"Copying &#123;file&#125; to /data/&#123;file&#125;"</span><span class='o'>)</span><span class='o'>)</span>
  
  <span class='nv'>req</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://httr.r-lib.org/reference/POST.html'>POST</a></span><span class='o'>(</span><span class='nv'>b_url</span>, body <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/list.html'>list</a></span><span class='o'>(</span>req <span class='o'>=</span> <span class='nf'><a href='https://httr.r-lib.org/reference/upload_file.html'>upload_file</a></span><span class='o'>(</span><span class='nv'>file</span><span class='o'>)</span><span class='o'>)</span><span class='o'>)</span>
  
  <span class='nf'><a href='https://rdrr.io/r/base/invisible.html'>invisible</a></span><span class='o'>(</span><span class='nv'>req</span><span class='o'>)</span>
<span class='o'>&#125;</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>upload</span><span class='o'>(</span><span class='s'>"data/test.xls"</span><span class='o'>)</span></code></pre>

</div>

> Note: I recommend using the `ui_*()` functions from {usethis} to provide informative messages to the user. <br> Second note: If you intend on only allowing a file to be uploaded once, as this function does, you should probably actually be using a PUT request.

Badah-bing badah-boom. You now have the ability to create a micro-service with plumber that is able to handle Microsoft Excel files. That is no small feat! What's next? You should create a nice little python wrapper for your newly created API. The python wrapper will be a great asset to your team and now your R based tools are accessible to anyone or anything that can make HTTP requests!!!

[^1]: <https://putrinprod.com/>

[^2]: Please see [*Ten simple rules for making research software more robust*](https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1005412) (Taschuk, and Wilson, 2017).

[^3]: <https://medium.com/@ericjwhuang/restful-api-vs-microservice-eea903ac3e73>

[^4]: <https://www.w3schools.com/tags/ref_httpmethods.asp>

[^5]: <https://www.w3schools.com/tags/ref_httpmethods.asp>

[^6]: <https://github.com/rstudio/plumber/issues/75>

[^7]: <https://github.com/rstudio/plumber/issues/75>

