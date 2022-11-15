---
title: "xgboost feature importance"
subtitle: "Extracting and plotting feature importance"
slug: xgb-feature-importance
date: "2018-12-01"
categories:
  - R
  - machine learning
tags:
  - R
  - machine learning
  - decision trees
  - tutorial
  - ggplot
rmd_hash: ffac7c1be06df827

---

This post will go over extracting feature (variable) importance and creating a function for creating a ggplot object for it. I will draw on the simplicity of Chris Albon's post. For steps to do the following in Python, I recommend his [post](https://chrisalbon.com/machine_learning/trees_and_forests/feature_importance/).

<img alt="Feature Importance by Chris Albon" src="https://chrisalbon.com/images/machine_learning_flashcards/Feature_Importance_print.png"/>

If you've ever created a decision tree, you've probably looked at measures of feature importance. In the above flashcard, impurity refers to how many times a feature was use and lead to a misclassification. Here, we're looking at the importance of a feature, so how much it helped in the classification or prediction of an outcome.

This example will draw on the build in data `Sonar` from the `mlbench` package.

### Prepping the Environment

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://github.com/topepo/caret/'>caret</a></span><span class='o'>)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://github.com/dmlc/xgboost'>xgboost</a></span><span class='o'>)</span>
<span class='kr'><a href='https://rdrr.io/r/base/library.html'>library</a></span><span class='o'>(</span><span class='nv'><a href='https://tidyverse.tidyverse.org'>tidyverse</a></span><span class='o'>)</span></code></pre>

</div>

### Loading the data

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/r/utils/data.html'>data</a></span><span class='o'>(</span><span class='s'>"Sonar"</span>, package <span class='o'>=</span> <span class='s'>"mlbench"</span><span class='o'>)</span></code></pre>

</div>

### Train the decision tree

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>xgb_fit</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/pkg/caret/man/train.html'>train</a></span><span class='o'>(</span><span class='nv'>Class</span> <span class='o'>~</span> <span class='nv'>.</span>,
                 data <span class='o'>=</span> <span class='nv'>Sonar</span>,
                 method <span class='o'>=</span> <span class='s'>"xgbLinear"</span><span class='o'>)</span>
</code></pre>

</div>

<div class="highlight">

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>xgb_fit</span>
<span class='c'>#&gt; eXtreme Gradient Boosting </span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; 208 samples</span>
<span class='c'>#&gt;  60 predictor</span>
<span class='c'>#&gt;   2 classes: 'M', 'R' </span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; No pre-processing</span>
<span class='c'>#&gt; Resampling: Bootstrapped (25 reps) </span>
<span class='c'>#&gt; Summary of sample sizes: 208, 208, 208, 208, 208, 208, ... </span>
<span class='c'>#&gt; Resampling results across tuning parameters:</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt;   lambda  alpha  nrounds  Accuracy   Kappa    </span>
<span class='c'>#&gt;   0e+00   0e+00   50      0.7954411  0.5856736</span>
<span class='c'>#&gt;   0e+00   0e+00  100      0.7955067  0.5859968</span>
<span class='c'>#&gt;   0e+00   0e+00  150      0.7955067  0.5859968</span>
<span class='c'>#&gt;   0e+00   1e-04   50      0.7931161  0.5807902</span>
<span class='c'>#&gt;   0e+00   1e-04  100      0.7896880  0.5736899</span>
<span class='c'>#&gt;   0e+00   1e-04  150      0.7896880  0.5736899</span>
<span class='c'>#&gt;   0e+00   1e-01   50      0.7974045  0.5899654</span>
<span class='c'>#&gt;   0e+00   1e-01  100      0.8007978  0.5965433</span>
<span class='c'>#&gt;   0e+00   1e-01  150      0.8018652  0.5987027</span>
<span class='c'>#&gt;   1e-04   0e+00   50      0.7936100  0.5817500</span>
<span class='c'>#&gt;   1e-04   0e+00  100      0.7902008  0.5746993</span>
<span class='c'>#&gt;   1e-04   0e+00  150      0.7902008  0.5746993</span>
<span class='c'>#&gt;   1e-04   1e-04   50      0.7916874  0.5777943</span>
<span class='c'>#&gt;   1e-04   1e-04  100      0.7883283  0.5708511</span>
<span class='c'>#&gt;   1e-04   1e-04  150      0.7883283  0.5708511</span>
<span class='c'>#&gt;   1e-04   1e-01   50      0.7974045  0.5899654</span>
<span class='c'>#&gt;   1e-04   1e-01  100      0.8007978  0.5965433</span>
<span class='c'>#&gt;   1e-04   1e-01  150      0.8018652  0.5987027</span>
<span class='c'>#&gt;   1e-01   0e+00   50      0.7937810  0.5824365</span>
<span class='c'>#&gt;   1e-01   0e+00  100      0.7958099  0.5863334</span>
<span class='c'>#&gt;   1e-01   0e+00  150      0.7958099  0.5863334</span>
<span class='c'>#&gt;   1e-01   1e-04   50      0.7953707  0.5854209</span>
<span class='c'>#&gt;   1e-01   1e-04  100      0.7963228  0.5873658</span>
<span class='c'>#&gt;   1e-01   1e-04  150      0.7963228  0.5873658</span>
<span class='c'>#&gt;   1e-01   1e-01   50      0.7987849  0.5923712</span>
<span class='c'>#&gt;   1e-01   1e-01  100      0.8034709  0.6018293</span>
<span class='c'>#&gt;   1e-01   1e-01  150      0.8049729  0.6047501</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt; Tuning parameter 'eta' was held constant at a value of 0.3</span>
<span class='c'>#&gt; Accuracy was used to select the optimal model using the largest value.</span>
<span class='c'>#&gt; The final values used for the model were nrounds = 150, lambda = 0.1, alpha = 0.1</span>
<span class='c'>#&gt;  and eta = 0.3.</span></code></pre>

