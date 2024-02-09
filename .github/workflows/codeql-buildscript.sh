#!/usr/bin/env bash

sudo apt-get install -y libglfw3-dev libglu1-mesa-dev
make test
make examples
