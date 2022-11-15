---
title: "APIs: the language agnostic love story"
subtitle: "Moving Beyond the R & Python love story"
author: Josiah Parry
date: '2021-03-28'
tags:
  - data science
  - production
  - R
  - python
  - microservices
  - apis
  
rmd_hash: 4acb46b88f64d26a

---

## Extending the love story through design choices

You may have noticed by now that we at RStudio have been emphatic about the R & Python love story. This is driven by our efforts to unify data science teams and bridge the language divide. Our efforts are largely characterized by our development of the package [{reticulate}](https://rstudio.github.io/reticulate/) and our professional product suite, RStudio Team.

<div class="highlight">

<img src="https://rstudio.github.io/reticulate/images/reticulated_python.png" width="35%" style="float:right; padding:10px" style="display: block; margin: auto;" />

</div>

The R & Python love story is based on the axiom that data scientists shouldn't be forced to use a single tool. They should be able to use whichever tools that a) they prefer or b) gets the job done effectively. For the most part, this is a statement of "use R *or* Python, we will support you." But we can go beyond that to "use R *and* Python." It's a bit of leap in your mental framework.

When it comes to RStudio Team, the software will support either language exceptionally well. The exceptionally brief overview of the capabilities are as follows:

-   RStudio Package Manager: provides a centralized location to install R *and* Python libraries.
-   RStudio Connect: provides a way to deploy R and Python content.
-   RStudio Server Pro: provides a centralized and scalable location to develop R and Python.

<div class="highlight">

<img src="/images/rstudio-team.png" width="75%" style="display: block; margin: auto;" />

</div>

## The R & Python love story

The R and Python love story is something that we have been leaning into here at RStudio. It is most evidenced by our development of the {reticulate} R library which has enabled features such as executing Python code chunks in R Markdown documents which gives you all of the power of R Markdown and Python. Additionally the RStudio IDE has been improved to display Python objects in your environment.

One of the key challenges is that reticulate cannot be used to help Python users call R. reticulate assists R users who want to utilize Python, but not the other way around. There exists the library [`rpy2`](https://rpy2.github.io/doc/latest/html/overview.html#background) but it is not as easy to use as reticulate---which comes with the same complexities as any other Python installation. So where do we go from here?

## Moving beyond R & Python

It is clear that the R & Python love story is wanting to some degree. reticulate is an R native way of using Python within R code. reticulate also increases the likelihood of running into environment management challenges. Rather than managing just an R or Python environment, you're now managing the intersection of two environments. Someone can probably provide the math to illustrate the possible complications here, but it's not me!

You might be thinking that I sound like a pessimist. To that I'd say, just you wait a minute! One of the solutions has been sitting in front of us this whole time---APIs.

## APIs in very brief

Application programming interfaces (APIs) are a way to enable the use of a tool by another regardless of the underlying implementation. APIs, simply put, enable machines to talk to machines. There are many different types of APIs though most commonly you'll encounter RESTful APIs. RESTful APIs work over HTTP (hyper text transfer protocol)---the stuff the internet uses. Every tool worth its salt can make an HTTP API call. In R we have [`{httr}`](https://httr.r-lib.org/) and in Python we have [`requests`](https://requests.readthedocs.io/en/master/).

If you've ever tried to get Tweets from twitter using `{rtweet}` or [`Tweepy`](https://www.tweepy.org/), you've interacted with an API. Both rtweet and tweepy end up making the same exact HTTP requests. The difference is the language that started the request. The key thing to take away is that APIs do not care what language you use to call them.

## Making the leap

APIs enable us to develop exceptionally modular code / functionality. Just like how if you want to make some functionality reproducible, you might make a function to make that easier, you might make a API instead. Think of an API as a function that can be made available to absolutely anyone. If your target audience is *just* R users, you might make a package. However, say you have a team of web developers who are making a new app and they want to utilize your model predictions, you would make an API. Or, say you have a data science team that has developed a neural network in PyTorch and you want to deliver the predictions through a shiny application, they would host the model in an API.

## The tools

The tools to make your R and Python code already exist. And they are already superbly supported in RStudio Connect. Within the R ecosystem the [plumber](https://www.rplumber.io/) package will let you turn any R function into a RESTful API endpoint. And within the Python ecosystem, Flask serves the same purpose. Now when you need to have cross team tool sharing, it is my recommendation to consider creating an API. Your colleagues won't have to translate your code, learn a new language, or create a clunky intermediate step of data export and conversion.

<div class="highlight">

<img src="https://raw.githubusercontent.com/rstudio/plumber/master/logo/plumber.svg" width="10%" style="float:right; padding:10px" style="display: block; margin: auto;" />

</div>

By incorporating APIs into your data science toolkit, you can greatly extend the reach of your work. You can build incredible standalone infrastructure that can be utilized to by people regardless of their language of choice. Rather than thinking about how you can achieve a tasks in a single R or Python script, we can begin thinking about how existing APIs can be tied together to create an immaculate data pipeline. APIs let us move beyond the dichotomy of R and Python and become language agnostic thereby embracing *every* language that may be useful to your team and your work.

## Motivating examples

In another post which can be found [here](http://josiahparry.com/post/2021-03-28-python-r/), I will briefly go over two motivating examples that illustrate how a plumber API can be used to fit an XGBoost model with Scikit Learn and an exploratory analysis of financial data provided from a Flask app.

*Disclaimer: This is a personal opinion and not endorsed or published by RStudio. My statements represent no one but myself---sometimes not even that.*

