---
title: Make your R scripts Databricks notebooks

date: '2022-05-11'
slug: [r-scripts-as-notebooks]
categories: [databricks, r]
tags: [databricks, r, tutorial]

---


I've never had a good reason to deviate from the canonical `.R` file extension until today. 

As you may have seen over the past few month from my numerous rage-tweets and [Databricks related threads](https://twitter.com/JosiahParry/status/1517195634293489664), I've been doing _a lot_ of work getting figuring out Databricks as an R user so we can get onboard with adoption here at NPD. 

One of my biggest qualms about Databricks is that it's tailored to their notebooks. The notebooks get magical superpowers that aren't available anywhere else. Notebooks get root permissions, they have access to `dbutils`, and are the only thing that can actually be scheduled by Databricks outside of a jar file or SparkSQL code. 

I've spent quite a bit of time thinking about how we can schedule R scripts through a notebook. If you're wondering, have the notebook kickoff the R script with a shell command. 

But, alas, I've learned something today. If you connect your Git repo to Databricks through their `"Repos"`, you can have your R scripts be accessible as notebooks with quite literally only two changes. 

First, R scripts need to have the less-preferable, though equally functional, file extension `.r`. Second, the first line of the script should be a comment that says `# Databricks notebook source`. And that's it. Then once the git repo has been connected, it will recognize those as notebooks.

If you want to create cells in your code write a comment `# COMMAND ----------`—that's 10 hyphens at the end. 

If you create a file `main.r` which contains the body 

```
# Databricks notebook source

print("Hello world")

# COMMAND ---------

print("Databricks! I've figured you out—sorta....")

# COMMAND ---------

print("I feel powerful.")
```

You will have an R script that is recognized as a notebook by Databricks that can be scheduled using Databricks' scheduling wizard. 

