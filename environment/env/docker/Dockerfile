### Development image ###
FROM ubuntu:18.04

# Remove dpkg exclusions on man pages, so man pages are installed
RUN sed -E -i -e "s%(path-exclude=/usr/share/man)%#\1%g" /etc/dpkg/dpkg.cfg.d/excludes

# Update the base image and install build environment
# For cleanliness, keep this list in sync with packages in your production Dockerfile
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    curl \
    httpie \
    libboost-log-dev \
    libboost-regex-dev \
    libboost-signals-dev \
    libboost-system-dev \
    libgtest-dev \
    netcat 

# Set up Google Cloud SDK repository
RUN echo "deb http://packages.cloud.google.com/apt cloud-sdk-bionic main" | tee -a /etc/apt/sources.list.d/google-cloud-sdk.list && \
    curl -fsSL https://packages.cloud.google.com/apt/doc/apt-key.gpg | apt-key add -

# Set up Docker repository
RUN echo "deb [arch=amd64] https://download.docker.com/linux/ubuntu bionic stable" | tee -a /etc/apt/sources.list.d/docker.list && \
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -

# Install local dev tools
RUN apt-get update && apt-get install -y \
    docker-ce-cli \
    gcovr \
    gdbserver \
    git \
    git-review \
    google-cloud-sdk \
    less \
    lldb \
    locales \
    man \
    nano \
    sudo \
    vim

# Install user specified packages
COPY user_pkg_list.tmp /etc/user_pkg_list
RUN cat /etc/user_pkg_list | xargs apt-get install -y

# Set UTF-8 locale
RUN locale-gen en_US.UTF-8
RUN update-locale LANG=en_US.UTF-8 LANGUAGE=en_US:en LC_ALL=en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

# Add arguments for developer info, with some sane defaults
ARG user="devel"
ARG user_id=1000
ARG user_name="Devel User"
ARG group="devel"
ARG group_id=1000

# Add group and user to system
RUN groupadd -f -g ${group_id} ${group}
RUN useradd -c "${user_name}" -d /home/${user}  -g ${group_id} -G sudo,users -m -s /bin/bash -u ${user_id} ${user}
RUN chown -R ${user}:${group} /home/${user}

# Allow user to run sudo without password
RUN echo "Defaults:${user} !authenticate" >> /etc/sudoers

# Copy startup files to /usr/local/bin
COPY *.sh /usr/local/bin/

# Set /usr/src/projects as the working directory
WORKDIR /usr/src/projects

# Set some global environments
ENV DEVEL_SHELL 1
ENV TERM xterm-color
ENV USER ${user}

# Run as configured user
USER ${user_id}:${group_id}

# Expose ports for LLDB debug server
EXPOSE 7100-7110

ENTRYPOINT ["/bin/bash"]