</div>

### Extract feature importance

Since we are using the `caret` package we can use the built in function to extract feature importance, or the function from the `xgboost` package. We will do both.

#### `caret` feature importance

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>caret_imp</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/pkg/caret/man/varImp.html'>varImp</a></span><span class='o'>(</span><span class='nv'>xgb_fit</span><span class='o'>)</span>
<span class='c'>#&gt; Warning in value[[3L]](cond): The model had been generated by XGBoost version 1.0.0 or earlier and was loaded from a RDS file. We strongly ADVISE AGAINST using saveRDS() function, to ensure that your model can be read in current and upcoming XGBoost releases. Please use xgb.save() instead to preserve models for the long term. For more details and explanation, see https://xgboost.readthedocs.io/en/latest/tutorials/saving_model.html</span>
<span class='c'>#&gt; [16:44:09] WARNING: amalgamation/../src/learner.cc:940: Loading model from XGBoost &lt; 1.0.0, consider saving it again for improved compatibility</span>
<span class='nv'>caret_imp</span>
<span class='c'>#&gt; xgbLinear variable importance</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt;   only 20 most important variables shown (out of 60)</span>
<span class='c'>#&gt; </span>
<span class='c'>#&gt;     Overall</span>
<span class='c'>#&gt; V11 100.000</span>
<span class='c'>#&gt; V45  26.941</span>
<span class='c'>#&gt; V16  24.354</span>
<span class='c'>#&gt; V21  21.665</span>
<span class='c'>#&gt; V51  18.798</span>
<span class='c'>#&gt; V4   18.140</span>
<span class='c'>#&gt; V48  14.366</span>
<span class='c'>#&gt; V9   12.607</span>
<span class='c'>#&gt; V31  12.489</span>
<span class='c'>#&gt; V27  12.347</span>
<span class='c'>#&gt; V15  11.269</span>
<span class='c'>#&gt; V34   9.125</span>
<span class='c'>#&gt; V37   8.805</span>
<span class='c'>#&gt; V20   8.792</span>
<span class='c'>#&gt; V52   8.114</span>
<span class='c'>#&gt; V28   7.162</span>
<span class='c'>#&gt; V32   5.897</span>
<span class='c'>#&gt; V55   4.584</span>
<span class='c'>#&gt; V17   4.490</span>
<span class='c'>#&gt; V49   4.129</span></code></pre>

</div>

#### `xgboost` feature importance

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nv'>xgb_imp</span> <span class='o'>&lt;-</span> <span class='nf'><a href='https://rdrr.io/pkg/xgboost/man/xgb.importance.html'>xgb.importance</a></span><span class='o'>(</span>feature_names <span class='o'>=</span> <span class='nv'>xgb_fit</span><span class='o'>$</span><span class='nv'>finalModel</span><span class='o'>$</span><span class='nv'>feature_names</span>,
               model <span class='o'>=</span> <span class='nv'>xgb_fit</span><span class='o'>$</span><span class='nv'>finalModel</span><span class='o'>)</span>

<span class='nf'><a href='https://rdrr.io/r/utils/head.html'>head</a></span><span class='o'>(</span><span class='nv'>xgb_imp</span><span class='o'>)</span>
<span class='c'>#&gt;    Feature       Gain      Cover  Frequency</span>
<span class='c'>#&gt; 1:     V11 0.25619825 0.12851518 0.03283582</span>
<span class='c'>#&gt; 2:     V45 0.06902206 0.04458378 0.03582090</span>
<span class='c'>#&gt; 3:     V16 0.06239349 0.04163116 0.01492537</span>
<span class='c'>#&gt; 4:     V21 0.05550596 0.03471532 0.02686567</span>
<span class='c'>#&gt; 5:     V51 0.04816044 0.04527491 0.05373134</span>
<span class='c'>#&gt; 6:      V4 0.04647539 0.03484924 0.03880597</span></code></pre>

</div>

### Plotting feature importance

#### caret

You have a few options when it comes to plotting feature importance. You can call plot on the saved object from caret as follows:

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://r-spatial.github.io/sf/reference/plot.html'>plot</a></span><span class='o'>(</span><span class='nv'>caret_imp</span><span class='o'>)</span>
</code></pre>
<img src="figs/unnamed-chunk-8-1.png" width="700px" style="display: block; margin: auto;" />

</div>

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggplot.html'>ggplot</a></span><span class='o'>(</span><span class='nv'>caret_imp</span><span class='o'>)</span> <span class='o'>+</span>
  <span class='nf'><a href='https://ggplot2.tidyverse.org/reference/ggtheme.html'>theme_minimal</a></span><span class='o'>(</span><span class='o'>)</span>
</code></pre>
<img src="figs/unnamed-chunk-9-1.png" width="700px" style="display: block; margin: auto;" />

</div>

#### `xgboost`

You can use the plot functionality from `xgboost`

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/pkg/xgboost/man/xgb.plot.importance.html'>xgb.plot.importance</a></span><span class='o'>(</span><span class='nv'>xgb_imp</span><span class='o'>)</span>
</code></pre>
<img src="figs/unnamed-chunk-10-1.png" width="700px" style="display: block; margin: auto;" />

</div>

Or use their ggplot feature

<div class="highlight">

<pre class='chroma'><code class='language-r' data-lang='r'><span class='nf'><a href='https://rdrr.io/pkg/xgboost/man/xgb.plot.importance.html'>xgb.ggplot.importance</a></span><span class='o'>(</span><span class='nv'>xgb_imp</span><span class='o'>)</span></code></pre>

</div>

