---
title: SLICED! a brief reflection

date: '2021-06-18'
slug: []
categories: []
tags: [machine learning, tidymodels]
rmd_hash: 6dc77f5f460e7ea4

---

A few weeks ago I was a contestant on the machine learning game show #SLICED! The format of the challenge is as follows:

-   The competition is a total of 2 hours 15 mins
-   The first 15 minutes are spent looking at a data set---no programming allowed!
-   The next two hours are dedicated to building the best machine learning model possible
-   Predictions are submitted to Kaggle, only one prediction set can be used
-   Person with the best model wins
-   Points are allocated for:
    -   Best model
    -   Best graphing
    -   Crowd favorites

My stream is uploaded to youtube so you can catch it in all of its glory.

<iframe width="560" height="315" src="https://www.youtube.com/embed/4RnegRlUKVw" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen>
</iframe>

## How I got roped in

About a month ago I saw the below tweet from Jesse Mostipak. Naturally, it piqued my interest.

<blockquote class="twitter-tweet">
<p lang="en" dir="ltr">
Come play with meeeeee! I promise to make you look good. <a href="https://t.co/U7DRXM8deP">https://t.co/U7DRXM8deP</a>
</p>
--- Jesse Mostipak is making mirepoix for #SLICED (@kierisi) <a href="https://twitter.com/kierisi/status/1390828946191753217?ref_src=twsrc%5Etfw">May 8, 2021</a>
</blockquote>
<script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>

Everyone's favorite Tidy-Tuesday-Tom essentially voluntold me. I decided to put my name in the hat and see if I can compete. The challenge, though, is that I have only ever dabbled in machine learning. In May it was something that I had only done a handful of times and with a much older toolset-e.g. caret. If there is one thing I know about myself, it's that there is nothing like a deadline and a concrete objective to get me to learn something.

I am a strong believer in [Parkinson's Law](https://en.wikipedia.org/wiki/Parkinson's_law)---you can thank my father for that---which is characterized by the saying "If you wait until the last minute, it only takes a minute to do." Or, more formally, "work expands so as to fill the time available for its completion."

In essence, the best way for me to get better at machine learning would be to put myself in a situation---as uncomfortable it may be---where I would have to do machine learning. Alternatively, I could just faily miserably but I don't like that.

## Getting a grip on {tidymodels}

I have been loosely following the tidymodels ecosystem since the beginning. Previously my understanding of tidymodels only included, recipes, rsample, and parsnip. These three packages can get you exceptionally far, but there are so many additional packages that are instrumental to improving the ML workflow for useRs. These are tune, workflows, and workflowsets.

The most challenging part of getting started with tidymodels was understanding where each package fits in during the process. The challenging task was to figure out which packages were low level APIs and which were abstractions.

## Understanding tidymodels libraries

The most basic component of a tidymodel ML process is a recipe ({recipes}) and a model specification ({parsnip}). The recipe determines the features used and any preprocessing steps. The model specification determines which model will be trained. Additionally, we often want to include resampling---e.g. bootstrap or cross validation (called a resamples ({rsample}) object in tidymodels). With these three components we can then utilize the {tune} package to train our model on our resamples. We can build a layer of abstraction from these four components which is called a `workflow`.

In the ML process we want to train many models. And rather than just repeating the steps manually for each model, the package `workflowsets` will create many workflows for you and help you train all of those models quickly. Workflowsets were essential in my approach to sliced.

## Tidymodels resources

Getting up to speed with tidymodels was a bit of a challenge. The packages are still actively under development so building corresponding documentation must be a bit of a challenge for the team! With that said, there are so many resources that you can use to get started. Below are some of the ones that I utilized and found helpful.

-   [Tidymodeling with R](http://tmwr.org/)
-   [Workflowsets vignette](https://workflowsets.tidymodels.org/articles/evaluating-different-predictor-sets.html)
-   [Julia Silge's blog](http://juliasilge.com/)
-   And a bunch of the help docs

