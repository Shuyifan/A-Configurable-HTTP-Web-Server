### Build/test container ###
# Define builder stage
FROM cracking-the-web:base as builder

# Share work directory
COPY . /usr/src/project
WORKDIR /usr/src/project/build

# Build and test
RUN cmake ..
RUN make
RUN ctest --output-on_failure


### Deploy container ###
# Define deploy stage
FROM ubuntu:latest as deploy

# TODO(!): Copy server output binary to "."
COPY --from=builder /usr/src/project/build/bin/server .

# Copy config files
COPY conf/* conf/

# Copy static files
COPY files/static/ /usr/src/project/files/static/

# Copy images for meme generation
COPY files/images/ /usr/src/project/files/images/

# Copy files for test
COPY tests/files/images/ /usr/src/project/tests/files/images/
COPY tests/files/static/ /usr/src/project/tests/files/static/
COPY tests/files/userMemes/ /usr/src/project/tests/files/userMemes/

# TODO(!): Expose some port(s)
EXPOSE 80 12347

# Use ENTRYPOINT to specify the binary name
# TODO(!): Update with real server name
ENTRYPOINT ["./server"]

# Use CMD to specify arguments to ENTRYPOINT
# TODO(!): Update with real server args
CMD ["conf/deploy.conf"]
