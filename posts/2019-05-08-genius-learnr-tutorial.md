---
title: "genius tutorial"
date: "2019-05-08"
#slug: genius-learnr

categories:
  - R
tags:
  - R
  - genius
  - tutorial
#output: learnr::tutorial
#runtime: shiny_prerendered
rmd_hash: f742b46429dadf31

---

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'>knitr</span><span class='nf'>::</span><span class='nv'><a href='https://rdrr.io/pkg/knitr/man/opts_chunk.html'>opts_chunk</a></span><span class='o'>$</span><span class='nf'>set</span><span class='o'>(</span>eval <span class='o'>=</span> <span class='kc'>FALSE</span><span class='o'>)</span></code></pre>

</div>

## Introducing genius

You want to start analysing song lyrics, where do you go? There have been music information retrieval papers written on the topic of programmatically extracting lyrics from the web. Dozens of people have gone through the laborious task of scraping song lyrics from websites. Even a recent winner of the Shiny competition scraped lyrics from Genius.com.

I too have been there. Scraping websites is not always the best use of your time. `genius` is an R package that will enable you to programatically download song lyrics in a tidy format ready for analysis. To begin using the package, it first must be installed, and loaded. In addition to `genius`, we will need our standard data manipulation tools from the `tidyverse`.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/utils/install.packages.html'>install.packages</a></span><span class='o'>(</span><span class='s'>"genius"</span><span class='o'>)</span></code></pre>

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://github.com/josiahparry/genius'>genius</a></span><span class='o'>)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span></code></pre>

</div>

## Single song lyrics

The simplest method of extracting song lyrics is to get just a single song at a time. This is done with the `genius_lyrics()` function. It takes two main arguments: `artist` and `song`. These are the quoted name of the artist and song. Additionally there is a third argument `info` which determines what extra metadata you can get. The possible values are `title`, `simple`, `artist`, `features`, and `all`. I recommend trying them all to see how they work.

In this example we will work to retrieve the song lyrics for the upcoming musician [Renny Conti](https://rennyconti.bandcamp.com).

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>floating</span> <span class='o'>&lt;-</span> <span class='nf'>genius_lyrics</span><span class='o'>(</span><span class='s'>"renny conti"</span>, <span class='s'>"people floating"</span><span class='o'>)</span>
<span class='nv'>floating</span></code></pre>

</div>

## Album Lyrics

Now that you have the intuition for obtaining lyrics for a single song, we can now create a larger dataset for the lyrics of an entire album using `genius_album()`. Similar to `genius_lyrics()`, the arguments are `artist`, `album`, and `info`.

In the exercise below the lyrics for [Snail Mail's](https://www.snailmail.band/) album Lush. Try retrieving the lyrics for an album of your own choosing.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>lush</span> <span class='o'>&lt;-</span> <span class='nf'>genius_album</span><span class='o'>(</span><span class='s'>"Snail Mail"</span>, <span class='s'>"Lush"</span><span class='o'>)</span>
<span class='nv'>lush</span></code></pre>

</div>

## Adding Lyrics to a data frame

### Multiple songs

A common use for lyric analysis is to compare the lyrics of one artist to another. In order to do that, you could potentially retrieve the lyrics for multiple songs and albums and then join them together. This has one major issue in my mind, it makes you create multiple object taking up precious memory. For this reason, the function `add_genius()` was developed. This enables you to create a tibble with a column for an artists name and their album or song title. `add_genius()` will then go through the entire tibble and add song lyrics for the tracks and albums that are available.

Let's try this with a tibble of three songs.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>three_songs</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://tibble.tidyverse.org/reference/tribble.html'>tribble</a></span><span class='o'>(</span>
  <span class='o'>~</span> <span class='nv'>artist</span>, <span class='o'>~</span> <span class='nv'>title</span>,
  <span class='s'>"Big Thief"</span>, <span class='s'>"UFOF"</span>,
  <span class='s'>"Andrew Bird"</span>, <span class='s'>"Imitosis"</span>,
  <span class='s'>"Sylvan Esso"</span>, <span class='s'>"Slack Jaw"</span>
<span class='o'>)</span>

<span class='nv'>song_lyrics</span> <span class='o'>&lt;-</span> <span class='nv'>three_songs</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'>add_genius</span><span class='o'>(</span><span class='nv'>artist</span>, <span class='nv'>title</span>, type <span class='o'>=</span> <span class='s'>"lyrics"</span><span class='o'>)</span>

<span class='nv'>song_lyrics</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://dplyr.tidyverse.org/reference/count.html'>count</a></span><span class='o'>(</span><span class='nv'>artist</span><span class='o'>)</span>
</code></pre>

</div>

### Multiple albums

`add_genius()` also extends this functionality to albums.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>albums</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://tibble.tidyverse.org/reference/tribble.html'>tribble</a></span><span class='o'>(</span>
  <span class='o'>~</span> <span class='nv'>artist</span>, <span class='o'>~</span> <span class='nv'>title</span>,
  <span class='s'>"Andrew Bird"</span>, <span class='s'>"Armchair Apocrypha"</span>,
  <span class='s'>"Andrew Bird"</span>, <span class='s'>"Things are really great here sort of"</span>
<span class='o'>)</span>

<span class='nv'>album_lyrics</span> <span class='o'>&lt;-</span> <span class='nv'>albums</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'>add_genius</span><span class='o'>(</span><span class='nv'>artist</span>, <span class='nv'>title</span>, type <span class='o'>=</span> <span class='s'>"album"</span><span class='o'>)</span>

<span class='nv'>album_lyrics</span></code></pre>

</div>

What is important to note here is that the warnings for this function are somewhat informative. When a 404 error occurs, this may be because that the song does not exist in Genius. Or, that the song is actually an instrumental which is the case here with Andrew Bird.

### Albums and Songs

In the scenario that you want to mix single songs and lyrics, you can supply a column with the type value of each row. The example below illustrates this. First a tibble with artist, track or album title, and type columns are created. Next, the tibble is piped to `add_genius()` with the unquote column names for the artist, title, and type columns. This will then iterate over each row and fetch the appropriate song lyrics.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>song_album</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://tibble.tidyverse.org/reference/tribble.html'>tribble</a></span><span class='o'>(</span>
  <span class='o'>~</span> <span class='nv'>artist</span>, <span class='o'>~</span> <span class='nv'>title</span>, <span class='o'>~</span> <span class='nv'>type</span>,
  <span class='s'>"Big Thief"</span>, <span class='s'>"UFOF"</span>, <span class='s'>"lyrics"</span>,
  <span class='s'>"Andrew Bird"</span>, <span class='s'>"Imitosis"</span>, <span class='s'>"lyrics"</span>,
  <span class='s'>"Sylvan Esso"</span>, <span class='s'>"Slack Jaw"</span>, <span class='s'>"lyrics"</span>,
  <span class='s'>"Movements"</span>, <span class='s'>"Feel Something"</span>, <span class='s'>"album"</span>
<span class='o'>)</span>

