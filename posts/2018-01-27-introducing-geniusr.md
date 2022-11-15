---
title: Introducing geniusR
author: Josiah Parry
date: '2018-01-27'
slug: introducing-geniusr
categories:
  - R
  - geniusR
tags:
  - R
  - geniusR
  - nlp
  - text mining
rmd_hash: 1a4647186ab3bdfc

---

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>knitr</span><span class='nf'>::</span><span class='nv'><a href='https://rdrr.io/pkg/knitr/man/opts_chunk.html'>opts_chunk</a></span><span class='o'>$</span><span class='nf'>set</span><span class='o'>(</span>eval<span class='o'>=</span><span class='kc'>FALSE</span><span class='o'>)</span></code></pre>

</div>

# Introducing geniusR

`geniusR` enables quick and easy download of song lyrics. The intent behind the package is to be able to perform text based analyses on songs in a tidy\[text\] format.

This package was inspired by the release of Kendrick Lamar's most recent album, **DAMN.**. As most programmers do, I spent way too long to simplify a task, that being accessing song lyrics. Genius (formerly Rap Genius) is the most widly accessible platform for lyrics.

The functions in this package enable easy access of individual song lyrics, album tracklists, and lyrics to whole albums.

## Install and load the package

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>devtools</span><span class='nf'>::</span><span class='nf'><a href='https://devtools.r-lib.org/reference/remote-reexports.html'>install_github</a></span><span class='o'>(</span><span class='s'>"josiahparry/geniusR"</span><span class='o'>)</span></code></pre>

</div>

Load the package:

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'>geniusR</span><span class='o'>)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span> <span class='c'># For manipulation</span></code></pre>

</div>

# Getting Lyrics

## Whole Albums

`genius_album()` allows you to download the lyrics for an entire album in a `tidy` format. There are two arguments `artists` and `album`. Supply the quoted name of artist and the album (if it gives you issues check that you have the album name and artists as specified on [Genius](https://genius.com)).

This returns a tidy data frame with three columns:

-   `title`: track name
-   `track_n`: track number
-   `text`: lyrics

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>emotions_math</span> <span class='o'>&lt;-</span> <span class='nf'>genius_album</span><span class='o'>(</span>artist <span class='o'>=</span> <span class='s'>"Margaret Glaspy"</span>, album <span class='o'>=</span> <span class='s'>"Emotions and Math"</span><span class='o'>)</span>
<span class='nv'>emotions_math</span></code></pre>

</div>

## Multiple Albums

If you wish to download multiple albums from multiple artists, try and keep it tidy and avoid binding rows if you can. We can achieve this in a tidy workflow by creating a tibble with two columns: `artist` and `album` where each row is an artist and their album. We can then iterate over those columns with `purrr:map2()`.

In this example I will extract 3 albums from Kendrick Lamar and Sara Bareilles (two of my favotire musicians). The first step is to create the tibble with artists and album titles.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>albums</span> <span class='o'>&lt;-</span>  <span class='nf'><a href='https://tibble.tidyverse.org/reference/tibble.html'>tibble</a></span><span class='o'>(</span>
  artist <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span>
    <span class='nf'><a href='https://rdrr.io/r/base/rep.html'>rep</a></span><span class='o'>(</span><span class='s'>"Kendrick Lamar"</span>, <span class='m'>3</span><span class='o'>)</span>, 
    <span class='nf'><a href='https://rdrr.io/r/base/rep.html'>rep</a></span><span class='o'>(</span><span class='s'>"Sara Bareilles"</span>, <span class='m'>3</span><span class='o'>)</span>
    <span class='o'>)</span>,
  album <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span>
    <span class='s'>"Section 80"</span>, <span class='s'>"Good Kid, M.A.A.D City"</span>, <span class='s'>"DAMN."</span>,
    <span class='s'>"The Blessed Unrest"</span>, <span class='s'>"Kaleidoscope Heart"</span>, <span class='s'>"Little Voice"</span>
    <span class='o'>)</span>
<span class='o'>)</span>

<span class='nv'>albums</span></code></pre>

</div>

