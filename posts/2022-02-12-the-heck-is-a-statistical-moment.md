---
title: The heck is a statistical moment??
author: Josiah Parry
date: '2022-02-12'
slug: [tf-is-a-statistical-moment]
categories: [spatial]
tags: []

rmd_hash: d1877a0fc675b4a3

---

I wrote myself a short story to help me remember what the moments are.

> "The first moment I looked at the distribution I thought only of the average. The second, I thought of the variance. Soon after, I thought then of the skewness. Only then did I think about the kurtosis."

This all started when reading Luc Anselin's ["Spatial Regression Analysis in R: A Workbook"](https://dces.wisc.edu/wp-content/uploads/sites/128/2013/08/W14_Anselin2007.pdf), I encountered the following:

> "Under the normal assumption for the null, the theoretical moments of Moran's I only depend on the characteristics of the weights matrix."

The moments? The what? Under the normal assumption of my study habits I would skip over this word and continue to the next sentence. However, this was critical for understanding the formula for Moran's I: $E[I] = \frac{-1}{n - 1}$.

Wikipedia was likely written by the same gatekeepers. I turned to the article on ["Method of moments (statistics)"](https://en.wikipedia.org/wiki/Method_of_moments_(statistics)) which writes

> "Those expressions are then set equal to the sample moments. The number of such equations is the same as the number of parameters to be estimated. Those equations are then solved for the parameters of interest. The solutions are estimates of those parameters."

Naturally, I turned to twitter to vent.

<blockquote class="twitter-tweet">
<p lang="en" dir="ltr">
The use of the word \"moment\" in statistics is cruel.
</p>
--- jos (@JosiahParry) <a href="https://twitter.com/JosiahParry/status/1452014741765906436?ref_src=twsrc%5Etfw">October 23, 2021</a>
</blockquote>
<script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>

Thanks to Nicole Radziwill for a very helpful [link](https://www.usna.edu/Users/oceano/pguth/md_help/html/moment_stats_2.htm) from the US Naval Academy.

The method of moments is no more than simple summary statistics from a distribution. There are four "moments".

1.  Mean
2.  Variance
3.  Skew
4.  Kurtosis

Why would one use these words? To gatekeep, of course. Academia uses [needlessly complex language](https://www.theatlantic.com/education/archive/2015/10/complex-academic-writing/412255/) quite often.

Remember, friends. Use clear and concise language. Let's remove "moments" from our statistical lexicon.

