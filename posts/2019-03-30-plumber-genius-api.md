---
title: "genius Plumber API"
subtile: "Closing language gaps with Plumber and Docker"
date: "2019-03-30"
slug: genius-api
categories:
  - R
  - Docker
  - Python
tags:
  - Docker
  - Python
  - R
  - genius
  - plumber
  - api
  - tutorial
rmd_hash: b5a07c89a4f965b4

---

*get started [here](https://github.com/JosiahParry/genius-api)*

Since I created `genius`, I've wanted to make a version for python. But frankly, that's a daunting task for me seeing as my python skills are intermediate at best. But recently I've been made aware of the package `plumber`. To put it plainly, `plumber` takes your R code and makes it accessible via an API.

I thought this would be difficult. I was so wrong.

![](https://media.giphy.com/media/l0MYOMxG9ijrXhF3q/giphy.gif)

## Using plumber

Plumber works by using `roxygen` like comments (`#*`). Using a single comment, you can define the request type and the end point. Following that you define a function. The arguments to the funciton become the query parameters.

The main `genius` functions only require two main arguments `artist` and `album` or `song`. Making these accessible by API is as simple as:

    #* @get /track
    function(artist, song) {
      genius::genius_lyrics(artist, song)
    }

With this line of code I created an endpoint called `track` to retrieve song lyrics. The two parameters as defined by the anonymous function are `artist` and `song`. This means that song lyrics are accessible with a query looking like `http://hostname/track?artist=artist_name&song=song_name`.

But as it stands, this isn't enough to host the API locally. Save your functions with plumber documentation into a file (I named mine `plumber.R`).

### Creating the API

Creating the API is probably the easiest part. It takes quite literally, two lines of code. The function `plumb()` takes two arguments, the `file` which contains your plumber commented code, and the `dir`ectory that houses it.

`plumb()` creates a router which is "responsible for taking an incoming request, submitting it through the appropriate filters."

I created a plumber router which would be used to route income queries.

    pr <- plumb("plumber.R")

The next step is to actually run the router. Again, this is quite simple by calling the `run()` method of the `pr` object. All I need to do is specify the `port` that the API will listen on, and optionally the `host` address.

    pr$run(port = 80, host = "0.0.0.0")

Now I can construct queries in my browser. An example query is `http://localhost/track?artist=andrew%20bird&song=proxy%20war`. Sending this request produces a very friendly `json` output.

    [
    {"track_title":"Proxy War","line":1,"lyric":"He don't have to get over her"},
    {"track_title":"Proxy War","line":2,"lyric":"She don't to have get over him"},
    {"track_title":"Proxy War","line":3,"lyric":"With all their words preserved forevermore"},
    {"track_title":"Proxy War","line":4,"lyric":"You don't have to get over her"},
    {"track_title":"Proxy War","line":5,"lyric":"She don't have to get over you"},
    {"track_title":"Proxy War","line":6,"lyric":"It's true these two have never met before"},
    {"track_title":"Proxy War","line":7,"lyric":"At least not in real life"},
    {"track_title":"Proxy War","line":8,"lyric":"Where your words cut like a knife"},
    {"track_title":"Proxy War","line":9,"lyric":"Conjuring blood, biblical floods"},
    {"track_title":"Proxy War","line":10,"lyric":"Looks that stop time"},
    {"track_title":"Proxy War","line":11,"lyric":"You don't have to remember"},
    {"track_title":"Proxy War","line":12,"lyric":"We forget what memories are for"},
    {"track_title":"Proxy War","line":13,"lyric":"Now we store them in the atmosphere"},
    {"track_title":"Proxy War","line":14,"lyric":"If you don't want to get too close"},
    {"track_title":"Proxy War","line":15,"lyric":"You don't have to get too close"},
    {"track_title":"Proxy War","line":16,"lyric":"It's just what we're calling peer-to-peer"},
    {"track_title":"Proxy War","line":17,"lyric":"You don't want to get over"},
    {"track_title":"Proxy War","line":18,"lyric":"You don't have to get over"},
    {"track_title":"Proxy War","line":19,"lyric":"We store them in the atmosphere"},
    {"track_title":"Proxy War","line":20,"lyric":"If you don't want to get over her"},
    {"track_title":"Proxy War","line":21,"lyric":"She don't have to get over you"},
    {"track_title":"Proxy War","line":22,"lyric":"It's true these two have never met before"},
    {"track_title":"Proxy War","line":23,"lyric":"At least not in real life"},
    {"track_title":"Proxy War","line":24,"lyric":"Where your words cut like a knife"},
    {"track_title":"Proxy War","line":25,"lyric":"Conjuring blood, biblical floods"},
    {"track_title":"Proxy War","line":26,"lyric":"Looks that stop time"},
    {"track_title":"Proxy War","line":27,"lyric":"If you don't want to get over her"},
    {"track_title":"Proxy War","line":28,"lyric":"You don't have to get over her"},
    {"track_title":"Proxy War","line":29,"lyric":"If you don't want to get too close"},
    {"track_title":"Proxy War","line":30,"lyric":"You don't have to get too close"},
    {"track_title":"Proxy War","line":31,"lyric":"If you want to remember"},
    {"track_title":"Proxy War","line":32,"lyric":"If you don't want to get over"},
    {"track_title":"Proxy War","line":33,"lyric":"If you don't want to get over"},
    {"track_title":"Proxy War","line":34,"lyric":"If you don't want to get over"}
    ]

## Writing a Python Wrapper

One of the appeals of writing an API is that it can be accessed from any language. This was the inspiriation of creating this API. I want to be able to call R using Python. Creating an API is a great intermediary as writing an API wrapper is much easier for me than recreating all of the code that I wrote in R.

I want to be able to recreate the three main functions of `genius`. These are `genius_lyrics()`, `genius_album()`, and `genius_tracklist()`. In doing this there are two steps I have to consider. The first is creating query urls, and the second is parsing json.

To create the urls, the `requests` library is used. Next, I created a template for the urls.

    import requests
    url_template = "http://localhost:80/track?artist={}&song={}"

The idea here is that the [`{}`](https://rdrr.io/r/base/Paren.html) characters will be filled with provided parameters by using the `.format()` method.

For example, if I wanted to get lyrics for Proxy War by Andrew Bird, I would supply `"Andrew Bird"` and `"Proxy War"` as the arguments to [`format()`](https://rdrr.io/r/base/format.html). It's important to note that these arguments are taken positionally. The url is created using this method.

    url = url_template.format("andrew bird", "proxy war")

Now I am at the point where I can ping the server to receive the json. This is accomplished by using the `.get()` method from `requests`.

    response = requests.get(url)

This returns an object that contains the json response. Next, in order to get this into a format that can be analysed, it needs to be parsed. I prefer a Pandas `DataFrame`, and fortunately Pandas has a lovely `read_json` function. I will call the `.content` attribute of the `response` objectm and feed that into the `read_json()` function.

    import pandas as pd

    proxy_war = pd.read_json(response.content)

    proxy_war.head()

        line    lyric                                   track_title
    0   1   He don't have to get over her               Proxy War
    1   2   She don't to have get over him              Proxy War
    2   3   With all their words preserved forevermore  Proxy War
    3   4   You don't have to get over her              Proxy War
    4   5   She don't have to get over you              Proxy War

Beautiful. Song lyrics are available through this API and can easily be accessed via python. The next step is to generalize this and the other two functions. The below is the code to create the `genius_lyrics()` function in python. It works almost identically as in R. However, at this moment it does not have the ability to set the `info` argument. But this can be changed easily in the original `plumber.R` file.

    # Define genius_lyrics()
    def genius_lyrics(artist, song):

        url_template = "http://localhost:80/track?artist={}&song={}"
        
        url = url_template.format(artist, song)

        response = requests.get(url)
        
        song = pd.read_json(response.content)
        
        return(song)

At this point I'm feeling extremely stoked on the fact that I can use `genius` with python. Who says R and python practitioners can't work together?

![](https://media.giphy.com/media/l2JJyDYEX1tXFmCd2/giphy.gif)

## Containerize with Docker

![](https://media.giphy.com/media/cUMNWzWZ5n75LvcCIe/giphy.gif)

To make the process of setting up this genius API up easier for those who don't necessarily interact with R, I created a lightweight-ish Docker container. The idea for this was to be able to pull a Docker image, run a command, and then the API will be available on a local port without having to interact with R at all.

I'm not the most experience person with creating Docker containers but I can borrow code quite well. Fortunately I came across some wonderful [slides](https://nolisllc.com/assets/presentations/r-tensorflow-api.pdf) from `rstudio::conf 2019`. [Heather Nollis](https://twitter.com/heatherklus) and [Jacqueline Nolis](https://twitter.com/skyetetra) presented on "API development with R and TensorFlow at T-Mobile".

This container needs two things: a linux environment and an installation of R with plumber, genius, and its dependencies. An organization called [The Rocker Project](https://www.rocker-project.org/) has created a number of Docker images that are stable and easy to install.

Since `genius` relies on many packages from the [tidyverse](https://tidyverse.org), the [`rocker/tidyverse`](https://hub.docker.com/r/rocker/tidyverse) image was used. To use their wonderful image, only one line is needed in my `Dockerfile`.

    # Import existing Docker image
    FROM rocker/tidyverse:3.5.2

Now, not knowing exactly what I was doing, I copied code from Jacqueline and Heather's sample `Dockerfile` in their slides. Their comment says that this is necessary to have the *"needed linux libraries for plumber"*, I went with it.

    # install needed linux libraries for plumber
    RUN apt-get update -qq && apt-get install -y \
      libssl-dev \
      libcurl4-gnutls-dev

`genius` and `plumber` are not part of the tidyverse image and have to be installed manually. The following lines tell Docker to run the listed R commands. For some unknown reason there was an issue with installing `genius` from CRAN so the `repos` argument was stated explicitly.

    # Install R packages
    RUN R -e "install.packages('genius', repos = 'http://cran.rstudio.com/')"
    RUN R -e "install.packages('plumber')"

In addition to the `Dockerfile` there are two files in my directory which are used to launch the API. The `plumber.R` and `launch_api.R` files. These need to be copied into the container. The line `COPY / /` copies from the location `/` in my directory to the location `/` in the container.

The Docker image has the libraries and files needed, but it needs to be able to actually launch the API. Since the `plumber.R` file specifies that the API will be listening on port `80`, I need to expose that port in my Docker image using `EXPOSE 80`.

The last part of this is to run the `launch_api.R` so the API is available. The `ENTRYPOINT` command tells Docker what to run when the container is launched. In this case `ENTRYPOINT ["Rscript", "launch_api.R"]` tells Docker to run the `Rscript` command with the argument `launch_api.R`. And with that, the `Dockerfile` is complete and read to run.

The image needs to be built and ran. The simplest way to do this for me was to work from [Dockerhub](https://cloud.docker.com/repository/docker/josiahparry/genius-api). Thus to run this container only three lines of code are needed!

    docker pull josiahparry/genius-api:working

    docker build -t josiahparry/genius-api .

    docker run --rm -p 80:80 josiahparry/genius-api

![](https://media.giphy.com/media/3o7qDEq2bMbcbPRQ2c/giphy.gif)

Boom, now you have an API that will be able to use the functionality of `genius`. If you wish to use Python with the API, I wrote a simple script which creates a nice tidy [wrapper](https://github.com/JosiahParry/genius-api/blob/master/genius.py) around it.

------------------------------------------------------------------------

If anyone is interested in writing a more stable Python library that can call the functionality described above I'd love your help to make `genius` more readily available to the python community.