<span class='nv'>mixed_lyrics</span> <span class='o'>&lt;-</span> <span class='nv'>song_album</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'>add_genius</span><span class='o'>(</span><span class='nv'>artist</span>, <span class='nv'>title</span>, <span class='nv'>type</span><span class='o'>)</span></code></pre>

</div>

## Self-similarity

Another feature of `genius` is the ability to create self-similarity matrices to visualize lyrical patterns within a song. This idea was taken from Colin Morris' wonderful javascript based [Song Sim](https://colinmorris.github.io/SongSim/#/gallery) project. Colin explains the interpretation of a self-similarity matrix in their [TEDx talk](https://www.youtube.com/watch?v=_tjFwcmHy5M). An even better description of the interpretation is available in [this post](https://colinmorris.github.io/blog/weird-pop-songs).

To use Colin's example we will look at the structure of Ke\$ha's Tik Tok.

The function `calc_self_sim()` will create a self-similarity matrix of a given song. The main arguments for this function are the tibble (`df`), and the column containing the lyrics (`lyric_col`). Ideally this is one line per observation as is default from the output of `genius_*()`. The tidy output compares every ith word with every word in the song. This measures repetition of words and will show us the structure of the lyrics.

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>tik_tok</span> <span class='o'>&lt;-</span> <span class='nf'>genius_lyrics</span><span class='o'>(</span><span class='s'>"Ke$ha"</span>, <span class='s'>"Tik Tok"</span><span class='o'>)</span>

<span class='nv'>tt_self_sim</span> <span class='o'>&lt;-</span> <span class='nf'>calc_self_sim</span><span class='o'>(</span><span class='nv'>tik_tok</span>, <span class='nv'>lyric</span>, output <span class='o'>=</span> <span class='s'>"tidy"</span><span class='o'>)</span>

<span class='nv'>tt_self_sim</span>

<span class='nv'>tt_self_sim</span> <span class='o'><a href='https://magrittr.tidyverse.org/reference/pipe.html'>%&gt;%</a></span> 
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggplot.html'>ggplot</a></span><span class='o'>(</span><span class='nf'><a href='https://ggplot2.tidyverse.org/reference/aes.html'>aes</a></span><span class='o'>(</span>x <span class='o'>=</span> <span class='nv'>x_id</span>, y <span class='o'>=</span> <span class='nv'>y_id</span>, fill <span class='o'>=</span> <span class='nv'>identical</span><span class='o'>)</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/geom_tile.html'>geom_tile</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/scale_manual.html'>scale_fill_manual</a></span><span class='o'>(</span>values <span class='o'>=</span> <span class='nf'><a href='https://rdrr.io/r/base/c.html'>c</a></span><span class='o'>(</span><span class='s'>"white"</span>, <span class='s'>"black"</span><span class='o'>)</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggtheme.html'>theme_minimal</a></span><span class='o'>(</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/theme.html'>theme</a></span><span class='o'>(</span>legend.position <span class='o'>=</span> <span class='s'>"none"</span>,
        axis.text <span class='o'>=</span> <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/element.html'>element_blank</a></span><span class='o'>(</span><span class='o'>)</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/scale_continuous.html'>scale_y_continuous</a></span><span class='o'>(</span>trans <span class='o'>=</span> <span class='s'>"reverse"</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/labs.html'>labs</a></span><span class='o'>(</span>title <span class='o'>=</span> <span class='s'>"Tik Tok"</span>, subtitle <span class='o'>=</span> <span class='s'>"Self-similarity matrix"</span>, x <span class='o'>=</span> <span class='s'>""</span>, y <span class='o'>=</span> <span class='s'>""</span>, 
       caption <span class='o'>=</span> <span class='s'>"The matrix displays that there are three choruses with a bridge between the last two. The bridge displays internal repetition."</span><span class='o'>)</span></code></pre>

</div>

