#!/bin/bash

docker build -f docker/base.Dockerfile -t cracking-the-web:base .
docker build -f docker/Dockerfile -t my_image .