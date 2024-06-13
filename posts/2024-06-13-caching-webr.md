---
title: "Caching WebR from CDN"
date: "2024-06-13"
subtitle: "Notes from developing `webr-js-rs`"
categories: [webr, rust, r]
---


I am developing Rust bindings to [WebR](https://webr.r-wasm.org/). Because WebR is not compiled for WASI and only WebAssembly, native Rust bindings are not possible. 
Instead, bindings are done through [wasm-bindgen](https://rustwasm.github.io/docs/wasm-bindgen/) which creates bindings to JavaScript. 

The WIP Rust crate is called [`webr-js-rs`](https://github.com/JosiahParry/webr-js-rs/).


:::{.callout-tip}
I'm building these bindings to support **[flrsh.dev](https://flrsh.dev/)**.

Sign up for an account to be notified when I launch our first course (a deep dive on DuckDB)! 
:::

`webr-js-rs` works _only_ on wasm targets.


:::{.callout-note}
- See [this informative blog post](https://hacks.mozilla.org/2019/03/standardizing-wasi-a-webassembly-system-interface/) from Mozilla on what WASI is.
- There is an [outstanding issue](https://github.com/r-wasm/webr/issues/166) on the WebR GitHub.
:::


The problem I was encountering: 

**WebR wasn't caching the binaries!**

Turns out that this is because my code had this: 

```rs
#[wasm_bindgen(module = "https://webr.r-wasm.org/latest/webr.mjs")]
```

I brought this issue up in a GitHub issue. [@GeorgeStagg](https://github.com/georgestagg) pointed out

> "The webR CDN assets under /latest/ are intentionally served with Cache-Control: no-cache so that the latest commit is always downloaded by the browser."

This makes sense! It means since there is no cache instruction, it will fetch the binaries every time! Instead he recommended to use a tagged version

> "The longer-term builds under e.g. /v0.3.3/ are served with the HTTP header cache-control: max-age=604800, and so the webR assets should automatically be **cached by browsers for 1 week**."

👆🏼 emphasis mine.

This works! So I've changed webr-js-rs to use a fixed version. 

The one challenge with this, though, is that even though the binaries are cached, the R session will be restarted from scratch if the browser is refreshed. So that is something I need to figure out next! 

