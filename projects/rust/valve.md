---
categories: [rust, r, production]
---

# Valve

`valve` creates multi-threaded [Plumber APIs](https://www.rplumber.io/) powered by Rust's [tokio](https://github.com/tokio-rs/tokio) and [axum](https://github.com/tokio-rs/axum) web frameworks. Plumber connections are automatically spawned, pooled, and terminated using [deadpool](https://docs.rs/deadpool/). App connections are automatically pooled by [hyper](https://docs.rs/hyper/latest/hyper/client/index.html).

[Website](https://valve.josiahparry.com/)