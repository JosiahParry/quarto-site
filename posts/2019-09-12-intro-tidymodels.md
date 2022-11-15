---
title: "Intro to Tidy Modeling"
date: "2019-09-12"

rmd_hash: 0a0b896d489221b6

---

This is a resource intended to provide a top-level introduction into the main aspects of [tidymodels](https://github.com/tidymodels/tidymodels). The introduction into tidyverse concepts largely adapted from Thomas Mock's [Intro to the Tidyverse](https://github.com/jthomasmock/intro-tidyverse). Content was also adapted from the [Max Kuhn](https://github.com/topepo)'s [Applied Machine Learning](https://github.com/tidymodels/aml-training) content as well as [Edgar Ruiz](https://github.com/edgararuiz)'s [Gentle Introduction to Tidymodels](https://rviews.rstudio.com/2019/06/19/a-gentle-intro-to-tidymodels/).

View source code [here](https://github.com/JosiahParry/intro-tidymodels).

------------------------------------------------------------------------

## Intro to Tidy modeling

-   [Prediction API](https://colorado.rstudio.com/rsc/genre-pred/)
-   [Prediction App](https://colorado.rstudio.com/rsc/hiphop-or-country/)

This repository contains the resources used for a brief (\~1hr) introduction to tidymodels.

The slides `xaringan.Rmd` briefly introduce the fundamentals of the tidymodels (parsnip, recipes, and rsample).

The API and app are based on the model created in `audio_classifier.R`. This creates a lyric classifier as outlined in [mirr](https://mirr.netlify.com).

This model is then "productionalized" with plumber (`plumber.R`), and then wrapped in a small Shiny app (`app.R`).

#### Important notes

In order to run the prediction plumber API and Shiny App, a Spotify API key will be needed. The training data was collected with the `spotifyr` package. Refer to the spotifyr [README](https://github.com/charlie86/spotifyr/) for insstructions. I stored the credentials in an `.Renviron` file.

<div class="highlight">

<iframe src="/slides/tidymodels/xaringan.html" width="700px" height="400px" data-external="1">
</iframe>

</div>