No we can iterate over each row using the `map2` function. This allows us to feed each value from the `artist` and `album` columns to the `genius_album()` function. Utilizing a `map` call within a [`dplyr::mutate()`](https://dplyr.tidyverse.org/reference/mutate.html) function creates a list column where each value is a `tibble` with the data frame from `genius_album()`. We will later unnest this.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'>## We will have an additional artist column that will have to be dropped</span>
<span class='nv'>album_lyrics</span> <span class='o'>&lt;-</span> <span class='nv'>albums</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate.html'>mutate</a></span><span class='o'>(</span>tracks <span class='o'>=</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map2.html'>map2</a></span><span class='o'>(</span><span class='nv'>artist</span>, <span class='nv'>album</span>, <span class='nv'>genius_album</span><span class='o'>)</span><span class='o'>)</span>

<span class='nv'>album_lyrics</span></code></pre>

</div>

Now when you view this you will see that each value within the `tracks` column is `<tibble>`. This means that that value is infact another `tibble`. We expand this using [`tidyr::unnest()`](https://tidyr.tidyverse.org/reference/nest.html).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># Unnest the lyrics to expand </span>
<span class='nv'>lyrics</span> <span class='o'>&lt;-</span> <span class='nv'>album_lyrics</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://tidyr.tidyverse.org/reference/nest.html'>unnest</a></span><span class='o'>(</span><span class='nv'>tracks</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>    <span class='c'># Expanding the lyrics </span>
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/arrange.html'>arrange</a></span><span class='o'>(</span><span class='nf'><a href='https://dplyr.tidyverse.org/reference/desc.html'>desc</a></span><span class='o'>(</span><span class='nv'>artist</span><span class='o'>)</span><span class='o'>)</span> <span class='c'># Arranging by artist name </span>

<span class='nf'><a href='https://rdrr.io/r/utils/head.html'>head</a></span><span class='o'>(</span><span class='nv'>lyrics</span><span class='o'>)</span></code></pre>

</div>

## Song Lyrics

### `genius_lyrics()`

Getting lyrics to a single song is pretty easy. Let's get in our **ELEMENT.** and checkout **DNA.** by *Kendrick Lamar*. But first, note that the `genius_lyrics()` function takes two main arguments, `artist` and `song`. Be sure to spell the name of the artist and the song correctly.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>DNA</span> <span class='o'>&lt;-</span> <span class='nf'>genius_lyrics</span><span class='o'>(</span>artist <span class='o'>=</span> <span class='s'>"Kendrick Lamar"</span>, song <span class='o'>=</span> <span class='s'>"DNA."</span><span class='o'>)</span>

<span class='nv'>DNA</span></code></pre>

</div>

This returns a `tibble` with three columns `title`, `text`, and `line`. However, you can specifiy additional arguments to control the amount of information to be returned using the `info` argument.

-   `info = "title"` (default): Return the lyrics, line number, and song title.
-   `info = "simple"`: Return just the lyrics and line number.
-   `info = "artist"`: Return the lyrics, line number, and artist.
-   `info = "all"`: Return lyrics, line number, song title, artist.

## Tracklists

`genius_tracklist()`, given an `artist` and an `album` will return a barebones `tibble` with the track title, track number, and the url to the lyrics.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>genius_tracklist</span><span class='o'>(</span>artist <span class='o'>=</span> <span class='s'>"Basement"</span>, album <span class='o'>=</span> <span class='s'>"Colourmeinkindness"</span><span class='o'>)</span> </code></pre>

</div>

## Nitty Gritty

`genius_lyrics()` generates a url to Genius which is fed to `genius_url()`, the function that does the heavy lifting of actually fetching lyrics.

I have not figured out all of the patterns that are used for generating the Genius.com urls, so errors are bound to happen. If `genius_lyrics()` returns an error. Try utilizing `genius_tracklist()` and `genius_url()` together to get the song lyrics.

For example, say "(No One Knows Me) Like the Piano" by *Sampha* wasn't working in a standard `genius_lyrics()` call.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>piano</span> <span class='o'>&lt;-</span> <span class='nf'>genius_lyrics</span><span class='o'>(</span><span class='s'>"Sampha"</span>, <span class='s'>"(No One Knows Me) Like the Piano"</span><span class='o'>)</span></code></pre>

</div>

We could grab the tracklist for the album *Process* which the song is from. We could then isolate the url for *(No One Knows Me) Like the Piano* and feed that into \`genius_url().

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='c'># Get the tracklist for </span>
<span class='nv'>process</span> <span class='o'>&lt;-</span> <span class='nf'>genius_tracklist</span><span class='o'>(</span><span class='s'>"Sampha"</span>, <span class='s'>"Process"</span><span class='o'>)</span>

<span class='c'># Filter down to find the individual song</span>
<span class='nv'>piano_info</span> <span class='o'>&lt;-</span> <span class='nv'>process</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/filter.html'>filter</a></span><span class='o'>(</span><span class='nv'>title</span> <span class='o'>==</span> <span class='s'>"(No One Knows Me) Like the Piano"</span><span class='o'>)</span>

<span class='c'># Filter song using string detection</span>
<span class='c'># process %&gt;% </span>
<span class='c'>#  filter(stringr::str_detect(title, coll("Like the piano", ignore_case = TRUE)))</span>

<span class='nv'>piano_url</span> <span class='o'>&lt;-</span> <span class='nv'>piano_info</span><span class='o'>$</span><span class='nv'>track_url</span></code></pre>

</div>

Now that we have the url, feed it into `genius_url()`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>genius_url</span><span class='o'>(</span><span class='nv'>piano_url</span>, info <span class='o'>=</span> <span class='s'>"simple"</span><span class='o'>)</span></code></pre>

</div>

------------------------------------------------------------------------

# On the Internals

## Generative functions

This package works almost entirely on pattern detection. The urls from *Genius* are (mostly) easily reproducible (shout out to [Angela Li](https://twitter.com/CivicAngela) for pointing this out).

The two functions that generate urls are `gen_song_url()` and `gen_album_url()`. To see how the functions work, try feeding an artist and song title to `gen_song_url()` and an artist and album title to `gen_album_url()`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>gen_song_url</span><span class='o'>(</span><span class='s'>"Laura Marling"</span>, <span class='s'>"Soothing"</span><span class='o'>)</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>gen_album_url</span><span class='o'>(</span><span class='s'>"Daniel Caesar"</span>, <span class='s'>"Freudian"</span><span class='o'>)</span></code></pre>

</div>

`genius_lyrics()` calls `gen_song_url()` and feeds the output to `genius_url()` which preforms the scraping.

Getting lyrics for albums is slightly more involved. It first calls `genius_tracklist()` which first calls `gen_album_url()` then using the handy package `rvest` scrapes the song titles, track numbers, and song lyric urls. Next, the song urls from the output are iterated over and fed to `genius_url()`.

To make this more clear, take a look inside of `genius_album()`

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>genius_album</span> <span class='o'>&lt;-</span> <span class='kr'>function</span><span class='o'>(</span><span class='nv'>artist</span> <span class='o'>=</span> <span class='kc'>NULL</span>, <span class='nv'>album</span> <span class='o'>=</span> <span class='kc'>NULL</span>, <span class='nv'>info</span> <span class='o'>=</span> <span class='s'>"simple"</span><span class='o'>)</span> <span class='o'>&#123;</span>

  <span class='c'># Obtain tracklist from genius_tracklist</span>
  <span class='nv'>album</span> <span class='o'>&lt;-</span> <span class='nf'>genius_tracklist</span><span class='o'>(</span><span class='nv'>artist</span>, <span class='nv'>album</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>

    <span class='c'># Iterate over the url to the song title</span>
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/mutate.html'>mutate</a></span><span class='o'>(</span>lyrics <span class='o'>=</span> <span class='nf'><a href='https://purrr.tidyverse.org/reference/map.html'>map</a></span><span class='o'>(</span><span class='nv'>track_url</span>, <span class='nv'>genius_url</span>, <span class='nv'>info</span><span class='o'>)</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>

    <span class='c'># Unnest the tibble with lyrics</span>
    <span class='nf'><a href='https://tidyr.tidyverse.org/reference/nest.html'>unnest</a></span><span class='o'>(</span><span class='nv'>lyrics</span><span class='o'>)</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span>
    
    <span class='c'># Deselect the track url</span>
    <span class='nf'><a href='https://dplyr.tidyverse.org/reference/select.html'>select</a></span><span class='o'>(</span><span class='o'>-</span><span class='nv'>track_url</span><span class='o'>)</span>


  <span class='kr'><a href='https://rdrr.io/r/base/function.html'>return</a></span><span class='o'>(</span><span class='nv'>album</span><span class='o'>)</span>
<span class='o'>&#125;</span></code></pre>

</div>

### Notes:

As this is my first *"package"* there will be many issues. Please submit an issue and I will do my best to attend to it.

There are already issues of which I am present (the lack of error handling). If you would like to take those on, please go ahead and make a pull request. Please contact me on [Twitter](twitter.com/josiahparry).

