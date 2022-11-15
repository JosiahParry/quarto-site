---
title: "OSINT in 7 minutes"
date: "2021-04-04"
tags:
  - osint
  - python
rmd_hash: 148c870e3e2e3eb4

---

As I was perusing the bowels of YouTube, as one does, I stumbled across a video titled ["Using My Python Skills To Punish Credit Card Scammers"](https://www.youtube.com/watch?v=StmNWzHbQJU&ab_channel=EngineerMan). The video is both whimsical, informative, and largely educational. It teaches us about:

-   web scraping / resource discovery
-   sending API requests
-   scaling requests using process threading.

<iframe width="560" height="315" src="https://www.youtube.com/embed/StmNWzHbQJU" title="Using My Python Skills To Punish Credit Card Scammers" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen>
</iframe>

Engineer Man received a phishing text message. He opens up the url in an incognito browser and begins to walk us through the steps. He fills out the fake Amazon form and begins toying with the submit form with fake data. Upon reading the error messages, he gets the sense that the form might be utilizing a real service. And doing so is never free. He takes this as an opportunity to exploit the hacker's use of the service.

He does this by taking advantage of my latest favorite thing---API requests. Engineer Man walks us through the process of opening up the web developer tools in a browser and using them to our advantage to understand the network traffic. He identifies the POST endpoint and shows us how to make a request from the information that is provided.

But this isn't where it ends. Engineer Man's face lights up as he begins to talk about threading. He utilizes the threading library to spin up 50 infinite loops.

This video sparked my excitement as it is a wonderful expample of open source intelligence (OSINT). OSINT is the process of collecting publicly all available data to be used by attackers. While attacking someone is *not* OSINT, the information gathering process is. This video is a wonderful resource for illustrating how to utilize the browser tools and public, though, hidden API endpoints.

