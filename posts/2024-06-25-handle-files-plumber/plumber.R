library(plumber)
# R -e 'plumber::plumb("posts/2024-06-25-handle-files-plumber/plumber.R")$run(port = 3000)'
# R -e 'plumber::plumb("plumber.R")$run(port = 3000)'
#* @post /upload
upload <- function(req, res) {
  mp <- mime::parse_multipart(req)
  mp
}

#* @post /read_csv
function(req, res) {
  mp <- mime::parse_multipart(req)
  readr::read_csv(mp$file$datapath)
}
