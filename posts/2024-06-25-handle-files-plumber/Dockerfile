FROM rhub/r-minimal:4.3.1

# Install additional packages and perform other setup steps
RUN apk add --no-cache --update-cache \
    --repository http://nl.alpinelinux.org/alpine/v3.11/main \
    autoconf=2.69-r2 \
    automake=1.16.1-r0 && \
    installr -d \
    -t "libsodium-dev curl-dev linux-headers autoconf automake" \
    -a libsodium \
    plumber

EXPOSE 8000
COPY plumber.R /api/plumber.R
COPY run-plumber.R /api/run-plumber.R

ENTRYPOINT [ "Rscript", "--slave", "/api/run-plumber.R" ]