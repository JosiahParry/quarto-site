message("hello")
pr <- plumber::plumb("/api/plumber.R")
# pr <- plumber::plumb("plumber.R")
pr <- pr |>
  pr_hook("exit", function() {
    print("Bye bye!")
  })
pr$run(host = "0.0.0.0", port = 8000)
