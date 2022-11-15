---
title: Secure R Package Environments
date: '2020-12-09'
slug: secure-package-environment
categories: [r-admin, security]
tags: [r-admin, security, packages, RSPM]
---

## Securing your R Package Environment

One of the biggest challenges faced by public sector organizations and other security conscious groups is package management. These groups are typically characterized air gapped network environments—i.e. no internet connectivity to the outside world. The purpose of an air gapped network is to get rid of any possibility of an intrusion in your network from an unwanted visitor. Air gapped installations come with some challenges particularly with package management.

Typically, when you want to install a new package it comes from The Comprehensive R Archive Network (CRAN). While CRAN has a comprehensive testing system as part of their [software development life cycle](https://www.r-project.org/doc/R-SDLC.pdf), security teams are still hesitant to trust any domains outside of their network. 

At RStudio, our solution to this is our [RStudio Package Manager](https://rstudio.com/products/package-manager/) or RSPM for short.  "RStudio Package Manager is a repository management server to organize and centralize R packages across your team, department, or entire organization." With RSPM there are a few different ways of addressing this concern. Here I'll walk through some different approaches. Each subsequent approach is stricter than the last. 

### Approach 1: trusted connect

The first approach to do this is to install and configure RSPM in your air gapped network. However, RSPM will need special permission to reach out to our sync service (`https://rspm-sync.rstudio.com`). In many cases security teams are willing to open up an outbound internet connection to just RStudio's sync service—we hope you trust us! 

This solution is the easiest as it is a quick configuration. Moreover, packages will only be installed as they are requested. Giving access to the sync service also enables your team to be able to download the latest versions of packages from CRAN. 

### Approach 2: Air-gapped CRAN

The limitation of the first approach is that is permits a constant outbound internet connection. For some groups, this is a no go. The next best approach then is to have a completely air-gapped CRAN mirror. To do this you will need an internet connection for a brief amount of time—there's no way to have data magically appear on your server! During the brief period in which your proxy is open you will have to copy all of CRAN, binaries and source, into your server. RSPM provides a [utility tool](https://docs.rstudio.com/rspm/admin/appendix/airgapped-installs/) to do this. Once complete, you can close your network again and be confident that there is no possibility of having any connection with the outside world. 

Once you've completed moving data into your server everything behaves as expected—just ensure your `options('repos')` is set properly. The one downside to this approach is that you will not be able to have access to the latest versions of packages. To rectify this, you can sync on periodic basis. 

### Approach 3: Air-gapped validated set

Often there are even further restrictions placed on data scientists which limit what packages can be used for their work. We refer to this as a [validated set](https://environments.rstudio.com/validated.html) of packages or a [curated CRAN](https://docs.rstudio.com/rspm/admin/quickstarts.html#quickstart-curated-cran). Packages are often "validated" and through that validation process are promoted to the CRAN repository. The upside to this approach is that teams can be confident in the packages their team are using.

Some approaches to validating the package environment include selecting the top `n` packages from CRAN (post on identifying those packages [here](http://josiahparry.com/post/cran-2019/)), having a subject matter expert provide a list of preferred packages, or a ticketing system. The ticketing system is the least scalable, most restrictive, and will likely hinder your work. I don't recommend it. 

The limitations with this are rather straight forward: your data scientists do not have too much leeway in utilizing packages that may expedite or even enable their work.

### Approach 4: Approach 3 but stricter 

With approach 3 there are usually two repositories: 1) a mirror of CRAN and 2) a subset of CRAN. While the subset of CRAN is preferred there is nothing stopping users from using the CRAN repository if they know the URL. To prevent this you can implement strict rules with your proxy to prevent users installing from the CRAN mirror thus forcing users to use the subset. In essence, approach 4 is approach 3 but with an enforcement mechanism. 

## Review

Package management isn't easy. It's even tougher in an offline environment. You're not going to be able to know exactly what every package does. You're going to have to make tradeoffs. You can secure your package environment by migrating packages into your own network. You can implement progressively stricter rules to reduce your exposure to potential packages. RStudio Package Manager is a wonderful tool that will make accomplishing all of this a whole lot easier. 

Feel free to reach out to me via [twitter](https://twitter.com/josiahparry) or [email](mailto:josiah@rstudio.com) and we can talk this through. 


